// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <XInput.h>
#include <map>
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")



void MyThreadFunction()
{
   
    HMODULE exeBase = GetModuleHandleA("MZZXLC.exe");
    if (exeBase == NULL) {
        return;
    }

    uint8_t* exeBasePtr = (uint8_t*)exeBase;   
    
    //rank
    uint8_t* pMMZ1Rank  = exeBasePtr + 0x2511A49;
    uint8_t* pMMZ2Rank  = exeBasePtr + 0x251A2B5;
    uint8_t* pMMZ3Rank  = exeBasePtr + 0x251DE35;
    uint8_t* pMMZ4Rank  = exeBasePtr + 0x2521665;

    uint8_t& nMMZ1RankValue = *pMMZ1Rank;
    uint8_t& nMMZ2RankValue = *pMMZ2Rank;
    uint8_t& nMMZ3RankValue = *pMMZ3Rank;
    uint8_t& nMMZ4RankValue = *pMMZ4Rank;


    bool x = true;
    while (x)
    {
        //mmz1 checks for unlocked weapons and chips
        nMMZ1RankValue = 6;
        nMMZ2RankValue = 6;
        nMMZ3RankValue = 6;
        nMMZ4RankValue = 6;

        Sleep(100);
    }
    MessageBox(NULL, L"Thread terminated", NULL, MB_ICONEXCLAMATION);
}

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT void mod_open() {
    HMODULE exeBase = GetModuleHandleA("MZZXLC.exe");
    if (exeBase == NULL) {
        return;
    }
    std::thread myThread(MyThreadFunction);
    myThread.detach();
    return;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

