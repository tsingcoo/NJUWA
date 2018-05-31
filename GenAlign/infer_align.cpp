//
// Created by 王青龙 on 06/11/2016.
//

#include <set>
#include <algorithm>
#include "infer_align.h"
//#include "word2vec.h"

//void InferAlign::ReadTTable(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int index_ch;
//    int index_en;
//    double prop_t;
//    while (std::getline(fin, line)) {
//        std::istringstream ss(line);
//        ss >> index_ch;
//        if (t_table_.find(index_ch) == t_table_.end()) {
//            t_table_[index_ch] = std::unordered_map<int, double>();
//        }
//        ss >> index_en;
//        ss >> prop_t;
//        t_table_[index_ch].insert({index_en, prop_t});
//    }
//    fin.close();
//}

//void InferAlign::ReadCorpusIndexCh(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    std::string index;
//    int line_num = 0;
//    while (std::getline(fin, line)) {
//        std::istringstream ss(line);
//        corpus_index_ch_.push_back(std::vector<int>());
//        corpus_index_ch_[line_num].push_back(0);//在源语言开头加入空字符
//        while (ss >> index) {
//            corpus_index_ch_[line_num].push_back(std::stoi(index));
//        }
//        ++line_num;
//    }
//    fin.close();
//}

//void InferAlign::ReadCorpusIndexEn(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    std::string index;
//    int line_num = 0;
//    while (std::getline(fin, line)) {
//        std::istringstream ss(line);
//        corpus_index_en_.push_back(std::vector<int>());
//        while (ss >> index) {
//            corpus_index_en_[line_num].push_back(std::stoi(index));
//        }
//        ++line_num;
//    }
//    fin.close();
//}


//void InferAlign::OutputAlign(std::string outputFile) {
//    std::ofstream fout(outputFile);
//    double tmp_max = 0.0;
//    int tmp_index = 0;
//    bool first_word = true;
//
//    for (auto i = 0; i < corpus_index_ch_.size(); ++i) {
//        first_word = true;
//        std::vector<int> viterbi_alignment(corpus_index_en_[i].size(), 0);
//        std::vector<double> viterbi_alignment_prop(corpus_index_en_[i].size(), 0.0);
//
//        std::vector<int> unaligned_en(corpus_index_en_[i]);//用来存放未对齐的英文
//        std::vector<int> unaligned_ch(corpus_index_ch_[i]);//用来存放未对齐的中文
//
//        for (auto j = 0; j < corpus_index_en_[i].size(); ++j) {
//            tmp_max = 0.0;
//            tmp_index = 0;
//            for (auto k = 0; k < corpus_index_ch_[i].size(); ++k) {
//                if (t_table_.find(corpus_index_ch_[i][k]) != t_table_.end() &&
//                    t_table_[corpus_index_ch_[i][k]].find(corpus_index_en_[i][j]) !=
//                    t_table_[corpus_index_ch_[i][k]].end()) {
//                    double tmp_t = t_table_[corpus_index_ch_[i][k]][corpus_index_en_[i][j]];
//                    if (tmp_max < tmp_t) {
//                        tmp_max = tmp_t;
//                        tmp_index = k;
//                    }
//                }
//            }
//
//            viterbi_alignment[j] = tmp_index;
//            viterbi_alignment_prop[j] = tmp_max;
//
//        }
//
//        /*
//
//        for (auto j = 0; j != corpus_index_en_[i].size(); ++j) {
//            if (viterbi_alignment_prop[j] <= 0.001) {
//                viterbi_alignment[j] = 0;
//            }
//
//            if (word_freq_ch_[corpus_index_ch_[i][viterbi_alignment[j]]] < 15) {
//                viterbi_alignment[j] = 0;
//            }
//
//            if (viterbi_alignment[j] != 0) {//把已对齐的从未对齐词中删除
//                unaligned_en[j] = -1;
//                unaligned_ch[viterbi_alignment[j]] = -1;
//            }
//        }
//
//        //第二次找对齐
//        for (auto j = 0; j < corpus_index_en_[i].size(); ++j) {
//            tmp_max = 0.0;
//            tmp_index = 0;
//            if (unaligned_en[j] != -1 && word_freq_en_[unaligned_en[j]] > 15) {
//                for (auto k = 0; k < corpus_index_ch_[i].size(); ++k) {
//                    if (unaligned_ch[k] != -1 && word_freq_ch_[unaligned_ch[k]] > 15) {
//                        if (t_table_.find(corpus_index_ch_[i][k]) != t_table_.end() &&
//                            t_table_[corpus_index_ch_[i][k]].find(corpus_index_en_[i][j]) !=
//                            t_table_[corpus_index_ch_[i][k]].end()) {
//                            double tmp_t = t_table_[corpus_index_ch_[i][k]][corpus_index_en_[i][j]];
//                            if (tmp_max < tmp_t) {
//                                tmp_max = tmp_t;
//                                tmp_index = k;
//                            }
//                        }
//                    }
//                }
//                viterbi_alignment[j] = tmp_index;
//                viterbi_alignment_prop[j] = tmp_max;
//            }
//        }
//
//        for (auto j = 0; j != corpus_index_en_[i].size(); ++j) {
//            if (viterbi_alignment_prop[j] <= 0.0001) {
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
//        for (auto j = 0; j < corpus_index_en_[i].size(); ++j) {
//            tmp_max = 0.0;
//            tmp_index = 0;
//            if (unaligned_en[j] != -1 && word_freq_en_[unaligned_en[j]] <= 15) {
//                for (auto k = 0; k < corpus_index_ch_[i].size(); ++k) {
//                    if (unaligned_ch[k] != -1) {
//                        if (t_table_.find(corpus_index_ch_[i][k]) != t_table_.end() &&
//                            t_table_[corpus_index_ch_[i][k]].find(corpus_index_en_[i][j]) !=
//                            t_table_[corpus_index_ch_[i][k]].end()) {
//                            double tmp_t = t_table_[corpus_index_ch_[i][k]][corpus_index_en_[i][j]];
//                            if (tmp_max < tmp_t) {
//                                tmp_max = tmp_t;
//                                tmp_index = k;
//                            }
//                        }
//                    }
//                }
//                viterbi_alignment[j] = tmp_index;
//                viterbi_alignment_prop[j] = tmp_max;
//                if (viterbi_alignment_prop[j] <= 0.1) {
//                    viterbi_alignment[j] = 0;
//                }
//            }
//        }
//
//
//        for (auto j = 0; j != corpus_index_en_[i].size(); ++j) {
//
//            if (viterbi_alignment[j] != 0) {//把已对齐的从未对齐词中删除
//                unaligned_en[j] = -1;
//                unaligned_ch[viterbi_alignment[j]] = -1;
//            }
//        }
//
//         */
//
//        for (auto j = 0; j != corpus_index_en_[i].size(); ++j) {
//            if (viterbi_alignment[j] != -1 && viterbi_alignment[j] != 0) {
//                if (first_word) {
//                    first_word = false;
//                } else {
//                    fout << " ";
//                }
////                fout << viterbi_alignment[j] - 1 << "-" << j;//中英对齐方向
//                fout << j << "-" << viterbi_alignment[j] - 1;//英中对齐方向
////                fout << index_word_ch_[corpus_index_ch_[i][viterbi_alignment[j]]] << "-"
////                     << index_word_en_[corpus_index_en_[i][j]] << "(" << viterbi_alignment_prop[j] << ")";
//            }
//        }
//        fout << std::endl;
//
//        /*
//
//        //在这里补上未对齐的稀疏词
//        std::cout << i << std::endl;
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
//
//         */
//
//    }
//    fout.close();
//}

//void InferAlign::ReadGizaVcbCh(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int index;
//    std::string word;
//    int freq;
//    while (std::getline(fin, line)) {
//        std::istringstream iss(line);
//        iss >> index;
//        iss >> word;
//        iss >> freq;
//        word_freq_ch_.insert({index, freq});
//        index_word_ch_.insert({index, word});
//        word_index_ch_.insert({word, index});
//    }
//    fin.close();
//}


//void InferAlign::ReadMyVcbCh(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int index;
//    std::string word;
//    while(std::getline(fin, line)){
//        std::istringstream iss(line);
//        iss>>index;
//        iss >>word;
//        index_word_ch_.insert({index, word});
//        word_index_ch_.insert({word, index});
//    }
//    fin.close();
//}

//void InferAlign::ReadMyVcbEn(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int index;
//    std::string word;
//    while(std::getline(fin, line)){
//        std::istringstream iss(line);
//        iss>>index;
//        iss>>word;
//        index_word_en_.insert({index, word});
//        word_index_en_.insert({word, index});
//    }
//    fin.close();
//}

//void InferAlign::ReadGizaVcbEn(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int index;
//    std::string word;
//    int freq;
//    while (std::getline(fin, line)) {
//        std::istringstream iss(line);
//        iss >> index;
//        iss >> word;
//        iss >> freq;
//        word_freq_en_.insert({index, freq});
//        index_word_en_.insert({index, word});
//        word_index_en_.insert({word, index});
//    }
//    fin.close();
//}

//void InferAlign::ReadDefs(std::string str1, std::string str2) {
//    std::ifstream fin1(str1);
//    std::ifstream fin2(str2);
//    std::string line1, line2;
//    std::string word1, word2;
//
//    while (std::getline(fin1, line1) && std::getline(fin2, line2)) {
//        std::istringstream iss1(line1);
//        std::istringstream iss2(line2);
//
//        iss1 >> word1;
//
//        if (word_index_en_.find(word1) != word_index_en_.end()) {
//            std::vector<int> temp;
//            while (iss2 >> word2) {
//                if (word_index_ch_.find(word2) != word_index_ch_.end()) {
//                    temp.push_back(word_index_ch_[word2]);
//                }
//            }
////            defs_[word_index_en_[word1]] = temp;
//            defs_.insert({word_index_en_[word1], temp});
//        }
//    }
////
////    for (auto it = defs_.begin(); it != defs_.end(); ++it) {
////        std::cout << it->first << " ";
////        for (auto v = it->second.begin(); v != it->second.end(); ++v) {
////            std::cout << *v << " ";
////        }
////        std::cout << std::endl;
////    }
//
//    fin1.close();
//    fin2.close();
//}

//void InferAlign::ReadIndexVecEn(std::string inputFile) {
//    std::ifstream fin(inputFile);
//    std::string line;
//    int index;
//
//    std::vector<double> emb(500, 0.0);//存储整个向量,500维
//    double e;//存储向量的一维
//
//    while (std::getline(fin, line)) {
//        std::istringstream iss(line);
//        iss >> index;
//
//        for (int j = 0; j != 500; ++j) {
//            iss >> e;
//            emb[j] = e;
//        }
//        embedings.insert({index, emb});
//
//    }
//    fin.close();
//}



