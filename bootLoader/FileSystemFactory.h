#pragma once
#include "FileSystemClass.h"

class FileSystemFactory {
public:
	static char* getFileSystemName(char devName);
	static FileSystemClass* createFSobject(char devName);
};
