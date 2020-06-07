#pragma once

#include "utils\types.h"

class system {
public:
	static bool reboot(_in unsigned timeout_sec = 0);
protected:
	struct privileges {
		static bool adjust();
	};
};
