#include <Windows.h>
#include <stdio.h>
#include "pch.h"
#include <iostream>
#include "FileSystemClass.h"
#include "NTFS_FileSystem.h"
#include "exFAT_FileSystem.h"
#include "FAT_FileSystem.h"

using namespace std;

FileSystemClass * FileSystemClass::getFSinformation() {
	CHAR volumeName[MAX_PATH + 1] = { 0 };
	CHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;

	string rootPathName;

	cout << "Please enter disk name. (One letter: A, B, C)" << endl;
	rootPathName = getchar();

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

	printf(("Volume Name: %s\n"), volumeName);
	printf(("Serial Number: %lu\n"), serialNumber);
	printf(("File System Name: %s\n"), fileSystemName);
	printf(("Max Component Length: %lu\n"), maxComponentLen);

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
