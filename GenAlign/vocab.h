//
// Created by 王青龙 on 06/11/2016.
//

#ifndef INCREALIGN_VOCAB_H
#define INCREALIGN_VOCAB_H

#include <string>
#include <unordered_set>
#include <fstream>
#include <sstream>

class Vocab{
public:
    void readFile(std::string inputFile);
    void CorpusFindOOV(std::string inputFile, std::string outputFile);

private:
    std::unordered_set<std::string> vocab_;
};

#endif //INCREALIGN_VOCAB_H
