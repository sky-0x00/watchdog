#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include "types.h"		// macros-only

namespace Winapi {
	typedef LSTATUS Status;

	namespace StdHandle {
		HANDLE Get(_in DWORD Type);
	}
	namespace Console {
		struct Handle {
			enum /*class*/ Type {
				Input = STD_INPUT_HANDLE,
				Output = STD_OUTPUT_HANDLE,
				Error = STD_ERROR_HANDLE
			};
			static HANDLE Get(_in Type Type);
		};
		bool Attach(_in DWORD ProcessId);
		bool Free();
		
		namespace Output {
			struct ScreenBufferInfo : CONSOLE_SCREEN_BUFFER_INFO {
				ScreenBufferInfo() noexcept;
				static bool Get(_in HANDLE hConsoleOutput, _out ScreenBufferInfo &ScreenBufferInfo);
			};
			struct TextAttribute {
				static bool Set(_in HANDLE hConsoleOutput, _in WORD TextAttribute);
			};
			bool Read(_in HANDLE hConsoleOutput, _out PCHAR_INFO Buffer, _in COORD BufferSize, _in COORD BufferCoord, _in _out PSMALL_RECT ReadRegion);
			bool ReadCharacter(_in HANDLE hConsoleOutput, _out LPWSTR Character, _in DWORD Length, _in COORD ReadCoord, _out LPDWORD NumberOfCharsRead);
		};
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

	struct Shutdown {
		Shutdown(_in LPWSTR MachineName = nullptr);
	//public:
		bool Initiate(_in DWORD TimeoutSecs, _in bool IsForceAppsClosed, _in bool IsRebootAfterShutdown, _in LPWSTR Message = nullptr);
		bool Abort();
	//public:
		const LPWSTR MachineName;
	};

}	// namespace Winapi
