//
// Created by 王青龙 on 07/11/2016.
//

//#include <curses.h>
#include "two_vocab_infer_align.h"

void TwoVocab::ReadVocabs(std::string vocab_train_file, std::string vocab_test_file) {

    std::string line;
    std::string word;
    int index;

    std::ifstream fin_train(vocab_train_file);
    while (std::getline(fin_train, line)){
        std::istringstream iss(line);
        iss >> index;
        iss >> word;
        vocab_train_.insert({word, index});
    }
    fin_train.close();

    std::ifstream fin_test(vocab_test_file);
    while (std::getline(fin_test, line)){
        std::istringstream iss(line);
        iss >> index;
        iss >> word;
        vocab_test_.insert({word, index});
    }
    fin_test.close();
}

void TwoVocab::Corpus2Index(std::string input_file, std::string output_file) {
    std::ifstream fin(input_file);
    std::ofstream fout(output_file);
    std::string line;
    std::string word;
    int index;
    bool firstword = true;

    while (std::getline(fin, line)){
        firstword = true;
        std::istringstream iss(line);
        while (iss >> word) {
            if (firstword) {
                firstword = false;
            } else{
                fout << " ";
            }
            if (vocab_train_.find(word) == vocab_train_.end()) {
                index = vocab_test_[word];
                fout << "-" << index;//若不在训练集中的词则认为是稀疏词，利用测试机中的index表示，前面加符号以示区别
            } else{
                index = vocab_train_[word];
                fout << index;
            }
        }
        fout << std::endl;
    }
    fin.close();
    fout.close();
}
