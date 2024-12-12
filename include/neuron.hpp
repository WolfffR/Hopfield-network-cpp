
#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>
#include <string>

class Neuron {
  private:
    int state;
    int id;
    std::string name;
    std::vector<float> weights;



  public:

    Neuron(int init_state = 0, std::string init_name = "", int init_id = 0)
    : state(init_state), name(std::move(init_name)), weights(0), id(init_id) {}

    int getId() const {
        return id;
    }

    int getState() const {
        return state;
    }

    std::string getName() const {
        return name;
    }

    void setState(int newState) {
        state = newState;
    }

    std::vector<float> getWeights() const {
      if (weights.size() == 0)
        throw std::invalid_argument("Neuron::getWeights() вызван для пустого вектора");

      return weights;
    }

    void weights_in(std::vector<float> input_weights){
      // if (input_weights.size() != weights.size())
      // throw std::invalid_argument("neuron input_weights.size() != weights.size(), входящий вектор не той размерности");

      weights = input_weights;
    }

    int activation_function(auto x);

    float computeWeightedSum(const std::vector<Neuron>& previousStates) const {
        float sum = 0.0;
        for (size_t i = 0; i < previousStates.size(); ++i) {
            if (previousStates[i].getId() != id) { // Исключаем себя
                sum += previousStates[id].getWeights()[i] * previousStates[i].getState();
            }
        }
        return sum;
    }
    bool updateState(const std::vector<Neuron>& inputs) {
        float weightedSum = computeWeightedSum(inputs);
        int newState = activation_function(weightedSum);
        if (state != newState) {
            state = newState; // обновляем состояние нейрона
            return true; // фиксируем изменение
        }
        else {
            return false;
        }

    }
};


int Neuron::activation_function(auto x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}




#endif //NEURON_HPP
