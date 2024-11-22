#include <tchar.h>
#include <iostream>
#include "DataAccessLibConnect.h"
#include <string.h>
#include <fstream>
#include <codecvt>
#include <chrono>
#include <locale>
#include <set>
#include <nlohmann/json.hpp>


using namespace std;
using namespace std::chrono;
using json = nlohmann::json;


int NumberOfKeyWords = 0;
int MaxObj = 0;
int MinObj = 0;
int FileForCopy = 0;
int FileAfterCopy = 0;
int TimeError = 0;
int ManyeStreamesFile = 0;
int HashFlag = 0;




string filename = "output.json";
nlohmann::json storageTest;


void ErrorCheck(const std::string& functionName);
vector<int> StorageTest(wstring StoragePath);

void PrintStorageType(StorageType type);
void PrintFileSystemType(FileSystemTypeEnum type);

void DirTest(vector<int> partitions, wstring imageStoragePath, json config);
int Dir(FileHandle fileObject, FileSystemHandle fileSystem, int& NumberOfFile, int& NumberOfDir);

void FragmentTest(vector<int> partitions, wstring imageStoragePath);
int Fragment(FileHandle fileObject, FileSystemHandle fileSystem);

void MaxTest(vector<int> partitions, wstring imageStoragePath);
int Max(FileHandle fileObject, FileSystemHandle fileSystem);

void CopyTest();
int Copy(FileHandle fileObject, FileSystemHandle fileSystem);
void AfterCopyTest();
int AfterCopy(FileHandle fileObject, FileSystemHandle fileSystem);

void TimeTest();
int Time(FileHandle fileObject, FileSystemHandle fileSystem);

int Stream(FileHandle fileObject, FileSystemHandle fileSystem);
void StreamTest();

void HashTest();
int Hash(FileHandle fileObject, FileSystemHandle fileSystem, string HashSum);


int main()
{

    setlocale(LC_ALL, "russian");

    std::string diskImagePath;
    std::wcout << L"Введите путь к образу диска: ";
    std::getline(std::cin, diskImagePath);

    // Конвертация пути в формат для Windows (если необходимо, вы можете оставить как есть для других ОС)
    std::wstring storagePath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(diskImagePath);

    // Запрос пути к JSON-файлу с информацией о заполнении
    std::string jsonFilePath;
    std::cout << "Введите путь к JSON-файлу с данными о заполнении: ";
    std::getline(std::cin, jsonFilePath);

    /// Открытие JSON файла
    std::ifstream configFile(jsonFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Не удалось открыть файл конфигурации: " << jsonFilePath << std::endl;
        return 1;
    }

    /// Загрузка содержимого JSON в объект
    json config;

    try {
        configFile >> config;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при разборе JSON: " << e.what() << std::endl;
        return 1;
    }

    configFile.close();


    if (DataAccessLibInit()) {

        wcout << L"Библиотека открыта успешно" << endl;

        vector<int> partitions = StorageTest(storagePath);
        
        cout << endl;
        wcout << L"Введите без пробела номера сценариев, которые хотите запустить: " << endl;
        wcout << L"1 - Оценка охвата каталогов и файлов " << endl;
        //wcout << L"2 - Поиск ключевого слова в фрагментированных файлах " << endl;
        //wcout << L"3 - Поиск максимального и минимального кол-во объектов в каталоге в файловой системе " << endl;
        //wcout << L"4 - Копирование объектов с сохранением структуры каталогов из гостевой в основную файловую систему" << endl;
        //wcout << L"5 - Счет ошибок при чтении временных меток файла " << endl;
        //wcout << L"6 - Поиск файлов имеющих более одного потока данных " << endl;
        //wcout << L"7 - Поиск файла по его хэшу " << endl;
        //wcout << L"F - Запуск всех сценариев " << endl;
        string test;
        cin >> test;
        for (int i = 0; i < size(test); i++)
        {
            switch (test[i]) 
            {
            case '1': DirTest(partitions, storagePath, config); break;
            //case '2': FragmentTest(partitions, storagePath); break;
            //case '3': MaxTest(partitions, storagePath); break;
            //case '4': CopyTest(); break;
            //case '5': TimeTest(); break;
            //case '6': StreamTest(); break;
            //case '7': HashTest(); break;
            //case 'F': DirTest(partitions, storagePath, config), FragmentTest(partitions, storagePath), MaxTest(partitions, storagePath), CopyTest(), TimeTest(), StreamTest(), HashTest(); break;
            default: wcout << L"Неверный символ: " << test[i] << endl;
            }
        }
    }
    else {
        wcout << L"Произошла ошибка при открытии библиотеки" << endl;
    }

    DataAccessLibCleanup();

    system("pause");

    return 0;
}
/////////////////////////////////////////////////////////////////////ПРОВЕРКА ОШИБОК\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

bool isFirstRun = true;

// Функция для конвертации строки wchar_t в UTF-8
std::string ConvertToUtf8(const std::wstring& wstr) {
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Size > 0) {
        std::string utf8Str(utf8Size, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], utf8Size, nullptr, nullptr);
        return utf8Str;
    }
    return "";
}

// Функция для записи ошибок в лог-файл
void ErrorCheck(const std::string& functionName) {
    if (HasError()) {  // Пример функции, которая проверяет наличие ошибки
        WCHAR ErrorStringW[1024];
        GetLastErrorStringW(ErrorStringW);  // Получаем строку ошибки

        std::ofstream logFile;
        if (isFirstRun) {
            logFile.open("errorFile.txt", std::ios::out | std::ios::trunc | std::ios::binary);
            const unsigned char bom[] = { 0xEF, 0xBB, 0xBF }; // BOM для UTF-8
            logFile.write(reinterpret_cast<const char*>(bom), sizeof(bom)); // Записываем BOM
            isFirstRun = false;
        }
        else {
            logFile.open("errorFile.txt", std::ios::out | std::ios::app | std::ios::binary);
        }

        if (logFile.is_open()) {
            // Конвертируем "В функции" и "произошла ошибка" в UTF-8
            std::string utf8Prefix = "In function ";
            std::string utf8Suffix = " error: ";
            std::string utf8Error = ConvertToUtf8(ErrorStringW);
            std::string utf8FunctionName = ConvertToUtf8(std::wstring(functionName.begin(), functionName.end()));

            // Запись в файл
            logFile << utf8Prefix << utf8FunctionName << utf8Suffix << utf8Error << std::endl;
            logFile.close();
        }
        else {
            std::wcerr << L"Ошибка открытия файла лога ошибок" << std::endl;
        }
    }
}


/////////////////////////////////////////////////////////////////////ХРАНИЛИЩЕ\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

vector<int> StorageTest(wstring StoragePath) {

    StorageHandle storage = CreateStorageHandle(StoragePath.c_str());

    vector<int> partitionsWithFS;

    if (storage == STORAGE_ERROR) {
        wcerr << L"Ошибка открытия файла-образа!" << endl; 
        ErrorCheck("CreateStorageHandle");
        return partitionsWithFS;
    }

    StorageType storageType;
    storageType = GetStorageType(storage);

    if (storageType == StorageType::ErrorType) {
        wcerr << L"Ошибка получения типа хранилища!" << endl;
        ErrorCheck("GetStorageType");
        return partitionsWithFS;
    }

    ///Если тип хранилища - физическое устройство, то выводим информацию о нем
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
            ErrorCheck("GetPhysicalDriveInfo");
            return partitionsWithFS;
        }
    }

    std::wcout << L"Тип хранилища: ";
    PrintStorageType(storageType);

    int NumberOfPartitions = GetNumberOfPartitions(storage);
    if (NumberOfPartitions < 0) {
        wcerr << L"Ошибка при получении числа разделов хранлища" << endl;
        ErrorCheck("GetNumberOfPartitions");
    }

    std::wcout << L"Число разделов хранилища: " << NumberOfPartitions << std::endl;

    PartitionInfoStruct* PartitionInfo = new PartitionInfoStruct;


    if (!SetStorageBlockSize(storage, DefaultSectorSize)) {
        wcerr << L"Ошибка задания размера блока!" << endl;
        ErrorCheck("SetStorageBlockSize");
        return partitionsWithFS;
    }

    unsigned __int64 totalSizeSectors = 0;

    ///коллекция файловых систем, для которых дальнейшие сценарии не могут быть реализованы
    static const set<FileSystemTypeEnum> excludedFileSystems = {
        FileSystemTypeEnum::FS_None,
        FileSystemTypeEnum::FS_Error,
        FileSystemTypeEnum::FS_Debug,
        FileSystemTypeEnum::MBR,
        FileSystemTypeEnum::PMBR,
        FileSystemTypeEnum::EMBR,
        FileSystemTypeEnum::GPT,
        FileSystemTypeEnum::GPT_4K,
        FileSystemTypeEnum::PTFS
    };


    ///Цикл получения информации о разделах
    for (unsigned int i = 1; i <= NumberOfPartitions; i++) {
        GetPartitionInfoByNumber(storage, i, PartitionInfo);

        std::wcout << L"Номер раздела: " << i << std::endl;
        std::wcout << L"Файловая система: ";

        FileSystemTypeEnum fs = PartitionInfo->RecognizedType;

        PrintFileSystemType(fs);
        std::wcout << L" || ";

        FileSystemTypeEnum type = RecognizeFileSystem(storage, PartitionInfo->FirstSector * DefaultSectorSize);

        PrintFileSystemType(type);
        std::wcout << L"(определено по сигнатуре)" << std::endl;

        std::wcout << L"Идентификатор типа раздела: " << PartitionInfo->RawType << std::endl;
        std::wcout << L"Первый сектор: " << PartitionInfo->FirstSector << std::endl;
        std::wcout << L"Последний сектор: " << PartitionInfo->LastSector << std::endl;
        std::wcout << L"Размер в секторах: " << PartitionInfo->SizeInSectors << std::endl;

        totalSizeSectors += PartitionInfo->SizeInSectors;

        std::wcout << L"GUID типа раздела: " << PartitionInfo->TypeGuid << std::endl;
        std::wcout << L"Флаги раздела: " << PartitionInfo->Flags << std::endl;
        std::wcout << L"Имя: " << PartitionInfo->Name << std::endl;

        std::wcout << L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

        /// Проверка для записи раздела, как доступного к тестированию
        if (excludedFileSystems.find(fs) == excludedFileSystems.end()) {
            partitionsWithFS.push_back(i);
        }
    }

    delete PartitionInfo;
    std::cout << std::endl;


    ///Создание итератора последовательного обхода секторов хранилища
    StorageIteratorHandle it = GetStorageIterator(storage);
    if (it == STORAGE_ERROR) {
        wcerr << L"ошибка создания итератора хранилища" << endl;
        ErrorCheck("GetStorageIterator");
        return partitionsWithFS;
    }

    int k = 0;
    for (StorageBlockFirst(it); !StorageBlockIsDone(it); StorageBlockNext(it)) {
        k++;
    }

    ///Сравнение количества секторов 
    if (totalSizeSectors != k) {
        wcerr << L"Количество секторов пройденное итератором хранилища не совпадает с реальным количеством секторов" << endl;
    }

    return partitionsWithFS;

}



void PrintStorageType(StorageType type) {

    switch (type) {
    case StorageType::LogicalDrive: std::wcout << L"LogicalDrive" << std::endl; break;
    case StorageType::PhysicalDrive: std::wcout << L"PhysicalDrive" << std::endl; break;
    case StorageType::ImageFile: std::wcout << L"ImageFile" << std::endl; break;
    case StorageType::DataStream: std::wcout << L"DataStream" << std::endl; break;
    case StorageType::DirectoryFile: std::wcout << L"DirectoryFile" << std::endl; break;
    case StorageType::RAIDx: std::wcout << L"RAIDx" << std::endl; break;
    case StorageType::VMDK: std::wcout << L"VMDK" << std::endl; break;
    case StorageType::VDI: std::wcout << L"VDI" << std::endl; break;
    case StorageType::VHD: std::wcout << L"VHD" << std::endl; break;
    case StorageType::VHDX: std::wcout << L"VHDX" << std::endl; break;
    case StorageType::QEMU: std::wcout << L"QEMU" << std::endl; break;
    case StorageType::HDD: std::wcout << L"HDD" << std::endl; break;
    case StorageType::ErrorType: std::wcout << L"ErrorType" << std::endl; break;
    case StorageType::UnknownType: std::wcout << L"UnknownType" << std::endl; break;

    }
}


void PrintFileSystemType(FileSystemTypeEnum type) {
    switch (type) {
    case FileSystemTypeEnum::NTFS:        std::wcout << L"NTFS"; break;
    case FileSystemTypeEnum::FAT12:       std::wcout << L"FAT12"; break;
    case FileSystemTypeEnum::FAT16:       std::wcout << L"FAT16"; break;
    case FileSystemTypeEnum::FAT32:       std::wcout << L"FAT32"; break;
    case FileSystemTypeEnum::exFAT:       std::wcout << L"exFAT"; break;
    case FileSystemTypeEnum::Ext2:        std::wcout << L"Ext2"; break;
    case FileSystemTypeEnum::Ext3:        std::wcout << L"Ext3"; break;
    case FileSystemTypeEnum::Ext4:        std::wcout << L"Ext4"; break;
    case FileSystemTypeEnum::DFS:         std::wcout << L"DFS"; break;
    case FileSystemTypeEnum::MBR:         std::wcout << L"MBR"; break;
    case FileSystemTypeEnum::PMBR:        std::wcout << L"PMBR (защитный MBR для GPT)"; break;
    case FileSystemTypeEnum::EMBR:        std::wcout << L"EMBR (расширенный MBR)"; break;
    case FileSystemTypeEnum::GPT:         std::wcout << L"GPT"; break;
    case FileSystemTypeEnum::GPT_4K:      std::wcout << L"GPT_4K"; break;
    case FileSystemTypeEnum::PTFS:        std::wcout << L"PTFS (виртуальные файловые системы)"; break;
    case FileSystemTypeEnum::FS_Custom:   std::wcout << L"Custom FS"; break;
    case FileSystemTypeEnum::FS_None:     std::wcout << L"No FS"; break;
    case FileSystemTypeEnum::Encrypted:   std::wcout << L"Encrypted"; break;
    case FileSystemTypeEnum::FS_Error:    std::wcout << L"Error"; break;
    case FileSystemTypeEnum::FS_Debug:    std::wcout << L"Debug"; break;
    case FileSystemTypeEnum::VMDK:        std::wcout << L"VMDK"; break;
    case FileSystemTypeEnum::VDI:         std::wcout << L"VDI"; break;
    case FileSystemTypeEnum::VHD:         std::wcout << L"VHD"; break;
    case FileSystemTypeEnum::VHDX:        std::wcout << L"VHDX"; break;
    case FileSystemTypeEnum::QEMU:        std::wcout << L"QEMU"; break;
    case FileSystemTypeEnum::HDD:         std::wcout << L"HDD (Parallels)"; break;
    case FileSystemTypeEnum::TAR:         std::wcout << L"TAR"; break;
    default:                              std::wcout << L"Unknown"; break;
    }
}



/////////////////////////////////////////////////////////////////////ОХВАТ КАТАЛОГО И ФАЙЛОВ\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
int Dir(FileHandle fileObject, FileSystemHandle fileSystem, int& NumberOfFile, int& NumberOfDir)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle ); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                //!В этот момент происходит ошибочное чтение пути к каталогу и функция GetFullPathW() записывает в массив fullPath некоректный путь с \.\.\ 
                GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив
                //После записи в массив fullPath некоректного пути приходится убирать лишние символы:
               //Убираем лишние символы из пути каталога
                if (fullPath[3] =='.' && fullPath[4] == '\\')
                {
                    int i = 2;
                    int j = 0;
                    while (i < fullPathLength+1)
                    {
                        fullPath[j] = fullPath[i];
                        i++;
                        j++;
                    }
                }
                
                int fileNameLength = GetFileNameW(dirHandle, NULL);
                WCHAR* fileName = new WCHAR[fileNameLength + 1];
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {

                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        //wcout << L"Атрибуты: "<< attrStr << endl;
                        if (attrStr[1] != 'h' || attrStr[2] != 's')//Проверка атрибутов файла 
                        {
                            NumberOfDir++;
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                //wcout << fileName << L" : " << endl;

                                //Производится вывод пути каталога
                                //wcout << fullPath << L" : " << endl;

                                Dir(currentFileObject, fileSystem, NumberOfFile, NumberOfDir);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            NumberOfFile++;
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            //wcout << "\t" << fileName << L"  " << endl;
                            //cout << attrStr << endl;
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

void DirTest(vector<int> partitions, wstring imageStoragePath, json config)
{

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    int partitionInJson = 1;

    for (const auto it : partitions) {
        unsigned int selectedNumber = it;
        
        int NumberOfFile = 0;
        int NumberOfDir = 0;

        PartitionInfoStruct* selectedPartitionInfo = new PartitionInfoStruct;
        if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, selectedPartitionInfo)) {
            wcout << L"Ошибка получения информации о разделе!" << endl;	return;
        }

        /// Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
        if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

        ULONGLONG startOffset = selectedPartitionInfo->FirstSector * DefaultSectorSize;
        ULONGLONG partitionSize = selectedPartitionInfo->SizeInSectors * DefaultSectorSize;

        /// Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
        FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

        if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
            wcout << L"Файловая система не распознана!" << endl;
            CloseStorageHandle(dataStorage);
            return;
        }

        /// Открытие файловой системы
        FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

        if (fileSystem == FILESYSTEM_ERROR) {
            wcout << L"Ошибка открытия файловой системы!" << endl;
            CloseStorageHandle(dataStorage);
            return;
        }




        /// Открытие файла (создание дискриптора)
        FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");

        if (fileObject != FILE_OBJECT_ERROR)
        {
            /// Проверка, является ли файл директорией
            if (FileIsDir(fileObject))
            {
                Dir(fileObject, fileSystem, NumberOfFile, NumberOfDir);

                cout << "------------------------------------------------------------" << endl;
                wcout << L"Файловая система ";
                PrintFileSystemType(fsType); cout << endl;
                wcout << L"Количество каталогов: " << NumberOfDir << endl;
                wcout << L"Количество файлов: " << NumberOfFile << endl;

                //wcout << L"Всего файловых объектов: " << NumberOfDir + NumberOfFile << endl;

                /// Получение данных из переданного JSON
                std::string partitionKey = "Partition " + std::to_string(partitionInJson);
                if (config.contains(partitionKey)) {
                    const json& partitionData = config[partitionKey];

                    /// Извлечение данных из JSON для DirTest
                    int directoryCount = partitionData["DirTest"]["directoryCount"];
                    int filesCount = partitionData["DirTest"]["filesCount"];

                    cout << endl;
                    wcout << L"Потеря файлов: " << filesCount - NumberOfFile << endl;
                    wcout << L"Потеря каталогов: " <<  directoryCount - NumberOfDir << endl;
                }
                else {
                    wcout << L"Нет данных для раздела " << partitionInJson << L" в JSON!" << endl;
                }
            }
        }
        else
        {
            wcout << L"Файловый объект НЕ открыт" << endl;
        }

        partitionInJson++;

        delete selectedPartitionInfo;
        CloseFileHandle(fileObject);
        CloseFileSystemHandle(fileSystem);
    }

    CloseStorageHandle(dataStorage);
}



/////////////////////////////////////////////////////////////////////ФРАГМЕНТИРОВАННЫЕ ФАЙЛЫ\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
int Fragment(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {

        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);
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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {
                       
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                Fragment(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            if (fileName[fileNameLength -3] == 't' && fileName[fileNameLength -2] == 'x' && fileName[fileNameLength -1] == 't')
                            {
                                LONGLONG fileDataSize = GetFileDataSize(currentFileObject, 0);
                                BYTE* dataBuffer = new BYTE[fileDataSize];
                                SetFilePosition(currentFileObject, 0, 0, OriginEnum::Begin);
                                ULONGLONG readSize = ReadFileData(currentFileObject, fileDataSize, dataBuffer);
                                wstring KeyWord(L"Hello");
                                int i = 0;
                                int j = 0;
                                while (i < fileDataSize)
                                {
                                    if (KeyWord[j] == dataBuffer[i])
                                    {
                                        j++;
                                        i++;
                                        if (j == size(KeyWord))
                                        {
                                            NumberOfKeyWords++;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        i++;
                                        j = 0;
                                    }
                                }
                                delete[] dataBuffer;
                            }
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }

    }
    return 0;
}
void FragmentTest(vector<int> partitions, wstring imageStoragePath)
{
    
    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    for (const auto it : partitions) {
        unsigned int selectedNumber = it;
        PartitionInfoStruct selectedPartitionInfo;
        if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
            wcout << L"Ошибка получения информации о разделе!" << endl;	return;
        }

        // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
        if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

        ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
        ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

        // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
        FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

        if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
            wcout << L"Файловая система не распознана!" << endl;
            CloseStorageHandle(dataStorage);
            return;
        }

        // Открытие файловой системы
        FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

        if (fileSystem == FILESYSTEM_ERROR) {
            wcout << L"Ошибка открытия файловой системы!" << endl;
            CloseStorageHandle(dataStorage); 	return;
        }
        else
        {
            wcout << L"Файловая система открыта!" << endl;
        }

        //Открытие файла (создание дискриптора)
        FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");

        if (fileObject != FILE_OBJECT_ERROR)
        {
            wcout << L"Файловый объект открыт" << endl;
            Fragment(fileObject, fileSystem);
            wcout << L"Кол-во файлов с ключевыми словами: " << NumberOfKeyWords << endl;

        }
        else
        {
            wcout << L"Файловый объект НЕ открыт" << endl;
        }


        CloseFileHandle(fileObject);
        CloseFileSystemHandle(fileSystem);
    }
    CloseStorageHandle(dataStorage);
}


/////////////////////////////////////////////////////////////////////МАКСИМАЛЬНОЕ/МИНИМАЛЬНОЕ КОЛИЧЕСТВО ОБЪЕКТОВ В КАТАЛОГЕ\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
int Max(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);
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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                int NewMax = GetNumberOfFiles(fileObject)-2;
                                if (MaxObj < NewMax)
                                {
                                    MaxObj = NewMax;
                                }
                                if (MinObj > NewMax)
                                {
                                    MinObj = NewMax;
                                }
                       
                                Max(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }

    }
    return 0;
}
void MaxTest(vector<int> partitions, wstring imageStoragePath)
{
    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    for (const auto it : partitions) {
        unsigned int selectedNumber = it;
        PartitionInfoStruct selectedPartitionInfo;
        if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
            wcout << L"Ошибка получения информации о разделе!" << endl;	return;
        }

        // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
        if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

        ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
        ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

        // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
        FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

        if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
            wcout << L"Файловая система не распознана!" << endl;
            CloseStorageHandle(dataStorage);
            return;
        }

        // Открытие файловой системы
        FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

        if (fileSystem == FILESYSTEM_ERROR) {
            wcout << L"Ошибка открытия файловой системы!" << endl;
            CloseStorageHandle(dataStorage); 	return;
        }
        else
        {
            wcout << L"Файловая система открыта!" << endl;
        }

        //Открытие файла (создание дискриптора)
        FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");

        if (fileObject != FILE_OBJECT_ERROR)
        {
            wcout << L"Файловый объект открыт" << endl;
            Max(fileObject, fileSystem);
            wcout << L"Максимальное количество объектов в каталоге: " << MaxObj << endl;
            wcout << L"Минимальное количество объектов в каталоге: " << MinObj << endl;

            ifstream inFile(filename);
            if (inFile.is_open()) {
                // Считываем JSON из файла
                inFile >> storageTest;
                inFile.close();
                // Извлечение данных из JSON
                int MaxCount = storageTest["MaxTest"]["MaxCount"];
                int MinCount = storageTest["MaxTest"]["MinCount"];

                wcout << L"Разница максимумов: " << MaxObj - MaxCount << endl;
                wcout << L"Разница минимумов: " << MinObj - MinCount << endl;
            }
            else {
                cerr << "Не удалось открыть файл json" << filename << " для чтения!" << endl;
            }
        }
        else
        {
            wcout << L"Файловый объект НЕ открыт" << endl;
        }


        CloseFileHandle(fileObject);
        CloseFileSystemHandle(fileSystem);
    }
    CloseStorageHandle(dataStorage);
}

/////////////////////////////////////////////////////////////////////КОПИРОВАНИЕ ФАЙЛОВ\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

int Copy(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив

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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                if (!SaveFileWithPath(currentFileObject, L"C:\\"))
                                {
                                    wcout << L"Ошибка \"быстрого\" сохранения каталога!" << endl;
                                }
                                else
                                {
                                    FileForCopy++;
                                }
                                Copy(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            if (!SaveFileWithPath(currentFileObject, L"E:\\Test"))//Куда копируем
                            {
                                wcout << L"Ошибка \"быстрого\" сохранения каталога!" << endl;
                            }
                            else
                            {
                                FileForCopy++;
                            }
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

int AfterCopy(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив

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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                FileAfterCopy++;
                                AfterCopy(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            FileAfterCopy++;
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

void AfterCopyTest()
{
    //Путь до фйла образа или корневой каталог
    wstring imageStoragePath(L"E:\\Test");

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;
    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        wcout << L"Ошибка получения информации о разделе!" << endl;	return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        wcout << L"Файловая система не распознана!" << endl;
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        wcout << L"Ошибка открытия файловой системы!" << endl;
        CloseStorageHandle(dataStorage); 	return;
    }
    //Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");//Откуда копируем

    if (fileObject != FILE_OBJECT_ERROR)
    {
        AfterCopy(fileObject, fileSystem);

    }
    else
    {
        wcout << L"Файловый объект НЕ открыт" << endl;
    }


    CloseFileHandle(fileObject);
    CloseFileSystemHandle(fileSystem);
    CloseStorageHandle(dataStorage);
}

void CopyTest()
{
    //Путь до фйла образа или корневой каталог
    cout << "Введите путь до фйла образа или корневой каталог: " << endl;
    wstring imageStoragePath;
    wcin >> imageStoragePath;

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;
    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        wcout << L"Ошибка получения информации о разделе!" << endl;	return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        wcout << L"Файловая система не распознана!" << endl;
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        wcout << L"Ошибка открытия файловой системы!" << endl;
        CloseStorageHandle(dataStorage); 	return;
    }
    else
    {
        wcout << L"Файловая система открыта!" << endl;
    }

    //Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.\\Second1");//Откуда копируем

    if (fileObject != FILE_OBJECT_ERROR)
    {
        wcout << L"Файловый объект открыт" << endl;
        Copy(fileObject, fileSystem);
        wcout << L"Скопированно объектов: " << FileForCopy<< endl;
        AfterCopyTest();
        wcout << L"Проверка: " << FileAfterCopy << endl;

    }
    else
    {
        wcout << L"Файловый объект НЕ открыт" << endl;
    }


    CloseFileHandle(fileObject);
    CloseFileSystemHandle(fileSystem);
    CloseStorageHandle(dataStorage);
}

/////////////////////////////////////////////////////////////////////ЧТЕНИЕ ВРЕМЕННЫХ МЕТОК\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
int Time(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив

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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {

                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {

                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                char timeStampCreate[40], timeStampModify[40], timeStampAccess[40];
                                ULONGLONG timeCreate = GetTimeFileCreate(currentFileObject, timeStampCreate,1, 0);
                                ULONGLONG timeModify = GetTimeFileModify(currentFileObject, timeStampModify, 1, 0);
                                ULONGLONG timeAccess = GetTimeFileAccess(currentFileObject, timeStampAccess, 1, 0);
                                //wcout << L"Время создания: " << timeStampCreate << endl;
                                //wcout << L"Время изменения: " << timeStampModify << endl;
                                //wcout << L"Время доступа: " << timeStampAccess << endl;
                                if (timeCreate == 0 || timeModify == 0 || timeAccess == 0)
                                {
                                    wcout << L"Ошибка чтения меток каталога: " << fullPath << endl;
                                    TimeError++;
                                    wcout << L"Время создания: " << timeStampCreate << endl;
                                    wcout << L"Время изменения: " << timeStampModify << endl;
                                    wcout << L"Время доступа: " << timeStampAccess << endl;
                                }
                                Time(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            char timeStampCreate[40], timeStampModify[40], timeStampAccess[40];
                            ULONGLONG timeCreate = GetTimeFileCreate(currentFileObject, timeStampCreate, 1, 0);
                            ULONGLONG timeModify = GetTimeFileModify(currentFileObject, timeStampModify, 1, 0);
                            ULONGLONG timeAccess = GetTimeFileAccess(currentFileObject, timeStampAccess, 1, 0);
                            //wcout << L"Время создания: " << timeStampCreate << endl;
                            //wcout << L"Время изменения: " << timeStampModify << endl;
                            //wcout << L"Время доступа: " << timeStampAccess << endl;
                            if (timeCreate == 0 || timeModify == 0 || timeAccess == 0)
                            {
                                wcout << L"Ошибка чтения меток файла: " << fullPath <<  endl;
                                TimeError++;
                                wcout << L"Время создания: " << timeStampCreate << endl;
                                wcout << L"Время изменения: " << timeStampModify << endl;
                                wcout << L"Время доступа: " << timeStampAccess << endl;
                            }
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

void TimeTest()
{
    //Путь до фйла образа или корневой каталог
    cout << "Введите путь до файла образа или корневой каталог: " << endl;
    wstring imageStoragePath;
    wcin >> imageStoragePath;

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;
    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        wcout << L"Ошибка получения информации о разделе!" << endl;	return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        wcout << L"Файловая система не распознана!" << endl;
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        wcout << L"Ошибка открытия файловой системы!" << endl;
        CloseStorageHandle(dataStorage); 	return;
    }
    else
    {
        wcout << L"Файловая система открыта!" << endl;
    }

    //Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");

    if (fileObject != FILE_OBJECT_ERROR)
    {
        wcout << L"Файловый объект открыт" << endl;
        //Проверка, является ли файл директорией
        if (FileIsDir(fileObject))
        {
            Time(fileObject, fileSystem);
            wcout << L"Кол-во ошибок при чтении временных меток: " << TimeError << endl;
        }
    }
    else
    {
        wcout << L"Файловый объект НЕ открыт" << endl;
    }

    CloseFileHandle(fileObject);
    CloseFileSystemHandle(fileSystem);
    CloseStorageHandle(dataStorage);
}


/////////////////////////////////////////////////////////////////////СЧЕТ КОЛ-ВО ПОТОКОВ ДАННЫХ\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
int Stream(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив

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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {

                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {

                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                if (GetNumberOfStreams(currentFileObject)>1)
                                {
                                    wcout << L"Файл с более чем 1 потоком данных: " << fullPath << endl;
                                    ManyeStreamesFile++;
                                }
                                Stream(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            if (GetNumberOfStreams(currentFileObject) > 1)
                            {
                                wcout << L"Файл имеющий более одного потока данных: " << fullPath << endl;
                                wcout << L"Кол-во потоков: " << GetNumberOfStreams(currentFileObject) << endl;
                                ManyeStreamesFile++;
                            }
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

void StreamTest()
{
    //Путь до фйла образа или корневой каталог
    cout << "Введите путь до фйла образа или корневой каталог: " << endl;
    wstring imageStoragePath;
    wcin >> imageStoragePath;

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;
    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        wcout << L"Ошибка получения информации о разделе!" << endl;	return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        wcout << L"Файловая система не распознана!" << endl;
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        wcout << L"Ошибка открытия файловой системы!" << endl;
        CloseStorageHandle(dataStorage); 	return;
    }
    else
    {
        wcout << L"Файловая система открыта!" << endl;
    }

    //Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.");

    if (fileObject != FILE_OBJECT_ERROR)
    {
        wcout << L"Файловый объект открыт" << endl;
        //Проверка, является ли файл директорией
        if (FileIsDir(fileObject))
        {
            Stream(fileObject, fileSystem);
            wcout << L"Кол-во файлов имеющих более одного потока данных: " << ManyeStreamesFile << endl;

            ifstream inFile(filename);
            if (inFile.is_open()) {
                // Считываем JSON из файла
                inFile >> storageTest;
                inFile.close();
                // Извлечение данных из JSON
                int ManyStreamCount = storageTest["StreamTest"]["ManyStreamCount"];

                wcout << L"Разница многопоточных файлов: " << ManyeStreamesFile - ManyStreamCount << endl;
            }
            else {
                cerr << "Не удалось открыть файл json" << filename << " для чтения!" << endl;
            }

        }
    }
    else
    {
        wcout << L"Файловый объект НЕ открыт" << endl;
    }

    CloseFileHandle(fileObject);
    CloseFileSystemHandle(fileSystem);
    CloseStorageHandle(dataStorage);
}

/////////////////////////////////////////////////////////////////////ПРОВЕРКА ХЭША ФАЙЛА\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
int Hash(FileHandle fileObject, FileSystemHandle fileSystem, string HashSum)
{
    if (FileIsDir(fileObject))//Если объект это каталог
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                int fullPathLength = GetFullPathW(dirHandle, NULL);
                WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив

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
                GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))//Проверка имени файла
                {

                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог
                    if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                    {

                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr); //Чтение атрибутов файла
                        if (attrStr[1] != 'h')//Проверка атрибутов файла 
                        {

                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                            if ((fileName[0] != '.') || (fileName[0] != '.' && fileName[1] != '.'))
                            {
                                unsigned int fileHash[5];
                                GetFileHash(currentFileObject, 0, fileHash);
                                WCHAR tempStr[80];
                                swprintf_s(tempStr, _countof(tempStr), L"%08X %08X %08X %08X %08X", fileHash[0], fileHash[1], fileHash[2], fileHash[3], fileHash[4]);
                                int i = 0;
                                int j = 0;
                                while (i < size(tempStr))
                                {
                                    if (HashSum[j] == tempStr[i])
                                    {
                                        j++;
                                        i++;
                                        if (j == size(HashSum))
                                        {
                                            wcout << L"Это хэш каталога: " << fullPath << endl;
                                            wcout << L"SHA1: " << wstring(tempStr) << endl;
                                            HashFlag++;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                Hash(currentFileObject, fileSystem, HashSum);//Опять запускаем функцию с новым открытым каталогом
                            }
                            delete[] fileName;
                        }
                        delete[] fullPath;
                        CloseFileHandle(currentFileObject);
                    }

                    else//Если объект это файл
                    {
                        char attrStr[20];
                        GetFileStdAttributes(currentFileObject, attrStr);//Чтение атрибутов файла
                        if (attrStr[1] != 'h')
                        {
                            int fileNameLength = GetFileNameW(dirHandle, NULL);
                            WCHAR* fileName = new WCHAR[fileNameLength + 1];
                            GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                            unsigned int fileHash[5];
                            GetFileHash(currentFileObject, 0, fileHash);
                            WCHAR tempStr[80];
                            swprintf_s(tempStr, _countof(tempStr), L"%08X %08X %08X %08X %08X", fileHash[0], fileHash[1], fileHash[2], fileHash[3], fileHash[4]);
                            int i = 0;
                            int j = 0;
                            while (i < size(tempStr))
                            {
                                if (HashSum[j] == tempStr[i])
                                {
                                    j++;
                                    i++;
                                    if (j == size(HashSum))
                                    {
                                        wcout << L"Это хэш файла: " << fullPath << endl;
                                        wcout << L"SHA1: " << wstring(tempStr) << endl;
                                        HashFlag++;
                                        break;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            CloseDirectoryIterator(dirHandle);
        }
    }
    return 0;
}

void HashTest()
{
    //Путь до фйла образа или корневой каталог
    cout << "Введите путь до фйла образа или корневой каталог: " << endl;
    wstring imageStoragePath;
    wcin >> imageStoragePath;

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;
    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        wcout << L"Ошибка получения информации о разделе!" << endl;	return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        wcout << L"Файловая система не распознана!" << endl;
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage, startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        wcout << L"Ошибка открытия файловой системы!" << endl;
        CloseStorageHandle(dataStorage); 	return;
    }
    else
    {
        wcout << L"Файловая система открыта!" << endl;
    }

    //Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.\\Test");

    if (fileObject != FILE_OBJECT_ERROR)
    {
        wcout << L"Файловый объект открыт" << endl;
        //Проверка, является ли файл директорией
        if (FileIsDir(fileObject))
        {
            ifstream inFile(filename);
            if (inFile.is_open()) {
                // Считываем JSON из файла
                inFile >> storageTest;
                inFile.close();
                // Извлечение данных из JSON
                string FileName = storageTest["HashTest"]["FileName"];
                string HashSum = storageTest["HashTest"]["HashFile"];
                Hash(fileObject, fileSystem, HashSum);
                cout << "Загаданный файл: " << FileName << endl;
            }
            else {
                cerr << "Не удалось открыть файл json" << filename << " для чтения!" << endl;
            }
        }
    }
    else
    {
        wcout << L"Файловый объект НЕ открыт" << endl;
    }

    CloseFileHandle(fileObject);
    CloseFileSystemHandle(fileSystem);
    CloseStorageHandle(dataStorage);
}
