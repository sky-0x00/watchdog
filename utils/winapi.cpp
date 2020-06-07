#include "winapi.h"

using namespace Winapi;

//---------------------------------------------------------------------------------------------------------------------------------------------
HANDLE StdHandle::Get(
	_in DWORD Type
) {
	return ::GetStdHandle(Type);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ HANDLE Console::Handle::Get(
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
bool Handle::Close(
	_in HANDLE hObject
) {
	return ::CloseHandle(hObject);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
Status LastError::Get(
) {
	return ::GetLastError();
}
void LastError::Set(
	_in Status Status
) {
	::SetLastError(Status);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
HANDLE Process::Open(
	_in DWORD ProcessId, _in DWORD DesiredAccess, _in bool IsInheritHandle /*= false*/
) {
	return ::OpenProcess(DesiredAccess, IsInheritHandle ? TRUE : FALSE, ProcessId);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
