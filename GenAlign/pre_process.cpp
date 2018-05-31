//
// Created by 王青龙 on 04/11/2016.
//

#include "pre_process.h"

//void PrePro::readFile(std::string str) {
//    std::ifstream fin(str);
//    std::string line;
//    std::string word;
//    int index;
//
//    while (std::getline(fin, line)) {
//        std::istringstream linestream(line);
//        linestream >> index;
//        linestream >> word;
//        word_index.insert({word, index});//单词映射下标
//    }
//    fin.close();
//}


//void PrePro::corpus2Index(std::string inputFile, std::string outputFile) {
//    std::ifstream fin(inputFile);
//    std::ofstream fout(outputFile);
//    std::string line;
//    std::string word;
//    int index;
//    bool firstword = true;
//
//    while (std::getline(fin, line)) {
//        firstword = true;
//        std::istringstream linestream(line);
//        while (linestream >> word) {
//            if (firstword) {
//                firstword = false;
//            } else {
//                fout << " ";
//            }
//            if (word_index.find(word) == word_index.end()) {
//                fout << -1;
//            } else {
//                index = word_index[word];
//                fout << index;
//            }
//        }
//        fout << std::endl;
//    }
//    fin.close();
//    fout.close();
//}
