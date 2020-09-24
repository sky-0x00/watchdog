#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include "types.h"		// macros-only

#ifdef GetFullPathName
	#undef GetFullPathName
#endif

namespace Winapi {
	struct Handle {
		typedef HANDLE Value;
		static _set_lasterror(bool) Close(_in Handle::Value hValue) noexcept;
	};
	
	struct Status {
		typedef LSTATUS Value;
		static constexpr Value Success = ERROR_SUCCESS;
		static constexpr Value NoMoreFiles = ERROR_NO_MORE_FILES;
	};	

	namespace StdHandle {
		Handle::Value Get(_in DWORD Type);
	}
	namespace Console {
		struct Handle {
			enum /*class*/ Type {
				Input = STD_INPUT_HANDLE,
				Output = STD_OUTPUT_HANDLE,
				Error = STD_ERROR_HANDLE
			};
			typedef Winapi::Handle::Value Value;
			static Value Get(_in Type Type);
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
	namespace LastError {
		Status::Value Get();
		void Set(_in Status::Value Status);
	}
	
	namespace Token {
		typedef Handle::Value Handle;
	}
	
	namespace Process {

		typedef Handle::Value Handle;
		typedef DWORD Id;

		struct Current {
			static Id GetId();
			static Handle Get();
		};

		struct StartupInfo: STARTUPINFOW {
			StartupInfo();
		};
		struct StartupInfoEx: STARTUPINFOEXW {
			StartupInfoEx();
		};

		struct SecurityAttributes: SECURITY_ATTRIBUTES {
			SecurityAttributes(_in bool IsInheritHandle);
		};
		
		struct CreateInfo: PROCESS_INFORMATION {
			CreateInfo();
		};

		_set_lasterror(bool) Create(
			_out CreateInfo&			CreateInfo,
			_in cstr_t					ApplicationName,
			_in cstr_t					CommandLine,
			_in DWORD					CreationFlags,
			_in const StartupInfo&		StartupInfo,			
			_in bool					IsInheritHandles = false,
			_in const SecurityAttributes* saProcess = nullptr,
			_in const SecurityAttributes* saThread = nullptr,
			_in cstr_t					CurrentDirectory = nullptr,
			_in pvoid_t					Environment = nullptr
		) noexcept;
		_set_lasterror(bool) CreateEx(
			_out CreateInfo&			CreateInfo,
			_in cstr_t					ApplicationName,
			_in cstr_t					CommandLine,
			_in DWORD					CreationFlags,				// также установить EXTENDED_STARTUPINFO_PRESENT
			_in const StartupInfoEx&	StartupInfoEx,
			_in bool					IsInheritHandles = false,
			_in const SecurityAttributes* saProcess = nullptr,
			_in const SecurityAttributes* saThread = nullptr,
			_in cstr_t					CurrentDirectory = nullptr,
			_in pvoid_t					Environment = nullptr
		) noexcept;

		_set_lasterror(bool) Open(_out Handle &hProcess, _in Id Id, _in DWORD DesiredAccess, _in bool IsInheritHandle = false) noexcept;
		_set_lasterror(Handle) Open(_in Id Id, _in DWORD DesiredAccess, _in bool IsInheritHandle = false) noexcept;

		_set_lasterror(bool) OpenToken(_out Token::Handle &hToken, _in DWORD DesiredAccess, _in Handle hProcess = Current::Get()) noexcept;				// обертка
		_set_lasterror(Token::Handle) OpenToken(_in DWORD DesiredAccess, _in Handle hProcess = Current::Get()) noexcept;									// 2nd реализация

	}	// namespace Process

	struct SystemShutdown {
		SystemShutdown(_in LPWSTR MachineName = nullptr);
	//public:
		bool Initiate(_in DWORD TimeoutSecs, _in bool IsForceAppsClosed, _in bool IsRebootAfterShutdown, _in LPWSTR Message = nullptr);
		bool Abort();
	//public:
		const LPWSTR MachineName;
	};

	struct TokenPrivilege {
		TokenPrivilege(_in LPCWSTR MachineName = nullptr);
	//public:
		bool LookupValue(_in LPCWSTR Name, _out LUID &Luid);
		LUID LookupValue(_in LPCWSTR Name) noexcept(false);
		
		// Get(): TODO - см. GetTokenInformation(TokenPrivileges)
		// Adjust(DisableAllPrivileges = TRUE): TODO
		// static bool Adjust__DAP_T(_in HANDLE hToken, _in const TOKEN_PRIVILEGES &NewState);
		// Adjust(DisableAllPrivileges = FALSE):
		static bool Adjust(_in HANDLE hToken, _in const TOKEN_PRIVILEGES &NewState);
		static bool Adjust(_in HANDLE hToken, _in const TOKEN_PRIVILEGES &NewState, _out TOKEN_PRIVILEGES &PrevState, _in DWORD PrevState_Size, _out PDWORD pPrevState_SizeReturned = nullptr);
	//public:
		const LPCWSTR MachineName;
	};

	namespace Time {
		void GetLocal(_out SYSTEMTIME &Time) noexcept;
		SYSTEMTIME GetLocal() noexcept;
	}

	namespace File {
		
		DWORD GetFullPathName(
			_in LPCWSTR	FileName,
			_out LPWSTR	Buffer,
			_in DWORD	BufferLength,
			_out _optional LPWSTR *pFilePart = nullptr
		) noexcept;

	}	// namespace File
	

}	// namespace Winapi
