#pragma once

#include "assemblycommand.hpp"

class x64_sub : public AssemblyCommand {
	DECLARE_ASSEMBLY_COMMAND (x64_sub);

public:
	x64_sub ();
	virtual ~x64_sub () = default;

	virtual bool Run (Cpu& cpu, const Cpu::Command& command) override;
};
