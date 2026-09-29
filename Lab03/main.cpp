#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cctype>

enum class MediaType {
    Audio,
    Video,
    Image,
    Document
};

struct Date {
    int day = 1;
    int month = 1;
    int year = 2000;
};

struct MediaFile {
    int id = 0;
    std::string filename = "";
    double size = 0.0;
    MediaType type = MediaType::Document;
    Date creation_date;
};


/*
 * Преобразование типа медиафайла в строку.
 *
 * @param type тип медиафайла.
 * @return возвращает название типа.
 */
std::string typeToString(const MediaType type) {
    switch(type) {
        case MediaType::Audio:
            return "Audio";
        case MediaType::Video:
            return "Video";
        case MediaType::Image:
            return "Image";
        case MediaType::Document:
            return "Document";
    }

    return "Unknown";
}


/*
 * Получение случайного целого числа.
 *
 * @param min минимальное значение.
 * @param max максимальное значение.
 * @return возвращает случайное целое число.
 */
int getRandomInt(const int min, const int max) {
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}


/*
 * Получение случайного вещественного числа.
 *
 * @param min минимальное значение.
 * @param max максимальное значение.
 * @return возвращает случайное вещественное число.
 */
double getRandomDouble(const double min, const double max) {
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(generator);
}


/*
 * Генерация случайной даты.
 *
 * @return возвращает случайную дату.
 */
Date generateRandomDate() {
    Date date;

    date.year = getRandomInt(2000, 2026);
    date.month = getRandomInt(1, 12);
    date.day = getRandomInt(1, 28);

    return date;
}


/*
 * Генерация случайного медиафайла.
 *
 * @param id уникальный идентификатор файла.
 * @return возвращает сгенерированный медиафайл.
 */
MediaFile generateRandomFile(const int id) {
    const std::string NAMES[] = {
        "music",
        "video",
        "photo",
        "document",
        "report",
        "holiday",
        "project",
        "lecture"
    };

    const std::string EXTENSIONS[] = {
        ".mp3",
        ".mp4",
        ".jpg",
        ".pdf"
    };

    MediaFile file;

    file.id = id;

    const int type_number = getRandomInt(0, 3);
    file.type = static_cast<MediaType>(type_number);

    const int name_number = getRandomInt(0, 7);

    file.filename =
        NAMES[name_number] +
        std::to_string(id) +
        EXTENSIONS[type_number];

    file.size = getRandomDouble(1.0, 1000.0);
    file.creation_date = generateRandomDate();

    return file;
}


/*
 * Заполнение массива случайными медиафайлами.
 *
 * @param files массив медиафайлов.
 */
void fillFiles(std::vector<MediaFile>& files) {
    for(int i = 0; i < static_cast<int>(files.size()); i++) {
        files[i] = generateRandomFile(i + 1);
    }
}


/*
 * Вывод информации об одном медиафайле.
 *
 * @param file медиафайл.
 */
void printFile(const MediaFile& file) {
    std::cout
        << "ID: " << file.id
        << " | Name: " << file.filename
        << " | Size: " << std::fixed << std::setprecision(2)
        << file.size << " MB"
        << " | Type: " << typeToString(file.type)
        << " | Date: "
        << std::setfill('0') << std::setw(2)
        << file.creation_date.day << "."
        << std::setw(2) << file.creation_date.month << "."
        << std::setfill(' ') << file.creation_date.year
        << '\n';
}


/*
 * Вывод массива медиафайлов.
 *
 * @param files массив медиафайлов.
 */
void printFiles(const std::vector<MediaFile>& files) {
    if(files.empty()) {
        std::cout << "Files not found.\n";
        return;
    }

    for(const MediaFile& file : files) {
        printFile(file);
    }
}


/*
 * Преобразование строки в нижний регистр.
 *
 * @param text исходная строка.
 * @return возвращает строку в нижнем регистре.
 */
std::string toLower(std::string text) {
    for(char& symbol : text) {
        symbol = static_cast<char>(
            std::tolower(static_cast<unsigned char>(symbol))
        );
    }

    return text;
}


/*
 * Поиск файлов по части имени без учета регистра.
 *
 * @param files массив медиафайлов.
 * @param substring искомая подстрока.
 * @return возвращает массив найденных файлов.
 */
std::vector<MediaFile> searchByName(
    const std::vector<MediaFile>& files,
    const std::string& substring
) {
    std::vector<MediaFile> result;

    const std::string search_text = toLower(substring);

    for(const MediaFile& file : files) {
        const std::string filename = toLower(file.filename);

        if(filename.find(search_text) != std::string::npos) {
            result.push_back(file);
        }
    }

    return result;
}


/*
 * Фильтрация файлов по типу и минимальному размеру.
 *
 * @param files массив медиафайлов.
 * @param type необходимый тип файла.
 * @param min_size минимальный размер файла.
 * @return возвращает массив подходящих файлов.
 */
std::vector<MediaFile> filterFiles(
    const std::vector<MediaFile>& files,
    const MediaType type,
    const double min_size
) {
    std::vector<MediaFile> result;

    for(const MediaFile& file : files) {
        if(file.type == type && file.size > min_size) {
            result.push_back(file);
        }
    }

    return result;
}


/*
 * Вывод статистики по массиву медиафайлов.
 *
 * @param files массив медиафайлов.
 */
void printStatistics(const std::vector<MediaFile>& files) {
    double total_size = 0.0;

    int audio_count = 0;
    int video_count = 0;
    int image_count = 0;
    int document_count = 0;

    for(const MediaFile& file : files) {
        total_size += file.size;

        switch(file.type) {
            case MediaType::Audio:
                audio_count++;
                break;

            case MediaType::Video:
                video_count++;
                break;

            case MediaType::Image:
                image_count++;
                break;

            case MediaType::Document:
                document_count++;
                break;
        }
    }

    std::cout << "\nStatistics:\n";
    std::cout << "Total files: " << files.size() << '\n';

    std::cout
        << "Total size: "
        << std::fixed << std::setprecision(2)
        << total_size << " MB\n";

    std::cout << "Audio: " << audio_count << '\n';
    std::cout << "Video: " << video_count << '\n';
    std::cout << "Image: " << image_count << '\n';
    std::cout << "Document: " << document_count << '\n';
}


/*
 * Сравнение двух дат.
 *
 * @param first первая дата.
 * @param second вторая дата.
 * @return возвращает true, если первая дата новее второй.
 */
bool isDateNewer(const Date& first, const Date& second) {
    if(first.year != second.year) {
        return first.year > second.year;
    }

    if(first.month != second.month) {
        return first.month > second.month;
    }

    return first.day > second.day;
}


/*
 * Сортировка файлов от новых к старым.
 * При одинаковой дате файлы сортируются по имени.
 *
 * @param files массив медиафайлов.
 */
void sortFiles(std::vector<MediaFile>& files) {
    std::sort(
        files.begin(),
        files.end(),
        [](const MediaFile& first, const MediaFile& second) {
            if(
                first.creation_date.year == second.creation_date.year &&
                first.creation_date.month == second.creation_date.month &&
                first.creation_date.day == second.creation_date.day
            ) {
                return first.filename < second.filename;
            }

            return isDateNewer(
                first.creation_date,
                second.creation_date
            );
        }
    );
}


/*
 * Преобразование номера в тип медиафайла.
 *
 * @param number номер типа от 1 до 4.
 * @return возвращает соответствующий тип медиафайла.
 */
MediaType numberToType(const int number) {
    switch(number) {
        case 1:
            return MediaType::Audio;
        case 2:
            return MediaType::Video;
        case 3:
            return MediaType::Image;
        default:
            return MediaType::Document;
    }
}


int main() {
    int n = 0;

    std::cout << "Enter number of files: ";
    std::cin >> n;

    if(n <= 0) {
        std::cout << "Incorrect number of files.\n";
        return 0;
    }

    std::vector<MediaFile> files(n);

    fillFiles(files);

    std::cout << "\nGenerated files:\n";
    printFiles(files);

    int choice = -1;

    while(choice != 0) {
        std::cout << "\nMenu:\n";
        std::cout << "1 - Search by name\n";
        std::cout << "2 - Filter by type and size\n";
        std::cout << "3 - Statistics\n";
        std::cout << "4 - Sort by creation date\n";
        std::cout << "5 - Show all files\n";
        std::cout << "0 - Exit\n";
        std::cout << "Choose action: ";

        std::cin >> choice;

        if(choice == 1) {
            std::string substring = "";

            std::cout << "Enter part of filename: ";
            std::cin >> substring;

            const std::vector<MediaFile> result =
                searchByName(files, substring);

            std::cout << "\nSearch result:\n";
            printFiles(result);
        }
        else if(choice == 2) {
            int type_number = 0;
            double min_size = 0.0;

            std::cout << "1 - Audio\n";
            std::cout << "2 - Video\n";
            std::cout << "3 - Image\n";
            std::cout << "4 - Document\n";
            std::cout << "Choose type: ";
            std::cin >> type_number;

            if(type_number < 1 || type_number > 4) {
                std::cout << "Incorrect type.\n";
                continue;
            }

            std::cout << "Enter minimum size (MB): ";
            std::cin >> min_size;

            const MediaType type = numberToType(type_number);

            const std::vector<MediaFile> result =
                filterFiles(files, type, min_size);

            std::cout << "\nFilter result:\n";
            printFiles(result);
        }
        else if(choice == 3) {
            printStatistics(files);
        }
        else if(choice == 4) {
            sortFiles(files);

            std::cout << "\nFiles sorted from newest to oldest:\n";
            printFiles(files);
        }
        else if(choice == 5) {
            std::cout << "\nAll files:\n";
            printFiles(files);
        }
        else if(choice != 0) {
            std::cout << "Incorrect menu item.\n";
        }
    }

    std::cout << "Program finished.\n";

    return 0;
}
