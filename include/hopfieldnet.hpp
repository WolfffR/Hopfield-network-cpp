#ifndef HOPFIELDNET_HPP
#define HOPFIELDNET_HPP

#include "neuron.hpp"
#include "prints.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <random>


int activation_function(auto x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}
// гиперболический тангенс
float tanhActivation(float h) {
    return std::tanh(h);
}
// Сигмоидальная функция активации
float sigmoid(float z) {
    return 1.0f / (1.0f + std::exp(-z));
}

class HopfieldNet {
private:
    size_t kolvo_neurons;
    std::vector<Neuron> neurons;
/*Вычисляет взвешенную сумму для нейрона. Указывается массив нейронов.*/
    float computeWeightedSum(size_t neuronIndex, std::vector<Neuron>& previousStates) const {
        float sum = 0.0;
        for (size_t i = 0; i < previousStates.size(); ++i) {
            if (i != neuronIndex) {
                sum += previousStates[neuronIndex].getWeights()[i] * previousStates[i].getState();;
            }
        }
        return sum;
    }

public:
    // Конструктор
    HopfieldNet(size_t kolvo_neurons) {
        this->kolvo_neurons = kolvo_neurons;
        // Создаем нейроны и добавляем их в вектор neurons
        for (size_t i = 0; i < kolvo_neurons; ++i) {
            neurons.emplace_back(0, "Neuron" + std::to_string(i));
            neurons.back().weights_in(std::vector<float>(kolvo_neurons, 0)); // Инициализируем веса нулями
        }
    }


    /*Нормализация данных для функции train*/
    void normalizeWeights(std::vector<std::vector<float> > &weights, int numPatterns) {
        float normalizationFactor = 1.0 / numPatterns;

        for (size_t i = 0; i < weights.size(); ++i) {
            for (size_t j = 0; j < weights[i].size(); ++j) {
                weights[i][j] = (weights[i][j] * normalizationFactor);
            }
        }
    }

    /*Обучение сети. Вычисление весовой матрицы. Занесение весов в нейроны*/
    void train(const std::vector<std::vector<int> > &patterns) {
        std::vector<std::vector<int>> weights(kolvo_neurons, std::vector<int>(kolvo_neurons, 0));

        //матрица весов
        for (size_t i = 0; i < kolvo_neurons; ++i) {
            for (size_t j = 0; j < i; ++j) {
                // Только нижний треугольник
                int sum = 0;
                for (const auto &image: patterns) {
                    sum += image[i] * image[j];
                }
                weights[i][j] = sum;
                weights[j][i] = sum; // Симметричная матрица
            }
        }


        std::vector<std::vector<float>> weightsFloat(weights.size(), std::vector<float>(weights[0].size()));
        for (size_t i = 0; i < weights.size(); ++i) {
            for (size_t j = 0; j < weights[i].size(); ++j) {
                weightsFloat[i][j] = static_cast<float>(weights[i][j]);
            }
        }
        //нормализация весов
        //normalizeWeights(weightsFloat, patterns[0].size());

        //printMatrix(weights);
        // std::cout << "_____После нормализации" << std::endl;
        // printMatrix(weightsFloat);

        //занесение весов в нейроны
        for (size_t i = 0; i < kolvo_neurons; ++i) {
            neurons[i].weights_in(weightsFloat[i]);
        }
    }


    /*Установка состояний нейронов*/
    void setStates(const std::vector<int> &newStates) {
        if (newStates.size() != neurons.size()) {
            throw std::invalid_argument("Размер входного состояния не совпадает с числом нейронов");
        }
        for (size_t i = 0; i < neurons.size(); ++i) {
            neurons[i].setState(newStates[i]);
        }
    }

    /*Получение текущих состояний нейронов*/
    std::vector<int> getStates() const {
        std::vector<int> states;
        for (const auto &neuron: neurons) {
            states.push_back(neuron.getState());
        }
        return states;
    }

    /*Установить веса для одного нейрона из сети*/
    void setWeights(size_t neuronIndex, const std::vector<float> &newWeights) {
        if (neuronIndex >= neurons.size()) {
            throw std::invalid_argument("Индекс нейрона выходит за пределы диапазона");
        }
        neurons[neuronIndex].weights_in(newWeights);
    }

    /*Получение весов конкретного нейрона*/
    std::vector<float> getWeights(size_t neuronIndex) const {
        if (neuronIndex >= neurons.size()) {
            throw std::invalid_argument("Индекс нейрона выходит за пределы диапазона");
        }
        return neurons[neuronIndex].getWeights();
    }


    /*Шаг в итерации сети*/
    bool doStep() {
        bool stateChanged = false;
        std::cout << "Энергия сети перед шагом = " << calculate_energy() << std::endl;

        //массив индексов нейронов
        std::vector<size_t> indices(neurons.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }

        // Перемешиваем индексы случайным образом
        std::random_device rd; // Источник случайности
        std::mt19937 gen(rd()); // Генератор псевдослучайных чисел
        std::shuffle(indices.begin(), indices.end(), gen);

        // Копируем текущие состояния нейронов
        std::vector<Neuron> previousStates;
        for (const auto& neuron : neurons) {
            previousStates.push_back(neuron); // Копируем объекты нейронов
        }

        // Обновляем состояния нейронов в случайном порядке
        for (size_t i: indices) {
            //std::cout << "Меняем нейрон под индексом  "<< i <<std::endl;
            float weightedSum = computeWeightedSum(i, previousStates);
            int newState = activation_function(weightedSum);

            if (neurons[i].getState() != newState) {
                neurons[i].setState(newState); // обновляем состояние нейрона
                stateChanged = true; // фиксируем изменение
            }
        }
        std::cout << "Энергия сети после шага = " << calculate_energy() << std::endl;
        //dollprint(this->getStates());

        return stateChanged;
    }


    float calculate_energy() {
        size_t n = kolvo_neurons;
        float energy = 0.0;

        for (size_t i = 0; i < n; ++i) {
            const auto &weights = neurons[i].getWeights(); // Кэшируем веса
            float state_i = neurons[i].getState(); // Кэшируем состояние нейрона

            for (size_t j = i + 1; j < n; ++j) {
                energy += weights[j] * state_i * neurons[j].getState();
            }
        }

        energy *= -1.0;
        return energy;
    }


    size_t runUntilStable() {
        size_t steps = 0;

        while (true) {
            ++steps;
            std::cout << "Шаг сети " << steps << "\n";

            if (!doStep()) {
                break;
            }

            if (steps > 140) {
                std::cout << "Стабильного состояния не удалось добиться\n";
                break;
            }
        }

        return steps;
    }
};


#endif //HOPFIELDNET_HPP
