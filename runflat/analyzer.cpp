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
	return std::make_shared<ASMFunction> (address, funcLength, links, asmSource, name);
}

Analyzer::Analyzer (const std::string& flatPath, const std::string& destPath) :
	mFlatPath (flatPath),
	mDestPath (destPath)
{
	//Fake disasm
	/*uint8_t src[] = { 0x48, 0x89, 0x54, 0x24, 0x10,
	0x48, 0x89, 0x4C, 0x24, 0x08,
	0x48, 0x83, 0xEC, 0x48,
	0x48, 0x8B, 0x44, 0x24, 0x50,
	0x48, 0x89, 0x44, 0x24, 0x28,
	0xEB, 0x0E,
	0x48, 0x8B, 0x44, 0x24, 0x28,
	0x48, 0x83, 0xC0, 0x08,
	0x48, 0x89, 0x44, 0x24, 0x28,
	0x48, 0x8B, 0x44, 0x24, 0x58,
	0x48, 0x39, 0x44, 0x24, 0x28,
	0x74, 0x38,
	0x48, 0x8B, 0x44, 0x24, 0x28,
	0x48, 0x83, 0x38, 0x00,
	0x75, 0x02,
	0xEB, 0xD9,
	0x48, 0x8B, 0x44, 0x24, 0x28,
	0x48, 0x8B, 0x00,
	0x48, 0x89, 0x44, 0x24, 0x30,
	0x48, 0x8B, 0x44, 0x24, 0x30,
	0xFF, 0x15, 0x4F, 0x10, 0x0E, 0x00,
	0x89, 0x44, 0x24, 0x20,
	0x83, 0x7C, 0x24, 0x20, 0x00,
	0x74, 0x06,
	0x8B, 0x44, 0x24, 0x20,
	0xEB, 0x04,
	0xEB, 0xAE,
	0x33, 0xC0,
	0x48, 0x83, 0xC4, 0x48,
	0xC3 };

	disassembler dis;
	char commandBuffer[1024];
	const uint8_t* ip = src;
	while (true) {
		uint32_t commandLength = dis.disasm64 (0x1000, 0x1000, ip, commandBuffer);
		ip += commandLength;
	}
*/
	//End fake disasm
	/*
	_initterm_e asm function bytes: 
	48 89 54 24 10       mov         qword ptr [rsp+10h],rdx
	48 89 4C 24 08       mov         qword ptr [rsp+8],rcx
	48 83 EC 48          sub         rsp,48h
	48 8B 44 24 50       mov         rax,qword ptr [rsp+50h]
	48 89 44 24 28       mov         qword ptr [rsp+28h],rax
	EB 0E                jmp         00007FF907F104B8
	48 8B 44 24 28       mov         rax,qword ptr [rsp+28h]
	48 83 C0 08          add         rax,8
	48 89 44 24 28       mov         qword ptr [rsp+28h],rax
	48 8B 44 24 58       mov         rax,qword ptr [rsp+58h]
	48 39 44 24 28       cmp         qword ptr [rsp+28h],rax
	74 38                je          00007FF907F104FC
	48 8B 44 24 28       mov         rax,qword ptr [rsp+28h]
	48 83 38 00          cmp         qword ptr [rax],0
	75 02                jne         00007FF907F104D1
	EB D9                jmp         00007FF907F104AA
	48 8B 44 24 28       mov         rax,qword ptr [rsp+28h]
	48 8B 00             mov         rax,qword ptr [rax]
	48 89 44 24 30       mov         qword ptr [rsp+30h],rax
	48 8B 44 24 30       mov         rax,qword ptr [rsp+30h]
	FF 15 4F 10 0E 00    call        qword ptr [7FF907FF1538h]
	89 44 24 20          mov         dword ptr [rsp+20h],eax
	83 7C 24 20 00       cmp         dword ptr [rsp+20h],0
	74 06                je          00007FF907F104FA
	8B 44 24 20          mov         eax,dword ptr [rsp+20h]
	EB 04                jmp         00007FF907F104FE
	EB AE                jmp         00007FF907F104AA
	33 C0                xor         eax,eax
	48 83 C4 48          add         rsp,48h
	C3                   ret
	*/
}

void Analyzer::Execute (std::shared_ptr<FlatBinary> binary) {
	//Store actual binary
	mBinary = binary;

	//TODO: walk, and alter all relocation originated to the entry point of the binary...
	//TODO: analyzer have to handle already walked function ranges...

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
	//TODO: handle byte dumps also in to the files...

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
