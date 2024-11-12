#ifndef TestScenariousH
#define TestScenariousH

#include "DataAccessLibConnect.h"
#include <iostream>
#include <cwchar>

void ErrorCheck();
void PrintStorageType(StorageType type);
void PrintStorageType(StorageType type);
void PrintFileSystemType(FileSystemTypeEnum type);
bool WriteDataFromBufferToFile(const BYTE* dataBuffer, LONGLONG bufferSize, const std::string& fileName);
void DirTest();
int Dir(FileHandle fileObject, FileSystemHandle fileSystem);
int NumberOfDir = 0;
int NumberOfFile = 0;

std::string wstringToSingleByte(const std::wstring& wstr, UINT codePage = CP_UTF8) {
    int bufferSize = WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(bufferSize, 0);
    WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, &result[0], bufferSize, nullptr, nullptr);
    return result;
}


bool ConnectLibrary(){
	if (DataAccessLibInit()) {
		std::cout << "Бибилиотека подключена" << std::endl;
        return true;
	}
	else {
		std::cout << "Не удалось подключить библиотеку" << std::endl;
        return false;
	}
}

void StorageTest() {
    std::cout << std::endl;
    std::cout << "ТЕСТОВЫЙ СЦЕНАРИЙ РАБОТА С ХРАНИЛИЩЕМ" << std::endl << std::endl;

    std::cout << "Введите путь к хранилищу (формат D: для логчиских дисков, \\\\.\\PhysicalDriveX для физических дисков, для образов укажите полный путь до файла образа)" << std::endl;
    std::wstring StoragePath;

    std::wcin >> StoragePath;


    StorageHandle storage = CreateStorageHandle(StoragePath.c_str());
    if (storage != STORAGE_ERROR) {
        std::cout << "Дескриптор хранилища создан" << std::endl;

        StorageType storageType;
        storageType = GetStorageType(storage);
        std::wcout << L"Тип хранилища: ";
        PrintStorageType(storageType);
        std::cout << std::endl;

        // Получаем информацию о физическом носитете
        if (storageType == StorageType::PhysicalDrive) {
            PhysicalDriveInfoStruct info = GetPhysicalDriveInfo(storage);
            if (!info.Error) {
                std::cout << "Имя устройства: " << info.Device << std::endl;
                std::cout << "Модель устройства: " << info.ProductId << std::endl;
                std::cout << "Серийный номер: " << info.Serial << std::endl;
                std::cout << "Размер в байтах: " << info.Size << std::endl;
            }
            else {
                std::cout << "Произошла ошибка при попытке получить информацию о физическом диске" << std::endl;
                ErrorCheck();
            }
        }
        std::cout << std::endl;

        int NumberOfPartitions = GetNumberOfPartitions(storage);
        std::cout << "Число разделов хранилища: " << NumberOfPartitions << std::endl;
        std::cout << std::endl;
        DWORD storageBlockSize = GetStorageBlockSize(storage);
        std::cout << "Размер блока определенный библиотекой: " << storageBlockSize << std::endl;

        // Вывод информации о всех разделах хранилища

        PartitionInfoStruct* PartitionInfo = new PartitionInfoStruct;
        for (unsigned int i = 1; i <= NumberOfPartitions; i++) {
            GetPartitionInfoByNumber(storage, i, PartitionInfo);
            std::cout << "Номер раздела: " << i << std::endl;
            std::cout << "Файлова система: ";
            PrintFileSystemType(PartitionInfo->RecognizedType);
            std::cout << " || ";
            FileSystemTypeEnum type = RecognizeFileSystem(storage, PartitionInfo->FirstSector * DefaultSectorSize);
            PrintFileSystemType(type);
            std::cout << "(определ по сигнатуре)" << std::endl;
            std::cout << "Идентификатор типа раздела: " << PartitionInfo->RawType << std::endl;
            std::cout << "Первый сектор: " << PartitionInfo->FirstSector << std::endl;
            std::cout << "Последний сектор: " << PartitionInfo->LastSector << std::endl;
            std::cout << "Размер в секторах: " << PartitionInfo->SizeInSectors << std::endl;
            std::cout << "GUID типа раздела: " << PartitionInfo->TypeGuid << std::endl;
            std::cout << "GUID раздела: " << reinterpret_cast<char*>(PartitionInfo->PartGuid) << std::endl;
            std::cout << "Флаги раздела: " << PartitionInfo->Flags << std::endl;
            std::wcout << PartitionInfo->Flags << std::endl;
            std::cout << "Имя: " << wstringToSingleByte(PartitionInfo->Name) << std::endl;
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        }
        delete PartitionInfo;
        std::cout << std::endl;

        // Определение размера сектора и задание нового размера сектора
        if (storageBlockSize != 0) {
            std::cout << "Введите новый размер сектора: " << std::endl;
            DWORD newStorageBlockSize;
            std::cin >> newStorageBlockSize;
            if (storageType == StorageType::PhysicalDrive || storageType == StorageType::LogicalDrive || storageType == StorageType::DataStream) {
                if (newStorageBlockSize % storageBlockSize == 0) {
                    if (SetStorageBlockSize(storage, newStorageBlockSize)) {
                        std::cout << "Размер сектора успешно изменен, новый размер: " << newStorageBlockSize << std::endl;
                    }
                    else {
                        std::cout << "Произошла ошибка при изменении размера сектора хранилища" << std::endl;
                        ErrorCheck();
                    }
                }
                else {
                    std::cout << "Новый размер сектора должен быть кратен старому!" << std::endl;
                }
            }
            else {
                if (SetStorageBlockSize(storage, newStorageBlockSize)) {
                    std::cout << "Размер сектора успешно изменен, новый размер: " << newStorageBlockSize << std::endl;
                }
                else {
                    std::cout << "Ошибка задания нового размера сектора" << std::endl;
                    ErrorCheck();
                }
            }

            // Обход всех секторов диска
            std::cout << "Происходит обход секторов хранилища..." << std::endl;
            StorageIteratorHandle it = GetStorageIterator(storage);

            if (it != STORAGE_ERROR) {
                int k = 0;
                auto start = std::chrono::high_resolution_clock::now();
                for (StorageBlockFirst(it); !StorageBlockIsDone(it); StorageBlockNext(it)) {
                    k++;
                }
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                std::cout << "Количество пройденных секторов: " << k << std::endl;
                std::cout << "Время работы цикла: " << duration << " миллисекунд" << std::endl;
                CloseStorageIterator(it);
            }
            else {
                std::cout << "Ошибка создания итератора хранилища" << std::endl;
                ErrorCheck();
            }
            /*
            // Считывание раздела хранилища в файл по его номеру
            StorageBlockFirst(it);
            std::cout << "Введите номер раздела, который необходимо считать в буфер или нажмите ENTER, если хотите пропустить этот шаг" << std::endl;

            std::string inputLine;
            std::getline(std::cin, inputLine);

            if (inputLine.empty()) {
                std::cout << "Операция считывания раздела была пропущена." << std::endl;
            }
            else {
                DWORD partitionNumber = stoi(inputLine);  

                PartitionInfoStruct* PartitionInfo = new PartitionInfoStruct;
                GetPartitionInfoByNumber(storage, partitionNumber, PartitionInfo);

                LONGLONG startBlockPosition = PartitionInfo->FirstSector;
                LONGLONG endBlockPosition = PartitionInfo->LastSector;

                std::cout << "Стартовая позиция: " << startBlockPosition << std::endl;
                std::cout << "Конечная позиция: " << endBlockPosition << std::endl;

                if (SetStoragePosition(storage, startBlockPosition, OriginEnum::Begin)) {
                    LONGLONG bytesToRead = (endBlockPosition - startBlockPosition + 1) * storageBlockSize;
                    std::cout << "Количество байтов для чтения: " << bytesToRead << std::endl;
                    BYTE* dataBuffer = new BYTE[bytesToRead];
                    LONGLONG BytesRead = ReadStorageData(storage, bytesToRead, dataBuffer);
                    if (BytesRead == bytesToRead) {
                        std::cout << "Раздел успешно считан в буфер" << std::endl;

                        if (WriteDataFromBufferToFile(dataBuffer, bytesToRead, "partition.bin")) {
                            std::cout << "Буффер записан в файл" << std::endl;
                        }
                    }
                    else {
                        std::cout << "Произошла ошибка при считывании раздела в буфер" << std::endl;
                        ErrorCheck();
                    }

                    delete[] dataBuffer;
                }
                else {
                    std::cout << "Ошибка позиционирования" << std::endl;
                    ErrorCheck();
                }
                delete PartitionInfo;
            } */
        }
        else {
            std::cout << "Ошибка получения размера сектора хранилища" << std::endl;
            ErrorCheck();
        }

        CloseStorageHandle(storage);
    }
    else {
        std::cout << "Произошла ошибка при создании дескриптора хранилища" << std::endl;
        ErrorCheck();
    }
}



bool isFirstRun = true;

// Функция для записи ошибок в лог-файл
void ErrorCheck()
{
    if (HasError())
    {
        WCHAR ErrorString[1024];
        GetLastErrorStringW(ErrorString);

        std::ofstream logFile;
        if (isFirstRun)
        {
            logFile.open("logfile.txt", std::ios::out | std::ios::trunc | std::ios::binary); // Очистка файла при первом запуске
            isFirstRun = false;
        }
        else
        {
            logFile.open("logfile.txt", std::ios::out | std::ios::app | std::ios::binary);
        }

        if (logFile.is_open())
        {
            // Конвертация wchar_t* в std::string с кодировкой UTF-8
            int utf8Size = WideCharToMultiByte(CP_UTF8, 0, ErrorString, -1, nullptr, 0, nullptr, nullptr);
            if (utf8Size > 0)
            {
                std::string utf8ErrorString(utf8Size, 0);
                WideCharToMultiByte(CP_UTF8, 0, ErrorString, -1, &utf8ErrorString[0], utf8Size, nullptr, nullptr);

                // Запись ошибки в файл
                logFile << utf8ErrorString << std::endl;
            }
            logFile.close();
        }
        else
        {
            std::cerr << "Ошибка открытия файла лога" << std::endl;
        }
    }
}

// Напечатать тип хранилища
void PrintStorageType(StorageType type) {
    switch (type) {
    case StorageType::LogicalDrive: std::cout << "LogicalDrive" << std::endl; break;
    case StorageType::PhysicalDrive: std::cout << "PhysicalDrive" << std::endl; break;
    case StorageType::ImageFile: std::cout << "ImageFile" << std::endl; break;
    case StorageType::DataStream: std::cout << "DataStream" << std::endl; break;
    case StorageType::DirectoryFile: std::cout << "DirectoryFile" << std::endl; break;
    case StorageType::RAIDx: std::cout << "RAIDx" << std::endl; break;
    case StorageType::VMDK: std::cout << "VMDK" << std::endl; break;
    case StorageType::VDI: std::cout << "VDI" << std::endl; break;
    case StorageType::VHD: std::cout << "VHD" << std::endl; break;
    case StorageType::VHDX: std::cout << "VHDX" << std::endl; break;
    case StorageType::QEMU: std::cout << "QEMU" << std::endl; break;
    case StorageType::HDD: std::cout << "HDD" << std::endl; break;
    case StorageType::ErrorType: std::cout << "ErrorType" << std::endl; break;
    case StorageType::UnknownType: std::cout << "UnknownType" << std::endl; break;
    }
}

// Напечатать тип файловой системы
void PrintFileSystemType(FileSystemTypeEnum type) {
    switch (type) {
    case FileSystemTypeEnum::NTFS:        std::cout << "NTFS"; break;
    case FileSystemTypeEnum::FAT12:       std::cout << "FAT12"; break;
    case FileSystemTypeEnum::FAT16:       std::cout << "FAT16"; break;
    case FileSystemTypeEnum::FAT32:       std::cout << "FAT32"; break;
    case FileSystemTypeEnum::exFAT:       std::cout << "exFAT"; break;
    case FileSystemTypeEnum::Ext2:        std::cout << "Ext2"; break;
    case FileSystemTypeEnum::Ext3:        std::cout << "Ext3"; break;
    case FileSystemTypeEnum::Ext4:        std::cout << "Ext4"; break;
    case FileSystemTypeEnum::DFS:         std::cout << "DFS"; break;
    case FileSystemTypeEnum::MBR:         std::cout << "MBR"; break;
    case FileSystemTypeEnum::PMBR:        std::cout << "PMBR (защитный MBR для GPT)"; break;
    case FileSystemTypeEnum::EMBR:        std::cout << "EMBR (расширенный MBR)"; break;
    case FileSystemTypeEnum::GPT:         std::cout << "GPT"; break;
    case FileSystemTypeEnum::GPT_4K:      std::cout << "GPT_4K"; break;
    case FileSystemTypeEnum::PTFS:        std::cout << "PTFS (виртуальные файловые системы)"; break;
    case FileSystemTypeEnum::FS_Custom:   std::cout << "Custom FS"; break;
    case FileSystemTypeEnum::FS_None:     std::cout << "No FS"; break;
    case FileSystemTypeEnum::Encrypted:   std::cout << "Encrypted"; break;
    case FileSystemTypeEnum::FS_Error:    std::cout << "Error"; break;
    case FileSystemTypeEnum::FS_Debug:    std::cout << "Debug"; break;
    case FileSystemTypeEnum::VMDK:        std::cout << "VMDK"; break;
    case FileSystemTypeEnum::VDI:         std::cout << "VDI"; break;
    case FileSystemTypeEnum::VHD:         std::cout << "VHD"; break;
    case FileSystemTypeEnum::VHDX:        std::cout << "VHDX"; break;
    case FileSystemTypeEnum::QEMU:        std::cout << "QEMU"; break;
    case FileSystemTypeEnum::HDD:         std::cout << "HDD (Parallels)"; break;
    case FileSystemTypeEnum::TAR:         std::cout << "TAR"; break;
    default:                              std::cout << "Unknown"; break;
    }
}

//Функция для записи данных в файл
bool WriteDataFromBufferToFile(const BYTE* dataBuffer, LONGLONG bufferSize, const std::string& fileName) {
    std::ofstream outFile(fileName, std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(dataBuffer), bufferSize);

        if (!outFile.fail()) {
            return true;
        }
        else {
            std::cout << "Произошла ошибка записи буфера в файл" << std::endl;
            return false;
        }
        outFile.close();
    }
    else {
        std::cerr << "Ошибка открытия файла для записи" << std::endl;
        return false;
    }
}

int Dir(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject)) // Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject); // Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle)) // Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath); // Запоминаем путь к каталогу через массив

                // Убираем лишние символы из пути каталога
                if (fullPath[3] == '.' && fullPath[4] == '\\')
                {
                    int i = 2;
                    int j = 0;
                    while (i < fullPathLength + 1)
                    {
                        fullPath[j] = fullPath[i];
                        i++;
                        j++;
                    }
                }

                int fileNameLength = GetFileNameW(dirHandle, NULL);
                WCHAR* fileName = new WCHAR[fileNameLength + 1];
                GetFileNameW(dirHandle, fileName); // Запоминаем имя каталога через массив

                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.')) // Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath); // Открываем каталог
                    if (DirFileIsDir(dirHandle)) // Если нашли каталог в каталоге
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); // Чтение атрибутов файла

                        if (attrStr[1] != 'h') // Проверка атрибутов файла 
                        {
                            NumberOfDir++;
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName); // Запоминаем имя каталога через массив

                            if ((fileName[1] != '.') || (fileName[1] != '.' && fileName[1] != '.'))
                            {
                                std::wcout << fullPath << L" : " << std::endl; // Выводим путь как wstring
                                Dir(currentFileObject, fileSystem); // Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }
                    else // Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); // Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            NumberOfFile++;
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];


                            GetFileNameW(dirHandle, fileName); // Запоминаем имя файла через массив

                    

                            //std::wcout << L"\t" << fileName << L"  " << std::endl; // Выводим имя файла как wstring
                            //std::cout << "\t" << fileNameChar << " " << std::endl;
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

void DirTest()
{
    // Путь до файла образа или корневой каталог
    
    std::cout << std::endl;
    std::cout << "ТЕСТОВЫЙ СЦЕНАРИЙ ПОЛНОТА ОБХОДА ФАЙЛОВ И КАТАЛОГОВ" << std::endl << std::endl;

    std::cout << "Введите путь до файла образа или корневой каталог в формате D:\\" << std::endl;

    std::wstring imageStoragePath;

    std::wcin >> imageStoragePath;
  
    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) {
        std::cout << "Ошибка открытия файла-образа!" << std::endl;
        ErrorCheck();
        return;
    }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;

    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        std::cout << "Ошибка получения информации о разделе!" << std::endl;
        ErrorCheck();
        return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) {
        std::cout << "Ошибка задания размера блока!" << std::endl;
        ErrorCheck();
        return;
    }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        std::cout << "Файловая система не распознана!" << std::endl;
        ErrorCheck();
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        std::cout << "Ошибка открытия файловой системы!" << std::endl;
        ErrorCheck();
        CloseStorageHandle(dataStorage);
        return;
    }
    else {
        std::cout << "Файловая система открыта!" << std::endl;
    }

    // Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");

    if (fileObject != FILE_OBJECT_ERROR) {
        std::cout << "Файловый объект открыт" << std::endl;
    }
    else {
        std::cout << "Файловый объект НЕ открыт" << std::endl;
        ErrorCheck();
        return;
    }

    // Проверка, является ли файл директорией
    if (FileIsDir(fileObject)) {
        Dir(fileObject, fileSystem);

        std::cout << "-------------------------------" << std::endl;
        std::cout << "Количество каталогов: " << NumberOfDir << std::endl;
        std::cout << "Количество файлов: " << NumberOfFile << std::endl;
        //std::cout << "Всего файловых объектов: " << NumberOfDir + NumberOfFile << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }

    CloseFileHandle(fileObject);
    CloseFileSystemHandle(fileSystem);
    CloseStorageHandle(dataStorage);
}


#endif
