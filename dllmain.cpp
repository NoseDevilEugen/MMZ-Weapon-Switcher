// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <XInput.h>
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")

enum ZXForms
{
    eHuman,
    eX,
    eZX,
    eHX,
    eFX,
    ePX,
    eLX,
    eOX
};

void IncreaseValueZX(uint8_t& nOldValue, bool bFlags[])
{
    if (nOldValue < 7)
    {
        nOldValue++;
        while (bFlags[nOldValue] != true && nOldValue < 7)
        {
            nOldValue++;
            if (nOldValue > 7)
            {
                nOldValue = 0;
                break;
            }
        }
    }
}

void DecreaseValueZX(uint8_t& nOldValue, bool bFlags[])
{

    if (nOldValue > 0)
    {
        nOldValue--;
        while (bFlags[nOldValue] != true && nOldValue >0)
        {
            nOldValue--;
            if (nOldValue < 0)
            {
                nOldValue = 0;
                break;
            }
        }
    }
}

void IncreaseValueZ4(uint8_t& nOldValue)
{
    if (nOldValue == 2)
        nOldValue = 0;
    else
        nOldValue++;
}

void DecreaseValueZ4(uint8_t& nOldValue)
{
    if (nOldValue == 0)
        nOldValue = 2;
    else
        nOldValue--;
}

void IncreaseValue(uint8_t& nOldValue)
{
    if (nOldValue == 3)
        nOldValue = 0;
    else
        nOldValue++;
}

void DecreaseValue(uint8_t& nOldValue)
{
    if (nOldValue == 0)
        nOldValue = 3;
    else
        nOldValue--;
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

    uint8_t& nMMZ1SubValue = *pMMZ1Sub;
    uint8_t& nMMZ1ChipValue = *pMMZ1Chip;

    uint8_t* pMMZ2Sub = exeBasePtr + 0x2529CA1;
    uint8_t* pMMZ2Chip = exeBasePtr + 0x2529CA3;

    uint8_t& nMMZ2SubValue = *pMMZ2Sub;
    uint8_t& nMMZ2ChipValue = *pMMZ2Chip;

    uint8_t* pMMZ3Sub = exeBasePtr + 0x2535A45;
    uint8_t* pMMZ3Chip = exeBasePtr + 0x2535A46;
    uint8_t* pMMZ3Body = exeBasePtr + 0x2535A48;

    uint8_t& nMMZ3SubValue = *pMMZ3Sub;
    uint8_t& nMMZ3ChipValue = *pMMZ3Chip;
    uint8_t& nMMZ3BodyValue = *pMMZ3Body;

    uint8_t* pMMZXBody = exeBasePtr + 0x28B5E70;
    
    uint8_t& nMMZXBodyValue = *pMMZXBody;

    bool flags[8];
    flags[eHuman] = true;

    uint8_t* pMMZXBodyX = exeBasePtr + 0x28B13A3;
    flags[eX] = *(int*)pMMZXBodyX & 1;

    uint8_t* pMMZXBodyZX = exeBasePtr + 0x28B13A4;
    flags[eZX] = *(uint8_t*)pMMZXBodyZX >> 0 & 1;
    flags[eHX] = *(uint8_t*)pMMZXBodyZX >> 1 & 1;
    flags[eFX] = *(uint8_t*)pMMZXBodyZX >> 5 & 1;
    flags[ePX] = *(uint8_t*)pMMZXBodyZX >> 7 & 1;
    flags[eLX] = *(uint8_t*)pMMZXBodyZX >> 3 & 1;


    uint8_t* pMMZXBodyO = exeBasePtr + 0x28B13A6;
    flags[eOX] = (*(int*)pMMZXBodyO >> 1) & 1;


    
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
                DecreaseValueZX(nMMZXBodyValue, flags);
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
                IncreaseValueZX(nMMZXBodyValue, flags);
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

