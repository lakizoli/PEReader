#pragma once

#include "importhandler.hpp"
#include <Windows.h>

class ApiCrt_InitTerm : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitTerm);

	uint8_t* mFirstParam;
	uint8_t* mSecondParam;

	ApiCrt_InitTerm () : mFirstParam (nullptr), mSecondParam (nullptr) {}

public:
	void ReadParameters (BX_CPU_C& cpu) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu) override;
};

class ApiCrt_InitTermE : public ImportHandler {
	DECLARE_IMPORT_HANDLER (ApiCrt_InitTermE);

	uint8_t* mFirstParam;
	uint8_t* mSecondParam;
	uint32_t mResultCode;

	ApiCrt_InitTermE () : mFirstParam (nullptr), mSecondParam (nullptr), mResultCode (0) {}

public:
	void ReadParameters (BX_CPU_C& cpu) override;
	void Call () override;
	bool WriteResults (BX_CPU_C& cpu) override;
};
