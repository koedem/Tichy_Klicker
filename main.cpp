#include "Calculator.h"
#include "Dijkstra.h"


int main() {
    double target_score = 1500000000;
    Calculator tichy(target_score);
    dijkstra(tichy);
    return 0;
}