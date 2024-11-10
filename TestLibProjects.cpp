// TestLibProjects.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include "FillDisk.h"
#include "TestScenarious.h"


#include <iostream>
#include <codecvt>

#ifdef _WIN32
#include <windows.h>
#endif

void FillDiskFunction();

int main()
{
    Logger logger("FillLog.txt");
    
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        //DiskFormatterWIN32 formatter(logger);
        //formatter.Run();


    #endif // _WIN32
    #ifdef __unix__
        DiskFormatterLinux formatter(logger);
        formatter.Run();
    #endif // __unix__
    
    std::cout << "БЛОК ЗАПОЛНЕНИЯ РАЗДЕЛА ДИСКА" << std::endl;

    //FillDiskFunction();


    std::cout << "БЛОК СЦЕНАРИЕВ" << std::endl;
    

    if (ConnectLibrary()) {
        //StorageTest();
        //DirTest();
    }

    DataAccessLibCleanup();

    return 0;
}


void FillDiskFunction() {

    Logger logfile("FillLog.txt");

    RandomGenerator generator;
    
    DirectoryManager directoryGenerator(logfile, generator);

    FileManager fileManager(logfile, generator);

    std::cout << "Введите раздел диска, который необходимо заполнить (формат D)" << std::endl;

    std::string directorypath;

    std::cin >> directorypath;

    std::string directoryPath = directorypath + ":\\" ;

    int depth = generator.generateRandomNumber(5, 7);
    int branches = generator.generateRandomNumber(3, 4);
    int createdCount = directoryGenerator.createNestedDirectories(directoryPath, depth, branches);

    std::cout << "Информация о заполнении" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    std::cout << "Максимальное число веток: " << branches << std::endl;
    std::cout << "Уровень вложенности: " << depth << std::endl;
    std::cout << "Всего создано директорий: " << createdCount << std::endl;

    std::cout << "---------------------------------------" << std::endl;

    iteration_func(directoryPath, logfile, generator, fileManager); // запустить итератор, создает в папках тхт файлики рандомно

    createFragmentedFile(directoryPath, fileManager, logfile); // cоздать фрагментированный файл

}


