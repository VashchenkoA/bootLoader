#pragma once

#ifdef BOOTREADER_EXPORTS
#define BOOTREADER_API __declspec(dllexport)
#else
#define BOOTREADER_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>
#include <stdio.h>

#pragma pack(push, 1)

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
    DWORD  clustersPerMFT;
    UINT32  clustersPerIndex;
    UINT64  volumeSerialNumber;
} BOOT_NTFS;

#pragma pack(pop)

extern "C++" BOOTREADER_API bool bootInfo(std::string fileNameFormated, BOOT_NTFS * pBootRecord);
extern "C++" BOOTREADER_API void PrintBootSectInfo(BOOT_NTFS pBootRecord);
