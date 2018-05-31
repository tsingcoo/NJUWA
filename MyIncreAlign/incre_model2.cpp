//
// Created by 王青龙 on 30/11/2016.
//

#include "incre_model2.h"


void IncreModel2::ConstructATable() {//这个a_table已经包含了所有的情况了
    for (int l = 1; l != kLongSentence; ++l) {
        a_table_.insert({l, std::unordered_map<int, std::unordered_map<int, double>>()});
        for (int j = 1; j != kLongSentence; ++j) {
            a_table_[l].insert({j, std::unordered_map<int, double>()});
            for (int i = 0; i != kLongSentence; ++i) {
                a_table_[l][j].insert({i, 1.0 / (l + 1)});
            }
        }
    }
}

void IncreModel2::ConstructATableCnt() {
    for (int l = 1; l != kLongSentence; ++l) {
        a_table_cnt_.insert({l, std::unordered_map<int, std::unordered_map<int, double>>()});
        for (int j = 1; j != kLongSentence; ++j) {
            a_table_cnt_[l].insert({j, std::unordered_map<int, double>()});
            for (int i = 0; i != kLongSentence; ++i) {
                a_table_cnt_[l][j].insert({i, 0.0});
            }
        }
    }
}

void IncreModel2::InitATableCnt() {
    for (int l = 1; l != kLongSentence; ++l) {
        for (int j = 1; j != kLongSentence; ++j) {
            for (int i = 0; i != kLongSentence; ++i) {
                a_table_cnt_[l][j][i] = 0.0;
            }
        }
    }
}

void IncreModel2::ReadBackgroundATable(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    int position_ch;
    int position_en;
    int length_ch;
    int max_length_en;//用不到
    double prop_a;

    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        iss >> position_ch;
        iss >> position_en;
        iss >> length_ch;
        iss >> max_length_en;
        iss >> prop_a;

        a_table_[length_ch][position_en][position_ch] = prop_a;

        fin.close();
    }
}

void IncreModel2::ReadBackgroundMyATable(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    int length_ch;
    int position_en;
    int position_ch;
    double prop_a;

    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        iss >> length_ch;
        iss >> position_en;
        iss >> position_ch;
        iss >> prop_a;
        a_table_[length_ch][position_en][position_ch] = prop_a;
    }
    fin.close();
}

void IncreModel2::EMLoop(int it) {

    for (int line = 0; line != corpus_index_ch_.size(); ++line) {
        int l = (int) corpus_index_ch_[line].size();//易错
        int j = 0;
        for (auto &e : corpus_index_en_[line]) {
            double denom = 0.0;
            std::vector<double *> sPterCache(l, 0);
            int i = 0;
            for (auto &c : corpus_index_ch_[line]) {
                sPterCache[i] = &new_table_t_[c][e];
                denom += std::max(*sPterCache[i], kProbSmooth) * std::max(a_table_[l - 1][j][i], kProbSmooth);
                ++i;
            }
            if (denom > 0) {
                double val = double(1) / denom;
                int i = 0;
                for (auto &c : corpus_index_ch_[line]) {
                    double ee = std::max(*sPterCache[i], kProbSmooth) * std::max(a_table_[l - 1][j][i], kProbSmooth);
                    double temp = ee * val;
                    new_table_t_cnt_[c][e] += temp;
                    a_table_cnt_[l - 1][j][i] += temp;//曾经把j和i写成e,c了
                    ++i;
                }
            }
            ++j;//曾经忘记写这一句了
        }

    }
}


void IncreModel2::NormalizeATableCnt() {
    for (auto it1 = a_table_cnt_.begin(); it1 != a_table_cnt_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            double sum = 0.0;
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                sum += it3->second;
            }
            if (sum > 0.0) {
                for (auto it4 = it2->second.begin(); it4 != it2->second.end(); ++it4) {
                    it4->second /= sum;
                }
            }
        }
    }
}

void IncreModel2::ATableCnt2ATable() {

    for (auto it1 = a_table_cnt_.begin(); it1 != a_table_cnt_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                a_table_[it1->first][it2->first][it3->first] = it3->second;
                it3->second = 0.0;
            }
        }
    }
}


void IncreModel2::PrintATable(std::string str) {
    std::ofstream fout(str);
    for (auto it1 = a_table_.begin(); it1 != a_table_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                fout << it1->first << " " << it2->first << " " << it3->first << " " << it3->second << std::endl;
            }
        }
    }
    fout.close();
}








//void IncreModel2::ConstrucNewATable() {
//    for (auto i = 0; i < corpus_index_ch_.size(); ++i) {
//        int l_ch = (int) corpus_index_ch_[i].size() - 1;//实际长度，不计入句首的空标记
//        int l_en = (int) corpus_index_en_[i].size();
//        if (background_table_a_.find(l_ch) == background_table_a_.end()) {
//            new_table_a_[l_ch] = std::unordered_map<int, std::unordered_map<int, double >>();
//        }
//        for (auto j = 1; j <= l_en; ++j) {
//            if (background_table_a_[l_ch].find(j) == background_table_a_.end()) {
//                new_table_a_[l_ch][j] = std::unordered_map<int, double>();
//                for (auto k = 0; k <= l_ch; ++k) {
//                    new_table_a_[l_ch][j][k] = 1.0 / (l_ch + 1);
//                }
//            } else {
//                for (auto k = 0; k <= l_ch; ++k) {
//                    new_table_a_[l_ch][j][k] = background_table_a_[l_ch][j][k];
//                }
//            }
//        }
//    }
//
//    new_table_a_cnt_ = new_table_a_;//拷贝一份在EMLoop中使用，这个待定，cnt有可能需要再次初始化为零
//}





