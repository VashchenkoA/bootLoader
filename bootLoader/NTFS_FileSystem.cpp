#include <stdio.h>
#include "pch.h"
#include "FileSystemClass.h"
#include <Windows.h>
#include <iostream>

using namespace std;


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
		WORD  sec_size;
		WORD  secs_cluster;
		BYTE    zeros_0[6];
		WORD  media_desc;
		BYTE    zeros_1;
		WORD  secs_track;
		WORD  num_heads;
		BYTE    zeros_2[8];
		DWORD32  defaultValue;
		ULONGLONG  num_secs;
		ULONGLONG  LCNofMFT;
		ULONGLONG  LCNofMFTMirr;
		DWORD   clustersPerMFT;
		DWORD32  clustersPerIndex;
		ULONGLONG  volumeSerialNumber;
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