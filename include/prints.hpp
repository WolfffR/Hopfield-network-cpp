#ifndef PRINTS_HPP
#define PRINTS_HPP
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>


namespace fs = std::filesystem;

/*Выводит двумерный массив*/
template<typename T>
void printMatrix(const std::vector<std::vector<T> > &matrix) {
    for (const auto &row: matrix) {
        for (const auto &element: row) {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
}

/*Читает файл из папки*/
void readFile(const std::string &filePath, std::vector<std::string> &fileContents, bool addToVector = true) {
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
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

/*Читает файлы из папки*/
void readFilesFromFolder(const std::string &folderPath, std::vector<std::string> &fileContents,
                         std::vector<std::string> &string_name) {
    try {
        if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
            throw std::runtime_error("Указанный путь не является папкой или не существует.");
        }

        std::vector<std::pair<std::string, std::string> > fileData;

        // Перебираем файлы в папке
        for (const auto &entry: fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                // Проверяем, что это файл
                std::ifstream file(entry.path());
                std::string fileName = fs::path(entry).filename().string();
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>()); // Читаем файл в строку
                    fileData.emplace_back(fileName, content);
                    // Добавляем имя файла и его содержимое во временный вектор
                    file.close();
                } else {
                    std::cerr << "Не удалось открыть файл: " << entry.path() << "\n";
                }
            }
        }

        // Сортируем по имени файла
        std::sort(fileData.begin(), fileData.end(),
                  [](const std::pair<std::string, std::string> &a, const std::pair<std::string, std::string> &b) {
                      return a.first < b.first;
                  });

        // Переносим данные в исходные массивы
        for (const auto &[fileName, content]: fileData) {
            string_name.push_back(fileName);
            fileContents.push_back(content);
        }
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}
/*Конвертирует std::string в std::vector<std::vector<int>> , при этом меняет 0 на -1*/
void convertToPatterns(const std::vector<std::string> &fileContents, std::vector<std::vector<int> > &patterns) {
    for (const auto &content: fileContents) {
        std::vector<int> pattern;
        for (char c: content) {
            if (c == '0') {
                pattern.push_back(-1);
            } else if (c == '1') {
                pattern.push_back(1);
            }
        }

        patterns.push_back(pattern);
    }
}
/*Конвертирует std::string в std::vector<int> , при этом меняет 0 на -1*/
void convertToPatterns(const std::string &fileContents, std::vector<std::vector<int>> &patterns) {
    std::vector<int> pattern;

    for (char c: fileContents) {
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

/*Возразает копию с изменениями. Меняет -1 на 0.*/
std::vector<int> replaceMinusOneWithZero(const std::vector<int> &input) {
    std::vector<int> result = input; // Создаем копию исходного вектора
    for (int &element: result) {
        // Проходим по всем элементам
        if (element == -1) {
            element = 0; // Заменяем -1 на 0
        }
    }
    return result; // Возвращаем обновленный вектор
}

/*Вывод вектора в читабельном формате*/
void dollprint(const std::string &input, std::ostream &out = std::cout, bool isFile = true) {
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
        content = input;
    }

    std::string filteredContent;
    for (char c: content) {
        if (c == '0' || c == '1') {
            filteredContent += c;
        }
    }

    size_t n = filteredContent.size();
    size_t blockSize = static_cast<size_t>(std::sqrt(n));

    size_t count = 0;
    for (char c: filteredContent) {
        if (c == '0') {
            out << " ";
        } else if (c == '1') {
            out << "$";
        }

        if (++count == blockSize) {
            out << "\n";
            count = 0;
        }
    }

    if (count != 0) {
        out << "\n";
    }
}
/*Вывод вектора в читабельном формате*/
void dollprint(const std::vector<int> &input, std::ostream &out = std::cout) {
    std::vector<int> post_input = replaceMinusOneWithZero(input);
    size_t n = input.size();
    size_t blockSize = static_cast<size_t>(std::sqrt(n));

    size_t count = 0;
    for (int c: post_input) {
        if (c == 0) {
            out << " ";
        } else if (c == 1) {
            out << "$";
        }

        if (++count == blockSize) {
            out << "\n";
            count = 0;
        }
    }

    if (count != 0) {
        out << "\n";
    }
}

/*Функция для расчета скалярного произведения двух векторов*/
int calculateDotProduct(const std::vector<int> &vec1, const std::vector<int> &vec2) {
    if (vec1.size() != vec2.size()) {
        throw std::invalid_argument("Векторы должны быть одинаковой длины.");
    }

    int dotProduct = 0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        dotProduct += vec1[i] * vec2[i];
    }
    return dotProduct;
}

/*Функция для расчета скалярного произведения между массивом эталонов*/
void calculateDotProducts(const std::vector<std::vector<int> > &patterns, std::vector<std::string> pattern_name,
                          std::ostream &out) {
    size_t numPatterns = patterns.size();
    out << "---Скалярное произведение эталонов---\n 0 - отсутствие корреляции(хорошо)"
            "\n Положительное значение - увеличивается за точные совпадения в элементах"
            "\n Отрицательные значение - увеличивается за несовпадения в элементах\n"
            "\n Выводы делаются по значению модуля"
            "\n Максимальное значение для данных эталонов = " << patterns[0].size() << std::endl;
    for (size_t i = 0; i < numPatterns; ++i) {
        for (size_t j = i + 1; j < numPatterns; ++j) {
            try {
                int dotProduct = calculateDotProduct(patterns[i], patterns[j]);
                out << "Скалярное произведение между паттернами " << pattern_name[i] << " и " << pattern_name[j] << ": "
                        << dotProduct << std::endl;
            } catch (const std::invalid_argument &e) {
                out << "Ошибка: " << e.what() << std::endl;
            }
        }
    }
}
#endif //PRINTS_HPP
