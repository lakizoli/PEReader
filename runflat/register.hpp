#pragma once

class Register {
	std::string mName; ///< The name of the register
	std::vector<uint8_t> mValue; ///< Little endian 64 bit register

public:
	Register (const std::string& name);
	~Register () = default;

public:
	void SetLB (uint8_t val) { mValue[0] = val; }
	uint8_t GetLB () const { return mValue[0]; }

	void SetHB (uint8_t val) { mValue[1] = val; }
	uint8_t GetHB () const { return mValue[1]; }

	void SetW (uint16_t val) { *(uint16_t*) &mValue[0] = val; }
	uint16_t GetW () const { return *(uint16_t*) &mValue[0]; }

	void SetDW (uint32_t val) { *(uint32_t*) &mValue[0] = val; }
	uint32_t GetDW () const { return *(uint32_t*) &mValue[0]; }

	void SetQW (uint64_t val) { *(uint64_t*) &mValue[0] = val; }
	uint64_t GetQW () const { return *(uint64_t*) &mValue[0]; }
};

