#ifndef TICHY_KLICKER_DIJKSTRA_H
#define TICHY_KLICKER_DIJKSTRA_H

#include <array>
#include <queue>
#include "Calculator.h"
#include "SimpleClock.h"

struct ConfigurationAndTime {
    ConfigurationAndTime(double time, long configuration) {
        this->time = time;
        this->configuration = configuration;
    }

    double time;
    long configuration;

    bool operator<(ConfigurationAndTime other) const {
        return time > other.time; // we are smaller / worse if we need more time
    }
};

constexpr long ONE = 30, TWO = 18, THREE = 10, FOUR = 20, FIVE = 30, SIX = 12, SEVEN = 10, EIGHT = 4;
constexpr std::array<long, 10> item_caps = { 1, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, 2 };
constexpr std::array<long, 9> cumulative_caps = { 1, ONE, ONE * TWO, ONE * TWO * THREE, ONE * TWO * THREE * FOUR,
                ONE * TWO * THREE * FOUR * FIVE, ONE * TWO * THREE * FOUR * FIVE * SIX,
                ONE * TWO * THREE * FOUR * FIVE * SIX * SEVEN, ONE * TWO * THREE * FOUR * FIVE * SIX * SEVEN * EIGHT };
std::array<double, cumulative_caps[8]> best_times;
std::priority_queue<ConfigurationAndTime> queue;

SimpleClock timer;

int item_to_buy = 2;

void print(std::array<int, 10> current_counts, double time, const Calculator& tichy);

void dijkstra(Calculator tichy) {
    for (double& d : best_times) {
        d = 100000;
    }

    long counter = 0;
    std::array<int, 10> current_counts = {};
    queue.emplace(ConfigurationAndTime(0, 1));
    best_times[1] = 0;
    double best_goal_time = 100000;
    long best_goal_conf = 0;
    while (!queue.empty()) {
        ConfigurationAndTime current = queue.top();
        queue.pop();
        counter++;

        if (best_times[current.configuration] < current.time) { // another entry of that configuration already got expanded
            continue;
        }

        if (current.time > best_goal_time) { // i.e. the solution can't be improved anymore
            current.configuration = best_goal_conf;
        }
        for (int i = 1; i <= 9; i++) {
            current_counts[i] = (int) ((current.configuration / cumulative_caps[i - 1]) % item_caps[i]);
        }
        if (current.time > best_goal_time) { // search is over
            print(current_counts, current.time, tichy);
            return;
        } else if (current_counts[item_to_buy] > 0) { // reached intermediate goal of buying next item level
            print(current_counts, current.time, tichy);
            timer.start();
            item_to_buy++;
        }

        for (int i = 1; i <= 8; i++) { // generate expansion candidates
            if (current_counts[i] < item_caps[i] - 1) { // otherwise we capped out that item
                double new_time = current.time + tichy.upgrade_time(i, current_counts);
                long new_config = current.configuration + cumulative_caps[i - 1];

                if (best_times[new_config] > new_time) { // we are better than the previous best value to here
                    best_times[new_config] = new_time;
                    queue.emplace(ConfigurationAndTime(new_time, new_config));
                }
            }
        }

        double new_time = current.time + tichy.upgrade_time(9, current_counts); // wait to reach target score
        long new_config = current.configuration + cumulative_caps[8];
        if (new_time < best_goal_time) { // possibly improve best solution
            best_goal_time = new_time;
            best_goal_conf = new_config;
        }
    }
}

long configuration(const std::array<int, 10>& current_counts) {
    long configuration = 0;
    for (int i = 8; i >= 1; i--) {
        configuration += current_counts[i];
        configuration *= item_caps[i - 1];
    }
    return configuration;
}

double epsilon = 0.0000001;

bool optimal(std::array<int, 10> counts, long conf, int item_to_reduce, const Calculator& tichy) {
    bool res = false;
    double time = best_times[conf];
    if (counts[item_to_reduce] > 0) {
        counts[item_to_reduce]--;
        conf = configuration(counts);
        double small_time = best_times[conf];

        if (small_time + tichy.upgrade_time(item_to_reduce, counts) < time + epsilon) {
            res = true;
        }
        counts[item_to_reduce]++;
    }
    return res;
}

void recover_path(std::array<int, 10> current_counts, const Calculator& tichy) {
    if (current_counts[9] > 0) {
        current_counts[9] = 0;
        std::cout << 9 << ", ";
    }
    long conf = configuration(current_counts);
    for (int i = 1; i <= 8; i++) {
        if (optimal(current_counts, conf, i, tichy)) {
            current_counts[i]--;
            std::cout << i << ", ";
            recover_path(current_counts, tichy);
            break;
        }
    }
}

void print(std::array<int, 10> current_counts, double time, const Calculator& tichy) {
    recover_path(current_counts, tichy);
    std::cout << "\nPlay time: " << time << "\nItems levels: [";
    for (int i = 1; i <= 8; i++) {
        std::cout << current_counts[i] << ", ";
    }
    std::cout << "]   \tCalculation time: " << timer.end() << ", Queue size: " << queue.size() << "\n" << std::endl;
}

#endif //TICHY_KLICKER_DIJKSTRA_H
