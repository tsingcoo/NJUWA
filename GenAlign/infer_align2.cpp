//
// Created by 王青龙 on 18/12/2016.
//

#include "infer_align2.h"

//void InferAlign2::ReadGizaATable(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int position_ch;
//    int position_en;
//    int length_ch;
//    int max_length_en;//用不到
//    double prop_a;
//
//    while (std::getline(fin, line)) {
//        std::istringstream iss(line);
//        iss >> position_ch;
//        iss >> position_en;
//        iss >> length_ch;
//        iss >> max_length_en;
//        iss >> prop_a;
//
//        a_table_[length_ch][position_en][position_ch] = prop_a;
//    }
//    fin.close();
//
//}

//void InferAlign2::ReadMyATable(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int length_ch;
//    int position_en;
//    int position_ch;
//    double prop_a;
//
//    while (std::getline(fin, line)) {
//        std::istringstream iss(line);
//        iss >> length_ch;
//        iss >> position_en;
//        iss >> position_ch;
//        iss >> prop_a;
//        a_table_[length_ch][position_en][position_ch] = prop_a;
//    }
//    fin.close();
//}

//void InferAlign2::ConstrucATable() {
//    for (int l = 1; l != long_sentence; ++l) {
//        a_table_.insert({l, std::unordered_map<int, std::unordered_map<int, double>>()});
//        for (int j = 1; j != long_sentence; ++j) {
//            a_table_[l].insert({j, std::unordered_map<int, double>()});
//            for (int i = 0; i != long_sentence; ++i) {
//                a_table_[l][j].insert({i, 1.0 / (l + 1)});
//            }
//        }
//    }
//}

//void InferAlign2::OutputAlign(std::string outputFile) {
//    std::ofstream fout(outputFile);
//    bool firstword = true;
//    double tmp_max = 0.0;
//    int tmp_i = 0;
//
//    for (auto line = 0; line < corpus_index_ch_.size(); ++line) {
//        firstword = true;
//        int l = (int) corpus_index_ch_[line].size() - 1;//下面的位置对齐概率可以用到
//
//        std::vector<int> viterbi_alignment(corpus_index_en_[line].size());
//        std::vector<double> viterbi_alignment_prop(corpus_index_en_[line].size(), 0.0);
//
//        std::vector<int> unaligned_en(corpus_index_en_[line].size(), -1);
//        std::vector<int> unaligned_ch(corpus_index_ch_[line].size(), -1);
//
//        for (int i = 0; i < unaligned_en.size(); ++i) {
//            unaligned_en[i] = corpus_index_en_[line][i];
//        }
//
//        for (int i = 0; i < unaligned_ch.size(); ++i) {
//            unaligned_ch[i] = corpus_index_ch_[line][i];
//        }
//
//        for (auto j = 0; j < corpus_index_en_[line].size(); ++j) {
//            tmp_max = 0.0;
//            tmp_i = 0;
//            for (auto i = 0; i < corpus_index_ch_[line].size(); ++i) {
//                if (t_table_.find(corpus_index_ch_[line][i]) != t_table_.end() &&
//                    t_table_[corpus_index_ch_[line][i]].find(corpus_index_en_[line][j]) !=
//                    t_table_[corpus_index_ch_[line][i]].end()) {
//                    double v = t_table_[corpus_index_ch_[line][i]][corpus_index_en_[line][j]] *
//                               a_table_[l][j][i];
//                    if (tmp_max < v) {
//                        tmp_max = v;
//                        tmp_i = i;
//                    }
//                }
//            }
//            viterbi_alignment[j] = tmp_i;
//            viterbi_alignment_prop[j] = tmp_max;
//        }
//
//        /*
//
//        //对于找到的对齐进行过滤
//        for (auto j = 0; j != corpus_index_en_[line].size(); ++j) {
//
//            //这个选择是过滤掉被源语言稀疏词影响的对齐
//            if (word_freq_ch_[corpus_index_ch_[line][viterbi_alignment[j]]] < 15) {
//                viterbi_alignment[j] = 0;
//            }
//
//            if (viterbi_alignment[j] != 0) {//把已对齐的从未对齐词中删除
//                unaligned_en[j] = -1;
//                unaligned_ch[viterbi_alignment[j]] = -1;
//            }
//        }
//
//
//
//        //第二次找对齐
//        for (auto j = 0; j < corpus_index_en_[line].size(); ++j) {
//            tmp_max = 0.0;
//            tmp_i = 0;
//            if (unaligned_en[j] != -1 && word_freq_en_[unaligned_en[j]] > 15) {
//                for (auto i = 0; i < corpus_index_ch_[line].size(); ++i) {
//                    if (unaligned_ch[i] != -1 && word_freq_ch_[unaligned_ch[i]] > 15) {
//                        if (t_table_.find(corpus_index_ch_[line][i]) != t_table_.end() &&
//                            t_table_[corpus_index_ch_[line][i]].find(corpus_index_en_[line][j]) !=
//                            t_table_[corpus_index_ch_[line][i]].end()) {
//                            double v = t_table_[corpus_index_ch_[line][i]][corpus_index_en_[line][j]] *
//                                       a_table_[l][j][i];
//                            if (tmp_max < v) {
//                                tmp_max = v;
//                                tmp_i = i;
//                            }
//                        }
//                    }
//                }
//                viterbi_alignment[j] = tmp_i;
//                viterbi_alignment_prop[j] = tmp_max;
//            }
//        }
//
//
//        for (auto j = 0; j != corpus_index_en_[line].size(); ++j) {
//            if (viterbi_alignment_prop[j] <= 0.00005) {
//                viterbi_alignment[j] = 0;
//            }
//
//            if (viterbi_alignment[j] != 0) {//把已对齐的从未对齐词中删除
//                unaligned_en[j] = -1;
//                unaligned_ch[viterbi_alignment[j]] = -1;
//            }
//        }
//
//
//        //稀疏词找对齐
//        for (auto j = 0; j < corpus_index_en_[line].size(); ++j) {
//            tmp_max = 0.0;
//            tmp_i = 0;
//            if (unaligned_en[j] != -1 && word_freq_en_[unaligned_en[j]] <= 15) {
//                for (auto i = 0; i < corpus_index_ch_[line].size(); ++i) {
//                    if (unaligned_ch[i] != -1) {
//                        if (t_table_.find(corpus_index_ch_[line][i]) != t_table_.end() &&
//                            t_table_[corpus_index_ch_[line][i]].find(corpus_index_en_[line][j]) !=
//                            t_table_[corpus_index_ch_[line][i]].end()) {
//                            double v = t_table_[corpus_index_ch_[line][i]][corpus_index_en_[line][j]] *
//                                       a_table_[l][j][i];
//                            if (tmp_max < v) {
//                                tmp_max = v;
//                                tmp_i = i;
//                            }
//                        }
//                    }
//                }
//                viterbi_alignment[j] = tmp_i;
//                viterbi_alignment_prop[j] = tmp_max;
//                if (viterbi_alignment_prop[j] < 0.005) {
//                    viterbi_alignment[j] = 0;
//                }
//            }
//        }
//
//        */
//
//        for (auto j = 0; j < corpus_index_en_[line].size(); ++j) {
//            if (viterbi_alignment[j] != 0) {//把已对齐的词从未对齐列表中删除
//                unaligned_en[j] = -1;
//                unaligned_ch[viterbi_alignment[j]] = -1;
//            }
//        }
//
//
//        for (auto j = 0; j < corpus_index_en_[line].size(); ++j) {
//            if (viterbi_alignment[j]) {
//                if (firstword) {
//                    firstword = false;
//                } else {
//                    fout << " ";
//                }
////                fout << viterbi_alignment[j] - 1 << "-" << j;//中英方向
//                fout << j << "-" << viterbi_alignment[j] - 1;//英中方向
////                fout << index_word_ch_[corpus_index_ch_[line][viterbi_alignment[j]]] << "-"
////                     << index_word_en_[corpus_index_en_[line][j]] << "(" << viterbi_alignment_prop[j] << ")";
//            }
//        }
//        fout << std::endl;
//
///*
//        //在这里补上未对齐的稀疏词
//        std::cout << line << std::endl;
//        for (auto &uc : unaligned_ch) {
//            if (uc != -1) {
//                std::cout << index_word_ch_[uc] << " ";
//            }
//        }
//        std::cout << "||| ";
//
//        for (auto &ue:unaligned_en) {
//            if (ue != -1) {
//                std::cout << index_word_en_[ue] << " ";
//            }
//        }
//        std::cout << "||| ";
//
//        for (auto &uc:unaligned_ch) {
//            if (uc != -1 && word_freq_ch_[uc] < 15) {
//                std::cout << index_word_ch_[uc] << " ";
//            }
//        }
//        std::cout << "||| ";
//
//        for (auto &ue:unaligned_en) {
//            if (ue != -1 && word_freq_en_[ue] < 15) {
//                std::cout << index_word_en_[ue] << " ";
//            }
//        }
//        std::cout << std::endl;
//*/
//
//    }
//    fout.close();
//}


