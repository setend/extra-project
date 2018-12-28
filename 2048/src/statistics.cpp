#include "statistics.hpp"

bool Stats::collectStatistics(){

    std::ifstream statistics("../data/statistics.text");
    if(statistics.fail())
        return false;

    statistics >>bestScore >> gameCount >> winCount>> totalMoveCount >> totalDuration;
    return true;
}
