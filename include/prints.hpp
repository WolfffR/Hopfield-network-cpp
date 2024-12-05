//
//

#ifndef PRINTS_HPP
#define PRINTS_HPP
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


template <typename T>
void printMatrix(const std::vector<std::vector<T>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
}

void readFile(const std::string& filePath, std::vector<std::string>& fileContents, bool addToVector = true) {
    try {
        // Проверяем существование файла
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл: " + filePath);
        }

        std::string line;
        if (addToVector) {
            // Добавляем строки файла в конец вектора
            while (std::getline(file, line)) {
                fileContents.push_back(line);
            }
        } else {

            fileContents.clear();
            while (std::getline(file, line)) {
                fileContents.push_back(line);
            }
        }

        file.close();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}


void readFilesFromFolder(const std::string& folderPath, std::vector<std::string>& fileContents) {
    try {

        if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
            throw std::runtime_error("Указанный путь не является папкой или не существует.");
        }

        // Перебираем файлы в папке
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) { //Проверяем, что это файл
                std::ifstream file(entry.path());
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                         std::istreambuf_iterator<char>()); //Читаем файл в строку
                    fileContents.push_back(content); //Добавляем содержимое в массив
                    file.close();
                } else {
                    std::cerr << "Не удалось открыть файл: " << entry.path() << "\n";
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void convertToPatterns(const std::vector<std::string>& fileContents, std::vector<std::vector<int>>& patterns) {
    for (const auto& content : fileContents) {
        std::vector<int> pattern;
        for (char c : content) {
            if (c == '0') {
                pattern.push_back(-1);
            } else if (c == '1') {
                pattern.push_back(1);
            }

        }

        patterns.push_back(pattern);
    }
}
void convertToPatterns(const std::string& fileContents, std::vector<std::vector<int>>& patterns) {
    std::vector<int> pattern;

    for (char c : fileContents) {
        if (c == '0') {
            pattern.push_back(-1);
        } else if (c == '1') {
            pattern.push_back(1);
        } else if (c == '\n') {

            if (!pattern.empty()) {
                patterns.push_back(pattern);
                pattern.clear();
            }
        }
        // Игнорируем все остальные символы
    }


    if (!pattern.empty()) {
        patterns.push_back(pattern);
    }
}



void dollprint(const std::string& input, bool isFile = true) {
    std::string content;

    if (isFile) {

        std::ifstream file(input);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл: " << input << "\n";
            return;
        }
        std::getline(file, content, '\0');
        file.close();
    } else {
        //Используем переданную строку напрямую
        content = input;
    }

    //Обработка содержимого
    size_t count = 0;
    for (char c : content) {
        if (c == '0') {
            std::cout << " ";
        } else if (c == '1') {
            std::cout << "$";
        } else {
            continue; //Пропускаем символы, которые не 0 или 1
        }

        //Переход на новую строку после 16 символов
        if (++count == 16) {
            std::cout << "\n";
            count = 0;
        }
    }

    //Завершаем строку если осталось что-то выводить
    if (count != 0) {
        std::cout << "\n";
    }
}

void dollprint(std::vector<int> input) {
    std::string content;

    //Проход по вектору и преобразование в строку
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == -1) {input[i] = 0;}
        content += std::to_string(input[i]); //Конвертация числа в строку
    }
    //Обработка содержимого
    size_t count = 0;
    for (char c : content) {
        if (c == '0') {
            std::cout << " ";
        } else if (c == '1') {
            std::cout << "$";
        } else {
            continue; //Пропускаем символы, которые не '0' или '1'
        }

        //Переход на новую строку после 16 символов
        if (++count == 16) {
            std::cout << "\n";
            count = 0;
        }
    }

    //Завершаем строку, если осталось что-то выводить
    if (count != 0) {
        std::cout << "\n";
    }
}
#endif //PRINTS_HPP
