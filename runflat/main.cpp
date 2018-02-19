// runflat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <flatbinary.hpp>
#include <bochs/bochs.h>
#include <bochs/cpu/cpu.h>
#include <bochs/memory/memory-bochs.h>
#include <bochs/disasm/disasm.h>

static int PrintUsage (const char* exe) {
	std::cout <<
		"Usage:" << std::endl <<
		"    " << std::experimental::filesystem::path (exe).filename ().string () << " <flat path>" << std::endl <<
		std::endl;

	return 1;
}

extern int bx_init_main (int argc, char *argv[]);

struct gdt_entry {
	Bit32u low;
	Bit32u high;
};

struct linux_setup_params {
	/* 0x000 */ Bit8u   orig_x;
	/* 0x001 */ Bit8u   orig_y;
	/* 0x002 */ Bit16u  memory_size_std;
	/* 0x004 */ Bit16u  orig_video_page;
	/* 0x006 */ Bit8u   orig_video_mode;
	/* 0x007 */ Bit8u   orig_video_cols;
	/* 0x008 */ Bit16u  unused1;
	/* 0x00a */ Bit16u  orig_video_ega_bx;
	/* 0x00c */ Bit16u  unused2;
	/* 0x00e */ Bit8u   orig_video_lines;
	/* 0x00f */ Bit8u   orig_video_isVGA;
	/* 0x010 */ Bit16u  orig_video_points;
	/* 0x012 */ Bit8u   pad1[0x40 - 0x12];
	/* 0x040 */ Bit8u   apm_info[0x80 - 0x40];
	/* 0x080 */ Bit8u   hd0_info[16];
	/* 0x090 */ Bit8u   hd1_info[16];
	/* 0x0a0 */ Bit8u   pad2[0x1e0 - 0xa0];
	/* 0x1e0 */ Bit32u  memory_size_ext;
	/* 0x1e4 */ Bit8u   pad3[0x1f1 - 0x1e4];
	/* 0x1f1 */ Bit8u   setup_sects;
	/* 0x1f2 */ Bit16u  mount_root_rdonly;
	/* 0x1f4 */ Bit16u  sys_size;
	/* 0x1f6 */ Bit16u  swap_dev;
	/* 0x1f8 */ Bit16u  ramdisk_flags;
	/* 0x1fa */ Bit16u  vga_mode;
	/* 0x1fc */ Bit16u  orig_root_dev;
	/* 0x1fe */ Bit16u  bootsect_magic;
	/* 0x200 */ Bit8u   pad4[0x210 - 0x200];
	/* 0x210 */ Bit32u  loader_type;
	/* 0x214 */ Bit32u  kernel_start;
	/* 0x218 */ Bit32u  initrd_start;
	/* 0x21c */ Bit32u  initrd_size;
	/* 0x220 */ Bit8u   pad5[0x400 - 0x220];
	/* 0x400 */ struct  gdt_entry gdt[128];
	/* 0x800 */ Bit8u   commandline[2048];
};

int CDECL main (int argc, char* argv[]) {
	if (argc < 2) {
		return PrintUsage (argc > 0 ? argv[0] : "runflat");
	}

	std::shared_ptr<FlatBinary> binary = FlatBinary::Load (argv[1]);
	if (binary == nullptr) {
		std::cout << "Error: cannot load flat binary from file at path: " << argv[1] << std::endl << std::endl;
		return 1;
	}

	//TEST Disassembly
	//{
	//	const std::vector<uint8_t>& data = binary->GetBinary ();
	//	uint64_t entryPoint = binary->GetEntryPoint ();
	//	uint64_t virtualBase = binary->GetVirtualBase ();
	//	disassembler dis;

	//	char commandBuffer[1024];
	//	const uint8_t* ip = &data[entryPoint - virtualBase];
	//	uint32_t res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
	//	while (true) {
	//		ip += res;
	//		res = dis.disasm64 (virtualBase, virtualBase, ip, commandBuffer);
	//	}
	//}
	//END TEST Disassembly

	//Run program
	{
		bx_init_siminterface ();
		if (bx_init_main (1, argv) < 0) {
			BX_INSTR_EXIT_ENV ();
			return 0;
		}

		SIM->get_param_enum ("cpu.model")->set (bx_cpudb_corei7_sandy_bridge_2600k);

		//std::shared_ptr<BX_MEM_C> mem (new BX_MEM_C ());
		//std::shared_ptr<BX_CPU_C> cpu (new BX_CPU_C ());

		uint64_t memSize = 16 * 1024 * 1024; //Define 16 mb memory in virtual environment
		uint64_t hostMemSize = 16 * 1024 * 1024; //Allocate 16 mb memory on host computer
		BX_MEM (0)->init_memory (memSize, hostMemSize);

		for (uint64_t i = 0; i < memSize / 0x1000; ++i) {
			uint8_t* ptr = BX_MEM (0)->get_vector (i * 0x1000);
			memset (ptr, 0, 0x1000);
		}

		const std::vector<uint8_t>& data = binary->GetBinary ();
		uint64_t virtualBase = binary->GetVirtualBase ();

		uint64_t dataSize = data.size ();
		uint64_t pageCount = dataSize / 0x1000;
		if (dataSize % 0x1000 > 0) {
			++pageCount;
		}

		for (uint64_t i = 0; i < pageCount; ++i) {
			uint64_t pos = i * 0x1000;
			uint64_t len = dataSize - pos > 0x1000 ? 0x1000 : dataSize - pos;

			uint8_t* ptr = BX_MEM (0)->get_vector (0x100000 + virtualBase + pos);
			memcpy (ptr, &data[pos], len);

			pos += len;
		}

		BX_CPU (0)->initialize ();
		BX_CPU (0)->sanity_checks ();
		BX_CPU (0)->register_state ();
		BX_INSTR_INITIALIZE (0);

		SIM->set_init_done (1);

		//Do a hardware reset
		BX_CPU (0)->reset (BX_RESET_HARDWARE);

		//Enter CPU to long mode directly
		{
			// Enable A20 line
			BX_SET_ENABLE_A20 (1);

			// Disable interrupts and NMIs
			BX_CPU (0)->clear_IF ();

			// EIP deltas
			BX_CPU (0)->prev_rip = BX_CPU (0)->gen_reg[BX_32BIT_REG_EIP].dword.erx = 0x100000;

			// CS deltas
			BX_CPU (0)->sregs[BX_SEG_REG_CS].cache.u.segment.base = 0x00000000;
			BX_CPU (0)->sregs[BX_SEG_REG_CS].cache.u.segment.limit_scaled = 0xFFFFFFFF;
			BX_CPU (0)->sregs[BX_SEG_REG_CS].cache.u.segment.l = 1; // long mode
			BX_CPU (0)->sregs[BX_SEG_REG_CS].cache.u.segment.g = 1; // page granularity
			BX_CPU (0)->sregs[BX_SEG_REG_CS].cache.u.segment.d_b = 1; // 32bit

			// DS deltas
			BX_CPU (0)->sregs[BX_SEG_REG_DS].cache.u.segment.base = 0x00000000;
			BX_CPU (0)->sregs[BX_SEG_REG_DS].cache.u.segment.limit_scaled = 0xFFFFFFFF;
			BX_CPU (0)->sregs[BX_SEG_REG_DS].cache.u.segment.l = 1; // long mode
			BX_CPU (0)->sregs[BX_SEG_REG_DS].cache.u.segment.g = 1; // page granularity
			BX_CPU (0)->sregs[BX_SEG_REG_DS].cache.u.segment.d_b = 1; // 32bit

			// CR0 deltas
			BX_CPU (0)->cr0.set_PE (1); // long mode (protection)
			BX_CPU (0)->cr0.set_PG (1); // long mode (paging)

			// CR4 deltas
			BX_CPU (0)->cr4.set_PAE (1);
			BX_CPU (0)->cr4.set_PGE (1);

			BX_CPU (0)->efer.set_LME (1);
			BX_CPU (0)->efer.set_LMA (1);

			// Set long mode
			BX_CPU (0)->cpu_mode = BX_MODE_LONG_64;
			BX_CPU (0)->espPageWindowSize = 0;
			BX_CPU (0)->fetchModeMask = (BX_CPU (0)->sse_ok << 2) | ((BX_CPU (0)->cpu_mode == BX_MODE_LONG_64) << 1) | (BX_CPU (0)->sregs[BX_SEG_REG_CS].cache.u.segment.d_b);
			BX_CPU (0)->user_pl = (BX_CPU (0)->sregs[BX_SEG_REG_CS].selector.rpl == 3);

			// Setup GDTR
			struct linux_setup_params *params = (struct linux_setup_params *) BX_MEM (0)->get_vector (0x00090000);

			memset (params, '\0', sizeof (*params));

			/* Video settings (standard VGA) */
			params->orig_x = 0;
			params->orig_y = 0;
			params->orig_video_page = 0;
			params->orig_video_mode = 3;
			params->orig_video_cols = 80;
			params->orig_video_lines = 25;
			params->orig_video_points = 16;
			params->orig_video_isVGA = 1;
			params->orig_video_ega_bx = 3;

			/* Memory size (total mem - 1MB, in KB) */
			params->memory_size_ext = (Bit32u) (BX_MEM (0)->get_memory_len () / (1024 * 1024) - 1) * 1024;

			/* Boot parameters */
			params->loader_type = 1;
			params->bootsect_magic = 0xaa55;
			params->mount_root_rdonly = 0;
			params->orig_root_dev = 0x0100;
			params->initrd_start = 0; // initrd_start;
			params->initrd_size = 0; // initrd_size;

			/* Initial GDT */
			params->gdt[2].high = 0x00cf9a00;
			params->gdt[2].low = 0x0000ffff;
			params->gdt[3].high = 0x00cf9200;
			params->gdt[3].low = 0x0000ffff;

			// load esi with real_mode
			BX_CPU (0)->gen_reg[BX_32BIT_REG_ESI].dword.erx = 0x90000;

			// Set up initial GDT
			BX_CPU (0)->gdtr.limit = 0x400;
			BX_CPU (0)->gdtr.base = 0x00090400;
		}

		// Jump to long mode entry point
		uint64_t startAddress = binary->GetEntryPoint ();
		BX_CPU (0)->prev_rip = BX_CPU (0)->gen_reg[BX_64BIT_REG_RIP].rrx = 0x100000 + startAddress;

		//BX_CPU (0)->jump_protected (NULL, 0x10, 0x100000 + startAddress);

		while (true) {
			BX_CPU (0)->cpu_loop ();
		}

		//TODO: ...
	}

    return 0;
}
