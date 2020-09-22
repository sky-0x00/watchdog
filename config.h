#pragma once

#include <map>
#include "utils\types.h"		// cstr_t, pid_t

namespace config {

	struct profile {
		
		//typedef unsigned count;

		enum /*class*/ type {
			xmrig = 1
		};
		enum /*class*/ details_traits {
			search = 1, information
		};

		template <details_traits> struct details;
		template <> struct details<details_traits::search> {
			cstr_t process_image_filename;
		};
		template <> struct details<details_traits::information> {
			pid_t process_id;
		};

		template <typename details> using map_t = std::map<type, details>;
		template <details_traits details_traits> using map_e = std::map<type, details<details_traits>>;

		static const map_e<details_traits::search> map;
	};	
};