#include <Windows.h>
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include "bootLoader.h"

using namespace std;

void PrintBootSectInfo(BOOT_NTFS pBootRecord)
{
    cout << "File system type: " << pBootRecord.name << endl;
    cout << "Sector size: " << pBootRecord.sec_size << endl;
    cout << "Sectors per cluster: " << pBootRecord.secs_cluster << endl;
    cout << "Volume descriptor: " << pBootRecord.media_desc << endl;
    cout << "Sectors per track: " << pBootRecord.secs_track << endl;
    cout << "Number of heads: " << pBootRecord.num_heads << endl;
    cout << "Number of sectors: " << pBootRecord.num_secs << endl;
    cout << "First cluster of MFT: " << pBootRecord.LCNofMFT << endl;
    cout << "First cluster of MFT mirror: " << pBootRecord.LCNofMFTMirr << endl;
    cout << "Clusters per MFT Record: " << pBootRecord.clustersPerMFT << endl;
    cout << "Clusters per Index Record: " << pBootRecord.clustersPerIndex << endl;
    cout << "Volume serial number: " << pBootRecord.volumeSerialNumber << endl;
}

bool bootInfo(const WCHAR *fileNameFormated, BOOT_NTFS* pBootRecord)
{
    //Open drive as file
    HANDLE fileHandle = CreateFileW(
        fileNameFormated,//convert string to LPCSTR
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        perror("Error: ");
    }

    //Positioning on zero offset
    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;

    //Setting a position
    unsigned long currentPosition = SetFilePointer(
        fileHandle,
        sectorOffset.LowPart,
        &sectorOffset.HighPart,
        FILE_BEGIN
    );

    if (currentPosition != sectorOffset.LowPart)
    {
        perror("Error: ");
    }

    //Memory allocation in stack
    BYTE dataBuffer[1024];

    //Result of reading bytes
    DWORD bytesRead;

    //Data reading
    bool readResult = ReadFile(
        fileHandle,
        dataBuffer,
        1024,
        &bytesRead,
        NULL
    );

    if (!readResult || bytesRead != 1024)
    {
        perror("Error: ");
    };

    //Initialize pointer
    pBootRecord = reinterpret_cast<BOOT_NTFS*>(dataBuffer);

    //Close file
    CloseHandle(fileHandle);

    return true;
};