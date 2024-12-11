
#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>
#include <string>

class Neuron {
  private:
    int state;
    std::string name;
    std::vector<float> weights;



  public:
    Neuron(int init_state = 0, std::string init_name = "")
    : state(init_state), name(std::move(init_name)), weights(0) {}


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

    void updateState(const std::vector<int>& inputs) {
        float weightedSum = 0.0;
        for (size_t i = 0; i < inputs.size(); ++i) {
            weightedSum += inputs[i] * weights[i];
        }
        state = activation_function(weightedSum);
    }
};



int Neuron::activation_function(auto x){
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}




#endif //NEURON_HPP
