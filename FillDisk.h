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

#ifdef _WIN32
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
                createdCount += createNestedDirectories(subDir, maxDepth - 1, currentBranches);
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
        logger.log(Logger::INFO, "Файл создан: " + fileName.u8string());
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
                logger.log(Logger::INFO, "Файл удален: " + fileName.u8string());
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
    bool fillFileWithRandomData(const std::string& p, int numberOfLines)
    {
        std::filesystem::path fileName = std::filesystem::u8path(p);

        std::ofstream file(fileName, std::ios::app); // Открываем файл в режиме добавления данных
        if (!file)
        {
            logger.log(Logger::ERR, "Не удалось открыть файл для записи: " + fileName.u8string());
            return false;
        }

        for (int i = 0; i < numberOfLines; ++i)
        {
            int randomNumber = randomGen.generateRandomNumber(1, 10000);   // Генерация случайного числа
            std::string randomString = randomGen.generateRandomString(1000);    // Генерация случайной строки длиной 1000 символов

            file << randomNumber << " " << randomString << std::endl;
        }

        file.close();
        logger.log(Logger::INFO, "Файл заполнен случайными данными: " + fileName.u8string());
        return true;
    }
};



/**
    @class DiskSpaceChecker
    @brief Класс для проверки свободного места на физическом диске
 */
class DiskSpaceChecker
{
private:
    Logger& logger; // Ссылка на экземпляр Logger

public:
    /**
        @brief Конструктор по умолчанию.
     */
    DiskSpaceChecker(Logger& logger) : logger(logger) {}

    /**
        @brief Проверяет, достаточно ли свободного места на диске.
        @param path Путь к директории, на которой нужно проверить свободное место.
        @param requiredSpace Необходимое количество свободного места в байтах.
        @return true, если на диске достаточно места, иначе false.
     */
    bool checkDiskSpace(const std::string& path, uintmax_t requiredSpace)
    {
        try
        {
            std::filesystem::space_info diskInfo = std::filesystem::space(path);

            std::cout << "Свободного места: " << diskInfo.available << " байт" << std::endl;
            std::cout << "Необходимо места: " << requiredSpace << " байт" << std::endl;

            if (diskInfo.available >= requiredSpace)
            {
                return true;
            }
            else
            {
                logger.log(Logger::WARNING, "Недостаточно свободного места на диске");
                return false;
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            logger.log(Logger::ERR, std::string("Ошибка доступа к файловой системе: ") + e.what());
            return false;
        }
    }
};


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

void iteration_func(const std::filesystem::path& directorypath, Logger& logfile, RandomGenerator& generator, FileManager& fileManager)
{
    //logfile.log(Logger::INFO, "Зашел в итератор");
    using directory_iterator = std::filesystem::directory_iterator;

    std::string fileName;
    std::string randomStr;
    int random_number;


    for (const auto& dirEntry : directory_iterator(directorypath))
    {
        logfile.log(Logger::DEBUG, "Обработка элемента: " + dirEntry.path().u8string());

        // Проверка на системные и скрытые папки
        auto status = std::filesystem::status(dirEntry);
        if ((status.permissions() & std::filesystem::perms::owner_read) == std::filesystem::perms::none ||
            dirEntry.path().filename() == "$RECYCLE.BIN" ||
            dirEntry.path().filename() == "System Volume Information" ||
            dirEntry.path().filename() == "Ngc")
        {
            logfile.log(Logger::INFO, "Пропуск системной или скрытой директории: " + dirEntry.path().string());
            continue; // не работает зараза
        }

        // Проверка на существование директории и доступность для чтения
        if (!exists(dirEntry) || !is_directory(dirEntry))
        {
            logfile.log(Logger::ERR, "Элемент не является доступной директорией: " + dirEntry.path().u8string());
            continue;
        }

        logfile.log(Logger::DEBUG, "Директория найдена: " + dirEntry.path().u8string());

        // Создание файлов в случайных подкаталогах
        if (generator.generateRandomNumber(0, 4) != 0)
        {
            random_number = generator.generateRandomNumber(1, 10);
            randomStr = generator.generateRandomString(random_number) + filetype(random_number % 6);
            fileName = (dirEntry.path() / std::filesystem::u8path(randomStr)).u8string();

            //logfile.log(Logger::DEBUG, "Создание файла с именем: " + fileName);

            try {
                fileManager.createFile(fileName);
            }
            catch (const std::exception& e) {
                logfile.log(Logger::ERR, "Ошибка создания файла: " + fileName + ". Ошибка: " + e.what());
                continue;
            }

            randomStr = generator.generateRandomString(random_number);

            try {
                if (generator.generateRandomNumber(0, 15) == 2) {
                    fileManager.fillFileWithRandomData(fileName, random_number);
                }
                else {
                    fileManager.fillFileWithRandomData(fileName, random_number);
                }
                logfile.log(Logger::DEBUG, "Файл заполнен данными: " + fileName);
            }
            catch (const std::exception& e) {
                logfile.log(Logger::ERR, "Ошибка при записи в файл: " + fileName + ". Ошибка: " + e.what());
                continue;
            }

            // Рекурсивный вызов для подкаталогов
            iteration_func(dirEntry, logfile, generator, fileManager);
        }
    }
}


void iteration_func1(const std::filesystem::path& directorypath, Logger& logfile, RandomGenerator& generator, FileManager& fileManager)
{
    // Используем recursive_directory_iterator вместо directory_iterator
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    std::string fileName;
    std::string randomStr;
    int random_number;

    for (const auto& dirEntry : recursive_directory_iterator(directorypath))
    {
        logfile.log(Logger::DEBUG, "Обработка элемента: " + dirEntry.path().u8string());

        // Проверка на системные и скрытые папки
        auto status = std::filesystem::status(dirEntry);
        if ((status.permissions() & std::filesystem::perms::owner_read) == std::filesystem::perms::none ||
            dirEntry.path().filename() == "$RECYCLE.BIN" ||
            dirEntry.path().filename() == "System Volume Information" ||
            dirEntry.path().filename() == "Ngc")
        {
            logfile.log(Logger::INFO, "Пропуск системной или скрытой директории: " + dirEntry.path().u8string());
            continue; // теперь это будет корректно пропускать системные файлы
        }

        // Проверка на существование директории и доступность для чтения
        if (!exists(dirEntry) || !is_directory(dirEntry))
        {
            logfile.log(Logger::ERR, "Элемент не является доступной директорией: " + dirEntry.path().u8string());
            continue;
        }

        logfile.log(Logger::DEBUG, "Директория найдена: " + dirEntry.path().u8string());

        // Создание файлов в случайных подкаталогах
        if (generator.generateRandomNumber(0, 4) != 0)
        {
            random_number = generator.generateRandomNumber(1, 10);
            randomStr = generator.generateRandomString(random_number) + filetype(random_number % 6);
            fileName = (dirEntry.path() / std::filesystem::u8path(randomStr)).u8string();

            try {
                fileManager.createFile(fileName);
            }
            catch (const std::exception& e) {
                logfile.log(Logger::ERR, "Ошибка создания файла: " + fileName + ". Ошибка: " + e.what());
                continue;
            }

            randomStr = generator.generateRandomString(random_number);

            try {
                fileManager.fillFileWithRandomData(fileName, random_number);
                logfile.log(Logger::DEBUG, "Файл заполнен данными: " + fileName);
            }
            catch (const std::exception& e) {
                logfile.log(Logger::ERR, "Ошибка при записи в файл: " + fileName + ". Ошибка: " + e.what());
                continue;
            }
        }
    }
}


void createFragmentedFile(const std::string& directory, FileManager& fileManager, int iterations = 5)
{
    // Определяем размеры файлов и количество
    std::vector<size_t> fileSizes = { 100 * 1024, 200 * 1024, 512 * 1024 }; // Размеры в байтах
    std::vector<int> fileCounts = { 25, 15, 30 }; // Количество файлов для каждого размера
    int fileCounter = 1;

    for (int iter = 0; iter < iterations; ++iter)
    {
        // Шаг 1: Создание файлов
        for (size_t i = 0; i < fileSizes.size(); ++i)
        {
            for (int j = 0; j < fileCounts[i]; ++j)
            {
                std::string filename = directory + "/file_" + std::to_string(fileCounter++) + ".txt";
                if (fileManager.createFile(filename))
                {
                    fileManager.fillFileWithRandomData(filename, fileSizes[i] / 1000); // Заполнение файла случайными данными
                }
            }
        }

        // Шаг 2: Удаление случайных файлов
        std::vector<std::string> files;
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path().string());
            }
        }

        std::shuffle(files.begin(), files.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

        int filesToRemove = 15 + (iter * 5); // Увеличиваем число удалений с каждой итерацией
        for (int i = 0; i < filesToRemove && i < files.size(); ++i)
        {
            fileManager.deleteFile(files[i]);
        }
    }

    // Завершающий этап: создание большого файла
    std::string largeFileName = directory + "/fragmented_file.txt";
    size_t largeFileSize = 200 * 1024 * 1024; // Размер в байтах
    if (fileManager.createFile(largeFileName))
    {
        fileManager.fillFileWithRandomData(largeFileName, largeFileSize/1000); // Заполняем файл до 200 МБ
    }
}




#ifdef _WIN32
/** \brief Класс для форматирования диска на Windows
 */
class DiskFormatterWIN32 {
private:
    Logger& logger;
public:
    /** \brief Конструктор класса
 */
    DiskFormatterWIN32(Logger& logger) : logger(logger) {
    }

    /** \brief Деструктор класса
 */
    ~DiskFormatterWIN32() {
    }
    /** \brief Метод для форматирования диска
    \param [in] drive Название диска, подлежащего форматированию
    \param [in] fileSystem Файловая система, в которую форматируется диск
*/
    bool FormatDisk(const std::string& drive, const std::string& fileSystem) {
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        /// Формирование команды для форматирования диска
        std::string commandLine = "cmd.exe /c format " + drive + ": /FS:" + fileSystem + " /Y /Q";//Можно добавить ключи /Y /Q

        /// Преобразование std::string в modifiable LPSTR
        LPSTR cmdLineChars = const_cast<LPSTR>(commandLine.c_str());

        /// Создание процесса для команды format
        BOOL result = CreateProcessA(
            NULL,           // Имя модуля (используем командную строку)
            cmdLineChars,   // Командная строка
            NULL,           // Атрибуты защиты процесса
            NULL,           // Атрибуты защиты потока
            FALSE,          // Флаги наследования
            0,              // Флаги создания
            NULL,           // Использование окружения родителя
            NULL,           // Использование текущего каталога родителя
            &si,            // Указатель на STARTUPINFOA
            &pi             // Указатель на PROCESS_INFORMATION
        );

        if (!result) {
            logger.log(Logger::ERR, "Ошибка при создании процесса: " + std::to_string(GetLastError()));
            return false;
        }

        /// Ожидание завершения процесса форматирования
        WaitForSingleObject(pi.hProcess, INFINITE);

        logger.log(Logger::INFO, "Форматирование диска " + drive + ": завершено.");

        /// Закрытие дескрипторов процесса и потока
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return true;
    }
    /** \brief Метод для проверки корректоности введённого названия диска
        \param [in] drive Введённое название диска
 */
    bool IsValidDriveLetter(const std::string& drive) {
        if (drive.length() == 1 && std::isalpha(drive[0])) {
            /// Проверка существования диска
            DWORD driveMask = GetLogicalDrives(); /// Получение битовой маски логических дисков системы
            int driveIndex = toupper(drive[0]) - 'A'; /// Преобразование буквы диска из строки в верхний регистр и вычисление индекса бита в маске
            if (driveMask & (1 << driveIndex)) {
                return true; // Диск существует
            }
            else {
                return false; // Диск не существует
            }
        }
        return false;
    }
    /** \brief Метод для проверки корректоности введённой файловой системы
    \param [in] fileSystem Введённое название файловой системы
*/
    bool IsValidFileSystem(const std::string& fileSystem) {
        const std::set<std::string> validFileSystems = { "NTFS", "FAT32", "EXFAT", "FAT" };
        std::string upperFileSystem = fileSystem;  /// Создание копии введенной строки fileSystem для преобразования в верхний регистр
        for (char& c : upperFileSystem) {
            c = std::toupper(static_cast<unsigned char>(c));
        }
        if (validFileSystems.find(upperFileSystem) == validFileSystems.end()) {
            return false;
        }
        return true;
    }
    /** \brief Метод для запуска форматирования
*/
    void Run() {
        std::string drive;
        std::string fileSystem;

        do {
            std::cout << "Введите букву диска для форматирования (например, D): ";
            std::cin >> drive;

            if (!IsValidDriveLetter(drive)) {
                std::cerr << "Ошибка: Неверно указана буква диска. Попробуйте еще раз." << std::endl;
                logger.log(Logger::ERR, "Неверно указана буква диска: " + drive);
            }
        } while (!IsValidDriveLetter(drive));

        do {
            std::cout << "Введите файловую систему для форматирования (NTFS, FAT32, exFAT): ";
            std::cin >> fileSystem;
            if (!IsValidFileSystem(fileSystem)) {
                std::cerr << "Ошибка: Неверно указана файловая система." << std::endl;
                logger.log(Logger::ERR, "Неверно указана файловая система.");
            }
        } while (!IsValidFileSystem(fileSystem));

        std::cout << "ВНИМАНИЕ: Все данные на диске " << drive << ": будут уничтожены. Продолжить? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            if (!FormatDisk(drive, fileSystem)) {
                logger.log(Logger::ERR, "Ошибка при форматировании диска.");
            }
        }
        else {
            std::cout << "Форматирование отменено пользователем." << std::endl;
            logger.log(Logger::INFO, "Форматирование отменено пользователем.");
        }
    }
};
#endif // _WIN32

#ifdef __unix__
/** \brief Класс для форматирования диска на Linux
 */
class DiskFormatterLinux {
private:
    Logger& logger;
public:
    /** \brief Конструктор класса
 */
    DiskFormatterLinux(Logger& logger) : logger(logger) {
    }
    /** \brief Деструктор класса
 */
    ~DiskFormatterLinux() {}

    /** \brief Метод для форматирования диска
    \param [in] drive Название диска, подлежащего форматированию
    \param [in] fileSystem Файловая система, в которую форматируется диск
*/
    bool FormatDisk(const std::string& drive, const std::string& fileSystem) {
        pid_t pid = fork(); // Создание нового процесса с помощью системного вызова fork()
        if (pid == -1) {
            logger.log(Logger::ERR, "Ошибка при создании процесса: " + std::string(strerror(errno)));
            return false;
        }
        else if (pid == 0) {
            // Дочерний процесс: вызов mkfs
            std::string command = "mkfs." + fileSystem + " " + drive;
            execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
            // Если execl успешно выполнится, то код ниже выполняться не будет
            exit(EXIT_FAILURE);
        }
        else {
            // Родительский процесс: ожидание завершения mkfs
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                logger.log(Logger::INFO, "Форматирование диска " + drive + " завершено.");
                return true;
            }
            else {
                logger.log(Logger::ERR, "Ошибка при форматировании диска " + drive);
                return false;
            }
        }
    }

    /** \brief Метод для проверки корректоности введённого названия диска
        \param [in] drive Введённое название диска
 */
    bool IsValidDriveLetter(const std::string& drive) {
        if (drive.rfind("/dev/", 0) == 0) {
            if (access(drive.c_str(), F_OK) != -1) {
                return true; // Диск существует
            }
            else {
                return false; // Диск не существует
            }
        }
        return false;
    }

    /** \brief Метод для проверки корректоности введённой файловой системы
\param [in] fileSystem Введённое название файловой системы
*/
    bool IsValidFileSystem(const std::string& fileSystem) {
        const std::set<std::string> validFileSystems = { "ext4", "ext3", "ext2", "vfat", "ntfs", "btrfs", "xfs" };
        if (validFileSystems.find(fileSystem) != validFileSystems.end()) {
            return true;
        }
        else {
            return false;
        }
    }

    /** \brief Метод для запуска форматирования
*/
    void Run() {
        std::string drive;
        std::string fileSystem;

        do {
            std::cout << "Введите путь к устройству для форматирования (например, /dev/sda): ";
            std::cin >> drive;
            if (!IsValidDriveLetter(drive)) {
                std::cerr << "Ошибка: Неверно указано имя диска. Попробуйте еще раз." << std::endl;
                logger.log(Logger::ERR, "Неверно указано имя диска: " + drive);
            }
        } while (!IsValidDriveLetter(drive));

        do {
            std::cout << "Введите файловую систему для форматирования (ext4, vfat, ntfs): ";
            std::cin >> fileSystem;
            if (!IsValidFileSystem(fileSystem)) {
                std::cerr << "Ошибка: Неверно указана файловая система." << std::endl;
                logger.log(Logger::ERR, "Неверно указана файловая система. ");
            }
        } while (!IsValidFileSystem(fileSystem));

        std::cout << "ВНИМАНИЕ: Все данные на диске " << drive << " будут уничтожены. Продолжить? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            if (!FormatDisk(drive, fileSystem)) {
                logger.log(Logger::ERR, "Ошибка при форматировании диска " + drive);
            }
        }
        else {
            std::cout << "Форматирование отменено пользователем." << std::endl;
            logger.log(Logger::INFO, "Форматирование отменено пользователем.");
        }
    }
};

#include <iostream>
#include <string>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <linux/loop.h> 

class MountLinux
{
public:
    MountLinux(Logger& l) : logger(l)
    {
        if (!isNbdLoaded()) {
            loadNbdModule();
        }
    }

    // чтобы монтировать виртуальные образы, нужно загрузить nbd
    bool isNbdLoaded() {
        FILE* pipe = popen("lsmod | grep nbd", "r");
        if (!pipe) {
            return false;
        }
        char buffer[128];
        bool loaded = false;
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            if (strstr(buffer, "nbd")) {
                loaded = true;
                break;
            }
        }
        pclose(pipe);
        return loaded;
    }

    void loadNbdModule() {
        if (system("sudo modprobe nbd max_part=8") != 0) {
            std::cerr << "Не удалось загрузить модуль nbd" << std::endl;
            logger.log(Logger::Level::ERR, "Не удалось загрузить модуль nbd");
        }
        else {
            logger.log(Logger::Level::INFO, "Модуль nbd успешно загружен");
        }
    }

    void umount_img(const char* mountPoint)
    {
        if (umount2(mountPoint, MNT_DETACH) == -1) {
            logger.log(Logger::Level::ERR, "Error unmounting: " + *mountPoint + *std::strerror(errno));
            return;
        }
    }

    void mount_img(const char* mountPoint, const char* imageFile, const char* loopDevice, const char* fileSystem)
    {
        if (mkdir(mountPoint, 0755) != 0 && errno != EEXIST) {
            logger.log(Logger::Level::ERR, ("Ошибка создания точки монтирования: " + std::string(mountPoint)).c_str());
            return;
        }

        int loopFd = open(loopDevice, O_RDWR);
        if (loopFd < 0) {
            logger.log(Logger::Level::ERR, "Ошибка открытия устройства: " + std::string(loopDevice));
            return;
        }

        int imgFd = open(imageFile, O_RDONLY);
        if (imgFd < 0) {
            logger.log(Logger::Level::ERR, "Ошибка открытия образа: " + std::string(imageFile));
            close(loopFd);
            return;
        }

        char command[256];
        std::snprintf(command, sizeof(command), "losetup %s %s", loopDevice, imageFile);

        int result = system(command);
        if (result == -1) {
            logger.log(Logger::Level::ERR, "Ошибка открытия: " + *std::strerror(errno));
        }
        else {
            logger.log(Logger::Level::INFO, "Образ успешно привязан к: " + std::string(loopDevice));
        }

        if (mount(loopDevice, mountPoint, fileSystem, 0, nullptr) == -1) {
            logger.log(Logger::Level::ERR, "Ошибка монтирования образа: " + std::string(std::strerror(errno)));
        }

        close(imgFd);
        ioctl(loopFd, LOOP_CLR_FD);
        close(loopFd);
    }

    void mount_VHDX(const std::string& fsType, const std::string& device, const std::string& vhdxFile, const std::string& mountPoint)
    {
        try {
            connectVHDX(vhdxFile, fsType, mountPoint, device);
            mountVHDX(fsType, device, vhdxFile, mountPoint);
        }
        catch (const std::runtime_error& e) {
            logger.log(Logger::Level::ERR, "Ошибка монтирования");
        }
    }

    void mount_VMDK(const std::string& vhdxFile, const std::string& mountPoint, const std::string& fsType)
    {
        try {
            mount_vmdk(vhdxFile, mountPoint);
            mount_vmdk2(vhdxFile, mountPoint);
        }
        catch (const std::runtime_error& e) {
            logger.log(Logger::Level::ERR, "Ошибка монтирования");
        }
    }

    void umountVirtual(const std::string& device)
    {
        if (system(("qemu-nbd -d " + device).c_str()) != 0) {
            logger.log(Logger::Level::ERR, "Ошибка отвязывания образа");
        }
        if (system(("umount " + device).c_str()) != 0) {
            logger.log(Logger::Level::ERR, "Ошибка отмонтирования образа");
        }
    }

private:
    Logger& logger;

    void connectVHDX(const std::string& vhdxFile, const std::string& fsType, const std::string& mountPoint, const std::string& device)
    {
        try {
            if (system(("test -f " + vhdxFile).c_str()) != 0) {
                logger.log(Logger::Level::ERR, "Файл " + vhdxFile + "не существует");
            }

            if (system(("qemu-nbd --connect=" + device + " " + vhdxFile).c_str()) != 0) {
                logger.log(Logger::Level::ERR, "Ошибка подключения к " + vhdxFile);
            }

            if (system(("mkdir -p " + mountPoint).c_str()) != 0) {
                logger.log(Logger::Level::ERR, "Ошибка создания точки монтирования " + mountPoint);
            }

        }
        catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void mountVHDX(const std::string& fsType, const std::string& device, const std::string& vhdxFile, const std::string& mountPoint)
    {
        if (system(("mount -t " + fsType + " " + device + "p2 " + mountPoint).c_str()) != 0) {
            logger.log(Logger::Level::ERR, "Ошибка монтирования в" + mountPoint);
        }

        logger.log(Logger::Level::INFO, "Файл смонтирован в " + mountPoint);
    }

    void mount_vmdk(const std::string& vmdk_path, const std::string& mount_point) {
        std::string nbd_device = "/dev/nbd0";
        std::string qemu_nbd_command = "sudo qemu-nbd -c " + nbd_device + " " + vmdk_path;
        if (system(qemu_nbd_command.c_str()) != 0) {
            logger.log(Logger::Level::ERR, "Ошибка подключения к " + vmdk_path);
        }
    }

    void mount_vmdk2(const std::string& vmdk_path, const std::string& mount_point)
    {
        std::string nbd_device = "/dev/nbd0";
        std::string mount_command = "sudo mount " + nbd_device + "p1 " + mount_point;
        if (system(mount_command.c_str()) != 0) {
            system(("sudo qemu-nbd -d " + nbd_device).c_str());
            logger.log(Logger::Level::ERR, "Ошибка монтирования образа" + vmdk_path);
        }
        logger.log(Logger::Level::INFO, "Файл смонтирован в " + mount_point);
    }
};







#endif // __unix__



#endif // FillDiskH
