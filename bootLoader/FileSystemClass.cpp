#include <Windows.h>
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include "FileSystemClass.h"

using namespace std;


class FileSystemClass {
public:
	virtual void PrintBootSectInfo() = 0;
	virtual bool bootInfo() = 0;
	virtual ~FileSystemClass() {}
	static char* getFileSystemName(char devName) {
		CHAR volumeName[MAX_PATH + 1] = { 0 };
		CHAR fileSystemName[MAX_PATH + 1] = { 0 };
		DWORD serialNumber = 0;
		DWORD maxComponentLen = 0;
		DWORD fileSystemFlags = 0;

		string rootPathName;
		rootPathName = devName;
		string rootPathNameFormatted = rootPathName + ":";

		GetVolumeInformationA(
			rootPathNameFormatted.c_str(),
			volumeName,
			sizeof(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			sizeof(fileSystemName));
		return fileSystemName;
	}
	static char getDevName() {
		char volumeName;
		printf("Enter device name letter (A, B, C and etc.):");
		scanf_s("%c", &volumeName, 1);
		return volumeName;
	}
	static FileSystemClass* createFSobject();
};

class FAT_FileSystem : public FileSystemClass
{
public:
	typedef unsigned char byte;

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
	} BOOT_FAT;

	void PrintBootSectInfo (BOOT_FAT pBootRecord){
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

	bool bootInfo(char devName, BOOT_FAT* pBootRecord){

		string fileNameFormated = "\\\\.\\"s + devName + ":";

		HANDLE fileHandle = CreateFileA(
			fileNameFormated.c_str(),//convert string to LPCSTR
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
		pBootRecord = reinterpret_cast<BOOT_FAT*>(dataBuffer);

		//Close file
		CloseHandle(fileHandle);

		return true;
	}
};

class exFAT_FileSystem : public FileSystemClass
{
public:
	typedef unsigned char byte;

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

	void PrintBootSectInfo(BOOT_exFAT pBootRecord) {
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

	bool bootInfo(const WCHAR* fileNameFormated, BOOT_exFAT* pBootRecord) {
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
	} BOOT_NTFS;
	//Back to standard alignment
	#pragma pack(pop)

	void PrintBootSectInfo(BOOT_NTFS pBootRecord) {
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

	bool bootInfo(const WCHAR* fileNameFormated, BOOT_NTFS* pBootRecord) {
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
	}

};

FileSystemClass* FileSystemClass::createFSobject() {

	CHAR devName = getDevName();
	const char* fileSystemName = getFileSystemName(devName);
	FileSystemClass* fs;

	if (strcmp(fileSystemName, "exFAT") == 0) {
		fs = new exFAT_FileSystem();
		return fs;
	}
	else if (strcmp(fileSystemName, "FAT") == 0) {
		fs = new FAT_FileSystem();
		return fs;
	}
	else if (strcmp(fileSystemName, "NTFS") == 0) {
		fs = new NTFS_FileSystem();
		return fs;
	}


}
