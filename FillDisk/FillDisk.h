#ifndef FillDiskH
#define FillDiskH

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <cstring>
#include <set>
#include <cctype> 
#include <numeric> 
#include <thread>
#include <atomic>
#include <exception>


#ifdef _WIN32
#define NOMINMAX // объявление min в windows.h перекрывает std::min
#include <windows.h>
#endif // _WIN32
#ifdef __unix__
#include <unistd.h>
#include <sys/wait.h>
#endif // __unix__



class Logger {
public:
    enum Level {
        INFO,
        WARNING,
        ERR,
        DEBUG
    };

    Logger(const std::string& filename) {
        // Открываем файл в режиме "trunc" для очистки содержимого перед записью
        log_file.open(filename, std::ios::out | std::ios::trunc);
        if (!log_file.is_open()) {
            std::cerr << "Предупреждение: не удалось открыть файл для записи: " << filename << std::endl;
        }
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(Level level, const std::string& message) {
        if (log_file.is_open()) {
            log_file << getCurrentTime() << ",[" << levelToString(level) << "]," << message << std::endl;
        }
        else {
            std::cerr << "Предупреждение: файл не открыт для записи." << std::endl;
        }
    }

private:
    std::ofstream log_file;

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        tm localTime;
        localtime_s(&localTime, &in_time_t);
        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y-%m-%d %X");
        return oss.str();
    }

    std::string levelToString(Level level) {
        switch (level) {
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERR: return "ERROR";
        case DEBUG: return "DEBUG";
        default: return "UNKNOWN";
        }
    }
};


/// Класс для генерации случайных чисел и строк
class RandomGenerator
{
private:
    std::default_random_engine dre; ///< Генератор случайных чисел

    /// Получение текущей даты и времени в виде строки
    std::string getCurrentTime()
    {
        // Получаем текущее время
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm;
        localtime_s(&now_tm, &now_time); // Используем localtime_s для безопасности

        // Буфер для форматированной строки времени
        char timeStr[20];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &now_tm);

        return std::string(timeStr); // Возвращаем строку с текущим временем
    }

public:
    /// Конструктор класса
    RandomGenerator() {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        dre = std::default_random_engine(seed);
    }

    /// Генерация случайного числа в диапазоне [min, max]
    int generateRandomNumber(int min, int max) {

        // Проверка, если min отрицательный
        if (min < 0) {
            std::cerr << "Предупреждение: min отрицательный, взят модуль." << std::endl;
            min = std::abs(min);
        }

        // Проверка, если max отрицательный
        if (max < 0) {
            std::cerr << "Предупреждение: max отрицательный, взят модуль." << std::endl;
            max = std::abs(max);
        }

        // Проверка min > max
        if (min > max) {
            std::cerr << "Предупреждение: минимум больше максимума, они поменяны местами." << std::endl;
            std::swap(min, max);
        }

        std::uniform_int_distribution<int> range(min, max);
        return range(dre);
    }

    /// Генерация случайной строки длиной length, состоящей из букв и цифр
    std::string generateRandomString(int length)
    {
        if (length <= 0)
        {
            std::cerr << "Предупреждение: некорректная длина строки, используется значение по умолчанию length = 10" << std::endl;
            length = 10;
        }

        const std::vector<std::string> englishChars = {
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
            "u", "v", "w", "x", "y", "z",
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
            "U", "V", "W", "X", "Y", "Z"
        };

        const std::vector<std::string> allChars = {
            // Китайские символы
            "家", "愛", "學", "書", "水", "火", "山", "土", "天", "人",
            "心", "明", "日", "月", "星", "風", "雪", "花", "鳥", "魚",
            // Арабские символы
            "ا", "ب", "ت", "ث", "ج", "ح", "خ", "د", "ذ", "ر",
            "ز", "س", "ش", "ص", "ض", "ط", "ظ", "ع", "غ", "ف",
            "ق", "ك", "ل", "م", "ن", "ه", "و", "ي",
            // Русские символы
            "а", "б", "в", "г", "д", "е", "ё", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т",
            "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я",
            "А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т",
            "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я",
            // Английские символы
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
            "u", "v", "w", "x", "y", "z",
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
            "U", "V", "W", "X", "Y", "Z",
            // Цифры
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
        };

        std::uniform_int_distribution<int> charDist(0, allChars.size() - 1);
        std::string result;

        for (int i = 0; i < length; ++i) {
            result += allChars[charDist(dre)]; // Добавление случайного символа
        }

        return result;
    }


    std::string generateRandomEnglishString(int length)
    {
        if (length <= 0)
        {
            std::cerr << "Предупреждение: некорректная длина строки, используется значение по умолчанию length = 10" << std::endl;
            length = 10;
        }

        const std::vector<std::string> englishChars = {
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
            "u", "v", "w", "x", "y", "z",
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
            "U", "V", "W", "X", "Y", "Z"
        };

        std::uniform_int_distribution<int> charDist(0, englishChars.size() - 1);
        std::string result;

        for (int i = 0; i < length; ++i) {
            result += englishChars[charDist(dre)]; // Добавление случайного символа
        }

        return result;
    }
};



/** \brief Класс для работы с каталогами*/
class DirectoryManager {
private:
    std::string currentPath; ///<Переменная для хранения текущего пути в файловой системе.
    RandomGenerator& generator; ///<Экземпляр генератора случайных чисел и строк.
    Logger& logger;

public:
    /** \brief Конструктор DirectoryManager*/
    DirectoryManager(Logger& logger, RandomGenerator& generator) : logger(logger), generator(generator) {}

    /** \brief Деструктор класса DirectoryManager*/
    ~DirectoryManager() {}

    /** \brief Метод для создания одной директории по заданному пути
    \param [in] dirPath Путь для создания директории
    \return Создана ли директория */
    bool createDirectory(const std::string& pathString) {


        std::filesystem::path dirPath = std::filesystem::u8path(pathString);

        try {
            if (!std::filesystem::exists(dirPath)) {
                std::filesystem::create_directories(dirPath);
                logger.log(Logger::INFO, "Создана директория: " + dirPath.u8string());
                return true;
            }
            else {
                std::cout << "Директория уже существует: " << dirPath << std::endl;
                logger.log(Logger::ERR, "Директория уже существует");
                return false; // Директория уже существует, не является ошибкой, но возвращаем false
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            logger.log(Logger::ERR, e.what());
            return false; // Возвращаем false при ошибке создания директории
        }
    }

    /** \brief Метод для создания случайной вложенной файловой структуры
    \param [in] rootPath - Путь для создания корневых каталогов,
    \param [in] maxDepth - Максимальное возможное количество вложенных каталогов,
    \param [in] maxBranches - Максимально количество каталогов по основному пути
    \return Количество созданных каталогов */
    int createNestedDirectories(const std::filesystem::path& rootPath, int maxDepth, int maxBranches) {
        if (maxDepth <= 0) return 0; // Если достигнут максимальный уровень вложенности

        int createdCount = 0;
        int currentBranches = generator.generateRandomNumber(1, maxBranches);

        for (int i = 0; i < currentBranches; ++i) {
            std::string dirName = generator.generateRandomString(generator.generateRandomNumber(10, 25));
            // Создаем объект path для поддиректории
            std::filesystem::path subDir = rootPath / dirName; // Оператор / работает с path

            
                
            if (createDirectory(subDir.string())) {
                createdCount++;
                // Рекурсивно создаем вложенные директории
                createdCount += createNestedDirectories(subDir, maxDepth - 1, maxBranches);
            }
        }
        return createdCount;
    }

    /** \brief Метод для рекурсивного удаления директории
    \param [in] path Путь для удаления директории
    \return Удалён ли каталог*/
    bool removeDirectory(const std::string& path) {
        try {
            // Возвращает количество удаленных файлов и директорий
            auto removed_count = std::filesystem::remove_all(path);
            if (removed_count > 0) {
                std::cout << "Удалено " << removed_count << " файлов/каталогов: " << path << std::endl;
                logger.log(Logger::INFO, "Удалено " + std::to_string(removed_count) + " файлов/каталогов: " + path);
                return true;
            }
            else {
                std::cout << "Не существует директория: " << path << std::endl;
                logger.log(Logger::WARNING, "Не существует директория: " + path);
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Ошибка удаления директории: " << e.what() << std::endl;
            logger.log(Logger::ERR, std::string("Ошибка удаления директории: ") + e.what());
            return false;
        }
        return false;
    }

    /** \brief Метод для изменения текущей рабочей директории процесса на указанный путь
    \param [in] path Путь изменения текущей директории
    \return Изменена ли текущая директория*/
    bool changeDirectory(const std::string& path) {
        try {
            if (std::filesystem::exists(path)) {
                currentPath = path;
                std::filesystem::current_path(path);
                std::cout << "Текущая директория сменена на: " << path << std::endl;
                return true;
            }
            else {
                std::cout << "Директория " << path << " не существует" << std::endl;
                logger.log(Logger::WARNING, "Директория не существует: " + path);
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Ошибка смены директории: " << e.what() << std::endl;
            logger.log(Logger::ERR, std::string("Ошибка смены директории: ") + e.what());
            return false;
        }
        return false;
    }
};



class FileManager
{
private:
    RandomGenerator& randomGen; ///< Экземпляр класса для генерации случайных данных
    Logger& logger;
    int filesCreated = 0;

public:
    FileManager(Logger& logger, RandomGenerator& randomGen) : logger(logger), randomGen(randomGen) {}



    /// Метод для создания файла
    bool createFile(const std::string& p)
    {
        std::filesystem::path fileName = std::filesystem::u8path(p);
        std::ofstream file(fileName);
        if (!file)
        {
            logger.log(Logger::ERR, "Не удалось создать файл: " + fileName.u8string());
            return false;
        }
        file.close();

        filesCreated++;
        logger.log(Logger::INFO, "Файл создан: " + fileName.u8string());
        logger.log(Logger::INFO, std::to_string(filesCreated));
        return true;
    }

    /// Метод для удаления файла
    bool deleteFile(const std::string& p)
    {
        std::filesystem::path fileName = std::filesystem::u8path(p);

        if (std::filesystem::exists(fileName))
        {
            if (std::filesystem::remove(fileName))
            {
                filesCreated--;
                logger.log(Logger::INFO, "Файл удален: " + fileName.u8string() + std::to_string(filesCreated));
                
                return true;
            }
            else
            {
                logger.log(Logger::ERR, "Не удалось удалить файл: " + fileName.u8string());
                return false;
            }
        }
        else
        {
            logger.log(Logger::ERR, "Файл не существует: " + fileName.u8string());
            return false;
        }
    }

    /// Метод для заполнения файла случайными данными
    bool fillFileWithRandomData(const std::string& p, std::uintmax_t targetSize) {
        std::filesystem::path fileName = std::filesystem::u8path(p);

        std::ofstream file(fileName, std::ios::binary | std::ios::app); // Режим добавления
        if (!file) {
            logger.log(Logger::ERR, "Не удалось открыть файл для записи: " + fileName.u8string());
            return false;
        }

        // Размер блока для записи (например, 1 МБ)
        const std::uintmax_t blockSize = 1 * 1024 * 1024;

        std::uintmax_t writtenSize = 0;
        std::string randomBlock = randomGen.generateRandomString(blockSize);
        while (writtenSize < targetSize) {
           
            std::size_t writeSize = std::min(blockSize, targetSize - writtenSize);
            file.write(randomBlock.data(), writeSize);
            writtenSize += writeSize;
        }

        file.close();
        logger.log(Logger::INFO, "Файл заполнен случайными данными: " + fileName.u8string());
        return true;
    }

    bool fillSmallFileWithRandomData(const std::string& p, std::uintmax_t targetSize)
    {
        std::filesystem::path fileName = std::filesystem::u8path(p);

        std::ofstream file(fileName, std::ios::app); // Открываем файл в режиме добавления данных
        if (!file)
        {
            logger.log(Logger::ERR, "Не удалось открыть файл для записи: " + fileName.u8string());
            return false;
        }

        std::string randomString = randomGen.generateRandomString(targetSize);    // Генерация случайной строки длиной 1000 символов

        file << randomString;

        file.close();
        logger.log(Logger::INFO, "Файл заполнен случайными данными: " + fileName.u8string());
        return true;
    }

    int getCreatedFilesAmount() {
        return filesCreated;
    }

};



class DiskSpaceChecker {
public:
    DiskSpaceChecker(const std::string& path, double fillPercent)
        : directoryPath(path) {
        auto spaceInfo = std::filesystem::space(directoryPath);
        totalCapacity = spaceInfo.capacity;
        targetFreeSpace = static_cast<uintmax_t>(totalCapacity * (1.0 - fillPercent / 100.0));
    }

    bool checkDiskSpace() {
        uintmax_t freeSpace = std::filesystem::space(directoryPath).available;
        if (freeSpace < targetFreeSpace) {
            return true;
        }
        else {
            return false;
        }
    }


    std::string directoryPath;
    uintmax_t totalCapacity;
    uintmax_t targetFreeSpace;
};



bool createFragmentedFile(const std::string& directory, FileManager& fileManager, Logger& logger, RandomGenerator& generator, DiskSpaceChecker& spaceChecker, std::string randomString, int iterations = 15)
{
    // Определяем размеры файлов и количество
    std::vector<size_t> fileSizes = { 100, 1024, 300, 1024 * 2, 400, 1024 * 3, 500, 1024 * 5, 1000, 1024 * 7 }; // Размеры в байтах (размер кластера - 4 КБ)
    std::vector<int> fileCounts = { 5, 15, 10, 15, 8, 15, 5, 7, 9, 12 }; // Количество файлов для каждого размера
    int fileCounter = 1;

    // Имя файла для фрагментации

    std::string fName = generator.generateRandomEnglishString(20);

    std::string largeFileName = directory + fName + "fragmented.txt";
    size_t largeFileSize = 512 * 1024 * 1024; // Размер в байтах

    // Создаем фрагментированный файл один раз в начале

    
    if (!std::filesystem::exists(largeFileName) && fileManager.createFile(largeFileName)) {
        logger.log(Logger::INFO, "Создан файл для накопления фрагментированных данных: " + largeFileName);
    }
   
    
    for (int iter = 0; iter < iterations; ++iter)
    {
        //std::cout << "Итерация " << iter + 1 << std::endl;
        // Шаг 1: Создание файлов

        for (size_t i = 0; i < fileSizes.size(); ++i)
        {
            for (int j = 0; j < fileCounts[i]; ++j)
            {
                //Проверка достаточно ли места на диске для создания нового файла


                if (spaceChecker.checkDiskSpace()) {
                    logger.log(Logger::WARNING, "Заполнение остановлено по сигналу проверки диска.");
                    return false;
                }

                std::string filename = directory  + fName + std::to_string(fileCounter++) + ".txt";
                {
                    if (fileManager.createFile(filename)) {
                        fileManager.fillSmallFileWithRandomData(filename, fileSizes[i]);
                    }
                }
            }
        }
        logger.log(Logger::INFO, "Удаляем файл");

        // Шаг 2: Удаление случайных файлов
        std::vector<std::string> files;
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_regular_file() && entry.path().u8string() != largeFileName) // Исключаем fragmented_file.txt
            {
                files.push_back(entry.path().u8string());
            }
        }

        // Перемешиваем список и удаляем случайные файлы
        std::shuffle(files.begin(), files.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

        int filesToRemove = 20 + (iter * 5); // Увеличиваем число удалений с каждой итерацией

        
        for (int i = 0; i < filesToRemove && i < files.size(); ++i)
        {
            if (spaceChecker.checkDiskSpace()) {
                logger.log(Logger::WARNING, "Заполнение остановлено по сигналу проверки диска.");
                return false;
            }

            // Проверяем, существует ли файл перед удалением
            if (std::filesystem::exists(files[i])) {
                //logger.log(Logger::INFO, "Файл существует, удаляем: " + files[i]);
                fileManager.deleteFile(files[i]);
            }
            else {
                logger.log(Logger::ERR, "Файл не найден для удаления: " + files[i]);
            }
        }


        // Шаг 3: Добавление данных в fragmented_file.txt

        
        if (spaceChecker.checkDiskSpace()) {
            logger.log(Logger::WARNING, "Заполнение остановлено по сигналу проверки диска.");
            return false;
        }


        uintmax_t additionSize = (largeFileSize / iterations) / 8;
        fileManager.fillFileWithRandomData(largeFileName, additionSize);

    }


    if (spaceChecker.checkDiskSpace()) {
        logger.log(Logger::WARNING, "Заполнение остановлено по сигналу проверки диска.");
        return false;
    }
    
    uintmax_t finalAdditionSize = largeFileSize - (largeFileSize / 8);
    fileManager.fillFileWithRandomData(largeFileName, finalAdditionSize);
    
    if (spaceChecker.checkDiskSpace()) {
        logger.log(Logger::WARNING, "Заполнение остановлено по сигналу проверки диска.");
        return false;
    }

    // Генерация случайной позиции для вставки строки (от 1 до 536870912 байт)
    size_t randomPosition = generator.generateRandomNumber(1, 536870912);

    // Открываем файл в бинарном режиме
    std::fstream file(largeFileName, std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        logger.log(Logger::ERR, "Не удалось открыть файл для добавления данных: " + largeFileName);
        return false;
    }

    // Перемещаемся в случайную позицию в файле
    file.seekp(randomPosition, std::ios::beg);

    // Записываем строку в эту позицию
    file.write(randomString.c_str(), randomString.size());

    // Закрываем файл
    file.close();

    logger.log(Logger::INFO, "Строка была добавлена в случайное место файла " + largeFileName);

    return true;
}

std::string filetype(int randomness) //свич для рандомизации типа файла
{
    switch (randomness)
    {
    case 0: return ".txt";
    case 1: return ".doc";
    case 2: return ".rtf"; //можно как txt открыть
    case 3: return ".xls";
    case 4: return ".xml"; //другой эксель просто
    case 5: return ".ppt";
    default: return ".txt";
    }
}


bool iteration_func(const std::filesystem::path& directorypath, Logger& logfile, RandomGenerator& generator, FileManager& fileManager, DiskSpaceChecker& spaceChecker)
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;


    std::string fileName;
    std::string randomStr;
    int random_number;
    int random_size;

    // Создаем итератор вручную, чтобы иметь доступ к методу disable_recursion_pending
    for (auto iter = recursive_directory_iterator(directorypath), end = recursive_directory_iterator(); iter != end; ++iter)
    {
        if (spaceChecker.checkDiskSpace()) {
            return false;
        }

        const auto& dirEntry = *iter; // Получаем текущий элемент
        //logfile.log(Logger::DEBUG, "Обработка элемента: " + dirEntry.path().u8string());
        auto currentPath = dirEntry.path().u8string();

        // Проверка на системные каталоги
        if (currentPath.find("$RECYCLE.BIN") != std::string::npos || currentPath.find("System Volume Information") != std::string::npos)
        {
            logfile.log(Logger::INFO, "Пропуск системного каталога или файла: " + currentPath);
            iter.disable_recursion_pending(); // Отключаем рекурсию для текущего итератора
            continue;
        }

        // Проверка на существование директории и доступность для чтения
        if (!exists(dirEntry) || !is_directory(dirEntry))
        {
            continue;
        }

        //Размеры файлов

        std::vector<std::uintmax_t> file_sizes = {
        700,                  // 700 байт
        1024,                 // 1 КБ
        5 * 1024,             // 5 КБ
        10 * 1024,            // 10 КБ
        100 * 1024,           // 100 КБ
        1 * 1024 * 1024,      // 1 МБ
        10 * 1024 * 1024,     // 10 МБ
        50 * 1024 * 1024,     // 50 МБ
        };
          
        int sigma = 30;
        
        // Создание случайного количества файлов в текущем каталоге

        int fileCount = generator.generateRandomNumber(0, 10); // Генерация случайного числа файлов от 0 до 10
        for (int i = 0; i < fileCount; ++i)
        {

            if (spaceChecker.checkDiskSpace()) {
                return false;
            }

            random_number = generator.generateRandomNumber(1, 50);

            random_size = generator.generateRandomNumber(0, 7);


            randomStr = generator.generateRandomString(random_number) + filetype(random_number % 6);
            fileName = (dirEntry.path() / std::filesystem::u8path(randomStr)).u8string();
                
            {
                fileManager.createFile(fileName);
                int fileSize = file_sizes[random_size];

                if (fileSize >= 1 * 1024 * 1024) {
                    fileManager.fillFileWithRandomData(fileName, fileSize);
                }
                else {
                    fileManager.fillSmallFileWithRandomData(fileName, fileSize);
                }
            }
           
        }

        if (spaceChecker.checkDiskSpace()) {
            return false;
        }

    }
}


#endif // FillDiskH
