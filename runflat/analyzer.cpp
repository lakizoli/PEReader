#include "stdafx.h"
#include "analyzer.hpp"
#include <bochs/disasm/disasm.h>

Analyzer::Operand1Address::Operand1Address () :
	foundValidAddress (false),
	isAbsoluteAddress (false),
	isDynamicCall (false),
	absoluteValue (0ull)
{
}

Analyzer::Operand1Address::Operand1Address (uint64_t absoluteAddress) :
	foundValidAddress (true),
	isAbsoluteAddress (true),
	isDynamicCall (false),
	absoluteValue (absoluteAddress)
{
}

Analyzer::Operand1Address::Operand1Address (bool isDynamicCall, int64_t offsetValue) :
	foundValidAddress (true),
	isAbsoluteAddress (false),
	isDynamicCall (isDynamicCall),
	offsetValue (offsetValue)
{
}

bool Analyzer::IsEndCommand (const std::string& command) const {
	std::vector<const char*> endCommands {
		"ret",
		"retn",
		"iret",
		"leave",
		"jmp",
		"int3"
	};

	for (const char* endCommand : endCommands) {
		if (command.find (endCommand) == 0) {
			return true;
		}
	}

	return false;
}

std::tuple<bool, int64_t> Analyzer::ConvertRelativeAddress (const std::string& relativeAddress) const {
	switch (relativeAddress[0]) {
	case '+':
		return std::make_tuple (true, std::stoull (relativeAddress.substr (1)));
	case '-':
		return std::make_tuple (true, std::stoull (relativeAddress));
	default: //Unhandled operator format
		break;
	}

	return std::make_tuple (false, 0ll);
}

Analyzer::Operand1Address Analyzer::CalculateOperand1Address (const std::string& command) const {
	size_t posFind = command.find (' ');
	if (posFind == std::string::npos) {
		return {}; //Invalid result
	}

	if (command.length () <= posFind + 1) {
		return {}; //Invalid result
	}

	std::string op1 = command.substr (posFind + 1);
	bool isDynamicCall = false;
	int64_t callOffset = 0;
	switch (op1[0]) {
	case '.': { //Relative address calls
			auto offsetRes = ConvertRelativeAddress (op1.substr (1));
			if (!std::get<0> (offsetRes)) {
				return {}; //Invalid result
			}

			callOffset = std::get<1> (offsetRes);
			break;
		}
	case 'r': //Register calls
	case 'e':
		break; //Ignore this call from detection
	default:
		posFind = op1.find ('['); //qword ptr ds:[rip+2101]
		if (posFind != std::string::npos) { //May be rip relative, or non detectable dynamic call
			posFind = op1.find ("[rip");
			if (posFind != std::string::npos) {
				auto offsetRes = ConvertRelativeAddress (op1.substr (posFind + 4));
				if (!std::get<0> (offsetRes)) {
					return {}; //Invalid result
				}

				callOffset = std::get<1> (offsetRes);
			}

			break; //Return rip relative offset, or ignore this call from detection
		} else { //Try another format found in 32 bit disasm output -> dword ptr ds:0x1015c2b8
			posFind = op1.find (":0x");
			if (posFind != std::string::npos) {
				const uint64_t addr = std::stoull (op1.substr (posFind + 3), nullptr, 16);
				return Operand1Address (addr);
			}
		}

		//Unhandled operator format
		return {}; //Invalid result
	}

	if (callOffset == 0) {
		isDynamicCall = true;
	}

	return Operand1Address (isDynamicCall, callOffset);
}

bool Analyzer::IsImportCall (uint64_t targetIP, const std::map<uint64_t, std::string>& imports) {
	return imports.find(targetIP) != imports.end();
}

bool Analyzer::IsDataSegmentCall (uint64_t targetIP) const {
	uint64_t virtualBase = mBinary->GetVirtualBase ();
	uint64_t codeSegmentEndAddress = virtualBase + mBinary->GetCodeSize ();
	return targetIP < virtualBase || targetIP >= codeSegmentEndAddress;
}

bool Analyzer::DetectNullSubCall (uint64_t targetIP) const {
	const std::map<uint64_t, std::string>& imports = mBinary->GetImports ();
	if (imports.size () > 0) {
		auto itLastImport = imports.end ();
		--itLastImport;

		uint64_t iatItemSize = 0;
		switch (mBinary->GetType ()) {
		case FlatBinary::BinaryTypes::Bit32:
			iatItemSize = 4;
			break;
		case FlatBinary::BinaryTypes::Bit64:
			iatItemSize = 8;
			break;
		default:
			return false;
		}

		uint64_t nullSubAddress = itLastImport->first + 2 * iatItemSize;
		if (targetIP == nullSubAddress) {
			return true;
		}
	}

	return false;
}

std::shared_ptr<ASMFunction> Analyzer::WalkFunction (uint64_t address, const std::string& name) const {
	const std::vector<uint8_t>& data = mBinary->GetBinary ();
	uint64_t virtualBase = mBinary->GetVirtualBase ();
	disassembler dis;

	std::map<uint64_t, std::string> asmSource;
	std::map<uint64_t, std::vector<uint8_t>> asmBytes;
	std::vector<std::shared_ptr<ASMLink>> links;
	char commandBuffer[1024];
	const uint8_t* ip = &data[address - virtualBase];
	const uint8_t* startIP = ip;
	bool endFound = false;

	do {
		uint32_t commandLength = 0;
		switch (mBinary->GetType ()) {
		case FlatBinary::BinaryTypes::Bit32:
			commandLength = dis.disasm32 (virtualBase, virtualBase, ip, commandBuffer);
			break;
		case FlatBinary::BinaryTypes::Bit64:
			commandLength = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
			break;
		default:
			return nullptr;
		}
		

		//Convert command
		std::string cmd (commandBuffer);
		if (cmd.length () < 2) { //Check minimal command length
			return nullptr;
		}

		uint64_t currentIP = uint64_t (ip - startIP) + address;
		asmSource.emplace (currentIP, cmd);
		asmBytes.emplace (currentIP, std::vector<uint8_t> (ip, ip + commandLength));

		//Check function end
		endFound = IsEndCommand (cmd);

		//Collect subroutines
		if (cmd.find ("call") == 0) { //Call instruction
			Operand1Address addressRes = CalculateOperand1Address (cmd);
			if (!addressRes.foundValidAddress) { //Something wrong
				return nullptr;
			}

			ASMLink::LinkTypes linkType = ASMLink::LinkTypes::Undefined;
			uint64_t targetIP = 0;

			if (addressRes.isDynamicCall) { //Dynamic call
				linkType = ASMLink::LinkTypes::DynamicCall;
			} else { //Direct call
				if (addressRes.isAbsoluteAddress) {
					targetIP = addressRes.absoluteValue;
				} else {
					targetIP = uint64_t (int64_t (currentIP + commandLength) + addressRes.offsetValue);
				}

				linkType = ASMLink::LinkTypes::DirectCall;

				//Detect special calls
				if (IsImportCall (targetIP, mBinary->GetImports ())) { //Import call
					linkType = ASMLink::LinkTypes::ImportCall;
				} else if (IsImportCall (targetIP, mBinary->GetDelayImports ())) { //Delayed import call
					linkType = ASMLink::LinkTypes::DelayedImportCall;
				} else if (IsDataSegmentCall (targetIP)) {
					linkType = ASMLink::LinkTypes::DataSegmentCall;
				}
			}

			links.push_back (std::make_shared<ASMLink> (linkType, currentIP, targetIP));

			//TODO: handle detection of exit calls (exit, _exit etc.)!

		} else if (cmd[0] == 'j') { //Jump instruction to a long distance
			Operand1Address addressRes = CalculateOperand1Address (cmd);
			if (!addressRes.foundValidAddress) { //Something wrong
				return nullptr;
			}

			ASMLink::LinkTypes linkType = ASMLink::LinkTypes::Undefined;
			uint64_t targetIP = 0;

			if (addressRes.isDynamicCall) { //Dynamic jump link
				linkType = ASMLink::LinkTypes::DynamicJump;
			} else if (cmd.find ("jmp") == 0) { //Direct jump link
				if (addressRes.isAbsoluteAddress) {
					targetIP = addressRes.absoluteValue;
				} else {
					targetIP = uint64_t (int64_t (currentIP + commandLength) + addressRes.offsetValue);
				}

				linkType = ASMLink::LinkTypes::DirectJump;

				//Detect special jumps
				if (IsImportCall (targetIP, mBinary->GetImports ())) { //Import jump
					linkType = ASMLink::LinkTypes::ImportJump;
				}  else if (IsImportCall (targetIP, mBinary->GetDelayImports ())) { //Delayed import jump
					linkType = ASMLink::LinkTypes::DelayedImportJump;
				} else if (DetectNullSubCall (targetIP)) { //Null sub jump
					linkType = ASMLink::LinkTypes::NullSubJump;
				} else if (IsDataSegmentCall (targetIP)) { //Data segment targeted dynamic jump
					linkType = ASMLink::LinkTypes::DataSegmentJump;
				}
			} else { //Conditional jump
				if (addressRes.isAbsoluteAddress) {
					targetIP = addressRes.absoluteValue;
				} else {
					targetIP = uint64_t (int64_t (currentIP + commandLength) + addressRes.offsetValue);
				}

				linkType = ASMLink::LinkTypes::ConditionalJump;
			}

			links.push_back (std::make_shared<ASMLink> (linkType, currentIP, targetIP));
		}

		//Advance IP to the next command
		ip += commandLength;
	} while (!endFound);

	//TODO: remove mangling and all other special chars from optional function name...

	uint64_t funcLength = ip - startIP;
	return std::make_shared<ASMFunction> (address, funcLength, links, asmSource, asmBytes, name);
}

Analyzer::Analyzer (const std::string& flatPath, const std::string& destPath) :
	mFlatPath (flatPath),
	mDestPath (destPath)
{
}

void Analyzer::Execute (std::shared_ptr<FlatBinary> binary) {
	//Store actual binary
	mBinary = binary;

	//TODO: walk, and alter all relocation originated to the entry point of the binary (only addresses point to the .text segment have to analyze!)...

	//Walk functions on all code path
	std::set<uint64_t> functionAddresses;
	std::map<uint64_t, std::string> functionNames;

	//Add entry point of the executable
	uint64_t entryPoint = mBinary->GetEntryPoint ();
	if (entryPoint >= mBinary->GetVirtualBase ()) { //If we have a valid entry point (dll's may have no entry point)
		functionAddresses.insert (mBinary->GetEntryPoint ());
		functionNames.emplace (mBinary->GetEntryPoint (), "StartExecutable");
	}

	//Add all exported functions
	const std::map<uint64_t, std::string>& exports = mBinary->GetExports ();
	for (const auto it : exports) {
		functionAddresses.insert (it.first);
		functionNames.emplace (it.first, it.second);
	}

	//Analyze all function
	while (functionAddresses.size () > 0) {
		auto itFirst = functionAddresses.begin ();
		uint64_t virtualAddress = *itFirst;
		functionAddresses.erase (itFirst);

		auto itName = functionNames.find (virtualAddress);
		std::string functionName;
		if (itName != functionNames.end ()) {
			functionName = itName->second;
		}

		std::shared_ptr<ASMFunction> func = WalkFunction (virtualAddress, functionName);
		if (func) {
			mFunctions.emplace (func->GetVirtualAddress (), func);

			const std::map<uint64_t, std::vector<std::shared_ptr<ASMLink>>>& links = func->GetLinksByTargetAddress ();
			for (const auto it : links) {
				for (std::shared_ptr<ASMLink> link : it.second) {
					uint64_t targetAddress = link->GetTargetAddress ();

					switch (link->GetType ()) {
					case ASMLink::LinkTypes::DirectCall:
						if (mFunctions.find (targetAddress) == mFunctions.end ()) {
							functionAddresses.insert (targetAddress);
						}
						break;
					case ASMLink::LinkTypes::DirectJump:
					case ASMLink::LinkTypes::ConditionalJump:
						if (!func->IsAddressInRange (targetAddress) && mFunctions.find (targetAddress) == mFunctions.end ()) { //Jump out from function
							//Check function already walked as part of other functions
							bool foundAsEmbedded = false;
							for (auto& it : mFunctions) {
								if (it.second->IsAddressInRange (targetAddress)) {
									foundAsEmbedded = true;
									break;
								}
							}

							//Add function to walk list
							if (!foundAsEmbedded) {
								functionAddresses.insert (targetAddress);
							}
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	//Remove functions whole embedded in other functions
	std::vector<uint64_t> removeAddresses;
	for (auto& it : mFunctions) {
		for (auto& itCheck : mFunctions) {
			if (it != itCheck && itCheck.second->IsAddressInRange (it.first)) {
				removeAddresses.push_back (it.first);
				break;
			}
		}
	}

	for (uint64_t removeFuncAddress : removeAddresses) {
		mFunctions.erase (removeFuncAddress);
	}
}

void Analyzer::SaveAssemblyFiles () const {
	for (const auto itFunc : mFunctions) {
		std::shared_ptr<ASMFunction> func = itFunc.second;
		if (func) {
			std::experimental::filesystem::path path (mDestPath);
			path /= func->GetName () + ".asm";

			std::fstream sourceFile (path.string (), std::ios::binary | std::ios::out | std::ios::trunc);
			if (sourceFile) {
				uint32_t maxByteWidth = func->GetMaxAsmByteWidth ();

				for (const auto itAsm : func->GetAsmSource ()) {
					sourceFile << "0x" << std::hex << std::setw (16) << std::right << std::setfill ('0') << itAsm.first << ":    ";

					const std::map<uint64_t, std::vector<uint8_t>>& asmBytes = func->GetAsmBytes ();
					const auto itBytes = asmBytes.find (itAsm.first);
					if (itBytes != asmBytes.end ()) {
						const std::vector<uint8_t>& bytes = itBytes->second;

						std::stringstream ss;
						for (uint8_t byte : bytes) {
							ss << ' ' << std::hex << std::setw (2) << std::setfill ('0') << (uint32_t)byte;
						}

						sourceFile << std::setw (maxByteWidth * 3 + 6) << std::left << std::setfill (' ') << ss.str ();
					}

					sourceFile << std::setw (50) << std::left << std::setfill (' ') << itAsm.second;

					if (itAsm.first == mBinary->GetEntryPoint ()) {
						sourceFile << "; Start of executable";
					}

					const std::map<uint64_t, std::shared_ptr<ASMLink>>& links = func->GetLinksBySourceAddress ();
					auto itLink = links.find (itAsm.first);
					if (itLink != links.end ()) {
						std::shared_ptr<ASMLink> link = itLink->second;
						if (link) {
							switch (link->GetType ()) {
							case ASMLink::LinkTypes::ImportCall:
							case ASMLink::LinkTypes::ImportJump: {
								const std::map<uint64_t, std::string>& imports = mBinary->GetImports ();
								auto itImport = imports.find (link->GetTargetAddress ());
								if (itImport != imports.end ()) {
									sourceFile << "; import: " << itImport->second;
								}
								break;
							}
							case ASMLink::LinkTypes::DelayedImportCall:
							case ASMLink::LinkTypes::DelayedImportJump: {
								const std::map<uint64_t, std::string>& imports = mBinary->GetDelayImports ();
								auto itImport = imports.find (link->GetTargetAddress ());
								if (itImport != imports.end ()) {
									sourceFile << "; delayed import: " << itImport->second;
								}
								break;
							}
							case ASMLink::LinkTypes::NullSubJump:
								sourceFile << "; [jump to null sub]";
								break;
							case ASMLink::LinkTypes::DirectCall:
							case ASMLink::LinkTypes::DirectJump:
							case ASMLink::LinkTypes::ConditionalJump:
								sourceFile << "; local target: 0x" << std::hex << std::setw (16) << std::right << std::setfill ('0') << link->GetTargetAddress ();
								break;
							case ASMLink::LinkTypes::DynamicCall:
							case ASMLink::LinkTypes::DynamicJump:
								sourceFile << "; dynamic";
								break;
							case ASMLink::LinkTypes::DataSegmentCall:
							case ASMLink::LinkTypes::DataSegmentJump:
								sourceFile << "; data segment address at: 0x" << std::hex << std::setw (16) << std::right << std::setfill ('0') << link->GetTargetAddress ();
								break;
							default:
								break;
							}
						}
					}
					sourceFile << std::endl;
				}
			}
		}
	}
}

void Analyzer::SaveUsedCommandsFile () const {
	std::set<std::string> usedCommands;

	for (const auto itFunc : mFunctions) {
		std::shared_ptr<ASMFunction> func = itFunc.second;
		if (func) {
			std::set<std::string> cmds = func->GatherUsedAssemblyCommands ();
			for (const std::string& cmd : cmds) {
				usedCommands.insert (cmd);
			}
		}
	}

	std::experimental::filesystem::path path (mDestPath);
	path /= "asm.commands";

	std::fstream cmdFile (path.string (), std::ios::binary | std::ios::out | std::ios::trunc);
	if (cmdFile) {
		for (const std::string& cmd : usedCommands) {
			cmdFile << cmd << std::endl;
		}
	}
}
