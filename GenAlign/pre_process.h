//
// Created by 王青龙 on 04/11/2016.
//

#ifndef INCREALIGN_PREPRO_H
#define INCREALIGN_PREPRO_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>

class PrePro{
public:
    void readFile(std::string str){
        std::ifstream fin(str);
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
    }

    void sentence2Index(std::string line, std::string outputFile){
        std::ofstream fout(outputFile);
        std::string word;
        int index;
        bool firstword = true;
        std::istringstream linestream(line);
        while (linestream >> word) {
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
    }

    void corpus2Index(std::string inputFile, std::string outputFile){
        std::ifstream fin(inputFile);
        std::ofstream fout(outputFile);
        std::string line;
        std::string word;
        int index;
        bool firstword = true;

        while (std::getline(fin, line)) {
            firstword = true;
            std::istringstream linestream(line);
            while (linestream >> word) {
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
        }
        fin.close();
        fout.close();
    }

    void wordDis2IndexDis(std::string inputFile, std::string outputFile){//仅仅是目标端的
        std::ifstream fin(inputFile);
        std::ofstream fout(outputFile);
        std::string line;
        std::string word;
        int index;
        bool firstword = true;
        while(std::getline(fin, line)){
            firstword = true;
            std::istringstream linestream(line);
            while(linestream >> word){
                if(firstword){
                    firstword = false;
                    if (word_index.find(word) == word_index.end()){
                        fout << -1;
                    } else{
                        index = word_index[word];
                        fout << index;
                    }
                }else{
                    fout<<" ";
                    fout << word;
                }

            }
            fout<<std::endl;
        }
        fin.close();
        fout.close();
    }

    void ReadIndexDis(std::string inputFile){
        std::ifstream fin(inputFile);
        std::string line;
        int index;
        int index1;
        double dis;
        while(std::getline(fin, line)){
            std::istringstream ss(line);
            ss >> index;
            if(index != -1){
                index_dis_[index] = std::unordered_map<int, double >();
                for(int i = 0; i < 10; ++i){
                    std::getline(fin, line);
                    std::istringstream ss1(line);
                    ss1 >> index1;
                    if(index1 != -1){
                        ss1 >> dis;
                        index_dis_[index].insert({index1, dis});
                    }
                }
            }
        }
        fin.close();
    }

    void ConstructTTableVec(){
        int ccc = 0;
        for(auto it_c = t_table_ori_.begin(); it_c != t_table_ori_.end(); ++it_c){
            ccc += 1;
            if(ccc % 100 == 0){
                std::cout<<ccc<<std::endl;
            }
            t_table_vec_[it_c->first] = std::unordered_map<int, double >();
            for(auto it_e = (it_c->second).begin(); it_e != (it_c->second).end();++it_e){
                //考虑更改概率的表达
                int n = 0;
                double nearby_contribute = 0;
                for(auto it_index1 = index_dis_[it_e->first].begin(); it_index1 != index_dis_[it_e->first].end(); ++it_index1){
                    n += 1;
                    nearby_contribute += t_table_ori_[it_c->first][it_index1->first];
                }
//                if(nearby_contribute != 0){
//                    std::cout<<it_c->first<<" "<<it_e->first<<" "<<nearby_contribute<<" "<<n<<std::endl;
//                }
                //我可以考虑先不用index_dis_里面的距离，仅仅是用里面的词
//                if(n != 0){
//                    nearby_contribute /= n;
//                }
//                t_table_vec_[it_c->first][it_e->first] = 0.9 * it_e->second + 0.1 * nearby_contribute;

                t_table_vec_[it_c->first][it_e->first] = (it_e->second + nearby_contribute)/(n+1);

//                double nearby_contribute = 0;
//
//                for(auto it_index1 = index_dis_[it_e->first].begin(); it_index1 != index_dis_[it_e->first].end(); ++it_index1){
//                    if(t_table_ori_[it_c->first][it_index1->first] > nearby_contribute){
//                        nearby_contribute = t_table_ori_[it_c->first][it_index1->first];
//                    }
//                }



//                t_table_vec_[it_c->first][it_e->first] = (it_e->second > nearby_contribute ? it_e->second : nearby_contribute);
            }
        }
    }

    void PrintTTableVec(std::string output_file){
        std::ofstream fout(output_file);
        for(auto it_c = t_table_vec_.begin(); it_c != t_table_vec_.end(); ++it_c){
            for(auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e){
                fout<<it_c->first << " "<<it_e->first<<" "<<it_e->second<<std::endl;
            }
        }
        fout.close();
    }

    void ReadTTableOri(std::string inputFile){
        std::ifstream fin(inputFile);
        std::string line;
        int index_ch;
        int index_en;
        double prop_t;
        while (std::getline(fin, line)) {
            std::istringstream ss(line);
            ss >> index_ch;
            if (t_table_ori_.find(index_ch) == t_table_ori_.end()) {
                t_table_ori_[index_ch] = std::unordered_map<int, double>();
            }
            ss >> index_en;
            ss >> prop_t;
            t_table_ori_[index_ch].insert({index_en, prop_t});
        }
        fin.close();
    }






private:
    std::unordered_map<std::string, int> word_index;
    std::unordered_map<int, std::unordered_map<int, double >> index_dis_;
    std::unordered_map<int, std::unordered_map<int, double >> t_table_ori_;
    std::unordered_map<int, std::unordered_map<int, double >> t_table_vec_;

};

#endif //INCREALIGN_PREPRO_H
