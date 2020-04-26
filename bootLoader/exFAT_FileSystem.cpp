#include <stdio.h>
#include "pch.h"
#include "FileSystemClass.h"
#include <Windows.h>
#include <iostream>

using namespace std;

class exFAT_FileSystem : public FileSystemClass {
public:
#pragma pack(push, 1)
	typedef struct
	{
		BYTE garbage[3];
		BYTE oemName[8]; //  
		BYTE garbage2[61];
		ULONGLONG secPerFS; // 
		DWORD firstFATSector; // 
		DWORD sectorsPerFAT; // 
		DWORD clusterBitmapStartSector; // 
		DWORD clustersPerFS; // 
		DWORD rootDirCluster; // 
		DWORD volumeSerialNumber; // 
		BYTE garbage3[4];
		BYTE degreeOfSectorSize; // 
		BYTE degreeOfClusterMultiplier; // 
		BYTE garbage4[400];
		BYTE signatureFAT[2]; //
	} Boot_Record;
#pragma pack(pop)
private:
	Boot_Record pBootRecord;

	void PrintBootSectInfo() {
		cout << "File system type: " << pBootRecord.oemName << endl;
		cout << "Sectors count: " << pBootRecord.secPerFS << endl;
		cout << "First FAT sector: " << pBootRecord.firstFATSector << endl;
		cout << "Sectors per FAT: " << pBootRecord.sectorsPerFAT << endl;
		cout << "Cluster bitmap start sector: " << pBootRecord.clusterBitmapStartSector << endl;
		cout << "Clusters count: " << pBootRecord.clustersPerFS << endl;
		cout << "Root directory cluster: " << pBootRecord.rootDirCluster << endl;
		cout << "Degree of sector size: " << pBootRecord.degreeOfSectorSize << endl;
		cout << "Degree of cluster multiplier: " << pBootRecord.degreeOfClusterMultiplier << endl;
		cout << "FAT signature: " << pBootRecord.signatureFAT << endl;
		cout << "Volume serial number: " << pBootRecord.volumeSerialNumber << endl;
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