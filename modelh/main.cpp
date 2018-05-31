
#include "Commons.h"
#include "Original.h"
#include "LeaveOneOut.h"
#include "LOOforKeyword.h"
#include "PriorEM.h"



void normalizeTableMy(unordered_map<int, unordered_map<int, double>> &t_count) {
    for (auto it1 = t_count.begin(); it1 != t_count.end(); ++it1) {
        double tmp_sum = 0.0;
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            tmp_sum += it2->second;
        }
        if (tmp_sum > 0.0) {
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
                it2->second /= tmp_sum;
            }
        }
    }
}


int main() {

    Original();
//    Prior();
//    LeaveOneOut();
//    LOOforKeyword();

    return 0;
}
