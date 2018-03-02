#include "stdafx.h"
#include "asmfunction.hpp"

ASMFunction::ASMFunction (uint64_t virtualAddress, uint64_t length, const std::vector<std::shared_ptr<ASMLink>>& links,
	const std::map<uint64_t, std::string>& asmSource, const std::string& optionalName) :
	mVirtualAddress (virtualAddress),
	mLength (length),
	mAsmSource (asmSource)
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
	std::set<std::string> result;

	for (const auto it : mAsmSource) {
		const std::string& command = it.second;
		if (command.find ("multibyte nop") == 0) {
			result.insert ("nop");
		} else if (command.find ("lock") == 0) {
			result.insert ("lock");

			size_t posSep = command.find (' ');
			if (posSep != std::string::npos && command.length() > posSep+1) {
				size_t posSep2 = command.find (' ', posSep + 1);
				if (posSep2 != std::string::npos) {
					result.insert (command.substr (posSep + 1, posSep2 - posSep - 1));
				}
			}
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
