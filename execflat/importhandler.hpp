#pragma once

#include <x86cpu.h>

#define DECLARE_IMPORT_HANDLER(cls)								\
	static std::shared_ptr<ImportHandler> __creator () {		\
		return std::shared_ptr<ImportHandler> (new cls ());		\
	}															\
	static uint32_t __register_creator_result;

#define IMPLEMENT_IMPORT_HANDLER(cls, importDeclaration)							\
	uint32_t cls::__register_creator_result = ImportHandler::RegisterCreator (importDeclaration, cls::__creator);

class ImportState;

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
	virtual void ReadParameters (BX_CPU_C& cpu, uint64_t injectBase, std::shared_ptr<ImportState> state) = 0;
	virtual void Call () = 0;
	virtual bool WriteResults (BX_CPU_C& cpu, std::shared_ptr<ImportState> state) = 0;
};

template<class T>
class ImportHandlerWithState : public ImportHandler {
protected:
	std::string mStateID;
	std::shared_ptr<T> mState;

	ImportHandlerWithState (const std::string& stateID) : mStateID (stateID) {}

	void ReadState (std::shared_ptr<ImportState> state);
	bool WriteState (std::shared_ptr<ImportState> state);

	uint64_t GetVirtualAddressOfStateItem (uint64_t itemOffset);
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

	uint64_t virtualStackAddress = cpu.gen_reg[BX_64BIT_REG_RSP].rrx;
	uint64_t virtualParamAddress = virtualStackAddress + 0x08 + 0x20 + (paramIndex * 0x08);

	uint64_t stackValue = 0;
	if (!CopyFromVirtualMemory (cpu, virtualParamAddress, sizeof (uint64_t), stackValue)) {
		return T ();
	}

	switch (sizeof (T)) {
	case 1: //byte
		return T (stackValue & 0xff);
	case 2: //word
		return T (stackValue & 0xffff);
	case 4: //dword
		return T (stackValue & 0xffffffff);
	case 8: //qword
		return T (stackValue);
	default:
		break;
	}

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

template <class T>
void ImportHandlerWithState<T>::ReadState (std::shared_ptr<ImportState> state) {
	mState = std::static_pointer_cast<T>(state->Read(mStateID));
}

template <class T>
bool ImportHandlerWithState<T>::WriteState (std::shared_ptr<ImportState> state) {
	if (mState == nullptr) {
		return false;
	}

	if (!state->Write (mStateID, mState)) {
		return false;
	}

	return true;
}

template <class T>
uint64_t ImportHandlerWithState<T>::GetVirtualAddressOfStateItem (uint64_t itemOffset) {
	if (mState == nullptr) {
		return 0;
	}

	std::tuple<uint64_t, uint32_t> extents = mState->GetExtents(mStateID);
	uint64_t virtualAddress = std::get<0>(extents);
	uint32_t size = std::get<1>(extents);
	if (virtualAddress == 0 || size == 0) {
		return 0;
	}

	return virtualAddress + itemOffset;
}
