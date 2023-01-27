#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <string>

using namespace std;

namespace Utils {
    ULONG64 getPidByName(wstring name) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(PROCESSENTRY32W);
        if (!Process32FirstW(hSnapshot, &pe)) {
            return 0;
        }
        do {
            if (wcscmp(pe.szExeFile, name.c_str()) == 0) {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } while (Process32NextW(hSnapshot, &pe));
        CloseHandle(hSnapshot);
        return 0;
    }
}