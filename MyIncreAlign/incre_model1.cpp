//
// Created by 王青龙 on 20/11/2016.
//

#include <random>
#include "incre_model1.h"


void IncreModel1::ReadBackgroundVocab(std::string filename, bool is_ch) {
    std::ifstream fin(filename);
    std::string line;
    std::string word;
    int index;
    int cnt;

    if (is_ch) {
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            iss >> index;
            iss >> word;
            iss >> cnt;
            background_vocab_word_index_ch_.insert({word, index});
//            background_vocab_index_cnt_ch_.insert({index, cnt});
        }
        old_vocab_num_ch_ = (int) background_vocab_word_index_ch_.size() + 2;//这个+2既体现出了老语料的个数，又表示了新语料开始的下标
    } else {
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            iss >> index;
            iss >> word;
            iss >> cnt;
            background_vocab_word_index_en_.insert({word, index});
//            background_vocab_index_cnt_en_.insert({index, cnt});
        }
        old_vocab_num_en_ = (int) background_vocab_word_index_en_.size() + 2;
    }
    fin.close();
}


void IncreModel1::ReadBackgroundTTable(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    int index_ch;
    int index_en;
    double prop_t;

    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        iss >> index_ch;
        if (background_table_t_.find(index_ch) == background_table_t_.end()) {
            background_table_t_[index_ch] = std::unordered_map<int, double>();
        }
        iss >> index_en;
        iss >> prop_t;
        background_table_t_[index_ch].insert({index_en, prop_t});
    }

    fin.close();
}

void IncreModel1::ReadBackgroundCorpus(std::string filename, bool is_ch) {
    std::ifstream fin(filename);
    std::string line;
    std::string word;
    int line_num = 0;

    if (is_ch) {
        while (getline(fin, line)) {
            std::istringstream iss(line);
            background_corpus_index_ch_.push_back(std::vector<int>());
            background_corpus_index_ch_[line_num].push_back(0);
            while (iss >> word) {
                if (background_vocab_word_index_ch_.find(word) == background_vocab_word_index_ch_.end()) {
                    std::cout << "不科学CH" << std::endl;
                } else {
                    background_corpus_index_ch_[line_num].push_back(background_vocab_word_index_ch_[word]);
                }
            }
            ++line_num;
        }
    } else {
        while (getline(fin, line)) {
            std::istringstream iss(line);
            background_corpus_index_en_.push_back(std::vector<int>());
            while (iss >> word) {
                if (background_vocab_word_index_en_.find(word) == background_vocab_word_index_en_.end()) {
                    std::cout << "不科学EN" << std::endl;
                } else {
                    background_corpus_index_en_[line_num].push_back(background_vocab_word_index_en_[word]);
                }
            }
            ++line_num;
        }
    }
}

void IncreModel1::ReadCorpus(std::string filename, bool is_ch) {
    std::ifstream fin(filename);
    std::string line;
    std::string word;
    int line_num = 0;

    int old_vocab_num;//总语料的个数就从这个老语料的个数开始
    if (is_ch) {
        old_vocab_num = old_vocab_num_ch_;
        while (getline(fin, line)) {
            std::istringstream iss(line);
            corpus_index_ch_.push_back(std::vector<int>());
            //源语言开头应该先加null
            corpus_index_ch_[line_num].push_back(0);//0表示开头的空，在background概率表中也是这样
            while (iss >> word) {
                if (background_vocab_word_index_ch_.find(word) == background_vocab_word_index_ch_.end()) {
                    background_vocab_word_index_ch_.insert({word, old_vocab_num});
                    corpus_index_ch_[line_num].push_back(old_vocab_num);
                    ++old_vocab_num;
                } else {
                    corpus_index_ch_[line_num].push_back(background_vocab_word_index_ch_[word]);
                }
            }
            ++line_num;
        }
        sum_cnt_ch_ = old_vocab_num;
    } else {
        old_vocab_num = old_vocab_num_en_;
        while (getline(fin, line)) {
            std::istringstream iss(line);
            corpus_index_en_.push_back(std::vector<int>());
            while (iss >> word) {
                if (background_vocab_word_index_en_.find(word) == background_vocab_word_index_en_.end()) {
                    background_vocab_word_index_en_.insert({word, old_vocab_num});
                    corpus_index_en_[line_num].push_back(old_vocab_num);
                    ++old_vocab_num;
                } else {
                    corpus_index_en_[line_num].push_back(background_vocab_word_index_en_[word]);
                }
            }
            ++line_num;
        }
        sum_cnt_en_ = old_vocab_num;
    }
}


void IncreModel1::ConstrucNewTTable() {
    std::cout << corpus_index_ch_.size() << std::endl;
    for (auto i = 0; i < corpus_index_ch_.size(); ++i) {
        for (auto c : corpus_index_ch_[i]) {
            if (background_table_t_.find(c) == background_table_t_.end()) {
                for (auto e : corpus_index_en_[i]) {
                    new_table_t_[c][e] = 1.0 / sum_cnt_en_;//这个设置成多少会影响最后的值，因为归一化并不是我认为的归一化
                }
            } else {
                for (auto e : corpus_index_en_[i]) {
                    if (background_table_t_[c].find(e) == background_table_t_[c].end()) {
                        new_table_t_[c][e] = kProbSmooth;
                    } else {
                        new_table_t_[c][e] = background_table_t_[c][e];
                    }
                }
            }
        }
    }
    //对新表进行归一化处理
    for (auto it1 = new_table_t_.begin(); it1 != new_table_t_.end(); ++it1) {
        double tmp_sum = 0.0;
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            tmp_sum += it2->second;
        }
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            it2->second /= tmp_sum;
        }
    }
}

void IncreModel1::ConstrucNewTTableCnt() {
    for (auto it1 = new_table_t_.begin(); it1 != new_table_t_.end(); ++it1) {
        new_table_t_cnt_.insert({it1->first, std::unordered_map<int, double>()});
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            new_table_t_cnt_[it1->first].insert({it2->first, it2->second});
        }
    }
}

void IncreModel1::EMLoop(int it) {
    double uniform = 1.0 / sum_cnt_en_;
    for (int line = 0; line < corpus_index_ch_.size(); ++line) {
        int l = (int) corpus_index_ch_.at(line).size();
        for (auto &e : corpus_index_en_.at(line)) {
            //在这里控制一下词频，仅仅改变稀有词的概率
//            if (background_vocab_index_cnt_en_.find(e) != background_vocab_index_cnt_en_.end() &&
//                background_vocab_index_cnt_en_[e] < 10000) {
            double denom = 0.0;
            //这里不适合用智能指针
            std::vector<double *> sPtrCache(l, 0);
            double **sPtrCachePtr;
            int index = 0;//index之所以在上面定义而不是在for循环里面是因为那样会影响到后面的sPtrCachePtr
            if (it == 1) {
                denom = uniform * l;
            } else {
                sPtrCachePtr = &sPtrCache[0];
                for (auto &c: corpus_index_ch_[line]) {
                    *sPtrCachePtr = &new_table_t_[c][e];
                    if (*(*sPtrCachePtr) > kProbSmooth) {
                        denom += *(*sPtrCachePtr);
                    } else {
                        denom += kProbSmooth;
                    }
                    ++sPtrCachePtr;
                }
            }
            if (denom > 0) {
                double val = 1.0 / denom;
                sPtrCachePtr = &sPtrCache[0];
                for (auto &c: corpus_index_ch_[line]) {
                    double ee = 0.0;
                    if (it == 1) {
                        ee = uniform;
                    } else {
                        if (*(*sPtrCachePtr) > kProbSmooth) {
                            ee = *(*sPtrCachePtr);
                        } else {
                            ee = kProbSmooth;
                        }
                        ++sPtrCachePtr;
                    }
                    double x = ee * val;
                    new_table_t_cnt_[c][e] += x;
                }
            } else {
                //不可能
            }
        }
    }

    //以下随机从原始语料中抽取若干行
//    std::default_random_engine generator(time(NULL));
//    std::uniform_int_distribution<int> dis(0, background_corpus_index_en_.size() - 1);
//    auto dice = std::bind(dis, generator);
//    for (int i = 0; i < 10000; ++i) {
//        int index = dice();//随机获得下标
//
//    }
}

void IncreModel1::NormalizeTTableCnt(std::vector<int> &n_ch, std::vector<int> &n_en, int it) {
    int tr_cnt_ch = sum_cnt_ch_;
    int tr_cnt_en = sum_cnt_en_;

    n_ch.resize(tr_cnt_ch);
    for (auto &n : n_ch) {
        n = 0;
    }

    n_en.resize(tr_cnt_en);
    for (auto &n: n_en) {
        n = 0;
    }

    std::vector<double> total(tr_cnt_ch, 0.0);
    for (auto iter1 = new_table_t_cnt_.begin(); iter1 != new_table_t_cnt_.end(); ++iter1) {
        for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2) {
            total[iter1->first] += iter2->second;//特定中文词对应所有英文词概率和
            ++n_ch[iter1->first];//每个ch对过几个en
            ++n_en[iter2->first];//每个en对过几个ch
        }
    }

    for (int k = 0; k != tr_cnt_ch; ++k) {
        if (n_ch[k]) {//这里的计算方式真的不知道怎么来的
            double prob_mass = (new_vocab_num_en_ - n_ch[k]) * kProbSmooth;
            total.at(k) += total.at(k) * prob_mass / (1 - prob_mass);
        }
    }

    double p;
    for (auto iter1 = new_table_t_cnt_.begin(); iter1 != new_table_t_cnt_.end(); ++iter1) {
        for (auto iter2 = (iter1->second).begin(); iter2 != (iter1->second).end(); ++iter2) {
            if (total.at(iter1->first) > 0.0) {
                p = 1.0 * iter2->second / total.at(iter1->first);//这样已处理，肯定就不是归一化了啊
            } else {
                p = 0.0;
            }
            if (p > kProbSmooth) {
                iter2->second = p;
            } else {
                iter2->second = 0.0;
            }
        }
    }

    if (it > 0) {
        NormalizeTTableCnt(n_ch, n_en, it - 1);
    }
}


void IncreModel1::CalcuVocabNum(bool is_ch) {
    std::unordered_set<int> word_index;
    int tmp_sum = 0;
    if (is_ch) {
        for (int i = 0; i < corpus_index_ch_.size(); ++i) {
            for (int j = 0; j < corpus_index_ch_.at(i).size(); ++j) {
                auto p = word_index.insert(corpus_index_ch_[i][j]);
                if (p.second) {
                    tmp_sum += 1;
                }
            }
        }
        new_vocab_num_ch_ = tmp_sum;
    } else {
        for (int i = 0; i < corpus_index_en_.size(); ++i) {
            for (int j = 0; j < corpus_index_en_.at(i).size(); ++j) {
                auto p = word_index.insert(corpus_index_en_[i][j]);
                if (p.second) {
                    tmp_sum += 1;
                }
            }
        }
        new_vocab_num_en_ = tmp_sum;
    }
}

void IncreModel1::NewTableCnt2NewTableT() {
    for (auto iter1 = new_table_t_.begin(); iter1 != new_table_t_.end(); ++iter1) {
        for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2) {
            iter2->second = new_table_t_cnt_.at(iter1->first).at(iter2->first);//cnt2t
            new_table_t_cnt_.at(iter1->first).at(iter2->first) = 0.0;//cnt2zero
        }
    }
}

void IncreModel1::PrintVocab(std::string filename, bool is_ch) {
    std::ofstream fout(filename);
    if (is_ch) {
        for (auto &w : background_vocab_word_index_ch_) {
            if ((w.second != 0) && (w.second != 1)) {
                fout << w.second << " " << w.first << std::endl;//输出格式：index 空格 Word 为了和Giza的输出格式一致
            }
        }
    } else {
        for (auto &w : background_vocab_word_index_en_) {
            if ((w.second != 0) && (w.second != 1)) {
                fout << w.second << " " << w.first << std::endl;
            }
        }
    }
    fout.close();
}

void IncreModel1::PrintTTable(std::string filename) {
    std::ofstream fout(filename);
    for (auto iter1 = new_table_t_.begin(); iter1 != new_table_t_.end(); ++iter1) {
        for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2) {
            fout << iter1->first << " " << iter2->first << " " << iter2->second << std::endl;
        }
    }
    fout.close();
}

void IncreModel1::Interpolation2T(double lamb) {
    for (auto iter1 = new_table_t_.begin(); iter1 != new_table_t_.end(); ++iter1) {
        for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); ++iter2) {
            if (background_table_t_.find(iter1->first) != background_table_t_.end() &&
                background_table_t_[iter1->first].find(iter2->first) != background_table_t_[iter1->first].end()) {
                iter2->second = lamb * background_table_t_[iter1->first][iter2->first] + (1 - lamb) * iter2->second;
            }
        }
    }
}

void IncreModel1::NormalizeTTableCntMy() {
    for (auto it1 = new_table_t_cnt_.begin(); it1 != new_table_t_cnt_.end(); ++it1) {
        double tmp_sum = 0.0;
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            tmp_sum += it2->second;
        }
        if (tmp_sum > 0.0) {
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
                it2->second /= tmp_sum;
            }
        }
    }
}







