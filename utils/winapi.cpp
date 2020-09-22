#include "winapi.h"

using namespace Winapi;

//---------------------------------------------------------------------------------------------------------------------------------------------
Handle::Value StdHandle::Get(
	_in DWORD Type
) {
	return ::GetStdHandle(Type);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ Console::Handle::Value Console::Handle::Get(
	_in Type Type
) {
	return StdHandle::Get(static_cast<DWORD>(Type));
}

bool Console::Attach(
	_in DWORD ProcessId
) {
	return FALSE != ::AttachConsole(ProcessId);
}
bool Console::Free(
) {
	return FALSE != ::FreeConsole();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
Console::Output::ScreenBufferInfo::ScreenBufferInfo(
) noexcept :
	CONSOLE_SCREEN_BUFFER_INFO {}
{}
/*static*/ bool Console::Output::ScreenBufferInfo::Get(
	_in HANDLE hConsoleOutput, _out ScreenBufferInfo &ScreenBufferInfo
) {
	return FALSE != ::GetConsoleScreenBufferInfo(hConsoleOutput, &ScreenBufferInfo);
}

/*static*/ bool Console::Output::TextAttribute::Set(
	_in HANDLE hConsoleOutput, _in WORD TextAttribute
) {
	return FALSE != ::SetConsoleTextAttribute(hConsoleOutput, TextAttribute);
}


bool Console::Output::Read(
	_in HANDLE hConsoleOutput, _out PCHAR_INFO Buffer, _in COORD BufferSize, _in COORD BufferCoord, _in _out PSMALL_RECT ReadRegion
) {
	return FALSE != ::ReadConsoleOutputW(hConsoleOutput, Buffer, BufferSize, BufferCoord, ReadRegion);
}
bool Console::Output::ReadCharacter(
	_in HANDLE hConsoleOutput, _out LPWSTR Character, _in DWORD Length, _in COORD ReadCoord, _out LPDWORD NumberOfCharsRead
) {
	return FALSE != ::ReadConsoleOutputCharacterW(hConsoleOutput, Character, Length, ReadCoord, NumberOfCharsRead);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
HANDLE Toolhelp32::CreateSnapshot(
	_in DWORD Flags, DWORD th32ProcessID /*= 0*/
) {
	return ::CreateToolhelp32Snapshot(Flags, th32ProcessID);
}

Toolhelp32::Process::Entry::Entry(
) :
	PROCESSENTRY32W {sizeof(PROCESSENTRY32W)}
{}

bool Toolhelp32::Process::First(
	_in HANDLE hSnapshot, _out Entry &Entry
) {
	return FALSE != ::Process32FirstW(hSnapshot, &Entry);
}
bool Toolhelp32::Process::Next(
	_in HANDLE hSnapshot, _out Entry &Entry
) {
	return FALSE != ::Process32NextW(hSnapshot, &Entry);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ bool Handle::Close(
	_in Handle::Value hValue
) noexcept {
	return FALSE != ::CloseHandle(hValue);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
Status::Value LastError::Get(
) {
	return ::GetLastError();
}
void LastError::Set(
	_in Status::Value Status
) {
	::SetLastError(Status);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
SystemShutdown::SystemShutdown(
	_in LPWSTR MachineName /*= nullptr*/
) :
	MachineName(MachineName)
{}

bool SystemShutdown::Initiate(
	_in DWORD TimeoutSecs, _in bool IsForceAppsClosed, _in bool IsRebootAfterShutdown, _in LPWSTR Message /*= nullptr*/
) {
	return FALSE != ::InitiateSystemShutdownW(
		MachineName, Message, TimeoutSecs, IsForceAppsClosed ? TRUE : FALSE, IsRebootAfterShutdown ? TRUE : FALSE
	);
}
bool SystemShutdown::Abort(
) {
	return FALSE != ::AbortSystemShutdownW(MachineName);
}

//-- Process::Current -------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ Process::Handle Process::Current::Get(
) {
	return ::GetCurrentProcess();
}
/*static*/ Process::Id Process::Current::GetId(
) {
	return ::GetCurrentProcessId();
}

//-- Process::StartupInfo,StartupInfoEx -------------------------------------------------------------------------------------------------------------------------------------------
Process::StartupInfo::StartupInfo(
) :
	STARTUPINFOW {sizeof(STARTUPINFOW)}
{}
Process::StartupInfoEx::StartupInfoEx(
) : 
	STARTUPINFOEXW {{sizeof(STARTUPINFOW)}}
{}

//-- Process::SecurityAttributes -------------------------------------------------------------------------------------------------------------------------------------------
Process::SecurityAttributes::SecurityAttributes(
	_in bool IsInheritHandle
) :
	SECURITY_ATTRIBUTES {sizeof(SECURITY_ATTRIBUTES)}
{
	bInheritHandle = IsInheritHandle ? TRUE : FALSE;
}

//-- Process -------------------------------------------------------------------------------------------------------------------------------------------
_set_lasterror(Process::Handle) Process::Open(
	_in Id Id, _in DWORD DesiredAccess, _in bool IsInheritHandle /*= false*/
) noexcept {
	return ::OpenProcess(DesiredAccess, IsInheritHandle ? TRUE : FALSE, Id);
}
_set_lasterror(bool) Process::Open(
	_out Handle &hProcess, _in Id Id, _in DWORD DesiredAccess, _in bool IsInheritHandle /*= false*/
) noexcept {
	hProcess = Open(Id, DesiredAccess, IsInheritHandle);
	return nullptr != hProcess;
}

_set_lasterror(bool) Process::OpenToken(
	_out Token::Handle &hToken, _in DWORD DesiredAccess, _in Handle hProcess /*= Current::Get()*/
) noexcept {
	return FALSE != ::OpenProcessToken(hProcess, DesiredAccess, &hToken);
}
_set_lasterror(Token::Handle) Process::OpenToken(
	_in DWORD DesiredAccess, _in Handle hProcess /*= Current::Get()*/
) noexcept {
	Token::Handle hToken /*= nullptr*/;
	return OpenToken(hToken, DesiredAccess, hProcess) ? hToken : nullptr;
}

_set_lasterror(bool) Process::Create(
	_out PROCESS_INFORMATION &Information,
	_in LPCWSTR               ApplicationName,
	_in LPWSTR                CommandLine,
	_in DWORD                 CreationFlags,
	_in StartupInfo			  &StartupInfo,
	_in bool                  IsInheritHandles /*= false*/,
	_in const SecurityAttributes *saProcess /*= nullptr*/,
	_in const SecurityAttributes *saThread /*= nullptr*/,
	_in LPCWSTR               CurrentDirectory /*= nullptr*/,
	_in LPVOID                Environment /*= nullptr*/
) noexcept {
	return ::CreateProcessW(
		ApplicationName, CommandLine, 
		reinterpret_cast<LPSECURITY_ATTRIBUTES>(const_cast<SecurityAttributes*>(saProcess)), reinterpret_cast<LPSECURITY_ATTRIBUTES>(const_cast<SecurityAttributes*>(saThread)),
		IsInheritHandles ? TRUE : FALSE, CreationFlags, Environment, CurrentDirectory,
		&StartupInfo, &Information
	);
}
_set_lasterror(bool) Process::Create(
	_out PROCESS_INFORMATION &Information,
	_in LPCWSTR               ApplicationName,
	_in LPWSTR                CommandLine,
	_in DWORD                 CreationFlags,				// также установить EXTENDED_STARTUPINFO_PRESENT
	_in StartupInfoEx		  &StartupInfoEx,
	_in bool                  IsInheritHandles /*= false*/,
	_in const SecurityAttributes *saProcess /*= nullptr*/,
	_in const SecurityAttributes *saThread /*= nullptr*/,
	_in LPCWSTR               CurrentDirectory /*= nullptr*/,
	_in LPVOID                Environment /*= nullptr*/
) noexcept {
	CreationFlags |= EXTENDED_STARTUPINFO_PRESENT;
	return ::CreateProcessW(
		ApplicationName, CommandLine,
		reinterpret_cast<LPSECURITY_ATTRIBUTES>(const_cast<SecurityAttributes*>(saProcess)), reinterpret_cast<LPSECURITY_ATTRIBUTES>(const_cast<SecurityAttributes*>(saThread)),
		IsInheritHandles ? TRUE : FALSE, CreationFlags, Environment, CurrentDirectory,
		&StartupInfoEx.StartupInfo, &Information
	);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TokenPrivilege::TokenPrivilege(
	_in LPCWSTR MachineName /*= nullptr*/
) :
	MachineName(MachineName)
{}

bool TokenPrivilege::LookupValue(
	_in LPCWSTR Name, _out LUID &Luid
) {
	return FALSE != LookupPrivilegeValueW(MachineName, Name, &Luid);
}
LUID TokenPrivilege::LookupValue(
	_in LPCWSTR Name
) noexcept(false) {
	LUID Luid {};
	if (LookupValue(Name, Luid))
		return Luid;
	throw exception();
}

/*static*/ bool TokenPrivilege::Adjust(
	_in HANDLE hToken, _in const TOKEN_PRIVILEGES &NewState
) {
	return FALSE != ::AdjustTokenPrivileges(hToken, FALSE, const_cast<PTOKEN_PRIVILEGES>(&NewState), 0, nullptr, nullptr);
}

/*static*/ bool TokenPrivilege::Adjust(
	_in HANDLE hToken, _in const TOKEN_PRIVILEGES &NewState, _out TOKEN_PRIVILEGES &PrevState, _in DWORD PrevState_Size, _out PDWORD pPrevState_SizeReturned /*= nullptr*/
) {
	DWORD PrevState_SizeReturned = 0;
	const auto IsOk = FALSE != ::AdjustTokenPrivileges(hToken, FALSE, const_cast<PTOKEN_PRIVILEGES>(&NewState), PrevState_Size, &PrevState, &PrevState_SizeReturned);
	if (pPrevState_SizeReturned)
		*pPrevState_SizeReturned = PrevState_SizeReturned;
	return IsOk;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
void Time::GetLocal(
	_out SYSTEMTIME &Time
) noexcept {
	::GetLocalTime(&Time);
}
SYSTEMTIME Time::GetLocal(
) noexcept {
	SYSTEMTIME Time;
	GetLocal(Time);
	return Time;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
DWORD File::GetFullPathName(
	_in LPCWSTR	FileName,
	_out LPWSTR	Buffer,
	_in DWORD	BufferLength,
	_out _optional LPWSTR *pFilePart /*= nullptr*/
) noexcept {
	return ::GetFullPathNameW(FileName, BufferLength, Buffer, pFilePart);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
