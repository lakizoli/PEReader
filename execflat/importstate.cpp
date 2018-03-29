#include "stdafx.h"
#include "importstate.hpp"

std::map<std::string, ImportStateItem::CreatorItem>& ImportStateItem::GetCreatorMap () {
	static std::map<std::string, CreatorItem> creatorMap;
	return creatorMap;
}

uint32_t ImportStateItem::RegisterCreator (const std::string& id, creator creator, uint32_t size) {
	GetCreatorMap ().emplace (id, CreatorItem {
		creator,
		0,
		size
	});

	return 42;
}

std::shared_ptr<ImportStateItem> ImportStateItem::Create (const std::string& id) {
	const auto& creatorMap = GetCreatorMap ();
	auto it = creatorMap.find (id);
	if (it == creatorMap.end ()) {
		return nullptr;
	}
	return it->second.creator ();
}

std::tuple<uint64_t, uint32_t> ImportStateItem::GetExtents (const std::string& id) {
	const auto& creatorMap = GetCreatorMap ();
	auto it = creatorMap.find (id);
	if (it == creatorMap.end ()) {
		return std::make_tuple<uint64_t, uint32_t> (0, 0);
	}
	return std::make_tuple (it->second.virtualAddress, it->second.size);
}

void ImportStateItem::WalkRegistrations (std::function<void (CreatorItem&)> callback) {
	auto& creatorMap = GetCreatorMap ();
	for (auto& it : creatorMap) {
		callback (it.second);
	}
}

bool ImportState::CopyFromVirtualMemory (BX_CPU_C& cpu, uint64_t virtualAddress, uint32_t size, std::shared_ptr<ImportStateItem> destination) {
	return cpu.access_read_linear (cpu.mem, virtualAddress, size, 3, BX_READ, 0, (void*) destination.get ()) == 0;
}

bool ImportState::CopyToVirtualMemory (BX_CPU_C& cpu, const std::shared_ptr<ImportStateItem> source, uint64_t virtualAddress, uint32_t size) {
	return cpu.access_write_linear (cpu.mem, virtualAddress, size, 3, 0, (void*) source.get ()) == 0;
}

ImportState::ImportState (BX_CPU_C& cpu, uint64_t stateBase) :
	mCpu (cpu),
	mStateBase (stateBase)
{
	uint64_t offset = 0;
	ImportStateItem::WalkRegistrations ([&] (ImportStateItem::CreatorItem& creatorItem) {
		creatorItem.virtualAddress = mStateBase + offset;

		std::shared_ptr<ImportStateItem> item = creatorItem.creator ();
		if (!CopyToVirtualMemory (mCpu, item, creatorItem.virtualAddress, creatorItem.size)) {
			//TODO: ... throw error...
		}

		offset += creatorItem.size;
	});
}

std::shared_ptr<ImportStateItem> ImportState::Read (const std::string& id) {
	std::tuple<uint64_t, uint32_t> extents = ImportStateItem::GetExtents (id);
	uint64_t virtualAddress = std::get<0> (extents);
	uint32_t size = std::get<1> (extents);
	if (virtualAddress == 0 || size == 0) {
		return nullptr;
	}

	std::shared_ptr<ImportStateItem> stateItem = ImportStateItem::Create (id);
	if (stateItem == nullptr) {
		return nullptr;
	}

	if (!CopyFromVirtualMemory (mCpu, virtualAddress, size, stateItem)) {
		return nullptr;
	}

	return stateItem;
}

bool ImportState::Write (const std::string& id, std::shared_ptr<ImportStateItem> stateItem) {
	std::tuple<uint64_t, uint32_t> extents = ImportStateItem::GetExtents (id);
	uint64_t virtualAddress = std::get<0> (extents);
	uint32_t size = std::get<1> (extents);
	if (virtualAddress == 0 || size == 0) {
		return false;
	}

	return CopyToVirtualMemory (mCpu, stateItem, virtualAddress, size);
}
