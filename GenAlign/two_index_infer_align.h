//
// Created by 王青龙 on 08/11/2016.
//

#ifndef INCREALIGN_TWO_INDEX_INFER_ALIGN_H
#define INCREALIGN_TWO_INDEX_INFER_ALIGN_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <cstdlib>

class TwoIndexInferAlign{
public:
    void ReadIndexChTrain(std::string index_ch_train_file);
    void ReadIndexChTest(std::string index_ch_test_file);
    void ReadIndexEnTrain(std::string index_en_train_file);
    void ReadIndexEnTest(std::string index_en_test_file);

    void ReadTTableTrain(std::string t_table_train_file);
    void ReadTTableTest(std::string t_table_test_file);

    void OutputAlign(std::string output_file);//该功能仅实现了常用词对常用词，稀疏词对稀疏词
    void OutputAlignCombin(std::string output_file);//该功能增加了稀疏词对常用词

private:
    std::vector<std::vector<int>> corpus_index_ch_train_;
    std::vector<std::vector<int>> corpus_index_ch_test_;
    std::vector<std::vector<int>> corpus_index_en_train_;
    std::vector<std::vector<int>> corpus_index_en_test_;

    std::unordered_map<int, std::unordered_map<int, double >> t_table_train_;
    std::unordered_map<int, std::unordered_map<int, double >> t_table_test_;


};

#endif //INCREALIGN_TWO_INDEX_INFER_ALIGN_H
