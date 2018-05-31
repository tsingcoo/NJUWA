//
// Created by 王青龙 on 04/11/2016.
//

#ifndef INCREALIGN_DISTANCE_H
#define INCREALIGN_DISTANCE_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

class Distance{
    std::unordered_map<std::string, std::vector<double>> embedding_list;
public:
    void readFile(std::string fileName);
    void cosDistance(std::string word, int N, std::vector<std::string>& result);
    void eucDistance(std::string word, int N, std::vector<std::string>& result);
};

#endif //INCREALIGN_DISTANCE_H
