//
// Created by 王青龙 on 06/11/2016.
//

#include "vocab.h"

void Vocab::readFile(std::string inputFile) {
    std::ifstream fin(inputFile);
    std::string line;
    std::string word;
    int index;
    while (std::getline(fin, line)){
        std::istringstream iss(line);
        iss >> index;
        iss >> word;
        vocab_.insert(word);
    }
    fin.close();
}

void Vocab::CorpusFindOOV(std::string inputFile, std::string outputFile) {
    std::ifstream fin(inputFile);
    std::ofstream fout(outputFile);
    std::string line;
    std::string word;
    int index;
    bool firstword = true;

    while (std::getline(fin, line)){
        firstword = true;
        std::istringstream iss(line);
        while(iss>>word){
            if(firstword){
                firstword = false;
            } else{
                fout << " ";
            }
            if(vocab_.find(word) == vocab_.end()){
                fout << -1;
            } else{
                fout << word;//保留原常用词
            }
        }
        fout << std::endl;
    }
    fin.close();
    fout.close();
}
