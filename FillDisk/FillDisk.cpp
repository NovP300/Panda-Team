// FillDiskProject.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "FillDisk.h"
#include <nlohmann/json.hpp>
#ifdef _WIN32
#include <windows.h>
#endif

using json = nlohmann::json;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(1251);
#endif

    std::string path;
    std::cout << "Введите путь к JSON-файлу: ";
    std::getline(std::cin, path);


    /// Открытие json файла
    std::ifstream configFile(path);
    if (!configFile.is_open()) {
        std::cerr << "Не удалось открыть файл конфигурации: " << path << std::endl;
        return 1;
    }

    /// Копирование структуры в объект json
    json config;
    try {
        configFile >> config;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка разбора JSON: " << e.what() << std::endl;
        return 1;
    }

    configFile.close();

    /// Создание итогового JSON объекта, в который сохранятся данные о заполнении
    json resultJson;

    /// Разбор JSON и инициализация разделов
    if (!config.contains("Partitions")) {
        std::cerr << "JSON должен содержать ключ 'Partitions'!" << std::endl;
        return 1;
    }


    const auto& partitions = config["Partitions"];
    int partitionIndex = 1; /// Индекс для идентификации разделов в итоговом JSON

    /// Заполнение разделов в цикле
    for (const auto& partition : partitions) {
        if (!partition.contains("Letter") || !partition.contains("FillPercentage")) {
            std::cerr << "Каждый раздел должен содержать 'Letter' и 'FillPercentage'!" << std::endl;
            return 1;
        }

        /// Извлечение данных из JSON
        std::string driveLetter = partition["Letter"].get<std::string>();
        double fillPercentage = partition["FillPercentage"].get<double>();

        /// Преобразование буквы диска в путь
        std::filesystem::path directory = driveLetter + ":\\";

        /// Инициализация классов менеджеров и инструментов
        Logger logger("log_" + driveLetter + ".txt");
        RandomGenerator generator;
        FileManager fileManager(logger, generator);
        DirectoryManager directoryGenerator(logger, generator);
        DiskSpaceChecker spaceChecker(directory.u8string(), fillPercentage);

        /// Генерация директорий
        int createdCount = directoryGenerator.createNestedDirectories(directory, 4, 5);
        std::cout << "На диске " << driveLetter << " создано " << createdCount << " директорий." << std::endl;

        /// Генерация параметров для создания фрагментированных файлов
        int iterations = generator.generateRandomNumber(1, 4); // количество фрагментированных файлов
        std::string randomString = generator.generateRandomString(150); // генерация строки, которая будет записано в случайное место фрагментированного файла

        int count = 0;

        /// Создание на разделе фрагментированных файлов
        for (int i = 0; i < iterations; i++) {

            if (createFragmentedFile(directory.u8string(), fileManager, logger, generator, spaceChecker, randomString)) {
                count++;
                continue;
            }
            else {
                break;
            }
        }

        if (count == 0) {
            randomString = "";
        }

        /// Основное заполнение
        for (int i = 0; i < 100; i++) {
            if (iteration_func(directory.u8string(), logger, generator, fileManager, spaceChecker)) {
                continue;
            }
            else {
                break;
            }
        }

        std::cout << "На диске " << driveLetter << " создано " << fileManager.getCreatedFilesAmount() << " файлов." << std::endl;
        std::cout << "Заполнение диска " << driveLetter << " завершено." << std::endl << std::endl;

        /// Сохранение информации в итоговый JSON
        json partitionJson;
        partitionJson["DirTest"] = {
            {"directoryCount", createdCount},
            {"filesCount", fileManager.getCreatedFilesAmount()}
        };
        partitionJson["FragmentTest"] = {
            {"count", count},
            {"string", randomString}
        };

        /// Добавление данных по текущему разделу
        resultJson["Partition " + std::to_string(partitionIndex)] = partitionJson;
        partitionIndex++;


    }






    /// Имя итогового файла
    std::string resultFilename = "output.json";

    /// Получение пути к папке входного JSON
    std::filesystem::path inputFilePath(path); // Путь к входному JSON
    std::filesystem::path outputDir = inputFilePath.parent_path(); // Папка, где находится JSON

    /// Формирование полного пути к итоговому файлу
    std::filesystem::path outputFilePath = outputDir / resultFilename;

    /// Запись итогового JSON в файл
    std::ofstream outFile(outputFilePath, std::ios::out);
    if (outFile.is_open()) {
        outFile << resultJson.dump(4); 
        outFile.close();
        std::cout << "Итоговый JSON записан в файл " << resultFilename << std::endl;
    }
    else {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
    }


    system("pause");

    return 0;
    
}


