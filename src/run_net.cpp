#include <iostream>
#include <vector>
#include <string>
#include "hopfieldnet.hpp"
#include "prints.hpp"


void hopfieldStart(int n = 256, std::string Pattern_Path = "", bool patternIsFolder = true, std::string Noise_Path = "",
                   bool noiseIsFolder = true) {
    std::cout << "-----Путь до эталонных векторов= "<< Pattern_Path<< std::endl;
    std::cout << "-----Путь до угадываемых векторов= "<< Noise_Path << std::endl;
    std::vector<std::string> fileContents;
    if (patternIsFolder) {
        //Читаем файлы из папки
        readFilesFromFolder(Pattern_Path, fileContents);
    } else {
        readFile(Pattern_Path, fileContents);
    }

    //Выводим содержимое всех файлов
    for (size_t i = 0; i < fileContents.size(); ++i) {
        std::cout << "Файл " << i + 1 << ":\n";
        dollprint(fileContents[i], false);
    }

    HopfieldNet g(n);

    std::vector<std::vector<int> > patterns;
    convertToPatterns(fileContents, patterns);
    g.train(patterns);

    //Входное состояние
    std::vector<std::string> noise_string;
    if (noiseIsFolder) {
        readFilesFromFolder(Noise_Path, noise_string);
    } else {
        readFile(Noise_Path, noise_string);
    }

    std::vector<std::vector<int> > noise_input;
    convertToPatterns(noise_string, noise_input);

    for (size_t i = 0; i < noise_string.size(); ++i) {
        std::cout << "Файл с шумом " << i + 1 << ":\n";
        dollprint(noise_string[i], false);

        g.setStates(noise_input[i]);
        //Запуск сети
        size_t steps = g.runUntilStable();


        std::cout << "Стабилизация достигнута за " << steps << " шагов." << std::endl;
        std::cout << "Итоговые состояния нейронов:" << std::endl;

        for (int state: g.getStates()) {
            std::cout << state << " ";
        }
        std::cout << std::endl;
        dollprint(g.getStates());
    }
}


int main() {
    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test1/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test1/noise", true);
    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test2/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test2/noise", true);
    // hopfieldStart(256, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test3/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test3/noise", true);
    hopfieldStart(1024, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32_2/input", true,
        "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test32_2/noise", true);
    // hopfieldStart(4096, "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test64_1/input", true,
    //     "/Users/egordruk/CLionProjects/Hopfield-network-cpp/Tests/Test64_1/noise", true);

}
