//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
#include "DataAccessLibConnect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
PHasError		HasError;
PListErrors	ListErrors;
PGetLastErrorStringW		GetLastErrorStringW;
PGetLastErrorString		GetLastErrorString;
PCreateStorageHandle		CreateStorageHandle;
PCloseStorageHandle		CloseStorageHandle;
PGetStorageType		GetStorageType;
PGetPhysicalDriveInfo		GetPhysicalDriveInfo;
PGetNumberOfPartitions		GetNumberOfPartitions;
PGetPartitionInfoByNumber		GetPartitionInfoByNumber;
PGetStorageBlockSize		GetStorageBlockSize;
PSetStorageBlockSize		SetStorageBlockSize;
PGetStorageIterator		GetStorageIterator;
PStorageBlockFirst		StorageBlockFirst;
PStorageBlockNext		StorageBlockNext;
PStorageBlockIsDone		StorageBlockIsDone;
PCloseStorageIterator		CloseStorageIterator;
PSetStoragePosition		SetStoragePosition;
PGetStoragePosition		GetStoragePosition;
PReadStorageData		ReadStorageData;
PRecognizeFileSystem		RecognizeFileSystem;
PCreateFileSystemHandle		CreateFileSystemHandle;
PFastCreateFileSystemHandle		FastCreateFileSystemHandle;
PCloseFileSystemHandle		CloseFileSystemHandle;
PGetFileSystemType		GetFileSystemType;
PGetFileSystemTypeStringW		GetFileSystemTypeStringW;
PGetFileSystemTypeString		GetFileSystemTypeString;
PGetNumberOfClusters		GetNumberOfClusters;
PGetFileSystemSize		GetFileSystemSize;
PGetUsedSpaceSize		GetUsedSpaceSize;
PGetFileSystemClusterSize		GetFileSystemClusterSize;
PGetVolumeLabel		GetVolumeLabel;
PGetVolumeLabelW		GetVolumeLabelW;
PGetVolumeSerial	GetVolumeSerial;
PGetVolumeSerialW		GetVolumeSerialW;
PGetVolumeInfo		GetVolumeInfo;
PGetClusterIterator		GetClusterIterator;
PClusterFirst		ClusterFirst;
PClusterNext		ClusterNext;
PClusterIsDone		ClusterIsDone;
PClusterIsUsed		ClusterIsUsed;
PGetCurrentCluster		GetCurrentCluster;
PCloseClusterIterator		CloseClusterIterator;
PSetFileSystemPosition		SetFileSystemPosition;
PReadFileSystemData		ReadFileSystemData;
PReadClustersByNumber		ReadClustersByNumber;
PCreateFileHandle		CreateFileHandle;
PCreateFileHandleByRecordId		CreateFileHandleByRecordId;
PGetFileRecordId		GetFileRecordId;
PFileIsDir		FileIsDir;
PFileIsLink		FileIsLink;
PGetLinkTargetW GetLinkTargetW;
PGetNumberOfStreams		GetNumberOfStreams;
PGetNumberOfFiles		GetNumberOfFiles;
PGetFileDataSize		GetFileDataSize;
PGetTimeFileCreate		GetTimeFileCreate;
PGetTimeFileModify		GetTimeFileModify;
PGetTimeFileAccess		GetTimeFileAccess;
PGetTimeFileRecordModify		GetTimeFileRecordModify;
PGetFileStdAttributes		GetFileStdAttributes;
PGetFileHash GetFileHash;
PFileGetNameW FileGetNameW;
PFileGetPathW FileGetPathW;
PFileGetFullPathW FileGetFullPathW;
PRecognizeFileCustomSignature		RecognizeFileCustomSignature;
PSaveFile		SaveFile;
PSaveFileWithPath		SaveFileWithPath;
PSetFilePosition		SetFilePosition;
PReadFileData		ReadFileData;
PCloseFileHandle		CloseFileHandle;
PCreateDirectoryIteratorHandle		CreateDirectoryIteratorHandle;
PCreateFileSystemLinearIteratorHandle		CreateFileSystemLinearIteratorHandle;
PCreateFileSystemMaskIteratorHandle		CreateFileSystemMaskIteratorHandle;
PDirFirst		DirFirst;
PDirNext		DirNext;
PDirIsDone		DirIsDone;
PDirFileIsDir		DirFileIsDir;
PDirFileIsLink		DirFileIsLink;
PGetFileNameW		GetFileNameW;
PGetFilePathW		GetFilePathW;
PGetFullPathW		GetFullPathW;
PGetCurrentFile	GetCurrentFile;
PCloseDirectoryIterator		CloseDirectoryIterator;
PRecognizeBufferCustomSignature		RecognizeBufferCustomSignature;

PWriteDataToFileW WriteDataToFileW;
//-----------------------------------------
HINSTANCE DataAccessLibDll = NULL;
//-----------------------------------------
void InitFunctionsFromLib()
{
	HasError = (PHasError)GetProcAddress(DataAccessLibDll, "HasError");
	ListErrors = (PListErrors)GetProcAddress(DataAccessLibDll, "ListErrors");
	GetLastErrorStringW = (PGetLastErrorStringW)GetProcAddress(DataAccessLibDll, "GetLastErrorStringW");
	GetLastErrorString = (PGetLastErrorString)GetProcAddress(DataAccessLibDll, "GetLastErrorString");
	CreateStorageHandle = (PCreateStorageHandle)GetProcAddress(DataAccessLibDll, "CreateStorageHandle");
	CloseStorageHandle = (PCloseStorageHandle)GetProcAddress(DataAccessLibDll, "CloseStorageHandle");
	GetStorageType = (PGetStorageType)GetProcAddress(DataAccessLibDll, "GetStorageType");
	GetPhysicalDriveInfo = (PGetPhysicalDriveInfo)GetProcAddress(DataAccessLibDll, "GetPhysicalDriveInfo");
	GetNumberOfPartitions = (PGetNumberOfPartitions)GetProcAddress(DataAccessLibDll, "GetNumberOfPartitions");
	GetPartitionInfoByNumber = (PGetPartitionInfoByNumber)GetProcAddress(DataAccessLibDll, "GetPartitionInfoByNumber");
	GetStorageBlockSize = (PGetStorageBlockSize)GetProcAddress(DataAccessLibDll, "GetStorageBlockSize");
	SetStorageBlockSize = (PSetStorageBlockSize)GetProcAddress(DataAccessLibDll, "SetStorageBlockSize");
	GetStorageIterator = (PGetStorageIterator)GetProcAddress(DataAccessLibDll, "GetStorageIterator");
	StorageBlockFirst = (PStorageBlockFirst)GetProcAddress(DataAccessLibDll, "StorageBlockFirst");
	StorageBlockNext = (PStorageBlockNext)GetProcAddress(DataAccessLibDll, "StorageBlockNext");
	StorageBlockIsDone = (PStorageBlockIsDone)GetProcAddress(DataAccessLibDll, "StorageBlockIsDone");
	CloseStorageIterator = (PCloseStorageIterator)GetProcAddress(DataAccessLibDll, "CloseStorageIterator");
	GetStoragePosition = (PGetStoragePosition)GetProcAddress(DataAccessLibDll, "GetStoragePosition");
	SetStoragePosition = (PSetStoragePosition)GetProcAddress(DataAccessLibDll, "SetStoragePosition");
	ReadStorageData = (PReadStorageData)GetProcAddress(DataAccessLibDll, "ReadStorageData");
	RecognizeFileSystem = (PRecognizeFileSystem)GetProcAddress(DataAccessLibDll, "RecognizeFileSystem");
	CreateFileSystemHandle = (PCreateFileSystemHandle)GetProcAddress(DataAccessLibDll, "CreateFileSystemHandle");
	FastCreateFileSystemHandle = (PFastCreateFileSystemHandle)GetProcAddress(DataAccessLibDll, "FastCreateFileSystemHandle");
	CloseFileSystemHandle = (PCloseFileSystemHandle)GetProcAddress(DataAccessLibDll, "CloseFileSystemHandle");
	GetFileSystemType = (PGetFileSystemType)GetProcAddress(DataAccessLibDll, "GetFileSystemType");
	GetFileSystemTypeStringW = (PGetFileSystemTypeStringW)GetProcAddress(DataAccessLibDll, "GetFileSystemTypeStringW");
	GetFileSystemTypeString = (PGetFileSystemTypeString)GetProcAddress(DataAccessLibDll, "GetFileSystemTypeString");
	GetNumberOfClusters = (PGetNumberOfClusters)GetProcAddress(DataAccessLibDll, "GetNumberOfClusters");
	GetFileSystemSize = (PGetFileSystemSize)GetProcAddress(DataAccessLibDll, "GetFileSystemSize");
	GetUsedSpaceSize = (PGetUsedSpaceSize)GetProcAddress(DataAccessLibDll, "GetUsedSpaceSize");
	GetFileSystemClusterSize = (PGetFileSystemClusterSize)GetProcAddress(DataAccessLibDll, "GetFileSystemClusterSize");
	GetVolumeLabel = (PGetVolumeLabel)GetProcAddress(DataAccessLibDll, "GetVolumeLabel");
	GetVolumeLabelW = (PGetVolumeLabelW)GetProcAddress(DataAccessLibDll, "GetVolumeLabelW");
	GetVolumeSerial = (PGetVolumeSerial)GetProcAddress(DataAccessLibDll, "GetVolumeSerial");
	GetVolumeSerialW = (PGetVolumeSerialW)GetProcAddress(DataAccessLibDll, "GetVolumeSerialW");
	GetVolumeInfo = (PGetVolumeInfo)GetProcAddress(DataAccessLibDll, "GetVolumeInfo");
	GetClusterIterator = (PGetClusterIterator)GetProcAddress(DataAccessLibDll, "GetClusterIterator");
	ClusterFirst = (PClusterFirst)GetProcAddress(DataAccessLibDll, "ClusterFirst");
	ClusterNext = (PClusterNext)GetProcAddress(DataAccessLibDll, "ClusterNext");
	ClusterIsDone = (PClusterIsDone)GetProcAddress(DataAccessLibDll, "ClusterIsDone");
	ClusterIsUsed = (PClusterIsUsed)GetProcAddress(DataAccessLibDll, "ClusterIsUsed");
	GetCurrentCluster = (PGetCurrentCluster)GetProcAddress(DataAccessLibDll, "GetCurrentCluster");
	CloseClusterIterator = (PCloseClusterIterator)GetProcAddress(DataAccessLibDll, "CloseClusterIterator");
	SetFileSystemPosition = (PSetFileSystemPosition)GetProcAddress(DataAccessLibDll, "SetFileSystemPosition");
	ReadFileSystemData = (PReadFileSystemData)GetProcAddress(DataAccessLibDll, "ReadFileSystemData");
	ReadClustersByNumber = (PReadClustersByNumber)GetProcAddress(DataAccessLibDll, "ReadClustersByNumber");

	CreateFileHandle = (PCreateFileHandle)GetProcAddress(DataAccessLibDll, "CreateFileHandle");
	CreateFileHandleByRecordId = (PCreateFileHandleByRecordId)GetProcAddress(DataAccessLibDll, "CreateFileHandleByRecordId");
	GetFileRecordId = (PGetFileRecordId)GetProcAddress(DataAccessLibDll, "GetFileRecordId");
	FileGetNameW = (PGetFileNameW)GetProcAddress(DataAccessLibDll, "FileGetNameW");
	FileGetPathW = (PGetFilePathW)GetProcAddress(DataAccessLibDll, "FileGetPathW");
	FileGetFullPathW = (PGetFullPathW)GetProcAddress(DataAccessLibDll, "FileGetFullPathW");
	FileIsDir = (PFileIsDir)GetProcAddress(DataAccessLibDll, "FileIsDir");
	FileIsLink = (PFileIsLink)GetProcAddress(DataAccessLibDll, "FileIsLink");
	GetLinkTargetW = (PGetLinkTargetW)GetProcAddress(DataAccessLibDll, "GetLinkTargetW");
	GetNumberOfStreams = (PGetNumberOfStreams)GetProcAddress(DataAccessLibDll, "GetNumberOfStreams");
	GetNumberOfFiles = (PGetNumberOfFiles)GetProcAddress(DataAccessLibDll, "GetNumberOfFiles");
	GetFileDataSize = (PGetFileDataSize)GetProcAddress(DataAccessLibDll, "GetFileDataSize");
	GetTimeFileCreate = (PGetTimeFileCreate)GetProcAddress(DataAccessLibDll, "GetTimeFileCreate");
	GetTimeFileModify = (PGetTimeFileModify)GetProcAddress(DataAccessLibDll, "GetTimeFileModify");
	GetTimeFileAccess = (PGetTimeFileAccess)GetProcAddress(DataAccessLibDll, "GetTimeFileAccess");
	GetTimeFileRecordModify = (PGetTimeFileRecordModify)GetProcAddress(DataAccessLibDll, "GetTimeFileRecordModify");
	GetFileStdAttributes = (PGetFileStdAttributes)GetProcAddress(DataAccessLibDll, "GetFileStdAttributes");
	GetFileHash = (PGetFileHash)GetProcAddress(DataAccessLibDll, "GetFileHash");
	RecognizeFileCustomSignature = (PRecognizeFileCustomSignature)GetProcAddress(DataAccessLibDll, "RecognizeFileCustomSignature");
	SaveFile = (PSaveFile)GetProcAddress(DataAccessLibDll, "SaveFile");
	SaveFileWithPath = (PSaveFileWithPath)GetProcAddress(DataAccessLibDll, "SaveFileWithPath");
	SetFilePosition = (PSetFilePosition)GetProcAddress(DataAccessLibDll, "SetFilePosition");
	ReadFileData = (PReadFileData)GetProcAddress(DataAccessLibDll, "ReadFileData");
	CloseFileHandle = (PCloseFileHandle)GetProcAddress(DataAccessLibDll, "CloseFileHandle");
	CreateDirectoryIteratorHandle = (PCreateDirectoryIteratorHandle)GetProcAddress(DataAccessLibDll, "CreateDirectoryIteratorHandle");
	CreateFileSystemLinearIteratorHandle = (PCreateFileSystemLinearIteratorHandle)GetProcAddress(DataAccessLibDll, "CreateFileSystemLinearIteratorHandle");
	CreateFileSystemMaskIteratorHandle = (PCreateFileSystemMaskIteratorHandle)GetProcAddress(DataAccessLibDll, "CreateFileSystemMaskIteratorHandle");
	DirFirst = (PDirFirst)GetProcAddress(DataAccessLibDll, "DirFirst");
	DirNext = (PDirNext)GetProcAddress(DataAccessLibDll, "DirNext");
	DirIsDone = (PDirIsDone)GetProcAddress(DataAccessLibDll, "DirIsDone");
	DirFileIsDir = (PDirFileIsDir)GetProcAddress(DataAccessLibDll, "DirFileIsDir");
	DirFileIsLink = (PDirFileIsLink)GetProcAddress(DataAccessLibDll, "DirFileIsLink");
	GetFileNameW = (PGetFileNameW)GetProcAddress(DataAccessLibDll, "GetFileNameW");
	GetFilePathW = (PGetFilePathW)GetProcAddress(DataAccessLibDll, "GetFilePathW");
	GetFullPathW = (PGetFullPathW)GetProcAddress(DataAccessLibDll, "GetFullPathW");
	GetCurrentFile = (PGetCurrentFile)GetProcAddress(DataAccessLibDll, "GetCurrentFile");
	CloseDirectoryIterator = (PCloseDirectoryIterator)GetProcAddress(DataAccessLibDll, "CloseDirectoryIterator");
	RecognizeBufferCustomSignature = (PRecognizeBufferCustomSignature)GetProcAddress(DataAccessLibDll, "RecognizeBufferCustomSignature");

	WriteDataToFileW = (PWriteDataToFileW)GetProcAddress(DataAccessLibDll, "WriteDataToFileW");
}
//---------------------------------------------------------------------------
bool HasNullFunctions()
{
	return (HasError == NULL || GetLastErrorStringW == NULL ||
		GetLastErrorString == NULL || ListErrors == NULL || CreateStorageHandle == NULL || CloseStorageHandle == NULL ||
		GetStorageType == NULL || GetPhysicalDriveInfo == NULL || GetNumberOfPartitions == NULL ||
		GetPartitionInfoByNumber == NULL || GetStorageBlockSize == NULL || SetStorageBlockSize == NULL ||
		GetStorageIterator == NULL || StorageBlockFirst == NULL || StorageBlockNext == NULL ||
		StorageBlockIsDone == NULL || CloseStorageIterator == NULL || SetStoragePosition == NULL || GetStoragePosition == NULL ||
		ReadStorageData == NULL || RecognizeFileSystem == NULL ||
		CreateFileSystemHandle == NULL || FastCreateFileSystemHandle == NULL || CloseFileSystemHandle == NULL ||
		GetFileSystemType == NULL || GetFileSystemTypeStringW == NULL || GetFileSystemTypeString == NULL ||
		GetNumberOfClusters == NULL || GetFileSystemSize == NULL || GetFileSystemClusterSize == NULL ||
		GetUsedSpaceSize == NULL || GetVolumeLabel == NULL ||	GetVolumeLabelW == NULL ||
		GetVolumeSerial == NULL || GetVolumeSerialW == NULL || GetVolumeInfo == NULL ||
		GetClusterIterator == NULL || ClusterFirst == NULL || ClusterNext == NULL ||
		ClusterIsDone == NULL || ClusterIsUsed == NULL || GetCurrentCluster == NULL ||
		CloseClusterIterator == NULL || SetFileSystemPosition == NULL || ReadFileSystemData == NULL ||
		ReadClustersByNumber == NULL || CreateFileHandle == NULL ||
		CreateFileHandleByRecordId == NULL || GetFileRecordId == NULL || FileIsDir == NULL || FileIsLink == NULL || GetLinkTargetW == NULL || GetNumberOfStreams == NULL ||
		GetNumberOfFiles == NULL || GetFileDataSize == NULL || GetTimeFileCreate == NULL ||
		GetTimeFileModify == NULL || GetTimeFileAccess == NULL || GetTimeFileRecordModify == NULL ||
		FileGetNameW == NULL || FileGetPathW == NULL || FileGetFullPathW == NULL ||
		GetFileStdAttributes == NULL || RecognizeFileCustomSignature == NULL || SaveFile == NULL ||
		SaveFileWithPath == NULL || SetFilePosition == NULL || ReadFileData == NULL ||
		CloseFileHandle == NULL || CreateDirectoryIteratorHandle == NULL || CreateFileSystemLinearIteratorHandle == NULL || CreateFileSystemMaskIteratorHandle == NULL ||
		DirFirst == NULL ||	DirNext == NULL || DirIsDone == NULL || DirFileIsDir == NULL || DirFileIsLink == NULL ||
		GetFileNameW == NULL || GetFilePathW == NULL || GetFullPathW == NULL || GetCurrentFile == NULL ||
		CloseDirectoryIterator == NULL || RecognizeBufferCustomSignature == NULL || WriteDataToFileW == NULL);
}
//---------------------------------------------------------------------------
void ClearFunctionPtrs()
{
	HasError = NULL;
	ListErrors = NULL;
	GetLastErrorStringW = NULL;
	GetLastErrorString = NULL;
	CreateStorageHandle = NULL;
	CloseStorageHandle = NULL;
	GetStorageType = NULL;
	GetPhysicalDriveInfo = NULL;
	GetNumberOfPartitions = NULL;
	GetPartitionInfoByNumber = NULL;
	GetStorageBlockSize = NULL;
	SetStorageBlockSize = NULL;
	GetStorageIterator = NULL;
	StorageBlockFirst = NULL;
	StorageBlockNext = NULL;
	StorageBlockIsDone = NULL;
	CloseStorageIterator = NULL;
	GetStoragePosition = NULL;
	SetStoragePosition = NULL;
	ReadStorageData = NULL;
	RecognizeFileSystem = NULL;
	CreateFileSystemHandle = NULL;
	FastCreateFileSystemHandle = NULL;
	CloseFileSystemHandle = NULL;
	GetFileSystemType = NULL;
	GetFileSystemTypeStringW = NULL;
	GetFileSystemTypeString = NULL;
	GetNumberOfClusters = NULL;
	GetFileSystemSize = NULL;
	GetUsedSpaceSize = NULL;
	GetFileSystemClusterSize = NULL;
	GetVolumeLabel = NULL;
	GetVolumeLabelW = NULL;
	GetVolumeSerial = NULL;
	GetVolumeSerialW = NULL;
	GetVolumeInfo = NULL;
	GetClusterIterator = NULL;
	ClusterFirst = NULL;
	ClusterNext = NULL;
	ClusterIsDone = NULL;
	ClusterIsUsed = NULL;
	GetCurrentCluster = NULL;
	CloseClusterIterator = NULL;
	SetFileSystemPosition = NULL;
	ReadFileSystemData = NULL;
	ReadClustersByNumber = NULL;

	CreateFileHandle = NULL;
	CreateFileHandleByRecordId = NULL;
	GetFileRecordId = NULL;
	FileIsDir = NULL;
	FileIsLink = NULL;
	GetNumberOfStreams = NULL;
	GetNumberOfFiles = NULL;
	GetFileDataSize = NULL;
	GetTimeFileCreate = NULL;
	GetTimeFileModify = NULL;
	GetTimeFileAccess = NULL;
	GetTimeFileRecordModify = NULL;
	GetFileStdAttributes = NULL;
	GetFileHash = NULL;
	FileGetNameW = NULL;
	FileGetPathW = NULL;
	FileGetFullPathW = NULL;
	RecognizeFileCustomSignature = NULL;
	SaveFile = NULL;
	SaveFileWithPath = NULL;
	SetFilePosition = NULL;
	ReadFileData = NULL;
	CloseFileHandle = NULL;
	CreateDirectoryIteratorHandle = NULL;
	DirFirst = NULL;
	DirNext = NULL;
	DirIsDone = NULL;
	DirFileIsDir = NULL;
	DirFileIsLink = NULL;
	GetFileNameW = NULL;
	GetFilePathW = NULL;
	GetFullPathW = NULL;
	GetCurrentFile = NULL;
	CreateFileSystemLinearIteratorHandle = NULL;
	CreateFileSystemMaskIteratorHandle = NULL;
	CloseDirectoryIterator = NULL;
	RecognizeBufferCustomSignature = NULL;

	WriteDataToFileW = NULL;
}
//---------------------------------------------------------------------------
bool __fastcall DataAccessLibInit()
{
	// Инициализировать все указатели
	ClearFunctionPtrs();

	// Загрузить библиотеку
	DataAccessLibDll = LoadLibraryW(L"DataAccessLib.dll");

	if(DataAccessLibDll == NULL)
	{
    // Ошибка подключения библиотеки
		return false;
	}

	// Подключить функции
	InitFunctionsFromLib();

	// Проверить корректность подключения
	if(HasNullFunctions())
	{
		// Ошибка подключения функций библиотеки
		FreeLibrary(DataAccessLibDll);
		DataAccessLibDll = NULL;
		return false;
	}
	else
	{
		return true;
	}
}
//---------------------------------------------------------------------------
void __fastcall DataAccessLibCleanup()
{
	if (DataAccessLibDll != NULL)
	{
		FreeLibrary(DataAccessLibDll);
		DataAccessLibDll = NULL;
	}

	ClearFunctionPtrs();
}
//---------------------------------------------------------------------------
