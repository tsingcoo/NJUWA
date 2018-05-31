//
// Created by 王青龙 on 26/12/2016.
//

#ifndef INCREALIGNFROMGAO_INCRE_MODELH_H
#define INCREALIGNFROMGAO_INCRE_MODELH_H

#include "incre_model1.h"
#include <algorithm>

const double kHMMAlignmentModelSmoothFactor = 0.2;
const double kGLOBALProbabilityForEmpty = 0.4;

class IncreModelh : public IncreModel1 {
public:

    void ReadBackgroundAlCount_(std::string filename);//直接读进AlCount_中

    void ReadBackgroundai_(std::string filename);

    void ReadBackgroundbi_(std::string filename);

    void ConstructNetN();

    void InitNetN(std::vector<int> &v_ch, std::vector<int> &v_en, const int &cnt_line_ch, const int &cnt_line_en);

    void ConstructNetE();

    void
    InitNetE(bool doInit, const int &cnt_line_ch);

    void ConstructAlCount_();

    void ConstructAlCount_cnt_();

    void Constructai_();

    void Constructbi_();

    void Constructai_cnt_();

    void Constructbi_cnt_();

    void EMLoop(int it);

    void MakeHMMNetwork(std::vector<int> &v_ch, std::vector<int> &v_en, bool doInit);

    void ForwardBackwardTraining(std::vector<int> &v_ch, std::vector<int> &v_en, std::vector<double> &betainit_global,
                                 bool doInit);

    void AddAlCount_cnt_(std::vector<int> &v_ch, std::vector<int> &v_en);

    void Addai_cnt_bi_cnt_(const int &cnt_line_ch);

    void ai_cnt_2ai_();

    void bi_cnt_2bi_();

    void AlCount_cnt_2AlCount_();

    void
    HMMRealViterbi(std::string output_file, std::vector<int> &v_ch, std::vector<int> &v_en,
                   std::vector<double> &betainit_global, bool doInit);

    void PrintAlCount_(std::string output_file);

    void Printai_(std::string output_file);

    void Printbi_(std::string output_file);

public:

    std::vector<std::vector<double >> net_n;//翻译
    std::vector<std::vector<double >> net_e;//转移

    std::vector<double> gamma_;
    std::vector<double> E_;
    std::vector<int> vit_;
    std::vector<int> viterbi_alignment_;

    std::unordered_map<int, double> AlCount_;
    std::unordered_map<int, double> AlCount_cnt_;

    std::unordered_map<int, std::vector<double >> ai_;
    std::unordered_map<int, std::vector<double >> bi_;
    std::unordered_map<int, std::vector<double >> ai_cnt_;
    std::unordered_map<int, std::vector<double >> bi_cnt_;



    /*
public:
    void ReadBackgroundVocab(std::string filename, bool is_ch);

    void ReadBackgroundTTable(std::string filename);

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
    std::unordered_map<std::string, int> background_vocab_word_index_ch_;//这个存储结构后来又加入了新的词表
    std::unordered_map<std::string, int> background_vocab_word_index_en_;
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
     */

};

#endif //INCREALIGNFROMGAO_INCRE_MODELH_H
