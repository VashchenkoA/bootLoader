#include <stdio.h>
#include "pch.h"
#include "FileSystemClass.h"
#include <Windows.h>
#include <iostream>

using namespace std;

class FAT_FileSystem : public FileSystemClass {
public:
#pragma pack(push, 1)
	typedef struct
	{
		BYTE jump[3];
		BYTE oemName[8]; //  
		WORD bytesPerSector; // 
		BYTE sectorsPerCluster; // 
		WORD reservedSectors; // 
		BYTE numberOfFATtables; // 
		WORD rootEntries; // 
		WORD smallSectors; // 
		BYTE mediaType; // 
		WORD sectorsPerFAT;
		WORD sectorsPerTrack; // 
		WORD numberOfHeads; // 
		DWORD hiddenSectors;
		DWORD largeSectors; // 
		BYTE signature;
	} Boot_Record;
#pragma pack(pop)
private:
	Boot_Record pBootRecord;

public:

	void PrintBootSectInfo() {
		cout << "File system type: " << pBootRecord.oemName << endl;
		cout << "Bytes per sector: " << pBootRecord.bytesPerSector << endl;
		cout << "Sectors per cluster: " << pBootRecord.sectorsPerCluster << endl;
		cout << "Reserved sectors: " << pBootRecord.reservedSectors << endl;
		cout << "Number of FAT tables: " << pBootRecord.numberOfFATtables << endl;
		cout << "Root entries: " << pBootRecord.rootEntries << endl;
		cout << "Number of small sectors: " << pBootRecord.smallSectors << endl;
		cout << "Media type: " << pBootRecord.mediaType << endl;
		cout << "Sectors per FAT: " << pBootRecord.sectorsPerFAT << endl;
		cout << "Sectors per track: " << pBootRecord.sectorsPerTrack << endl;
		cout << "Number of heads: " << pBootRecord.numberOfHeads << endl;
		cout << "Hidden sectors: " << pBootRecord.hiddenSectors << endl;
		cout << "Large sectors: " << pBootRecord.largeSectors << endl;
		cout << "FAT signature: " << pBootRecord.signature << endl;
	}

	void bootInfo(char devName) {
		string fileNameFormated = "\\\\.\\"s + devName + ":";
		HANDLE hDevice = NULL;
		hDevice = CreateFileA(
			fileNameFormated.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (!hDevice)
		{
			printf("Error! Device is Empty!");
		}
		BYTE bBootSector[512];
		DWORD dwBytesRead(0);
		Boot_Record pBootRecord;
		ReadFile(hDevice, bBootSector, 512, &dwBytesRead, NULL);
		CloseHandle(hDevice);
		memcpy(&pBootRecord, bBootSector, 512);
		this->pBootRecord = pBootRecord;
	}
};
