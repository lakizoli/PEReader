#include "stdafx.h"
#include "register.hpp"

Register::Register (const std::string& name) :
	mName (name)
{
	mValue.resize (8);
}
