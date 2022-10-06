#ifndef TICHY_KLICKER_CALCULATOR_H
#define TICHY_KLICKER_CALCULATOR_H

#include <cmath>
#include <array>

class Calculator {
    std::array<double, 10> costs = { 0, 10, 100, 1330, 18080, 290000, 1500000, 40000000, 333000000, 0 };
    const std::array<double, 10> incomes = { 0, 0.2, 1, 5, 125, 800, 1500, 25000, 99999, 0 };
    const std::array<double, 10> factor = { 0, 1.2, 1.3, 1.5, 1.3, 1.1, 1.2, 1.15, 1.05, 1 };

    double manual_speed(const std::array<int, 10>& counts) const {
        double result = 0.0;
        for (int i = 1; i <= 8; i++) {
            result += incomes[i] * counts[i];
        }
        return result;
    }

public:
    explicit Calculator(double target_score) {
        costs[9] = target_score;
    }

    double upgrade_time(int upgrade, const std::array<int, 10>& counts) const {
        double cost = costs[upgrade] * pow(factor[upgrade], counts[upgrade]);
        return cost / manual_speed(counts);
    }
};


#endif //TICHY_KLICKER_CALCULATOR_H
