#include "FileSystemClass.h"
#pragma once
class FAT_FileSystem : public FileSystemClass
{
public:
	void PrintBootSectInfo(BOOT_FAT pBootRecord);
	bool bootInfo(const WCHAR* fileNameFormated, BOOT_FAT* pBootRecord);
};
