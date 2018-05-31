//
// Created by wangqinglong on 2017/8/22.
//

#ifndef MODEL1_1_COMMON_H
#define MODEL1_1_COMMON_H


#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include <cmath>
#include <chrono>
#include <cstdio>
#include <ctime>

using namespace std;

#define PROB_SMOOTH 1e-7

void ReadVcb(string input_file, unordered_map<int, int>& word_freq_ch) {
    ifstream fin(input_file);
    string line;
    int index;
    string word;
    int freq;
    while (getline(fin, line)) {
        istringstream iss(line);
        iss >> index;
        iss >> word;
        iss >> freq;
        word_freq_ch[index] = freq;
    }
    fin.close();
}


void readfile(string str, vector<vector<int>>& vv, unsigned int& cnt, unsigned int& max_line, string output_vcb_file) {
    ifstream fin(str);
    string line;
    string word;
    int i = 0;
    int j = 0;
    unsigned int cnt_line = 0;
    max_line = 0;
    unordered_map<string, int> word2int;

    word2int["null_wangql"] = j;
    ++j;
    ++j;//让j从2开始

    while (getline(fin, line)) {
        istringstream linestream(line);
        vv.push_back(vector<int>());

//        bool first_word = true;//

        cnt_line = 0;
        vv[i].push_back(word2int["null_wangql"]);
        ++cnt_line;

        while (linestream >> word) {
            ++cnt_line;
            auto ret = word2int.insert({word, j});
            if (ret.second) {
                ++j;
            }
            vv[i].push_back(word2int[word]);
        }
        if (cnt_line > max_line) {
            max_line = cnt_line;
        }
        ++i;
    }
    cnt = j-1;//cnt数量包含空值，这里之所以需要减1是在最后此一次插入成功j+1了
    fin.close();

    ofstream fout(output_vcb_file);
    for (auto it = word2int.begin(); it != word2int.end(); ++it) {
        fout << it->second << " " << it->first << endl;
    }
    fout.close();
}

void WordCnt(vector<vector<int>>& vv, unordered_map<int, int>& word_freq_ch) {
    for (auto i = 0; i < vv.size(); ++i) {
        for (auto j = 1; j < vv[i].size(); ++j) {
            if (word_freq_ch.find(vv[i][j]) != word_freq_ch.end()) {
                word_freq_ch[vv[i][j]] += 1;
            } else {
                word_freq_ch[vv[i][j]] = 1;
            }
        }
    }
}


void
readfile_en(string str, vector<vector<int>>& vv, unsigned int& cnt, unsigned int& max_line, string output_vcb_file) {
    ifstream fin(str);
    string line;
    string word;
    int i = 0;
    int j = 2;//让j从2开始
    unsigned int cnt_line = 0;
    max_line = 0;
    unordered_map<string, int> word2int;

    while (getline(fin, line)) {
        istringstream linestream(line);
        vv.push_back(vector<int>());

//        bool first_word = true;//

        cnt_line = 0;
        while (linestream >> word) {
            ++cnt_line;
            auto ret = word2int.insert({word, j});

            if (ret.second) {
                ++j;
            }
            vv[i].push_back(word2int[word]);
        }

//        cout<<endl;

        if (cnt_line > max_line) {
            max_line = cnt_line;
        }
        ++i;
    }
    cnt = j - 2;//cnt的值，这里之所以要减2是因为序号从2算起，而且在最后一次插入成功后j加1了
    fin.close();

    ofstream fout(output_vcb_file);
    for (auto it = word2int.begin(); it != word2int.end(); ++it) {
        fout << it->second << " " << it->first << endl;
    }
    fout.close();

}

void init_t2(vector<vector<int>>& ch, vector<vector<int>>& en, unordered_map<int, unordered_map<int, double>>& t,
             unsigned int cnt) {
    for (auto i = 0; i != ch.size(); ++i) {
        for (auto& c : ch[i]) {
            if (t.find(c) == t.end()) {
                t.insert({c, unordered_map<int, double>()});
            }
            for (auto& e : en[i]) {
                if (t[c].find(e) == t[c].end()) {
                    t[c][e] = 1.0/cnt;
                }
            }
        }
    }
}

void init_t_ext(unordered_map<int, unordered_map<int, double>>& t, vector<vector<int>>& ch, vector<vector<int>>& en,
                unsigned int cnt_ch, unsigned int cnt_en) {
    for (auto i = 0; i != ch.size(); ++i) {
        for (auto& c : ch[i]) {
            if (t.find(c) == t.end()) {
                t.insert({c, unordered_map<int, double>()});
            }
            for (auto& e : en[i]) {
                if (t[c].find(e) == t[c].end()) {
                    t[c][e] = 1.0 / cnt_en;
                }
            }
        }
    }
    for(auto it = t.begin(); it != t.end(); ++it){
        int cnt = it->second.size();
        it->second[1] = (cnt_en-cnt)/cnt_en;
    }
}


void unordered_map_t_count2map_t(unordered_map<int, unordered_map<int, double>>& unordered_map_t,
                                 unordered_map<int, unordered_map<int, double>>& unordered_map_t_count) {
    for (auto it_c = unordered_map_t.begin(); it_c != unordered_map_t.end(); ++it_c) {
        for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
            it_e->second = unordered_map_t_count[it_c->first][it_e->first];
            unordered_map_t_count[it_c->first][it_e->first] = 0.0;
        }
    }
}

void init_cnt(unordered_map<int, unordered_map<int, double>>& cnt) {
    for (auto it_c = cnt.begin(); it_c != cnt.end(); ++it_c) {
        for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
            it_e->second = 0;
        }
    }
}


void normalizeTable(unordered_map<int, unordered_map<int, double>>& unordered_map_t_count, vector<int>& nCh,
                    vector<int>& nEn, const int cnt_ch, const int cnt_en, int it = 2) {
    int tr_cnt_ch = cnt_ch + 1;//加个1号占坑
    int tr_cnt_en = cnt_en + 2;//加个0号和1号占坑

    nCh.resize(tr_cnt_ch);
    for (int i = 0; i != tr_cnt_ch; ++i) {
        nCh[i] = 0;
    }

    nEn.resize(tr_cnt_en);
    for (int i = 0; i != tr_cnt_en; ++i) {
        nEn[i] = 0;
    }

    vector<double> total(tr_cnt_ch, 0.0);
    for (auto iter1 = unordered_map_t_count.begin(); iter1 != unordered_map_t_count.end(); ++iter1) {
        for (auto iter2 = (iter1->second).begin(); iter2 != (iter1->second).end(); ++iter2) {
            total[iter1->first] += iter2->second;
            ++nCh[iter1->first];//每个ch对过几个en
            ++nEn[iter2->first];//每个en对过几个ch
        }
    }

    for (int k = 0; k != tr_cnt_ch; ++k) {
        if (nCh[k]) {
            double probMass = (cnt_en - nCh[k]) * PROB_SMOOTH;
            total[k] += total[k] * probMass / (1 - probMass);
        }
    }

    double p;
    for (auto iter1 = unordered_map_t_count.begin(); iter1 != unordered_map_t_count.end(); ++iter1) {
        for (auto iter2 = (iter1->second).begin(); iter2 != (iter1->second).end(); ++iter2) {
            if (total[iter1->first] > 0.0) {
                p = 1.0 * iter2->second / total[iter1->first];
            } else {
                p = 0.0;
            }
            if (p > PROB_SMOOTH) {
                iter2->second = p;
            } else {
                iter2->second = 0.0;
            }
        }
    }
    if (it > 0) {
        normalizeTable(unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en, it - 1);
    }

}

void em4(unordered_map<int, int>& word_freq_ch, unordered_map<int, int>& word_freq_en, vector<vector<int>>& ch,
         vector<vector<int>>& en, unordered_map<int, unordered_map<int, double>>& unordered_map_t_count,
         unordered_map<int, unordered_map<int, double>>& unordered_map_t, unsigned int cnt_en, int it) {
    double uniform = double(1) / cnt_en;
    for (int line = 0; line != ch.size(); ++line) {
        int l = (int) ch[line].size();
        int m = (int) en[line].size();
        for (auto& e : en[line]) {
            double denom = 0.0;
            vector<double*> sPtrCache(l, 0);
            double** sPtrCachePtr;
            if (it == 1) {
                denom = uniform * l;
            } else {
                sPtrCachePtr = &sPtrCache[0];
                for (auto& c : ch[line]) {
                    *sPtrCachePtr = &unordered_map_t[c][e];
                    if (*(*sPtrCachePtr) > PROB_SMOOTH) {
                        denom += *(*sPtrCachePtr);
                    } else {
                        denom += PROB_SMOOTH;
                    }
                    ++sPtrCachePtr;
                }
            }
            if (denom > 0) {
                double val = double(1) / denom;
                sPtrCachePtr = &sPtrCache[0];
                for (auto& c : ch[line]) {
                    double ee = 0.0;
                    if (it == 1) {
                        ee = uniform;
                    } else {
                        if (*(*sPtrCachePtr) > PROB_SMOOTH) {
                            ee = *(*sPtrCachePtr);
                        } else {
                            ee = PROB_SMOOTH;
                        }
                        ++sPtrCachePtr;
                    }
                    double x = ee * val;
                    unordered_map_t_count[c][e] += x;
                }
            } else {
                //不会发生
            }
        }
    }
}

void print_t(string str, unordered_map<int, unordered_map<int, double>>& t) {
    ofstream fout(str);
    for (auto it_c = t.begin(); it_c != t.end(); ++it_c) {
        for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
            fout << it_c->first << " " << it_e->first << " " << it_e->second << endl;
        }
    }
}

void print_align2(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
                  unordered_map<int, unordered_map<int, double>>& t) {
    ofstream fout(str);
    double tmp_max = 0.0;
    int tmp_k = 0;
    bool firstword = true;
    for (auto i = 0; i < ch.size(); ++i) {
        firstword = true;
        vector<int> viterbi_alignment(en[i].size());//存储每个目标语言所对应的源语言号码
        for (auto j = 0; j < en[i].size(); ++j) {
            tmp_max = 0.0;
            tmp_k = 0;
            for (auto k = 0; k < ch[i].size(); ++k) {//k=0代表对空
                if (tmp_max < t[ch[i][k]][en[i][j]]) {
                    tmp_max = t[ch[i][k]][en[i][j]];
                    tmp_k = k;
                }
            }

//            if (tmp_max < 0.01) {//去掉较小的不可信的对齐
//                tmp_k = 0;
//            }

            viterbi_alignment[j] = tmp_k;//也就是说，在这里已经得到了每个目标语言应该对哪个源语言
        }

        for (auto j = 0; j != en[i].size(); ++j) {
            if (viterbi_alignment[j]) {//跳过k=0的对空
                if (firstword) {
                    firstword = false;
                } else {
                    fout << " ";
                }
                fout << viterbi_alignment[j] - 1 << "-" << j;//因为源语言开头有个空，所以这里需要-1
            }
        }
        fout << endl;

        //下面这一块用来生成文件Align1格式
//        vector<vector<int>> translations(ch[i].size());
//        for (auto j=0; j<en[i].size(); ++j) {
//            translations[viterbi_alignment[j]].push_back(j);
//        }
//        for (auto ii=0; ii<ch[i].size(); ++ii) {
//            cout<<ii<<" ({ ";
//            for (auto jj=0; jj<translations[ii].size(); ++jj) {
//                cout<<translations[ii][jj]<<" ";
//            }
//            cout<<"}) ";
//        }
//        cout<<endl;

    }

    fout.close();
}

void init_total(vector<vector<int>>& ch, unordered_map<int, double>& total) {
    for (int i = 0; i != ch.size(); ++i) {
        for (auto& c : ch[i]) {
            total[c] = 0.0;
        }
    }
}


#endif //MODEL1_1_COMMON_H
