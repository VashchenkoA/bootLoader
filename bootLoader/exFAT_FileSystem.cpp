#include "pch.h"
#include "exFAT_FileSystem.h"
#include <iostream>
#include <windows.h>

typedef unsigned char byte;
using namespace std;

typedef struct
{
	byte garbage[3];
	byte oemName[8]; //  
	byte garbage2[61];
	unsigned long long secPerFS; // 
	unsigned long firstFATSector; // 
	unsigned long sectorsPerFAT; // 
	unsigned long clusterBitmapStartSector; // 
	unsigned long clustersPerFS; // 
	unsigned long rootDirCluster; // 
	unsigned long volumeSerialNumber; // 
	byte garbage3[4];
	byte degreeOfSectorSize; // 
	byte degreeOfClusterMultiplier; // 
	byte garbage4[400];
	byte signatureFAT[2]; // 
} BOOT_exFAT;


void exFAT_FileSystem::PrintBootSectInfo(BOOT_exFAT pBootRecord)
{
    
	cout << "File system type: " << pBootRecord.oemName << endl;
	cout << "Sectors count: " << pBootRecord.secPerFS << endl;
	cout << "First FAT sector: " << pBootRecord.firstFATSector << endl;
	cout << "Sectors per FAT: " << pBootRecord.sectorsPerFAT << endl;
	cout << "Cluster bitmap start sector: " << pBootRecord.clusterBitmapStartSector << endl;
	cout << "Clusters count: " << pBootRecord.clustersPerFS << endl;
	cout << "NRoot directory cluster: " << pBootRecord.rootDirCluster << endl;
	cout << "Degree of sector size: " << pBootRecord.degreeOfSectorSize << endl;
	cout << "Degree of cluster multiplier: " << pBootRecord.degreeOfClusterMultiplier << endl;
	cout << "FAT signature: " << pBootRecord.signatureFAT << endl;
	cout << "Volume serial number: " << pBootRecord.volumeSerialNumber << endl;
}

bool exFAT_FileSystem::bootInfo(const WCHAR* fileNameFormated, BOOT_exFAT* pBootRecord)
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
    pBootRecord = reinterpret_cast<BOOT_exFAT*>(dataBuffer);

    //Close file
    CloseHandle(fileHandle);

    return true;
};
