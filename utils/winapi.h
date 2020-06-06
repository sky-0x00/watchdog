#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include "types.h"

//#ifndef DONOT_UNDEF_KNOWN_MACROS
//	#undef Process32First
//#endif

namespace Winapi {
	typedef LSTATUS Status;

	namespace StdHandle {
		HANDLE Get(_in DWORD Type);
	}
	namespace Console {
		struct ScreenBufferInfo : CONSOLE_SCREEN_BUFFER_INFO {
			static bool Get(_in HANDLE hConsoleOutput, _out ScreenBufferInfo &ScreenBufferInfo);
		};
		struct TextAttribute {
			static bool Set(_in HANDLE hConsoleOutput, _in WORD TextAttribute);
		};
		bool Attach(_in DWORD ProcessId);
		bool Free();
	}
	namespace Toolhelp32 {
		HANDLE CreateSnapshot(_in DWORD Flags, DWORD th32ProcessID = 0);
		namespace Process {
			struct Entry : PROCESSENTRY32W {
				Entry();
			};
			bool First(_in HANDLE hSnapshot, _out Entry &Entry);
			bool Next(_in HANDLE hSnapshot, _out Entry &Entry);
		}		
	}
	namespace Handle {
		bool Close(_in HANDLE hObject);
	}
	namespace LastError {
		Status Get();
		void Set(_in Status Status);
	}
	namespace Process {
		HANDLE Open(_in DWORD ProcessId, _in DWORD DesiredAccess, _in bool IsInheritHandle = false);
	}
}
