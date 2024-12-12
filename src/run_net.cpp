#include <iostream>
#include <vector>
#include <string>
#include "hopfieldnet.hpp"
#include "prints.hpp"


void hopfieldStart(int n = 256, std::string Pattern_Path = "", bool patternIsFolder = true, std::string Noise_Path = "",
                   bool noiseIsFolder = true, bool saveToFile = true) {
    std::cout << "-----Путь до эталонных векторов= " << Pattern_Path << std::endl;
    std::cout << "-----Путь до угадываемых векторов= " << Noise_Path << std::endl;

    std::vector<std::string> fileContents;
    std::vector<std::string> pattern_name;
    if (patternIsFolder) {
        readFilesFromFolder(Pattern_Path, fileContents, pattern_name);
    } else {
        readFile(Pattern_Path, fileContents);
    }

    std::ofstream outFile;
    if (saveToFile) {
        int fileIndex = 1;
        std::string resultsDir = "/Users/egordruk/CLionProjects/Hopfield-network-cpp/results";
        std::filesystem::create_directories(resultsDir);

        std::string baseName = resultsDir + "/output_" + std::to_string(n);
        std::string fileName;

        do {
            fileName = baseName + "_" + std::to_string(fileIndex) + ".txt";
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

    HopfieldNet g(n);

    std::vector<std::vector<int>> patterns;
    convertToPatterns(fileContents, patterns);
    g.train(patterns);

    std::vector<std::string> noise_string;
    std::vector<std::string> noise_name;
    if (noiseIsFolder) {
        readFilesFromFolder(Noise_Path, noise_string, noise_name);
    } else {
        readFile(Noise_Path, noise_string);
    }

    std::vector<std::vector<int>> noise_input;
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
        std::cout << "Результаты сохранены в файл." << std::endl;
    }
}


int main() {
    // hopfieldStart(1024, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32px_3/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32px_3/noise", true);
    hopfieldStart(1024, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32px_4/input", true,
        "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32px_4/noise", true);

    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_1/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_1/noise", true);
    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_2/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_2/noise", true);
    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_3/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_3/noise", true);
    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_4/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test16px_4/noise", true);
    // hopfieldStart(1024, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32px_2/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32px_2/noise", true);
    // hopfieldStart(4096, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test64px_1/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test64px_1/noise", true);

}
