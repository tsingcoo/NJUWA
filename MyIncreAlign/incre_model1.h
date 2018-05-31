//
// Created by 王青龙 on 20/11/2016.
//

#ifndef INCREALIGNFROMGAO_READT_H
#define INCREALIGNFROMGAO_READT_H

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

const int kLongSentence = 101;
const int kMaxNumBackgroundVocab = 10000;//之前的1000000可能会造成新加入的语料的概率较小对原有概率影响不到因此造成结果没有因为lambda的变化而像整体模型靠拢
const double kProbSmooth = 1e-7;

class IncreModel1 {
public:
    void ReadBackgroundVocab(std::string filename, bool is_ch);

    void ReadBackgroundTTable(std::string filename);

    void ReadBackgroundCorpus(std::string filename, bool is_ch);

    void ReadCorpus(std::string filename, bool is_ch);

    void ConstrucNewTTable();

    void ConstrucNewTTableCnt();

    void CalcuVocabNum(bool is_ch);

    virtual void EMLoop(int it);

    void NormalizeTTableCnt(std::vector<int> &n_ch, std::vector<int> &n_en, int it = 2);//Giza概率表归一化方式

    void NormalizeTTableCntMy();//我的常规的概率表归一化方式

    void NewTableCnt2NewTableT();

    void Interpolation2T(double lamb);

    void PrintVocab(std::string filename, bool is_ch);

    void PrintTTable(std::string filename);

    void PrintTTableCnt(std::string filename);

public:
    std::vector<std::vector<int>> background_corpus_index_ch_;
    std::vector<std::vector<int>> background_corpus_index_en_;
    std::unordered_map<std::string, int> background_vocab_word_index_ch_;//这个存储结构后来又加入了新的词表
    std::unordered_map<std::string, int> background_vocab_word_index_en_;
    std::unordered_map<int, int> background_vocab_index_cnt_ch_;//存储在原数据中每个词出现的次数
    std::unordered_map<int, int> background_vocab_index_cnt_en_;
    std::unordered_map<int, std::unordered_map<int, double >> background_table_t_;//概率表不需要区分中英文，每次用一个就行了
    std::unordered_map<int, std::unordered_map<int, double >> new_table_t_;//新概率表并没有完全包含老概率表的内容，他们只是部分重叠
    std::unordered_map<int, std::unordered_map<int, double >> new_table_t_cnt_;
    std::vector<std::vector<int>> corpus_index_ch_;
    std::vector<std::vector<int>> corpus_index_en_;
    int new_vocab_num_ch_;//新语料包含的词表大小
    int new_vocab_num_en_;
    int sum_cnt_ch_;//总语料包含的词表大小
    int sum_cnt_en_;
    int old_vocab_num_ch_;//老语料词表大小
    int old_vocab_num_en_;
};

#endif //INCREALIGNFROMGAO_READT_H
