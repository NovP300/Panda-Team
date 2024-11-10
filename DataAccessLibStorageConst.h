//---------------------------------------------------------------------------
#ifndef DataAccessLibStorageConstH
#define DataAccessLibStorageConstH
//---------------------------------------------------------------------------
#include <wtypes.h>
#include <iostream>
#include <vector>
//---------------------------------------------------------------------------
const unsigned int MinOffsetIncrement = 64;
const unsigned long DefaultSectorSize = 512;
const unsigned long SectorSize_4K = 4096;
const ULONGLONG MinPartitionWarningSize = 0x200000; // 2 МБ
//---------------------------------------------------------------------------
enum class StorageType : int
{
	LogicalDrive = 0,
	PhysicalDrive = 1,
	ImageFile = 2,
	DataStream = 3,
	DirectoryFile = 4,
	RAIDx = 5,
	VMDK = 6,
	VDI = 7,
	VHD = 10,
	VHDX = 12,
	QEMU = 14,
	HDD = 16,
	ErrorType = -1,
	UnknownType = -2
};
//---------------------------------------------------------------------------
enum class OpenStorageMode : int
{
	Logical = 0,
	Physical = 1,
	Image = 2,
	Directory = 3,
	Raid = 4,
	VMDK = 5,
	VDI = 6,
	VHD = 7,
	VHDX = 8,
	QEMU = 9,
	HDD = 10,
	Unknown = -1
};
//---------------------------------------------------------------------------
enum class SmartStatus : int
{
	Ok,
	Critical,
	PastThreshold,
	Error,
	Unknown
};
//---------------------------------------------------------------------------
typedef struct
{
	bool Error;
	char Device[24]; // \\.\PhysicalDriveXXXXX
	char ProductId[48];
	char Serial[48];
	ULONGLONG Size;

} PhysicalDriveInfoStruct, *PPhysicalDriveInfoStruct;
//---------------------------------------------------------------------------
typedef struct
{
	int PartIndex;          				// Порядковый номер раздела в таблице разделов или -1 для виртуальных разделов
	FileSystemTypeEnum RecognizedType; // Тип раздела, полученный в результате распознавания
	unsigned char RawType;    			// Идентификатор типа раздела (из MBR)
	unsigned __int64 FirstSector;  	// Начальный сектор раздела
	unsigned __int64 LastSector;  	// Конечный сектор раздела
	unsigned __int64 SizeInSectors; // Размер в секторах
	unsigned char TypeGuid[16];
	unsigned char PartGuid[16];
	unsigned __int64 Flags;        	// Флаги (из GPT или MBR)
	wchar_t Name[36];

} PartitionInfoStruct, *PPartitionInfoStruct;
//---------------------------------------------------------------------------
typedef struct
{
	bool Error;
	char Label[48];
	char Serial[24];
	ULONGLONG TotalSize;
	ULONGLONG UsedSize;

} VolumeInfoStruct, *PVolumeInfoStruct;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif