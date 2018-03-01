#include "stdafx.h"
#include "analyzer.hpp"
#include <bochs/disasm/disasm.h>

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

std::tuple<bool, int64_t> Analyzer::CalculateOperand1Offset (const std::string& command) const {
	size_t posSeparator = command.find (' ');
	if (posSeparator == std::string::npos) {
		return std::make_tuple (false, 0ll);
	}

	if (command.length () <= posSeparator + 1) {
		return std::make_tuple (false, 0ll);
	}

	std::string op1 = command.substr (posSeparator + 1);
	int64_t callOffset = 0;
	switch (op1[0]) {
	case '.': { //Relative address calls
			auto offsetRes = ConvertRelativeAddress (op1.substr (1));
			if (!std::get<0> (offsetRes)) {
				return offsetRes;
			}

			callOffset = std::get<1> (offsetRes);
			break;
		}
	case 'r': //Register calls
	case 'e':
		break; //Ignore this call from detection -> return tuple<true, callOffset == 0>
	default: {
			size_t posBracket = op1.find ('['); //qword ptr ds:[rip+2101]
			if (posBracket != std::string::npos) { //May be rip relative, or non detectable dynamic call
				posBracket = op1.find ("[rip");
				if (posBracket != std::string::npos) {
					auto offsetRes = ConvertRelativeAddress (op1.substr (posBracket + 4));
					if (!std::get<0> (offsetRes)) {
						return offsetRes;
					}

					callOffset = std::get<1> (offsetRes);
				}

				break; //Return rip relative offset, or ignore this call from detection -> return tuple<true, callOffset == 0>
			}

			//Unhandled operator format
			return std::make_tuple (false, 0ll);
		}
	}

	return std::make_tuple (true, callOffset);
}

std::tuple<bool, uint64_t> Analyzer::DetectImportCall (uint64_t targetIP) const {
	const std::map<uint64_t, std::string>& imports = mBinary->GetImports ();
	uint64_t importTableAddress = mBinary->GetImportTableAddress ();
	if (targetIP >= importTableAddress) {
		auto itImport = imports.find (targetIP - importTableAddress);
		if (itImport != imports.end ()) {
			return std::make_tuple (true, itImport->first);
		}
	}

	return std::make_tuple (false, 0ull);
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

		uint64_t importTableAddress = mBinary->GetImportTableAddress ();
		uint64_t nullSubAddress = importTableAddress + itLastImport->first + 2 * iatItemSize;
		if (targetIP == nullSubAddress) {
			return true;
		}
	}

	return false;
}

std::shared_ptr<ASMFunction> Analyzer::WalkFunction (uint64_t address) const {
	const std::vector<uint8_t>& data = mBinary->GetBinary ();
	uint64_t virtualBase = mBinary->GetVirtualBase ();
	disassembler dis;

	std::map<uint64_t, std::string> asmSource;
	std::vector<std::shared_ptr<ASMLink>> links;
	char commandBuffer[1024];
	const uint8_t* ip = &data[address - virtualBase];
	const uint8_t* startIP = ip;
	bool endFound = false;

	do {
		uint32_t commandLength = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);

		//Convert command
		std::string cmd (commandBuffer);
		if (cmd.length () < 2) { //Check minimal command length
			return nullptr;
		}

		uint64_t currentIP = uint64_t (ip - startIP) + address;
		asmSource.emplace (currentIP, cmd);

		//Check function end
		endFound = IsEndCommand (cmd);

		//Collect subroutines
		if (cmd.find ("call") == 0) { //Call instruction
			auto offsetRes = CalculateOperand1Offset (cmd);
			if (!std::get<0> (offsetRes)) { //Something wrong
				return nullptr;
			}

			int64_t offset = std::get<1> (offsetRes);
			uint64_t targetIP = uint64_t (int64_t (currentIP + commandLength) + std::get<1> (offsetRes));

			ASMLink::LinkTypes linkType = ASMLink::LinkTypes::Undefined;
			if (offset == 0) { //Dynamic call
				linkType = ASMLink::LinkTypes::DynamicCall;
			} else { //Direct call
				linkType = ASMLink::LinkTypes::DirectCall;

				//Detect special calls
				auto importRes = DetectImportCall (targetIP);
				if (std::get<0> (importRes)) { //Import call
					linkType = ASMLink::LinkTypes::ImportCall;
					targetIP = std::get<1> (importRes);
				}
			}

			links.push_back (std::make_shared<ASMLink> (linkType, currentIP, targetIP));

			//TODO: handle detection of exit calls (exit, _exit etc.)!

		} else if (cmd[0] == 'j') { //Jump instruction to a long distance
			auto offsetRes = CalculateOperand1Offset (cmd);
			if (!std::get<0> (offsetRes)) { //Something wrong
				return nullptr;
			}

			int64_t offset = std::get<1> (offsetRes);
			uint64_t targetIP = uint64_t (int64_t (currentIP + commandLength) + std::get<1> (offsetRes));

			ASMLink::LinkTypes linkType = ASMLink::LinkTypes::Undefined;
			if (offset == 0) { //Dynamic jump link
				linkType = ASMLink::LinkTypes::DynamicJump;
			} else if (cmd.find ("jmp") == 0) { //Direct jump link
				linkType = ASMLink::LinkTypes::DirectJump;

				//Detect special jumps
				auto importRes = DetectImportCall (targetIP);
				if (std::get<0> (importRes)) { //Import jump
					linkType = ASMLink::LinkTypes::ImportJump;
					targetIP = std::get<1> (importRes);
				} else if (DetectNullSubCall (targetIP)) { //Null sub jump
					linkType = ASMLink::LinkTypes::NullSubJump;
				}
			} else { //Conditional jump
				linkType = ASMLink::LinkTypes::ConditionalJump;
			}

			links.push_back (std::make_shared<ASMLink> (linkType, currentIP, targetIP));
		}

		//Advance IP to the next command
		ip += commandLength;
	} while (!endFound);

	uint64_t funcLength = ip - startIP;
	return std::make_shared<ASMFunction> (address, funcLength, links, asmSource);
}

Analyzer::Analyzer (const std::string& flatPath, const std::string& destPath) :
	mFlatPath (flatPath),
	mDestPath (destPath)
{
}

void Analyzer::Execute (std::shared_ptr<FlatBinary> binary) {
	//Store actual binary
	mBinary = binary;

	//Walk functions on all code path
	std::set<uint64_t> functionAddresses;
	functionAddresses.insert (mBinary->GetEntryPoint ());

	//TODO: add dll exports...

	while (functionAddresses.size () > 0) {
		auto itFirst = functionAddresses.begin ();
		uint64_t virtualAddress = *itFirst;
		functionAddresses.erase (itFirst);

		std::shared_ptr<ASMFunction> func = WalkFunction (virtualAddress);
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
							functionAddresses.insert (targetAddress);
						}
						break;
					default:
						break;
					}
				}
			}
		}
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
				for (const auto itAsm : func->GetAsmSource ()) {
					sourceFile << "0x" << std::hex << std::setw (16) << std::right << std::setfill ('0') << itAsm.first << ":    ";
					sourceFile << std::setw (50) << std::left << std::setfill (' ') << itAsm.second;

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
