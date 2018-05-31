//
// Created by 王青龙 on 07/11/2016.
//

#ifndef INCREALIGN_TWO_VOCAB_H
#define INCREALIGN_TWO_VOCAB_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class TwoVocab{
public:
    void ReadVocabs(std::string vocab_train_file, std::string vocab_test_file);
    void Corpus2Index(std::string input_file, std::string output_file);

private:
    std::unordered_map<std::string, int> vocab_train_;
    std::unordered_map<std::string, int> vocab_test_;
};

#endif //INCREALIGN_TWO_VOCAB_H
