#include "FileSystemClass.h"
#pragma once
class NTFS_FileSystem : public FileSystemClass
{
public:
	void PrintBootSectInfo(BOOT_NTFS pBootRecord);
	bool bootInfo(const WCHAR* fileNameFormated, BOOT_NTFS* pBootRecord);

};
