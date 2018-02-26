#pragma once

#include "cpuwrapper.hpp"

#define DECLARE_ASSEMBLY_COMMAND(cls)									\
	static std::shared_ptr<AssemblyCommand> __create () {				\
		return std::shared_ptr<AssemblyCommand> (new cls ());			\
	}																	\
	static uint32_t __creator_registration;

#define IMPLEMENT_ASSEMBLY_COMMAND(cls, cmd)							\
	uint32_t cls::__creator_registration = AssemblyCommand::RegisterAssebmlyCommand (cmd, cls::__create);

class AssemblyCommand {
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
	template<typename T, T lowRangeLimit, T highRangeLimit>
	static bool ExecuteUnsignedOperand (const Cpu::Command& command, uint32_t idx, T& result) {
		if (sizeof (T) != command.opSizes[idx]) {
			return false;
		}

		const std::string& op = command.cmd[idx + 1];
		switch (command.ops[idx]) {
		case Cpu::OperandTypes::Immediate:
		{
			size_t posHex = op.find ('x');
			if (posHex == 1 && op.length () > 2) { //Positive hexadecimal value
				uint64_t val = std::stoull (op.substr (posHex + 1), nullptr, 16);
				if (val < lowRangeLimit || val > highRangeLimit) { //Range check
					return false;
				}

				result = (T) val;
				return true;
			}

			//Unhandled immediate value format
			break;
		}
		case Cpu::OperandTypes::Memory:
		{
			break;
		}
		case Cpu::OperandTypes::Register:
		{
			break;
		}
		default:
			break;
		}

		//All unhandled case mapped to an error in execution...
		return false;
	}
};
