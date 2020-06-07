#pragma once

#include "utils\types.h"		// cstr_t

struct config {

	struct profile {
		enum /*class*/ type {
			xmrig = 1
		};
		struct details {
			cstr_t process_image_filename;
		};

		//inline profile(
		//	_in type type, _in const details &details
		//) :
		//	type(type), details(details)
		//{}
		//inline profile& operator =(
		//	_in const profile &profile
		//) {
		//	type = profile.type;
		//	details = profile.details;
		//	return *this;
		//}

		type type;
		details details;
	};

	static constexpr profile profile_s[] {
		{ profile::xmrig, { L"xmrig.exe" } }
	};
};