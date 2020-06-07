#pragma once

#include "utils\types.h"

struct config {

	struct profile {
		enum /*class*/ type {
			xmrig = 1
		};
		struct details {
			cstr_t process_image_filename;
		};
		type type;
		details details;
	};

	static constexpr profile profile_s[] {
		{ profile::xmrig, { L"xmrig.exe" } }
	};
};