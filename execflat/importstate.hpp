#pragma once

#include <x86cpu.h>

#define DECLARE_IMPORT_STATE_ITEM(cls)							\
	static std::shared_ptr<ImportStateItem> __creator () {		\
		return std::shared_ptr<ImportStateItem> (new cls ());	\
	}															\
	static uint32_t __register_creator_result;

#define IMPLEMENT_IMPORT_STATE_ITEM(cls, id)		\
	uint32_t cls::__register_creator_result = ImportStateItem::RegisterCreator (id, cls::__creator, sizeof (cls));

class ImportStateItem {
public:
	typedef std::shared_ptr<ImportStateItem> (*creator) ();

	struct CreatorItem {
		creator creator;
		uint64_t virtualAddress;
		uint32_t size;
	};

protected:
	ImportStateItem () = default;
	static std::map<std::string, CreatorItem>& GetCreatorMap ();

public:
	virtual ~ImportStateItem () = default;
	static uint32_t RegisterCreator (const std::string& id, creator creator, uint32_t size);

	static std::shared_ptr<ImportStateItem> Create (const std::string& id);
	static std::tuple<uint64_t, uint32_t> GetExtents (const std::string& id);

	static void WalkRegistrations (std::function<void (CreatorItem&)> callback);
};

class ImportState {
	BX_CPU_C& mCpu;
	uint64_t mStateBase;

	static bool CopyFromVirtualMemory (BX_CPU_C& cpu, uint64_t virtualAddress, uint32_t size, std::shared_ptr<ImportStateItem> destination);
	static bool CopyToVirtualMemory (BX_CPU_C& cpu, const std::shared_ptr<ImportStateItem> source, uint64_t virtualAddress, uint32_t size);

public:
	ImportState (BX_CPU_C& cpu, uint64_t stateBase);

	std::shared_ptr<ImportStateItem> Read (const std::string& id);
	bool Write (const std::string& id, std::shared_ptr<ImportStateItem> stateItem);
};
