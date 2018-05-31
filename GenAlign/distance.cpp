//
// Created by 王青龙 on 04/11/2016.
//

#include "distance.h"

void Distance::readFile(std::string fileName) {
    std::ifstream fin(fileName);
    std::string line;
    std::string item;
    std::string word;
    double embedding;
    bool firstword = true;

    while(std::getline(fin, line)){
        firstword = true;
        std::istringstream ss(line);
        while(ss>>item){
            if(firstword){
                word = item;
                embedding_list[word] = std::vector<double>();
                firstword = false;
            } else{
                embedding = std::stod(item);
                embedding_list[word].push_back(embedding);
            }
        }
        std::cout<<embedding_list[word].size()<<std::endl;
    }

    fin.close();
}

void Distance::eucDistance(std::string word, int N, std::vector<std::string> &result) {
    result = std::vector<std::string>(N);//vector应该定义成map这样才能返回word和distance
    if(embedding_list.find(word) != embedding_list.end()){
        for(auto &kv : embedding_list){
            if(kv.first != word){
                double ret = 0.0;
                for(auto i = 0; i != embedding_list[word].size(); ++i){//欧式距离，为了简便，用平方和替代
                    ret += pow(fabs(embedding_list[word].at(i) - embedding_list[kv.first].at(i)), 2);
                }
            }
        }
    }
}
