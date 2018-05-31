//
// Created by 王青龙 on 30/11/2016.
//

#ifndef INCREALIGNFROMGAO_INCRE_MODEL2_H
#define INCREALIGNFROMGAO_INCRE_MODEL2_H


#include "incre_model1.h"


class IncreModel2 : public IncreModel1 {
    //void ReadBackgroundVocab(std::string filename, bool is_ch);
    //void ReadBackgroundTTable(std::string filename);
    //void ReadCorpus(std::string filename, bool is_ch);
    //void ConstrucNewTTable();
    //void CalcuVocabNum(bool is_ch);
public:

    void ConstructATable();

    void ConstructATableCnt();

    void InitATableCnt();

    void ReadBackgroundATable(std::string filename);

    void ReadBackgroundMyATable(std::string filename);

//    void ConstrucNewATable();

//    void ConstrucNewAcntTable();

    void EMLoop(int it) override;

    void NormalizeATableCnt();

    void ATableCnt2ATable();

    void PrintATable(std::string str);


private:

//    std::unordered_map<std::string, int> background_vocab_word_index_ch_;
//    std::unordered_map<std::string, int> background_vocab_word_index_en_;
//    std::unordered_map<int, std::unordered_map<int, double >> background_table_t_;
//    std::unordered_map<int, std::unordered_map<int, double >> new_table_t_;
//    std::unordered_map<int, std::unordered_map<int, double >> new_table_t_cnt_;
//    std::vector<std::vector<int>> corpus_index_ch_;
//    std::vector<std::vector<int>> corpus_index_en_;

    //这里可以做一个trick，即一个table_a_包含所有可用的a
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, double >>> a_table_;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, double >>> a_table_cnt_;


};

#endif //INCREALIGNFROMGAO_INCRE_MODEL2_H
