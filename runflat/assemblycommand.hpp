#pragma once

#include "cpu.hpp"

#define DECLARE_ASSEMBLY_COMMAND(cls)									\
	static std::shared_ptr<AssemblyCommand> __create () {				\
		return std::shared_ptr<AssemblyCommand> (new cls ());			\
	}																	\
	static uint32_t __creator_registration;

#define IMPLEMENT_ASSEMBLY_COMMAND(cls, cmd)							\
	uint32_t cls::__creator_registration = AssemblyCommand::RegisterAssebmlyCommand (cmd, cls::__create);

class AssemblyCommand {
//Definitions
public:
	enum class OperandTypes {
		Unknown,
		Register,
		Memory,
		Immediate
	};

//Construction
protected:
	AssemblyCommand ();

public:
	typedef std::shared_ptr<AssemblyCommand> (*AssemblyCommandCreator) ();
	static uint32_t RegisterAssebmlyCommand (const std::string& cmd, AssemblyCommandCreator creator);
	static std::shared_ptr<AssemblyCommand> Create (const Cpu::Command& command);

	virtual ~AssemblyCommand () = default;

private:
	static std::map<std::string, AssemblyCommandCreator>& GetCreatorMap ();

//Interface
public:
	virtual bool Run (Cpu& cpu, const Cpu::Command& command) = 0;

//Implementation
protected:
	static uint32_t GetOperandSize (const std::string& operand);
};

