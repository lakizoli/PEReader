#pragma once

class ASMLink {
public:
	enum class LinkTypes {
		Undefined,

		DirectCall,
		ImportCall,
		DelayedImportCall,
		DynamicCall,

		DirectJump,
		DynamicJump,
		ConditionalJump,
		ImportJump,
		DelayedImportJump,
		NullSubJump ///< Special jump to the tail function in IAT (raise nullptr exception!)
	};

private:
	LinkTypes mType; ///< The type of the link
	uint64_t mSourceAddress; ///< The source address of the link
	uint64_t mTargetAddress; ///< The target address of the link (or import IAT address, when type == ImportCall)

public:
	ASMLink (LinkTypes type, uint64_t sourceAddress, uint64_t targetAddress);
	~ASMLink () {}

public:
	LinkTypes GetType () const {
		return mType;
	}

	uint64_t GetSourceAddress () const {
		return mSourceAddress;
	}

	uint64_t GetTargetAddress () const {
		return mTargetAddress;
	}
};
