#include <iostream>
#include <vector>
#include <string>
#include "hopfieldnet.hpp"
#include "prints.hpp"


int main() {

    std::cout << "Hello World!" << std::endl;

    HopfieldNet g(5);

    std::vector<std::vector<int>> patterns;
    patterns = {{1,1,1,1,-1}, {-1,1,-1,-1,1}, {-1,-1,-1,1,-1}};
    g.train(patterns);

    //Входное состояние
    std::vector<std::string> noise15_string = {"00001", "11111"};
    std::vector<std::vector<int>> noise15_input;
    convertToPatterns(noise15_string, noise15_input);

    for (size_t i = 0; i < noise15_string.size(); ++i) {
        std::cout << "Файлы с шумом 15% " << i + 1 << ":\n";
        dollprint(noise15_string[i], false);

        g.setStates(noise15_input[i]);
        // Запуск сети
        size_t steps = g.runUntilStable();

        // Результаты
        std::cout << "Стабилизация достигнута за " << steps << " шагов." << std::endl;
        std::cout << "Итоговые состояния нейронов:" << std::endl;

        for (int state : g.getStates()) {
            std::cout << state << " ";
        }
        std::cout << std::endl;
        dollprint( g.getStates());
    }


    return 0;
}