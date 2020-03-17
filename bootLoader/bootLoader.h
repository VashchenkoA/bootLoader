#pragma once

//Making DLL exportable and importable
#ifdef BOOTLOADER_EXPORTS
#define BOOTLOADER_API __declspec(dllexport)
#else
#define BOOTLOADER_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

//Making 1 byte alignment
#pragma pack(push, 1)

//Create structure of NTFS $Boot header
typedef struct _BOOT_NTFS
{
    BYTE    jump[3];
    BYTE    name[8];
    UINT16  sec_size;
    UINT16  secs_cluster;
    BYTE    zeros_0[6];
    UINT16  media_desc;
    BYTE    zeros_1;
    UINT16  secs_track;
    UINT16  num_heads;
    BYTE    zeros_2[8];
    UINT32  defaultValue;
    UINT64  num_secs;
    UINT64  LCNofMFT;
    UINT64  LCNofMFTMirr;
    DWORD   clustersPerMFT;
    UINT32  clustersPerIndex;
    UINT64  volumeSerialNumber;
} BOOT_NTFS;

//Back to standard alignment
#pragma pack(pop)

//External linkage of DLL functions
extern "C++" BOOTLOADER_API bool bootInfo(const WCHAR *fileNameFormated, BOOT_NTFS *pBootRecord);
extern "C++" BOOTLOADER_API void PrintBootSectInfo(BOOT_NTFS pBootRecord);
