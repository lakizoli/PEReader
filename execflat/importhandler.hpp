#pragma once

#define DECLARE_IMPORT_HANDLER(cls)									\
	static std::shared_ptr<ImportHandler> __creator () {			\
		return std::shared_ptr<ImportHandler> (new cls ());			\
	}																\
	static uint32_t __register_creator_result;

#define IMPLEMENT_IMPORT_HANDLER(cls, importDeclaration)							\
	uint32_t cls::__register_creator_result = ImportHandler::RegisterCreator (importDeclaration, cls::__creator);

#include <x86cpu.h>

class ImportHandler {
//Construction
protected:
	ImportHandler () = default;

	typedef std::shared_ptr<ImportHandler> (*creator) ();
	static std::map<std::string, creator>& GetCreatorMap ();

public:
	virtual ~ImportHandler () = default;
	static uint32_t RegisterCreator (const std::string& importDeclaration, creator creator);
	static std::shared_ptr<ImportHandler> Create (const std::string& importDeclaration);

//Implementation
private:
	template<typename T>
	static T ReadXMMParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex);

	template<typename T>
	static T ReadRegisterParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex);

	template<typename T>
	static T ReadStackParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex);

protected:
	template<typename T>
	static T ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex);

	template<class T>
	static bool CopyFromVirtualMemory (BX_CPU_C& cpu, uint64_t virtualAddress, uint32_t size, T& destination);

	template<class T>
	static bool CopyToVirtualMemory (BX_CPU_C& cpu, const T& source, uint64_t virtualAddress, uint32_t size);

	template<class T>
	static bool WriteValueToGPRegister (BX_CPU_C& cpu, T val, uint32_t reg);

	static uint8_t* GetAddressOfVirtualAddress (BX_CPU_C& cpu, uint64_t virtualAddress);

public:
	virtual void ReadParameters (BX_CPU_C& cpu) = 0;
	virtual void Call () = 0;
	virtual bool WriteResults (BX_CPU_C& cpu) = 0;
};

template<typename T>
inline T ImportHandler::ReadXMMParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex >= 4) { //Safety check
		return T ();
	}

	switch (sizeof (T)) {
	case 1: //byte
		return T (cpu.vmm[paramIndex].zmm_ubyte[0]);
	case 2: //word
		return T (cpu.vmm[paramIndex].zmm_u16[0]);
	case 4: //dword
		return T (cpu.vmm[paramIndex].zmm_u32[0]);
	case 8: //qword
		return T (cpu.vmm[paramIndex].zmm_u64[0]);
	default:
		break;
	}

	return T ();
}

template<typename T>
inline T ImportHandler::ReadRegisterParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex >= 4) { //Safety check
		return T ();
	}

	BxRegs64 paramRegs[] = { BX_64BIT_REG_RCX, BX_64BIT_REG_RDX, BX_64BIT_REG_R8, BX_64BIT_REG_R9 };

	switch (sizeof (T)) {
	case 1: //byte
		return T (cpu.gen_reg[paramRegs[paramIndex]].word.byte.rl);
	case 2: //word
		return T (cpu.gen_reg[paramRegs[paramIndex]].word.rx);
	case 4: //dword
		return T (cpu.gen_reg[paramRegs[paramIndex]].dword.erx);
	case 8: //qword
		return T (cpu.gen_reg[paramRegs[paramIndex]].rrx);
	default:
		break;
	}

	return T ();
}

template<typename T>
inline T ImportHandler::ReadStackParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) { //Safety check
		return T ();
	}

	paramIndex -= 4;

	//TODO: read stack ...

	return T ();
}

template<>
inline float ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadXMMParameter_64BitCallingCV<float> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<float> (cpu, paramIndex);
}

template<>
inline double ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadXMMParameter_64BitCallingCV<double> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<double> (cpu, paramIndex);
}

template<>
inline uint8_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<uint8_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<uint8_t> (cpu, paramIndex);
}

template<>
inline uint16_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<uint16_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<uint16_t> (cpu, paramIndex);
}

template<>
inline uint32_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<uint32_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<uint32_t> (cpu, paramIndex);
}

template<>
inline uint64_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<uint64_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<uint64_t> (cpu, paramIndex);
}

template<>
inline int8_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<int8_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<int8_t> (cpu, paramIndex);
}

template<>
inline int16_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<int16_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<int16_t> (cpu, paramIndex);
}

template<>
inline int32_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<int32_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<int32_t> (cpu, paramIndex);
}

template<>
inline int64_t ImportHandler::ReadSimpleParameter_64BitCallingCV (BX_CPU_C& cpu, uint32_t paramIndex) {
	if (paramIndex < 4) {
		return ReadRegisterParameter_64BitCallingCV<int64_t> (cpu, paramIndex);
	}

	return ReadStackParameter_64BitCallingCV<int64_t> (cpu, paramIndex);
}

template <class T>
bool ImportHandler::CopyFromVirtualMemory (BX_CPU_C& cpu, uint64_t virtualAddress, uint32_t size, T& destination) {
	return cpu.access_read_linear (cpu.mem, virtualAddress, size, 3, BX_READ, 0, (void*) &destination) == 0;
}

template<class T>
bool ImportHandler::CopyToVirtualMemory (BX_CPU_C& cpu, const T& source, uint64_t virtualAddress, uint32_t size) {
	return cpu.access_write_linear (cpu.mem, virtualAddress, size, 3, 0, (void*) &source) == 0;
}

template<class T>
bool ImportHandler::WriteValueToGPRegister (BX_CPU_C& cpu, T val, uint32_t reg) {
	switch (sizeof (T)) {
	case 1: //byte
		cpu.gen_reg[reg].word.byte.rl = (Bit8u) val;
		return true;
	case 2: //word
		cpu.gen_reg[reg].word.rx = (Bit16u) val;
		return true;
	case 4: //dword
		cpu.gen_reg[reg].dword.erx = (Bit32u) val;
		return true;
	case 8: //qword
		cpu.gen_reg[reg].rrx = (Bit64u) val;
		return true;
	default:
		break;
	}

	return false;
}
