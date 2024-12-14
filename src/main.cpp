#include <iostream>
#include <vector>
#include <string>
#include "hopfieldnet.hpp"
#include "prints.hpp"

/*Функция запуска прогона сети. Принимает количество нейронов в сети, имя теста,
значение "требуется сохранение в файл?"
 */
void hopfieldStart(int n = 256, std::string Test_Path = "", bool saveToFile = true) {
    // Получаем текущий путь проекта
    fs::path projectPath = fs::current_path().parent_path();


    // Формируем путь до директории тестов
    fs::path baseTestPath = projectPath / "Tests" / Test_Path;

    if (!fs::exists(baseTestPath) || !fs::is_directory(baseTestPath)) {
        std::cerr << "Ошибка: Указанный тест не существует или не является директорией: " << Test_Path << std::endl;
        return;
    }
    fs::path patternPath = baseTestPath / "input";
    fs::path noisePath = baseTestPath / "noise";

    if (!fs::exists(patternPath)) {
        std::cerr << "Ошибка: Директория input с эталонными векторами не найдена: " << patternPath << std::endl;
        return;
    }

    if (!fs::exists(noisePath)) {
        std::cerr << "Ошибка: Директория noise с зашумленными векторами не найдена: " << noisePath << std::endl;
        return;
    }

    // Вывод информации о путях
    std::cout << "-----Путь до эталонных векторов = " << patternPath << std::endl;
    std::cout << "-----Путь до зашумленных векторов = " << noisePath << std::endl;

    std::vector<std::string> fileContents;
    std::vector<std::string> pattern_name;
    readFilesFromFolder(patternPath, fileContents, pattern_name);
    std::filesystem::path fileName;
    std::ofstream outFile;
    if (saveToFile) {
        int fileIndex = 1;

        //относительный путь к папке результатов
        std::filesystem::path resultsDir = projectPath  / "results";

        std::filesystem::create_directories(resultsDir);

        //Базовое имя файла
        std::filesystem::path baseName = resultsDir / ("output_" + std::to_string(n));
        do {
            fileName = baseName.string() + "_" + std::to_string(fileIndex) + ".txt";
            fileIndex++;
        } while (std::filesystem::exists(fileName));

        outFile.open(fileName);
        if (!outFile) {
            std::cerr << "Ошибка: не удалось открыть файл для записи: " << fileName << std::endl;
            return;
        }
    }

    for (size_t i = 0; i < fileContents.size(); ++i) {
        std::string output = "Файл " + std::to_string(i + 1) + " " + pattern_name[i] + ":\n";
        std::cout << output;
        if (saveToFile) outFile << output;
        dollprint(fileContents[i], std::cout, false);
        if (saveToFile) dollprint(fileContents[i], outFile, false);
    }

    //инициализация сети
    HopfieldNet g(n);

    //подготовка эталонов
    std::vector<std::vector<int> > patterns;
    convertToPatterns(fileContents, patterns);

    //вычисление скалярного произведения эталонов
    calculateDotProducts(patterns, pattern_name, std::cout);
    if (saveToFile) calculateDotProducts(patterns, pattern_name, outFile);

    //обучение сети
    g.train(patterns);

    //подготовка "шумных" изображений
    std::vector<std::string> noise_string;
    std::vector<std::string> noise_name;
    readFilesFromFolder(noisePath, noise_string, noise_name);


    std::vector<std::vector<int> > noise_input;
    convertToPatterns(noise_string, noise_input);

    for (size_t i = 0; i < noise_string.size(); ++i) {
        std::string noiseOutput = "Файл с шумом " + std::to_string(i + 1) + " " + noise_name[i] + ":\n";
        std::cout << noiseOutput;
        if (saveToFile) outFile << noiseOutput;
        dollprint(noise_string[i], std::cout, false);
        if (saveToFile) dollprint(noise_string[i], outFile, false);

        g.setStates(noise_input[i]);
        size_t steps = g.runUntilStable();

        std::string stabilizationOutput = "Стабилизация достигнута за " + std::to_string(steps) + " шагов.\n";
        std::cout << stabilizationOutput;
        if (saveToFile) outFile << stabilizationOutput;

        std::string finalStatesOutput = "Итоговые состояния нейронов:\n";
        std::cout << finalStatesOutput;
        if (saveToFile) outFile << finalStatesOutput;

        dollprint(g.getStates(), std::cout);
        if (saveToFile) dollprint(g.getStates(), outFile);
    }

    if (saveToFile) {
        outFile.close();
        std::cout << "Результаты сохранены в файл. Путь " << fileName.string() << std::endl;
    }
}


int main() {

    // Тесты
    /* Если у вас возникают проблемы с чтением файлов тестов, проверьте корректно ли
     * программа определяет папку проекта (см projectPath в функции hopfieldStart).
     */
    hopfieldStart(256, "Test16px_1", true);

    // Адекватные образы
    hopfieldStart(1024, "Test32px_3", true) ;
    hopfieldStart(1024, "Test32px_4", true);

    // Все фигуры не коррелирующие
    hopfieldStart(1024, "Test32px_2_1", true);

    // Вертолет и человек, много коррелируют но их всего 2, Хороший результат
    hopfieldStart(1024, "Test32px_2_2", true);

    // Вертолет и человек и робот, много коррелируют но уже 3, Будет плохой результат
    hopfieldStart(1024, "Test32px_2_3", true);

    // Цифры
    hopfieldStart(256, "Test16px_1", true);

}
