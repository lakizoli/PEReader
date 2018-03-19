#include "stdafx.h"
#include "executor.hpp"
#include <flatbinary.hpp>
#include <x86cpu.h>
#include "importcpuhook.hpp"

bool Executor::RunBinary (std::shared_ptr<FlatBinary> binary) {
	std::unique_ptr<bx_pc_system_c> sys (new bx_pc_system_c ());
	std::unique_ptr<BX_MEM_C> mem (new BX_MEM_C ());
	std::unique_ptr<BX_CPU_C> cpu (new BX_CPU_C (mem.get (), sys.get ()));

	////////////////////////////////////////////////////////////
	//Init memory
	uint64_t memSize = 64 * 1024 * 1024; //Define 64 mb memory in virtual environment
	uint64_t hostMemSize = 64 * 1024 * 1024; //Allocate 64 mb memory on host computer
	mem->init_memory (memSize, hostMemSize);

	for (uint64_t i = 0; i < memSize / 0x1000; ++i) {
		uint8_t* ptr = mem->get_vector (cpu.get (), i * 0x1000);
		memset (ptr, 0, 0x1000);
	}

	////////////////////////////////////////////////////////////
	//Init PC
	cpu->initialize (bx_cpudb_broadwell_ult);
	sys->initialize (1000000000);
	sys->Reset (cpu.get (), BX_RESET_HARDWARE);

	////////////////////////////////////////////////////////////
	//Copy program into memory
	const std::vector<uint8_t>& data = binary->GetBinary ();
	uint64_t virtualBase = binary->GetVirtualBase ();
	uint64_t loadBase = 0x400000;

	uint64_t dataSize = data.size ();
	uint64_t pageCount = dataSize / 0x1000;
	if (dataSize % 0x1000 > 0) {
		++pageCount;
	}

	for (uint64_t i = 0; i < pageCount; ++i) {
		uint64_t pos = i * 0x1000;
		uint64_t len = dataSize - pos > 0x1000 ? 0x1000 : dataSize - pos;

		uint8_t* ptr = mem->get_vector (cpu.get (), loadBase + virtualBase + pos);
		memcpy (ptr, &data[pos], len);

		pos += len;
	}

	////////////////////////////////////////////////////////////
	//Shift program's relocations
	const std::map<FlatBinary::RelocationTypes, std::set<uint64_t>>& relocations = binary->GetRelocations ();
	uint64_t relocationBase = binary->GetRelocationBase ();
	int64_t delta = loadBase - relocationBase;

	for (auto& it : relocations) {
		for (uint64_t virtualAddress : it.second) {
			uint8_t* ptr = mem->get_vector (cpu.get (), loadBase + virtualAddress);

			switch (it.first) {
			case FlatBinary::RelocationTypes::UInt16Low: {
				*(int16_t*) ptr += (int16_t) delta;
				break;
			}
			case FlatBinary::RelocationTypes::UInt16High: {
				*(int16_t*) ptr += (int16_t) (delta >> 16);
				break;
			}
			case FlatBinary::RelocationTypes::UInt32: {
				*(int32_t*) ptr += (int32_t) delta;
				break;
			}
			case FlatBinary::RelocationTypes::UInt64: {
				*(int64_t*) ptr += delta;
				break;
			}
			default:
				break;
			}
		}
	}

	////////////////////////////////////////////////////////////
	//Init cpu descriptors
	// EIP deltas
	cpu->prev_rip = cpu->gen_reg[BX_32BIT_REG_EIP].dword.erx = 0x100000;

	// load esi with real_mode
	cpu->gen_reg[BX_32BIT_REG_ESI].dword.erx = 0x90000;

	// Set up initial GDT
	cpu->gdtr.limit = 0x400;
	cpu->gdtr.base = 0x00090400;

	// CS deltas
	cpu->sregs[BX_SEG_REG_CS].cache.u.segment.base = 0x00000000;
	cpu->sregs[BX_SEG_REG_CS].cache.u.segment.limit_scaled = 0xFFFFFFFF;
	cpu->sregs[BX_SEG_REG_CS].cache.u.segment.l = 1; // long mode
	cpu->sregs[BX_SEG_REG_CS].cache.u.segment.g = 1; // page granularity
	cpu->sregs[BX_SEG_REG_CS].cache.u.segment.d_b = 1; // 32bit

	// DS deltas
	cpu->sregs[BX_SEG_REG_DS].cache.u.segment.base = 0x00000000;
	cpu->sregs[BX_SEG_REG_DS].cache.u.segment.limit_scaled = 0xFFFFFFFF;
	cpu->sregs[BX_SEG_REG_DS].cache.u.segment.g = 1; // page granularity
	cpu->sregs[BX_SEG_REG_DS].cache.u.segment.d_b = 1; // 32bit

	// CR0 deltas
	cpu->cr0.set_PE (1); // protected mode

	cpu->efer.set_LME (1); // long mode
	cpu->efer.set_LMA (1);

	cpu->handleCpuModeChange ();

	////////////////////////////////////////////////////////////
	//Init stack
	cpu->gen_reg[BX_64BIT_REG_RSP].rrx = 0x2000000;

	////////////////////////////////////////////////////////////
	//Init cpu hooks
	ImportCpuHook importHook (*cpu, loadBase, loadBase - 0x100000, binary);
	cpu->SetImportHook (&importHook);

	////////////////////////////////////////////////////////////
	//Run program
	uint64_t entryPoint = binary->GetEntryPoint ();
	cpu->gen_reg[BX_64BIT_REG_RIP].rrx = loadBase + entryPoint; //Set RIP

	//cpu->cpu_loop ();
	cpu->cpu_loop_direct ();

	////////////////////////////////////////////////////////////
	//Release resources
	//...

	return true; //Success
}
