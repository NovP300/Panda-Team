//---------------------------------------------------------------------------
#ifndef DataAccessLibExportConstH
#define DataAccessLibExportConstH
//---------------------------------------------------------------------------
typedef int StorageHandle;
typedef int StorageIteratorHandle;
typedef int FileSystemHandle;
typedef int ClusterIteratorHandle;
typedef int FileHandle;
typedef int DirectoryIteratorHandle;
//---------------------------------------------------------------------------
const StorageHandle STORAGE_ERROR = -1;
//---------------------------------------------------------------------------
const int CUSTOM_SIGNATURE_ERROR = -1;
//---------------------------------------------------------------------------
const FileSystemHandle FILESYSTEM_ERROR = -1;
//---------------------------------------------------------------------------
const ClusterIteratorHandle CLUSTER_ITERATOR_ERROR = -1;
//---------------------------------------------------------------------------
const FileHandle FILE_OBJECT_ERROR = -1;
//---------------------------------------------------------------------------
const DirectoryIteratorHandle DIRECTORY_ITERATOR_ERROR = -1;
//---------------------------------------------------------------------------
const int FILENAME_ERROR = -1;
//---------------------------------------------------------------------------
const __int64 READ_ERROR = -1;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
enum class FileSystemTypeEnum : int
{
	NTFS 	= 0,
	FAT12 = 1,
	FAT16 = 2,
	FAT32 = 3,
	exFAT = 4,
	Ext2 	= 5,
	Ext3 	= 6,
	Ext4 	= 7,
	DFS 	= 12,
	MBR = 28,
	PMBR = 202, // Защитная MBR при GPT
	EMBR = 203, // Расширенный раздел MBR
	GPT = 29,
	GPT_4K = 205,
	PTFS = 204, // Обобщенная таблица разделов (может объединять MBR и GPT)
	FS_Custom 	= 35,   // Типы разделов, определяемые с использованием базы данных сигнатур
	FS_None	 		= 36,
	Encrypted   = 400,
	FS_Error 		= 37,
	FS_Debug 		= 38,
	VMDK        = 100,
	VDI	        = 101,
	VHD	        = 102,
	VHDX        = 103,
	QEMU        = 104,
	HDD         = 105, // Parallels
	TAR         = 506
};
//---------------------------------------------------------------------------
enum class OriginEnum : int
{
	Begin = 0,
	Current = 1,
	End = 2
};
//---------------------------------------------------------------------------
enum class DirectoryIteratorMode : int
{
	All,
	FileOnly,
	DirOnly,
	Ordered
};
//---------------------------------------------------------------------------
#endif
