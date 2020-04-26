#pragma once

class FileSystemClass {
public:
	virtual typedef struct Boot_Record;
	virtual void PrintBootSectInfo() = 0;
	virtual void bootInfo(char devName) = 0;
	virtual ~FileSystemClass() {}
};