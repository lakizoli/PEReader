#include "stdafx.h"
#include "asmfunction.hpp"

ASMFunction::ASMFunction (uint64_t virtualAddress, uint64_t length, const std::vector<std::shared_ptr<ASMLink>>& links,
	const std::map<uint64_t, std::string>& asmSource, const std::map<uint64_t, std::vector<uint8_t>>& asmBytes,
	const std::string& optionalName) :
	mVirtualAddress (virtualAddress),
	mLength (length),
	mAsmSource (asmSource),
	mAsmBytes (asmBytes)
{
	//Compose name
	std::stringstream ss;
	ss << "fn_";
	ss << std::hex << std::setfill ('0') << std::setw (16) << virtualAddress;
	if (!optionalName.empty ()) {
		ss << "_(" << optionalName << ")";
	}

	mName = ss.str ();

	//Compose links
	for (std::shared_ptr<ASMLink> link : links) {
		mLinksBySourceAddress.emplace (link->GetSourceAddress (), link);

		uint64_t targetAddress = link->GetTargetAddress ();
		auto itTarget = mLinksByTargetAddress.find (targetAddress);
		if (itTarget == mLinksByTargetAddress.end ()) {
			mLinksByTargetAddress.emplace (targetAddress, std::vector<std::shared_ptr<ASMLink>> { link });
		} else {
			itTarget->second.push_back (link);
		}
	}
}

std::set<std::string> ASMFunction::GatherUsedAssemblyCommands () const {
	std::set<std::string> multiCommands { "lock", "rep" };
	std::set<std::string> result;

	for (const auto it : mAsmSource) {
		const std::string& command = it.second;
		auto multi_it = std::find_if (multiCommands.begin (), multiCommands.end (), [&command] (const std::string& checkCommand) -> bool {
			return command.find (checkCommand) == 0;
		});

		if (multi_it != multiCommands.end ()) {
			result.insert (*multi_it);

			size_t posSep = command.find (' ');
			if (posSep != std::string::npos && command.length () > posSep + 1) {
				size_t posSep2 = command.find (' ', posSep + 1);
				if (posSep2 != std::string::npos) {
					result.insert (command.substr (posSep + 1, posSep2 - posSep - 1));
				}
			}
		} else if (command.find ("multibyte nop") == 0) {
			result.insert ("nop");
		} else {
			size_t posSep = command.find (' ');
			if (posSep != std::string::npos) {
				result.insert (command.substr (0, posSep));
			} else {
				result.insert (command);
			}
		}
	}

	return result;
}

uint32_t ASMFunction::GetMaxAsmByteWidth () const {
	uint32_t maxWidth = 0;
	for (auto& it : mAsmBytes) {
		if (it.second.size () > maxWidth) {
			maxWidth = (uint32_t) it.second.size ();
		}
	}
	return maxWidth;
}
