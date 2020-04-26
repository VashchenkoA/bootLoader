#include <Windows.h>
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include "FileSystemClass.h"

using namespace std;


class FileSystemClass {
public:
	virtual typedef struct Boot_Record;
	virtual void PrintBootSectInfo() = 0;
	virtual void bootInfo(char devName) = 0;
	virtual ~FileSystemClass() {}
};

class FAT_FileSystem : public FileSystemClass{
public:

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
		BYTE physicalDiskNumber;
		BYTE currentHead;
		BYTE signature;
		BYTE volumeSerialNumber[4];
		BYTE volumeLabel[11];
		BYTE systemID[8];
	} Boot_Record;

private:
	Boot_Record pBootRecord;

public:

	void PrintBootSectInfo (){
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
		cout << "Hidden sectors sectors: " << pBootRecord.hiddenSectors << endl;
		cout << "Large sectors: " << pBootRecord.largeSectors << endl;
		cout << "Physical disk number: " << pBootRecord.physicalDiskNumber << endl;
		cout << "Current head: " << pBootRecord.currentHead << endl;
		cout << "FAT signature: " << pBootRecord.signature << endl;
		cout << "Volume serial number: " << pBootRecord.volumeSerialNumber << endl;
		cout << "Volume label: " << pBootRecord.volumeLabel << endl;
		cout << "System ID: " << pBootRecord.systemID << endl;
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
	printf("Enter device name letter (A, B, C and etc.):\n");
	scanf_s("%c", &volumeName, 1);
	FileSystemClass* fs = FileSystemFactory::createFSobject(volumeName);
	fs->bootInfo(volumeName);
	fs->PrintBootSectInfo();
}