#include <iostream>
#include "bootLoader.h"

using namespace std;

int main()
{
	string fileName;

	// TODO: validation
	while (1)
	{
		cout << "Please enter disk name. Should be like C, D, and so on" << endl;
		fileName = cin.get();
		if (fileName.length() == 1)
		{
			break;
		}
	}


	string fileNameFormated = "\\\\.\\" + fileName + ":";


	BOOT_NTFS pBootRecord;

	if (bootInfo(fileNameFormated, &pBootRecord))
	{
		PrintBootSectInfo(pBootRecord);
	}


}