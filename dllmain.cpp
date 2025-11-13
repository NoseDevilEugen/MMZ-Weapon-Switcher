// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <XInput.h>
#include <map>
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")

enum ZXForms
{
    eHuman,
    eX,
    eZX,
    eHX,
    eFX,
    eLX,
    ePX,
    eOX
};

void IncreaseValueZX(uint8_t& nOldValue, std::map<uint8_t, bool> arrFlags, uint8_t& nVSpeed)
{
    if (nOldValue < 7)
    {
        nVSpeed = 1;
        nOldValue++;
        while (arrFlags[nOldValue] != true)
        {
            nOldValue++;
            if (nOldValue > 7)
            {
                nOldValue = 0;
            }
        }
        nVSpeed = 1;
    }
    else if (nOldValue == 7)
    {
        nVSpeed = 1;
        nOldValue = 0;
        while (arrFlags[nOldValue] != true)
        {
            nOldValue++;
            if (nOldValue > 7)
            {
                nOldValue = 0;
            }
        }
        nVSpeed = 1;
    }
}

void DecreaseValueZX(uint8_t& nOldValue, std::map<uint8_t, bool> arrFlags, uint8_t& nVSpeed)
{

    if (nOldValue > 0)
    {
        nVSpeed = 1;
        nOldValue--;
        while (arrFlags[nOldValue] != true)
        {
            nOldValue--;
            if (nOldValue < 0)
            {
                nOldValue = 7;
            }
        }
        nVSpeed = 1;
    }
    else if (nOldValue == 0)
    {
        nVSpeed = 1;
        nOldValue = 7;
        while (arrFlags[nOldValue] != true)
        {
            nOldValue--;
            if (nOldValue < 0)
            {
                nOldValue = 7;
            }
        }
        nVSpeed = 1;
    }
}

void IncreaseValueZ4(uint8_t& nOldValue, uint8_t& nMainValue)
{
    if (nOldValue < 3)
    {
        if (nOldValue == 2)
            nOldValue = 0;
        else
            nOldValue++;

        if (nOldValue == nMainValue)
        {
            if (nOldValue >= 2)
                nOldValue = 0;
            else
                nOldValue++;
        }

    }
}

void DecreaseValueZ4(uint8_t& nOldValue, uint8_t& nMainValue)
{
    if (nOldValue < 3)
    {
        if (nOldValue == 0)
            nOldValue = 2;
        else
            nOldValue--;
        
        if (nOldValue == nMainValue)
        {
            if (nOldValue <= 0)
                nOldValue = 2;
            else
                nOldValue--;
        }
    }
}

void IncreaseValue(uint8_t& nOldValue, uint8_t& nMainValue)
{

    if (nOldValue == 3)
        nOldValue = 0;
    else
        nOldValue++;

    if (nOldValue == nMainValue)
    {
        if (nOldValue >= 3)
            nOldValue = 0;
        else
            nOldValue++;
    }

}

void DecreaseValue(uint8_t& nOldValue, uint8_t& nMainValue)
{
    if (nOldValue == 0)
        nOldValue = 3;
    else
        nOldValue--;
   
    if (nOldValue == nMainValue)
    {
        if (nOldValue <= 0)
            nOldValue = 3;
        else
            nOldValue--;
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
    uint8_t* pMMZ1Main = exeBasePtr + 0x252236A;

    uint8_t& nMMZ1SubValue = *pMMZ1Sub;
    uint8_t& nMMZ1ChipValue = *pMMZ1Chip;
    uint8_t& nMMZ1MainValue = *pMMZ1Main;

    uint8_t* pMMZ2Sub = exeBasePtr + 0x2529CA1;
    uint8_t* pMMZ2Chip = exeBasePtr + 0x2529CA3;
    uint8_t* pMMZ2Main = exeBasePtr + 0x2529CA0;

    uint8_t& nMMZ2SubValue = *pMMZ2Sub;
    uint8_t& nMMZ2ChipValue = *pMMZ2Chip;
    uint8_t& nMMZ2MainValue = *pMMZ2Main;

    uint8_t* pMMZ3Sub = exeBasePtr + 0x2535A45;
    uint8_t* pMMZ3Chip = exeBasePtr + 0x2535A46;
    uint8_t* pMMZ3Body = exeBasePtr + 0x2535A48;
    uint8_t* pMMZ3Main = exeBasePtr + 0x2535A44;

    uint8_t& nMMZ3SubValue = *pMMZ3Sub;
    uint8_t& nMMZ3ChipValue = *pMMZ3Chip;
    uint8_t& nMMZ3BodyValue = *pMMZ3Body;
    uint8_t& nMMZ3MainValue = *pMMZ3Main;

    uint8_t* pMMZXBody = exeBasePtr + 0x28B5E70;
    
    uint8_t& nMMZXBodyValue = *pMMZXBody;

    std::map<uint8_t, bool> arrFlags;
    arrFlags[eHuman] = true;

    uint8_t* pMMZXBodyX = exeBasePtr + 0x28B13A3;

    uint8_t* pMMZXBodyZX = exeBasePtr + 0x28B13A4;

    uint8_t* pMMZXBodyO = exeBasePtr + 0x28B13A6;

    uint8_t* pMMZXReset = exeBasePtr + 0x28B5C99;
    uint8_t& pMMZXResetValue = *pMMZXReset;

   
    
    uint8_t* pMMZ4Sub = exeBasePtr + 0x2541235;
    uint8_t* pMMZ4Knuckle = exeBasePtr + 0x2541237;
    uint8_t* pMMZ4Toss = exeBasePtr + 0x254126E;
    uint8_t* pMMZ4Main = exeBasePtr + 0x2541234;

    uint8_t& nMMZ4SubValue = *pMMZ4Sub;
    uint8_t& nMMZ4Knuckle = *pMMZ4Knuckle;
    uint8_t& nMMZ4Toss = *pMMZ4Toss;
    uint8_t& nMMZ4MainValue = *pMMZ4Main;
    
    const BYTE TRIGGER_THRESHOLD = 50; // Adjust as needed
    bool x = true;
    bool ltPressed = false;
    bool rtPressed = false;
    while (x)
    {
        arrFlags[eX] = *pMMZXBodyX >>7 & 1;
        arrFlags[eZX] = *pMMZXBodyZX >> 0 & 1;
        arrFlags[eHX] = *pMMZXBodyZX >> 1 & 1;
        arrFlags[eFX] = *pMMZXBodyZX >> 5 & 1;
        arrFlags[ePX] = *pMMZXBodyZX >> 7 & 1;
        arrFlags[eLX] = *pMMZXBodyZX >> 3 & 1;
        arrFlags[eOX] = *pMMZXBodyO >> 1 & 1;

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
                DecreaseValue(nMMZ1SubValue, nMMZ1MainValue);
                DecreaseValue(nMMZ2SubValue, nMMZ2MainValue);
                DecreaseValue(nMMZ3SubValue, nMMZ3MainValue);
                DecreaseValueZX(nMMZXBodyValue, arrFlags, pMMZXResetValue);
                DecreaseValueZ4(nMMZ4SubValue, nMMZ4MainValue);
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
                IncreaseValue(nMMZ1SubValue, nMMZ1MainValue);
                IncreaseValue(nMMZ2SubValue, nMMZ2MainValue);
                IncreaseValue(nMMZ3SubValue, nMMZ3MainValue);
                IncreaseValueZX(nMMZXBodyValue, arrFlags, pMMZXResetValue);
                IncreaseValueZ4(nMMZ4SubValue, nMMZ4MainValue);
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

