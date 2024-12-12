//
//

#ifndef PRINTS_HPP
#define PRINTS_HPP
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>


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


void readFilesFromFolder(const std::string& folderPath, std::vector<std::string>& fileContents, std::vector<std::string>& string_name) {
    try {
        if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
            throw std::runtime_error("Указанный путь не является папкой или не существует.");
        }

        std::vector<std::pair<std::string, std::string>> fileData;

        // Перебираем файлы в папке
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) { // Проверяем, что это файл
                std::ifstream file(entry.path());
                std::string fileName = fs::path(entry).filename().string();
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                         std::istreambuf_iterator<char>()); // Читаем файл в строку
                    fileData.emplace_back(fileName, content); // Добавляем имя файла и его содержимое во временный вектор
                    file.close();
                } else {
                    std::cerr << "Не удалось открыть файл: " << entry.path() << "\n";
                }
            }
        }

        // Сортируем по имени файла
        std::sort(fileData.begin(), fileData.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
            return a.first < b.first;
        });

        // Переносим данные в исходные массивы
        for (const auto& [fileName, content] : fileData) {
            string_name.push_back(fileName);
            fileContents.push_back(content);
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



// void dollprint(const std::string& input, bool isFile = true) {
//     std::string content;
//
//     if (isFile) {
//
//         std::ifstream file(input);
//         if (!file.is_open()) {
//             std::cerr << "Не удалось открыть файл: " << input << "\n";
//             return;
//         }
//         std::getline(file, content, '\0');
//         file.close();
//     } else {
//         //Используем переданную строку напрямую
//         content = input;
//     }
//     // Удаляем все символы, кроме '0' и '1'
//     std::string filteredContent;
//     for (char c : content) {
//         if (c == '0' || c == '1') {
//             filteredContent += c;
//         }
//     }
//
//     // Рассчитываем размер блока для перевода строки
//     size_t n = filteredContent.size();
//     size_t blockSize = static_cast<size_t>(std::sqrt(n));
//
//     //Обработка содержимого
//     size_t count = 0;
//     for (char c : filteredContent) {
//         if (c == '0') {
//             std::cout << " ";
//         } else if (c == '1') {
//             std::cout << "$";
//         }
//
//         // Переход на новую строку после каждого blockSize символов
//         if (++count == blockSize) {
//             std::cout << "\n";
//             count = 0;
//         }
//     }
//
//     // Завершаем строку, если осталось что-то выводить
//     if (count != 0) {
//         std::cout << "\n";
//     }
// }
//
// void dollprint(std::vector<int> input) {
//     std::string content;
//
//     //Проход по вектору и преобразование в строку
//     for (size_t i = 0; i < input.size(); ++i) {
//         if (input[i] == -1) {input[i] = 0;}
//         content += std::to_string(input[i]); //Конвертация числа в строку
//     }
//     input.erase(std::remove(input.begin(), input.end(), -1), input.end());
//     //Обработка содержимого
//     // Вычисляем размер блока для перевода строки
//     size_t n = input.size();
//     size_t blockSize = static_cast<size_t>(std::sqrt(n));
//     size_t count = 0;
//     for (char c : content) {
//         if (c == '0') {
//             std::cout << " ";
//         } else if (c == '1') {
//             std::cout << "$";
//         }
//
//         // Переход на новую строку после каждого blockSize символа
//         if (++count == blockSize) {
//             std::cout << "\n";
//             count = 0;
//         }
//     }
//
//     // Завершаем строку, если осталось что-то выводить
//     if (count != 0) {
//         std::cout << "\n";
//     }
// }
std::vector<int> replaceMinusOneWithZero(const std::vector<int>& input) {
    std::vector<int> result = input;  // Создаем копию исходного вектора
    for (int& element : result) {     // Проходим по всем элементам
        if (element == -1) {
            element = 0;  // Заменяем -1 на 0
        }
    }
    return result;  // Возвращаем обновленный вектор
}
void dollprint(const std::string& input, std::ostream& out = std::cout, bool isFile = true) {
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
    for (char c : content) {
        if (c == '0' || c == '1') {
            filteredContent += c;
        }
    }

    size_t n = filteredContent.size();
    size_t blockSize = static_cast<size_t>(std::sqrt(n));

    size_t count = 0;
    for (char c : filteredContent) {
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

void dollprint(const std::vector<int>& input, std::ostream& out = std::cout) {
    std::vector<int>post_input = replaceMinusOneWithZero(input);
    size_t n = input.size();
    size_t blockSize = static_cast<size_t>(std::sqrt(n));

    size_t count = 0;
    for (int c : post_input) {
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
#endif //PRINTS_HPP
