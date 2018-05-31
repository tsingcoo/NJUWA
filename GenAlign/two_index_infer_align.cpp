//
// Created by 王青龙 on 08/11/2016.
//

#include "two_index_infer_align.h"

void TwoIndexInferAlign::ReadIndexChTrain(std::string index_ch_train_file) {
    std::ifstream fin(index_ch_train_file);
    std::string line;
    int index;
    int line_num = 0;
    while (std::getline(fin, line)){
        std::istringstream iss(line);
        corpus_index_ch_train_.push_back(std::vector<int>());
        while (iss >> index){
            corpus_index_ch_train_[line_num].push_back(index);
        }
        ++line_num;
    }
    fin.close();
}

void TwoIndexInferAlign::ReadIndexChTest(std::string index_ch_test_file) {
    std::ifstream fin(index_ch_test_file);
    std::string line;
    int index;
    int line_num = 0;
    while (std::getline(fin, line)){
        std::istringstream iss(line);
        corpus_index_ch_test_.push_back(std::vector<int>());
        while (iss >> index){
            corpus_index_ch_test_[line_num].push_back(index);
        }
        ++line_num;
    }
    fin.close();
}

void TwoIndexInferAlign::ReadIndexEnTrain(std::string index_en_train_file) {
    std::ifstream fin(index_en_train_file);
    std::string line;
    int index;
    int line_num = 0;
    while (std::getline(fin, line)){
        std::istringstream iss(line);
        corpus_index_en_train_.push_back(std::vector<int>());
        while (iss >> index){
            corpus_index_en_train_[line_num].push_back(index);
        }
        ++line_num;
    }
    fin.close();
}

void TwoIndexInferAlign::ReadIndexEnTest(std::string index_en_test_file) {
    std::ifstream fin(index_en_test_file);
    std::string line;
    int index;
    int line_num = 0;
    while (std::getline(fin, line)){
        std::istringstream iss(line);
        corpus_index_en_test_.push_back(std::vector<int>());
        while (iss >> index){
            corpus_index_en_test_[line_num].push_back(index);
        }
        ++line_num;
    }
    fin.close();
}

void TwoIndexInferAlign::ReadTTableTrain(std::string t_table_train_file) {
    std::ifstream fin(t_table_train_file);
    std::string line;
    int index_ch;
    int index_en;
    double prop_t;
    while(std::getline(fin, line)){
        std::istringstream ss(line);
        ss>>index_ch;
        if(t_table_train_.find(index_ch) == t_table_train_.end()){
            t_table_train_[index_ch] = std::unordered_map<int, double>();
        }
        ss>>index_en;
        ss>>prop_t;
        t_table_train_[index_ch].insert({index_en, prop_t});
    }
    fin.close();
}

void TwoIndexInferAlign::ReadTTableTest(std::string t_table_test_file) {
    std::ifstream fin(t_table_test_file);
    std::string line;
    int index_ch;
    int index_en;
    double prop_t;
    while(std::getline(fin, line)){
        std::istringstream ss(line);
        ss>>index_ch;
        if(t_table_test_.find(index_ch) == t_table_test_.end()){
            t_table_test_[index_ch] = std::unordered_map<int, double>();
        }
        ss>>index_en;
        ss>>prop_t;
        t_table_test_[index_ch].insert({index_en, prop_t});
    }
    fin.close();
}

void TwoIndexInferAlign::OutputAlign(std::string output_file) {
    std::ofstream fout(output_file);
    double tmp_max = 0.0;
    double tmp_t = 0.0;
    int tmp_index = 0;
    bool first_word = true;

    for(auto i = 0; i < corpus_index_ch_test_.size(); ++i){
        first_word = true;
        std::vector<int> viterbi_alignment(corpus_index_en_test_.size(), -1);
        for(auto j = 0; j < corpus_index_en_test_[i].size(); ++j){
            tmp_max = 0.0;
            tmp_t = 0.0;
            tmp_index = -1;
            for(auto k = 0; k < corpus_index_ch_test_[i].size(); ++k){

                if((t_table_train_.find(corpus_index_ch_train_[i][k]) != t_table_train_.end()) && (t_table_train_[corpus_index_ch_train_[i][k]].find(corpus_index_en_train_[i][j]) != t_table_train_[corpus_index_ch_train_[i][k]].end())){
                    tmp_t = t_table_train_[corpus_index_ch_train_[i][k]][corpus_index_en_train_[i][j]];
                } else{
                    if((corpus_index_ch_train_[i][k] == -1) && (corpus_index_en_train_[i][j] == -1)){
                        tmp_t = t_table_test_[corpus_index_ch_test_[i][k]][corpus_index_en_test_[i][j]];
                    }
                    //在这两个判断之间还遗漏了那些不是未登录词而是稀疏词，由于概率太小没有被计算到
                }

                if(tmp_max < tmp_t){
                    tmp_max = tmp_t;
                    tmp_index = k;
                }
            }

            viterbi_alignment[j] = tmp_index;

//            if(std::abs(tmp_index - j) < 15) {
//                viterbi_alignment[j] = tmp_index;
//            }
        }
        for(auto j = 0; j != corpus_index_en_test_[i].size(); ++j){
            if(viterbi_alignment[j] != -1){
                if(first_word){
                    first_word = false;
                } else{
                    fout << " ";
                }
                fout << viterbi_alignment[j] << "-" << j;
            }
        }
        fout << std::endl;
    }
    fout.close();

}

void TwoIndexInferAlign::OutputAlignCombin(std::string output_file) {
    std::ofstream fout(output_file);
    double tmp_max = 0.0;
    double tmp_t = 0.0;
    int tmp_index = 0;
    bool first_word = true;

    for(auto i = 0; i < corpus_index_ch_test_.size(); ++i){
        first_word = true;
        std::vector<int> viterbi_alignment(corpus_index_en_test_.size(), -1);
        for(auto j = 0; j< corpus_index_en_test_[i].size(); ++j){

            tmp_max = 0.0;
            tmp_t = 0.0;
            tmp_index = -1;

            for(auto k = 0; k < corpus_index_ch_test_[i].size(); ++k){//先用test对齐一遍
                tmp_t = t_table_test_[corpus_index_ch_test_[i][k]][corpus_index_en_test_[i][j]];
                if(tmp_max < tmp_t){
                    tmp_max = tmp_t;
                    tmp_index = k;
                }
            }

//            if((corpus_index_en_train_[i][j] != -1) && (corpus_index_ch_train_[i][tmp_index] != -1)){//如果test得到的对齐不包含稀疏词则认为不可靠，改用train对齐
//                for(auto k = 0; k < corpus_index_ch_test_[i].size(); ++k){
//                    if((t_table_train_.find(corpus_index_ch_train_[i][k]) != t_table_train_.end()) && (t_table_train_[corpus_index_ch_train_[i][k]].find(corpus_index_en_train_[i][j]) != t_table_train_[corpus_index_ch_train_[i][k]].end())){
//                        tmp_t = t_table_train_[corpus_index_ch_train_[i][k]][corpus_index_en_train_[i][j]];
//                    }
//                    if(tmp_max < tmp_t){
//                        tmp_max = tmp_t;
//                        tmp_index = k;
//                    }
//                }
//            }

            viterbi_alignment[j] = tmp_index;//上面的两种对齐（test对齐和train对齐）总有一种得到tmp_index
        }
        for(auto j = 0; j != corpus_index_en_test_[i].size(); ++j){
            if(viterbi_alignment[j] != -1){
                if(first_word){
                    first_word = false;
                } else{
                    fout << " ";
                }
                fout << viterbi_alignment[j] << "-" << j;
            }
        }
        fout << std::endl;
    }
    fout.close();
}
