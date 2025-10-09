// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <XInput.h>
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")



void IncreaseValueZ4(int& nOldValue)
{
    switch (nOldValue)
    {
    case 0:
    {
        nOldValue = 1;
        break;
    }
    case 1:
    {
        nOldValue = 2;
        break;
    }
    case 2:
    {
        nOldValue = 0;
        break;
    }
   
    default:
    {
        nOldValue = 0;
        break;
    }
    }
}

void DecreaseValueZ4(int& nOldValue)
{
    switch (nOldValue)
    {
    case 0:
    {
        nOldValue = 2;
        break;
    }
    case 1:
    {
        nOldValue = 0;
        break;
    }
    case 2:
    {
        nOldValue = 1;
        break;
    }
   
    default:
    {
        nOldValue = 2;
        break;
    }
    }
}

void IncreaseValue(int& nOldValue)
{
    switch (nOldValue)
    {
    case 0:
    {
        nOldValue = 1;
        break;
    }
    case 1:
    {
        nOldValue = 2;
        break;
    }
    case 2:
    {
        nOldValue = 3;
        break;
    }
    case 3:
    {
        nOldValue = 0;
        break;
    }
    default:
    {
        nOldValue = 0;
        break;
    }
    }
}

void DecreaseValue(int& nOldValue)
{
    switch (nOldValue)
    {
    case 0:
    {
        nOldValue = 3;
        break;
    }
    case 1:
    {
        nOldValue = 0;
        break;
    }
    case 2:
    {
        nOldValue = 1;
        break;
    }
    case 3:
    {
        nOldValue = 2;
        break;
    }
    default:
    {
        nOldValue = 3;
        break;
    }
    }
}

void MyThreadFunction()
{
   
    HMODULE exeBase = GetModuleHandleA("MZZXLC.exe");
    if (exeBase == NULL) {
        return;
    }
    uint8_t* exeBasePtr = (uint8_t*)exeBase;   

    
    uint8_t* pMMZ1Sub = exeBasePtr + 0x252236B;
    uint8_t* pMMZ1Chip = exeBasePtr + 0x252237D;

    int& nMMZ1SubValue = *(int*)pMMZ1Sub;
    int& nMMZ1ChipValue = *(int*)pMMZ1Chip;

    uint8_t* pMMZ2Sub = exeBasePtr + 0x2529CA1;
    uint8_t* pMMZ2Chip = exeBasePtr + 0x2529CA3;

    int& nMMZ2SubValue = *(int*)pMMZ2Sub;
    int& nMMZ2ChipValue = *(int*)pMMZ2Chip;

    uint8_t* pMMZ3Sub = exeBasePtr + 0x2535A45;
    uint8_t* pMMZ3Chip = exeBasePtr + 0x2535A46;
    uint8_t* pMMZ3Body = exeBasePtr + 0x2535A48;

    int& nMMZ3SubValue = *(int*)pMMZ3Sub;
    int& nMMZ3ChipValue = *(int*)pMMZ3Chip;
    int& nMMZ3BodyValue = *(int*)pMMZ3Body;

   /* uint8_t* pMMZ4Sub = exeBasePtr + 0x2541235;
    uint8_t* pMMZ4Knuckle = exeBasePtr + 0x2541237;
    uint8_t* pMMZ4Toss = exeBasePtr + 0x254126E;

    int& nMMZ4SubValue = *(int*)pMMZ4Sub;
    int& nMMZ4Knuckle = *(int*)pMMZ4Knuckle;
    int& nMMZ4Toss = *(int*)pMMZ4Toss;*/
    
    const BYTE TRIGGER_THRESHOLD = 50; // Adjust as needed
    bool x = true;
    bool ltPressed = false;
    bool rtPressed = false;
    while (x)
    {
        exeBasePtr = (uint8_t*)exeBase;
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        if (XInputGetState(0, &state) == ERROR_SUCCESS)
        {
            //subweapon switch goes brrrrr
            if (state.Gamepad.bLeftTrigger < TRIGGER_THRESHOLD)
            {
                ltPressed = false;
            }
            if (state.Gamepad.bLeftTrigger >= TRIGGER_THRESHOLD
               && ltPressed == false)

            {
                ltPressed = true;
                DecreaseValue(nMMZ1SubValue);
                DecreaseValue(nMMZ2SubValue);
                DecreaseValue(nMMZ3SubValue);
                //DecreaseValueZ4(nMMZ4SubValue);
                //nMMZ4Knuckle = 0;
                //nMMZ4Toss = 2;
            }
            if (state.Gamepad.bRightTrigger < TRIGGER_THRESHOLD)
            {
                rtPressed = false;
            }
            if (state.Gamepad.bRightTrigger >= TRIGGER_THRESHOLD
                && rtPressed == false)
            {
                rtPressed = true;
                IncreaseValue(nMMZ1SubValue);
                IncreaseValue(nMMZ2SubValue);
                IncreaseValue(nMMZ3SubValue);
                //IncreaseValueZ4(nMMZ4SubValue);
                //nMMZ4Knuckle = 0;
                //nMMZ4Toss = 2;
            }
            //element switch goes brrrrrr
            if (state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            {
                nMMZ1ChipValue = 0;
                nMMZ2ChipValue = 0;
                nMMZ3ChipValue = 0;
                nMMZ3BodyValue = 0;
            }
            if (state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            {
                nMMZ1ChipValue = 2;
                nMMZ2ChipValue = 2;
                nMMZ3ChipValue = 2;
                nMMZ3BodyValue = 4;
            }

            if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            {
                nMMZ1ChipValue = 1;
                nMMZ2ChipValue = 1;
                nMMZ3ChipValue = 1;
                nMMZ3BodyValue = 3;
            }
            if (state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            {
                nMMZ1ChipValue = 3;
                nMMZ2ChipValue = 3;
                nMMZ3ChipValue = 3;
                nMMZ3BodyValue = 5;
            }
        }
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

