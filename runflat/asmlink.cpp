#include "stdafx.h"
#include "asmlink.hpp"

ASMLink::ASMLink (LinkTypes type, uint64_t sourceAddress, uint64_t targetAddress) :
	mType (type),
	mSourceAddress (sourceAddress),
	mTargetAddress (targetAddress)
{
}
