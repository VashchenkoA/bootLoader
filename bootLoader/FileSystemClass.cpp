#include <Windows.h>
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include "FileSystemClass.h"

using namespace std;


class FileSystemClass {
public:
	typedef unsigned char byte;
	virtual typedef struct Boot_Record;
	virtual void PrintBootSectInfo() = 0;
	virtual void bootInfo(char devName) = 0;
	virtual ~FileSystemClass() {}
};

class FAT_FileSystem : public FileSystemClass{
public:
	
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
	} Boot_Record;

private:
	Boot_Record pBootRecord;

public:

	void PrintBootSectInfo (){
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

	void bootInfo(char devName){
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

class exFAT_FileSystem : public FileSystemClass{
public:

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
	} Boot_Record;

private:
	Boot_Record pBootRecord;

	void PrintBootSectInfo() {
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

class NTFS_FileSystem : public FileSystemClass
{
public:
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
	} Boot_Record;
	//Back to standard alignment
	#pragma pack(pop)

private:
	Boot_Record pBootRecord;

	void PrintBootSectInfo() {
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


class FileSystemFactory{
public:
	static char* getFileSystemName(char devName)
	{
		CHAR volumeName[MAX_PATH + 1] = { 0 };
		CHAR fileSystemName[MAX_PATH + 1] = { 0 };
		DWORD serialNumber = 0;
		DWORD maxComponentLen = 0;
		DWORD fileSystemFlags = 0;

		string rootPathName;

		rootPathName = devName;
		string rootPathNameFormatted = rootPathName + ":";
		// ïîëó÷åíèå òèïà ÔÑ íà íîñèòåëå
		GetVolumeInformationA(
			rootPathNameFormatted.c_str(),
			volumeName,
			sizeof(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			sizeof(fileSystemName));
		return(fileSystemName);
	}
	static FileSystemClass* createFSobject(char devName) {

		const char* fileSystemName = getFileSystemName(devName);

		if (strcmp(fileSystemName, "exFAT") == 0) {
			FileSystemClass* fs = new exFAT_FileSystem();
			return fs;
		}
		else if (strcmp(fileSystemName, "FAT") == 0) {
			FileSystemClass* fs = new FAT_FileSystem();
			return fs;
		}
		else if (strcmp(fileSystemName, "NTFS") == 0) {
			FileSystemClass* fs = new NTFS_FileSystem();
			return fs;
		}


	}
};

void getDevName() {
	char volumeName;
	printf("Enter device name letter (A, B, C and etc.):");
	scanf_s("%c", &volumeName, 1);
	FileSystemClass* fs = FileSystemFactory::createFSobject(volumeName);
	fs->bootInfo(volumeName);
	fs->PrintBootSectInfo();
}