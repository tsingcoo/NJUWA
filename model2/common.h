//
// Created by wangqinglong on 2017/8/24.
//

#ifndef MODEL2_COMMON_H
#define MODEL2_COMMON_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

const double kProbSmooth = 1e-7;
const int max_length = 101;

void readfile(string str, vector<vector<int>> &vv, unsigned int &cnt, unsigned int &max_line, string output_vcb_file) {
    ifstream fin(str);
    string line;
    string word;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int cnt_line = 0;
    max_line = 0;
    unordered_map<string, int> word2int;

    word2int["null_wangql"] = j;//先增加一个null_wangql
    ++j;
    ++j;//从2开始

    while (getline(fin, line)) {
        istringstream linestream(line);
        vv.push_back(vector<int>());

        cnt_line = 0;
        vv[i].push_back(word2int["null_wangql"]);
        ++cnt_line;//每行都增加一个null_wangql

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
    cnt = j-1;
    fin.close();

    ofstream fout(output_vcb_file);
    for (auto &wi : word2int) {
        fout << wi.second << " " << wi.first << endl;
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
readfile_en(string str, vector<vector<int>> &vv, unsigned int &cnt, unsigned int &max_line, string output_vcb_file) {
    ifstream fin(str);
    string line;
    string word;
    int i = 0;
    int j = 2;//从2开始
    unsigned int cnt_line = 0;
    max_line = 0;
    unordered_map<string, int> word2int;

    while (getline(fin, line)) {
        istringstream linestream(line);
        vv.push_back(vector<int>());
        cnt_line = 0;
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
    cnt = j - 1;
//    cout<<max_line<<endl;
    fin.close();

    ofstream fout(output_vcb_file);
    for (auto &wi : word2int) {
        fout << wi.second << " " << wi.first << endl;
    }
    fout.close();
}

void init_t(vector<vector<int>> &ch, vector<vector<int>> &en, unordered_map<int, unordered_map<int, double>> &t,
            unsigned int cnt) {
    for (unsigned int i = 0; i != ch.size(); ++i) {
        for (auto &c : ch[i]) {
            if (t.find(c) == t.end()) {
                t.insert({c, unordered_map<int, double>()});
            }
            for (auto &e : en[i]) {
                t[c][e] = 1.0 / cnt;
            }
        }
    }
}

void
init_t_cnt(vector<vector<int>> &ch, vector<vector<int>> &en, unordered_map<int, unordered_map<int, double>> &t_cnt) {
    for (int i = 0; i != ch.size(); ++i) {
        for (auto &c: ch[i]) {
            if (t_cnt.find(c) == t_cnt.end()) {
                t_cnt[c] = unordered_map<int, double>();
            }
            for (auto &e : en[i]) {
                t_cnt[c][e] = 0.0;
            }
        }
    }
}

void print_t(string str, unordered_map<int, unordered_map<int, double>> &t) {
    ofstream fout(str);
    for (auto it_c = t.begin(); it_c != t.end(); ++it_c) {
        for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
            fout << it_c->first << " " << it_e->first << " " << it_e->second << endl;
        }
    }
}

void normalizeTTable(unordered_map<int, unordered_map<int, double>> &unordered_map_t_count, vector<int> &nCh,
                     vector<int> &nEn, const int cnt_ch, const int cnt_en, int it = 2) {

    int tr_cnt_ch = cnt_ch + 1;
    int tr_cnt_en = cnt_en + 2;

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
            ++nCh[iter1->first];
            ++nEn[iter2->first];
        }
    }

    for (int k = 0; k != tr_cnt_ch; ++k) {
        if (nCh[k]) {
            double probMass = (cnt_en - nCh[k]) * kProbSmooth;
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
            if (p > kProbSmooth) {
                iter2->second = p;
            } else {
                iter2->second = 0.0;
            }
        }
    }
    if (it > 0) {
        normalizeTTable(unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en, it - 1);
    }
}

void normalizeATable(unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuua) {
    for (auto it1 = uuua.begin(); it1 != uuua.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            double total = 0.0;
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                total += it3->second;
            }
            if (total) {
                for (auto it4 = it2->second.begin(); it4 != it2->second.end(); ++it4) {
                    it4->second /= total;
                }
            }
        }
    }
}

void initA2(unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuua) {
//    int max_length = 101;
    for (int l = 1; l != max_length; ++l) {
        uuua.insert({l, unordered_map<int, unordered_map<int, double>>()});
        for (int j = 0; j != max_length; ++j) {
            uuua[l].insert({j, unordered_map<int, double>()});
            for (int i = 0; i <= l; ++i) {
                uuua[l][j][i] = 1.0 / (l + 1);
            }
        }
    }
}

void initA2_cnt(unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuua) {
//    int max_length = 101;
    for (int l = 1; l != max_length; ++l) {
        uuua.insert({l, unordered_map<int, unordered_map<int, double>>()});
        for (int j = 0; j != max_length; ++j) {
            uuua[l].insert({j, unordered_map<int, double>()});
            for (int i = 0; i <= l; ++i) {
                uuua[l][j][i] = 0.0;
            }
        }
    }
}


void uuu_a_cnt2uuu_a(unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuu_a,
                     unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuu_a_cnt) {


    for (auto it1 = uuu_a_cnt.begin(); it1 != uuu_a_cnt.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                uuu_a[it1->first][it2->first][it3->first] = it3->second;
                it3->second = 0.0;
            }
        }
    }
}

void em_4(vector<vector<int>> &ch,
          vector<vector<int>> &en, unordered_map<int, unordered_map<int, double>> &uu_t_cnt,
          unordered_map<int, unordered_map<int, double>> &uu_t,
          unordered_map<int, unordered_map<int, unordered_map<int, double>>> &a,
          unordered_map<int, unordered_map<int, unordered_map<int, double>>> &a_cnt, unsigned int cnt, int it) {
    for (int line = 0; line != ch.size(); ++line) {
        int l = (int) ch[line].size();
        int j = 0;
        for (auto &e : en[line]) {
            double denom = 0.0;
            vector<double *> sPtrCache(l, 0);
            int i = 0;
            for (auto &c : ch[line]) {
                sPtrCache[i] = &uu_t[c][e];
                denom += max(*sPtrCache[i], kProbSmooth) * max(a[l - 1][j][i], kProbSmooth);
                ++i;
            }
            if (denom > 0) {
                double val = double(1) / denom;
                int i = 0;
                for (auto &c : ch[line]) {
                    double ee = max(*sPtrCache[i], kProbSmooth) * max(a[l - 1][j][i], kProbSmooth);
                    double temp = ee * val;
                    uu_t_cnt[c][e] += temp;
                    a_cnt[l - 1][j][i] += temp;
                    ++i;
                }
            }
            ++j;
        }
    }
}

void print_a_3(string str, unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuua) {
    ofstream fout(str);
    for (auto it1 = uuua.begin(); it1 != uuua.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                fout << it1->first << " " << it2->first << " " << it3->first << " " << it3->second << endl;
            }
        }
    }
}


void print_em3_align(string str,
                     vector<vector<int>> &vv_ch, vector<vector<int>> &vv_en,
                     unordered_map<int, unordered_map<int, unordered_map<int, double>>> &uuu_a,
                     unordered_map<int, unordered_map<int, double>> &uu_t) {
    ofstream fout(str);
    bool firstword = true;
    double tmp_max = 0.0;
    int tmp_i = 0;
    auto lc = (int) vv_ch[0].size();

    for (int line = 0; line < vv_ch.size(); ++line) {
        firstword = true;
        lc = (int) vv_ch[line].size() - 1;
        vector<int> viterbi_alignment(vv_en[line].size());
        for (int j = 0; j < vv_en[line].size(); ++j) {
            tmp_max = 0.0;
            tmp_i = 0;
            for (int i = 0; i < vv_ch[line].size(); ++i) {
                double v = uu_t[vv_ch[line][i]][vv_en[line][j]] * uuu_a[lc][j][i];

                if (tmp_max < v) {
                    tmp_max = v;
                    tmp_i = i;
                }

            }
            viterbi_alignment[j] = tmp_i;
        }
        for (int j = 0; j < vv_en[line].size(); ++j) {
            if (viterbi_alignment[j]) {
                if (firstword) {
                    firstword = false;
                } else {
                    fout << " ";
                }
                fout << viterbi_alignment[j] - 1 << "-" << j;
            }
        }
        fout << endl;
    }
    fout.close();
}

#endif //MODEL2_COMMON_H
