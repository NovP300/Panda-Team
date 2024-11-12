// TestLibProjects.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include "FillDisk.h"
#include "TestScenarious.h"


#include <iostream>
#include <codecvt>

#ifdef _WIN32
#include <windows.h>
#endif

void FillDiskFunction(Logger& logfile);
bool mountWindowsVirtualDisk(Logger& logfile);
bool mountLinuxVirtualDisk(Logger& logfile);


std::string getOperatingSystem() {
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif defined(__unix__) || defined(__unix) || defined(__linux__) 
    return "Linux";
#else
    return "Unknown OS";
#endif
}

int main()
{
    std::string operatingSystem = getOperatingSystem();

    if (operatingSystem == "Windows") {
        SetConsoleOutputCP(CP_UTF8);
    }

    Logger logger("FillLog.txt"); //файл для логов 

    //std::cout << "___________________" << std::endl;
    //std::cout << "БЛОК МОНТИРОВАНИЯ" << std::endl;
    //std::cout << "-------------------" << std::endl;

    if (operatingSystem == "Windows") {
        //mountWindowsVirtualDisk(logger);
    }
    else if (operatingSystem == "Linux") {
        //mountLinuxVirtualDisk(logger);
    }

    std::cout << "___________________" << std::endl;
    std::cout << "БЛОК ФОРМАТИРОВАНИЯ" << std::endl;
    std::cout << "___________________" << std::endl << std::endl;

    std::cout << "Хотите отформатировать диск перед заполнением y[yes] | n[no]" << std::endl;
    std::string userInput;
    std::cin >> userInput;

    if (userInput == "y" || userInput == "Y") {

        if (operatingSystem == "Windows") {

            DiskFormatterWIN32 formatter(logger);
            formatter.Run();
        }
        else if (operatingSystem == "Linux") {
            //DiskFormatterLinux formatter(logger);
            //formatter.Run();
        }
    }

    std::cout << "_____________________________" << std::endl;
    std::cout << "БЛОК ЗАПОЛНЕНИЯ РАЗДЕЛА ДИСКА" << std::endl;
    std::cout << "_____________________________" << std::endl << std::endl;

    FillDiskFunction(logger); // Заполняем

    std::cout << "_____________________________" << std::endl;
    std::cout << "БЛОК СЦЕНАРИЕВ" << std::endl;
    std::cout << "_____________________________" << std::endl << std::endl;
    


    if (ConnectLibrary()) {

        //StorageTest();
        DirTest();

    }

    DataAccessLibCleanup();

    return 0;
}


void FillDiskFunction(Logger& logfile) {

    RandomGenerator generator;
    
    DirectoryManager directoryGenerator(logfile, generator);

    FileManager fileManager(logfile, generator);

    std::cout << "Введите раздел диска, который необходимо заполнить (формат D)" << std::endl;

    std::string directorypath;

    std::cin >> directorypath;

    std::string directoryPath = directorypath + ":\\" ;

    int depth = generator.generateRandomNumber(5, 6);
    int branches = generator.generateRandomNumber(3, 4);
    //int createdCount = directoryGenerator.createNestedDirectories(directoryPath, depth, branches);

    std::cout << "Информация о создании файловой структуры" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    std::cout << "Максимальное число веток: " << branches << std::endl;
    std::cout << "Уровень вложенности: " << depth << std::endl;
    //std::cout << "Всего создано директорий: " << createdCount << std::endl;

    std::cout << "---------------------------------------" << std::endl;

    //iteration_func(directoryPath, logfile, generator, fileManager); // запустить итератор заполнения папок файликами

    createFragmentedFile(directoryPath, fileManager, logfile); // cоздать фрагментированный файл (у большого файла размер 1ГБ)

    int created = fileManager.getCreatedFilesAmount();

    std::cout << "Файлов после заполнения: " << created << std::endl;

}


#ifdef _WIN32

bool mountWindowsVirtualDisk(Logger& logfile) {

    winMounter mounter;

    std::cout << "Хотите смонтировать виртуальный диск? y[yes] | n[no]" << std::endl;
    std::string userInput;
    std::cin >> userInput;

    if (userInput == "n" || userInput == "N") {
        return false;
    }

    if (mounter.mount()) {
        std::cout << "Диск успешно смонтирован." << std::endl;

        if (mounter.cleaner()) {
            std::cout << "Диск очищен" << std::endl;
        }
    }
    else {
        std::cerr << "Не удалось смонтировать диск." << std::endl;
        return false;
    }
}

#endif

#ifdef __unix__

bool mountLinuxVirtualDisk(Logger& logfile) {

    MountLinux mountLinux(l);

    const char* m = "/mnt/t";
    const char* i = "/mnt/images/i/fat16.img";
    const char* n = "/dev/nbd0";
    const char* f = "vfat";

    //mountLinux.mount_img(m, i, n, f);
    //mountLinux.umount_img(m);

    //mountLinux.mount_VHDX("ntfs", "/dev/nbd1", "/mnt/images/ntfs_GPT.vhd",  "/mnt/fat"); 
}

#endif
