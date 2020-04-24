#include "FileSystemClass.h"
#pragma once
class exFAT_FileSystem : public FileSystemClass
{
public:
	void PrintBootSectInfo(BOOT_exFAT pBootRecord);
	bool bootInfo(const WCHAR* fileNameFormated, BOOT_exFAT* pBootRecord);
};

