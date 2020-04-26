#pragma once

//Making DLL exportable and importable
#ifdef BOOTLOADER_EXPORTS
#define BOOTLOADER_API __declspec(dllexport)
#else
#define BOOTLOADER_API __declspec(dllimport)
#endif



//External linkage of DLL functions
extern "C" BOOTLOADER_API void getDevName();