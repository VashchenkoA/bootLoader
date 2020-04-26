#include <stdio.h>
#include "pch.h"
#include <windows.h>
#include <string>
#include "Header.h"
#include "FileSystemClass.h"
#include "FileSystemFactory.h"

void getDevName() {
	char volumeName;
	printf("Enter device name letter (A, B, C and etc.):\n");
	scanf_s("%c", &volumeName, 1);
	FileSystemClass* fs = FileSystemFactory::createFSobject(volumeName);
	fs->bootInfo(volumeName);
	fs->PrintBootSectInfo();
}