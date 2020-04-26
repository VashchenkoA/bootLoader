#include <Windows.h>
#include <iostream>
#include "pch.h"
#include "FileSystemFactory.h"
#include "FAT_FileSystem.cpp"
#include "exFAT_FileSystem.cpp"
#include "NTFS_FileSystem.cpp"

using namespace std;

char* FileSystemFactory::getFileSystemName(char devName)
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
FileSystemClass* FileSystemFactory::createFSobject(char devName) {

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