#pragma once

#include "types.h"

class monitor {
public:
	monitor(_in bool is_waitall = true);
private:
	const bool _is_waitall;
};
