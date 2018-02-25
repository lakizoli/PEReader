#include "stdafx.h"
#include "assemblycommand.hpp"

AssemblyCommand::AssemblyCommand () {
}

uint32_t AssemblyCommand::RegisterAssebmlyCommand (const std::string& cmd, AssemblyCommandCreator creator) {
	GetCreatorMap ().emplace (cmd, creator);
	return 42;
}

std::shared_ptr<AssemblyCommand> AssemblyCommand::Create (const Cpu::Command& command) {
	const std::map<std::string, AssemblyCommand::AssemblyCommandCreator>& creators = GetCreatorMap ();

	if (command.cmd.size () < 1) {
		return nullptr;
	}

	auto it = creators.find (command.cmd[0]);
	if (it == creators.end ()) {
		return nullptr;
	}

	return it->second ();
}

std::map<std::string, AssemblyCommand::AssemblyCommandCreator>& AssemblyCommand::GetCreatorMap () {
	static std::map<std::string, AssemblyCommand::AssemblyCommandCreator> inst;
	return inst;
}
