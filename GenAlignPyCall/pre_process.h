//
// Created by 王青龙 on 2018/1/31.
//

#ifndef GENALIGNPYCALL_PRE_PROCESS_H
#define GENALIGNPYCALL_PRE_PROCESS_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class PrePro {
public:
    int readFile(char* inputfile) {
        std::string ifs = inputfile;
        std::ifstream fin(ifs);
        std::string line;
        std::string word;
        int index;

        while (std::getline(fin, line)) {
            std::istringstream linestream(line);
            linestream >> index;
            linestream >> word;
            word_index.insert({word, index});//单词映射下标
        }
        fin.close();
        return 0;
    }

    int sentence2Index(char* line1, char* outputFile1) {
        std::string line = line1;
        std::string outputFile = outputFile1;
        std::ofstream fout(outputFile);
        std::string word;
        int index;
        bool firstword = true;
        std::istringstream linestream(line);
        while (linestream >> word) {
            std::cout << word << std::endl;
            if (firstword) {
                firstword = false;
            } else {
                fout << " ";
            }
            if (word_index.find(word) == word_index.end()) {
                fout << -1;
            } else {
                index = word_index[word];
                fout << index;
            }
        }
        fout << std::endl;
        fout.close();
        return 0;
    }

private:
    std::unordered_map<std::string, int> word_index;
};

#endif //GENALIGNPYCALL_PRE_PROCESS_H
