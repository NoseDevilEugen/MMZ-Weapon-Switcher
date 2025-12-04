// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <thread>
#include <windows.h>
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

enum ZeroElement
{
    eNone,
    eElec,
    eFire,
    eIce,
};

enum ZeroWeapon
{
    eBuster,
    eSaber,
    eRod,
    eShield,
};

void CheckAndSetZX(uint8_t& nOldValue, uint8_t nNewValue, std::map<uint8_t, bool> arrFlags, uint8_t& nVSpeed)
{
    if (nOldValue == nNewValue)
        return;

    if (arrFlags[nNewValue] == false)
        return;

    nOldValue = nNewValue;
    nVSpeed = 1;

}

//set flags for unlocked chips \ weapons for MMZ games
void SetFlags(std::map<uint8_t, bool>& arrFlags, uint8_t* pAddress)
{
    arrFlags[eBuster] = *pAddress >> eBuster & 1;//also no-chip
    arrFlags[eSaber] = *pAddress >> eSaber & 1;//also Elec chip
    arrFlags[eRod] = *pAddress >> eRod & 1;//also Fire chip
    arrFlags[eShield] = *pAddress >> eShield & 1;//also Ice chip
}

//set flags for unlocked bodies (MMZ3, elemental only)
void SetZ3Flags(std::map<uint8_t, bool>& arrFlags, uint8_t* pAddress)
{
    arrFlags[eNone] = *pAddress >> eNone & 1;
    arrFlags[eElec] = *pAddress >> eElec+2 & 1;
    arrFlags[eFire] = *pAddress >> eFire+2 & 1;
    arrFlags[eIce] = *pAddress >> eIce+2 & 1;
}

//change chip if unlocked (MMZ1\MMZ2\MMZ3)
void MMZSetChipValue(uint8_t nNewValue, uint8_t& nChipValue, std::map<uint8_t, bool> arrFlags)
{
    if (arrFlags[nNewValue] == true)
    {
        nChipValue = nNewValue;
    }
}

//change body value according to elemental chip value
uint8_t MMZSetBodyValue(uint8_t& nChipValue)
{
    if (nChipValue == 0)
        return 0;
    else
        return nChipValue + 2;
}

//switch to next available form (MMZX)
void IncreaseValueZX(uint8_t& nOldValue, std::map<uint8_t, bool> arrFlags, uint8_t& nVSpeed)
{
    if (nOldValue < 7)
    {
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

//switch to previous available form (MMZX)
void DecreaseValueZX(uint8_t& nOldValue, std::map<uint8_t, bool> arrFlags, uint8_t& nVSpeed)
{

    if (nOldValue > 0)
    {
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

//next subweapon in MMZ4 (does not work if weapon stolen)
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

//previous subweapon in MMZ4 (does not work if weapon stolen)
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

//next available subweapon in MMZ1\MMZ2\MMZ3
void IncreaseValue(uint8_t& nOldValue, uint8_t& nMainValue, std::map<uint8_t, bool> arrFlags)
{
    if (nOldValue == nMainValue)
        return;

    if (nOldValue == 3)
    {
        nOldValue = 0;
        while (arrFlags[nOldValue] != true || nOldValue == nMainValue)
        {
            nOldValue++;
        }
    }
    else
    {
        nOldValue++;
        while (arrFlags[nOldValue] != true || nOldValue == nMainValue)
        {
            nOldValue++;
            if (nOldValue > 3)
                nOldValue = 0;
        }

    }
}

//previous available subweapon in MMZ1\MMZ2\MMZ3
void DecreaseValue(uint8_t& nOldValue, uint8_t& nMainValue, std::map<uint8_t, bool> arrFlags)
{
    if (nOldValue == nMainValue)
        return;

    if (nOldValue == 0)
    {
        nOldValue = 3;
        while (arrFlags[nOldValue] != true || nOldValue == nMainValue)
        {
            nOldValue--;
        }
    }
    else
    {
        nOldValue--;
        while (arrFlags[nOldValue] != true || nOldValue == nMainValue)
        {
            nOldValue--;
            if (nOldValue < 0)
                nOldValue = 3;
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
    
    //MMZ1
    uint8_t* pMMZ1Sub = exeBasePtr + 0x252236B;
    uint8_t* pMMZ1Chip = exeBasePtr + 0x252237D;
    uint8_t* pMMZ1Main = exeBasePtr + 0x252236A;

    uint8_t& nMMZ1SubValue = *pMMZ1Sub;
    uint8_t& nMMZ1ChipValue = *pMMZ1Chip;
    uint8_t& nMMZ1MainValue = *pMMZ1Main;

    uint8_t* pMMZ1Weapons = exeBasePtr + 0x2522368;
    std::map<uint8_t, bool> arrMMZ1Weapons;

    uint8_t* pMMZ1Chips = exeBasePtr + 0x2522369;
    std::map<uint8_t, bool> arrMMZ1Chips;

    //MMZ2
    uint8_t* pMMZ2Sub = exeBasePtr + 0x2529CA1;
    uint8_t* pMMZ2Chip = exeBasePtr + 0x2529CA3;
    uint8_t* pMMZ2Main = exeBasePtr + 0x2529CA0;

    uint8_t& nMMZ2SubValue = *pMMZ2Sub;
    uint8_t& nMMZ2ChipValue = *pMMZ2Chip;
    uint8_t& nMMZ2MainValue = *pMMZ2Main;

    uint8_t* pMMZ2Weapons = exeBasePtr + 0x2529C9E;
    std::map<uint8_t, bool> arrMMZ2Weapons;

    uint8_t* pMMZ2Chips = exeBasePtr + 0x2529C9F;
    std::map<uint8_t, bool> arrMMZ2Chips;

    //MMZ3
    uint8_t* pMMZ3Sub = exeBasePtr + 0x2535A45;
    uint8_t* pMMZ3Chip = exeBasePtr + 0x2535A46;
    uint8_t* pMMZ3Body = exeBasePtr + 0x2535A48;
    uint8_t* pMMZ3Main = exeBasePtr + 0x2535A44;

    uint8_t* pMMZ3Weapons = exeBasePtr + 0x2535A4E;
    std::map<uint8_t, bool> arrMMZ3Weapons;

    uint8_t* pMMZ3Chips = exeBasePtr + 0x2535A50;
    std::map<uint8_t, bool> arrMMZ3Chips;

    uint8_t& nMMZ3SubValue = *pMMZ3Sub;
    uint8_t& nMMZ3ChipValue = *pMMZ3Chip;
    uint8_t& nMMZ3BodyValue = *pMMZ3Body;
    uint8_t& nMMZ3MainValue = *pMMZ3Main;

    //MMZX
    uint8_t* pMMZXBody = exeBasePtr + 0x28B5E70;
    
    uint8_t& nMMZXBodyValue = *pMMZXBody;

    std::map<uint8_t, bool> arrFlags;
    arrFlags[eHuman] = true;//always true

    uint8_t* pMMZXBodyX = exeBasePtr + 0x28B13A3;

    uint8_t* pMMZXBodyZX = exeBasePtr + 0x28B13A4;

    uint8_t* pMMZXBodyO = exeBasePtr + 0x28B13A6;

    uint8_t* pMMZXReset = exeBasePtr + 0x28B5C99;
    uint8_t& pMMZXResetValue = *pMMZXReset;

    //MMZ4
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
        //mmz1 checks for unlocked weapons and chips
        SetFlags(arrMMZ1Weapons, pMMZ1Weapons);
        SetFlags(arrMMZ1Chips, pMMZ1Chips);

        //mmz2 checks for unlocked weapons and chips
        SetFlags(arrMMZ2Weapons, pMMZ2Weapons);
        SetFlags(arrMMZ2Chips, pMMZ2Chips);

        //mmz3 checks for unlocked weapons and chips
        SetFlags(arrMMZ3Weapons, pMMZ3Weapons);
        SetZ3Flags(arrMMZ3Chips, pMMZ3Chips);

        //MMZX checks for unlocked forms
        arrFlags[eX] = *pMMZXBodyX >>7 & 1;
        arrFlags[eZX] = *pMMZXBodyZX >> 0 & 1;
        arrFlags[eHX] = *pMMZXBodyZX >> 1 & 1;
        arrFlags[eFX] = *pMMZXBodyZX >> 5 & 1;
        arrFlags[ePX] = *pMMZXBodyZX >> 7 & 1;
        arrFlags[eLX] = *pMMZXBodyZX >> 3 & 1;
        arrFlags[eOX] = *pMMZXBodyO >> 1 & 1;

        exeBasePtr = (uint8_t*)exeBase;
       /* XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        if (XInputGetState(0, &state) == ERROR_SUCCESS)
        {*/
            //previous subweapon / form
        if (!(GetAsyncKeyState(0x51) & 0x8000))
        {
            ltPressed = false;
        }
        if (GetAsyncKeyState(0x51) & 0x8000
            && ltPressed == false)

        {
            ltPressed = true;
            DecreaseValue(nMMZ1SubValue, nMMZ1MainValue, arrMMZ1Weapons);
            DecreaseValue(nMMZ2SubValue, nMMZ2MainValue, arrMMZ2Weapons);
            DecreaseValue(nMMZ3SubValue, nMMZ3MainValue, arrMMZ3Weapons);
            DecreaseValueZX(nMMZXBodyValue, arrFlags, pMMZXResetValue);
            DecreaseValueZ4(nMMZ4SubValue, nMMZ4MainValue);
        }

        //next subweapon / form
        if (!(GetAsyncKeyState(0x45) & 0x8000))
        {
            rtPressed = false;
        }
        if (GetAsyncKeyState(0x45) & 0x8000
            && rtPressed == false)
        {
            rtPressed = true;
            IncreaseValue(nMMZ1SubValue, nMMZ1MainValue, arrMMZ1Weapons);
            IncreaseValue(nMMZ2SubValue, nMMZ2MainValue, arrMMZ2Weapons);
            IncreaseValue(nMMZ3SubValue, nMMZ3MainValue, arrMMZ3Weapons);
            IncreaseValueZX(nMMZXBodyValue, arrFlags, pMMZXResetValue);
            IncreaseValueZ4(nMMZ4SubValue, nMMZ4MainValue);
        }

        //set element to None (right stick up)
        if (GetAsyncKeyState(0x31) & 0x8000)
        {
            MMZSetChipValue(eNone, nMMZ1ChipValue, arrMMZ1Chips);
            MMZSetChipValue(eNone, nMMZ2ChipValue, arrMMZ2Chips);
            MMZSetChipValue(eNone, nMMZ3ChipValue, arrMMZ3Chips);
            nMMZ3BodyValue = MMZSetBodyValue(nMMZ3ChipValue);
            CheckAndSetZX(nMMZXBodyValue, eHuman, arrFlags, pMMZXResetValue);//1 - hu
        }

        //set element to Fire (right stick down)
        if (GetAsyncKeyState(0x33) & 0x8000)
        {
            MMZSetChipValue(eFire, nMMZ1ChipValue, arrMMZ1Chips);
            MMZSetChipValue(eFire, nMMZ2ChipValue, arrMMZ2Chips);
            MMZSetChipValue(eFire, nMMZ3ChipValue, arrMMZ3Chips);
            nMMZ3BodyValue = MMZSetBodyValue(nMMZ3ChipValue);
            CheckAndSetZX(nMMZXBodyValue, eZX, arrFlags, pMMZXResetValue);//3 - zx
        }

        //set element to Elec (right stick right)
        if (GetAsyncKeyState(0x32) & 0x8000)
        {
            MMZSetChipValue(eElec, nMMZ1ChipValue, arrMMZ1Chips);
            MMZSetChipValue(eElec, nMMZ2ChipValue, arrMMZ2Chips);
            MMZSetChipValue(eElec, nMMZ3ChipValue, arrMMZ3Chips);
            nMMZ3BodyValue = MMZSetBodyValue(nMMZ3ChipValue);
            CheckAndSetZX(nMMZXBodyValue, eX, arrFlags, pMMZXResetValue);//2 - x
        }

        //set element to Ice (right stick left)
        if (GetAsyncKeyState(0x34) & 0x8000)
        {
            MMZSetChipValue(eIce, nMMZ1ChipValue, arrMMZ1Chips);
            MMZSetChipValue(eIce, nMMZ2ChipValue, arrMMZ2Chips);
            MMZSetChipValue(eIce, nMMZ3ChipValue, arrMMZ3Chips);
            nMMZ3BodyValue = MMZSetBodyValue(nMMZ3ChipValue);
            CheckAndSetZX(nMMZXBodyValue, eHX, arrFlags, pMMZXResetValue);//4 - hx
        }

        if (GetAsyncKeyState(0x35) & 0x8000)
        {
            CheckAndSetZX(nMMZXBodyValue, eFX, arrFlags, pMMZXResetValue);//5 - fx
        }

        if (GetAsyncKeyState(0x36) & 0x8000)
        {
            CheckAndSetZX(nMMZXBodyValue, eLX, arrFlags, pMMZXResetValue);//6 - lx
        }
        if (GetAsyncKeyState(0x37) & 0x8000)
        {
            CheckAndSetZX(nMMZXBodyValue, ePX, arrFlags, pMMZXResetValue);//7 - px
        }
        if (GetAsyncKeyState(0x38) & 0x8000)
        {
            CheckAndSetZX(nMMZXBodyValue, eOX, arrFlags, pMMZXResetValue);//8 - 0x
        }
        //}
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

