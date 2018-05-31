//
// Created by wangqinglong on 2017/8/29.
//

#ifndef MODEL1_1_LOOFORKEYWORD_H
#define MODEL1_1_LOOFORKEYWORD_H

#include "common.h"
#include "LOO.h"

vector<int> KeywordIndex(string str) {
    vector<int> res;
    ifstream fin(str);
    string line;
    int index;
    while (getline(fin, line)) {
        istringstream linestream(line);
        linestream >> index;
        res.push_back(index);
    }
    return res;
}

void PrintAlignLOOforKeyword(ofstream& fout, vector<int>& ch, vector<int>& en,
                             unordered_map<int, unordered_map<int, double>>& t, int index) {
//    ofstream fout(str);
    double tmp_max = 0.0;
    int tmp_k = 0;
    bool firstword = true;
    vector<int> viterbi_alignment(en.size());
    for (auto j = 0; j < en.size(); ++j) {
        tmp_max = 0.0;
        tmp_k = 0;
        for (auto k = 0; k < ch.size(); ++k) {
            if (tmp_max < t[ch[k]][en[j]]) {
                tmp_max = t[ch[k]][en[j]];
                tmp_k = k;
            }
        }

        viterbi_alignment[j] = tmp_k;
    }


    if (viterbi_alignment[index]) {
        fout << viterbi_alignment[index] - 1 << "-" << index;
    }
    fout << endl;

}

void EMWithLOOforKeyword(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
                         unordered_map<int, unordered_map<int, double>>& unordered_map_count_p,
                         unordered_map<int, unordered_map<int, double>>& unordered_map_count_q,
                         vector<unordered_map<int, unordered_map<int, double>>>& single_cnt, const int cnt_ch,
                         const int cnt_en, int it, vector<int>& Index) {

    double uniform = double(1) / cnt_en;
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {

        auto l = (int) ch[line].size();
        auto m = (int) en[line].size();

        unordered_map<int, unordered_map<int, double >> map_t;
        if (it != 1) {
            map_t = getLOOt(unordered_map_count_p, single_cnt, cnt_ch, cnt_en, line);
            if (it == 5) {
                PrintAlignLOOforKeyword(fout, ch[line], en[line], map_t, Index[line]);
            }
        }

        for (auto& e : en[line]) {
            double denom = 0.0;
            vector<double*> sPtrCache(l, 0);
            double** sPtrCachePtr;
            if (it == 1) {
                denom = uniform * l;
            } else {

                sPtrCachePtr = &sPtrCache[0];
                for (auto& c : ch[line]) {
                    *sPtrCachePtr = &map_t[c][e];
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
                    unordered_map_count_q[c][e] += x;
                    single_cnt[line][c][e] += x;
                }
            } else {
                //不会发生
            }
        }
    }
    fout.close();
}

void LOOforKeyword() {
    time_t start, stop;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.ch",
             vv_ch, cnt_ch, max_line_ch, "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.ch.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.en",
                vv_en, cnt_en, max_line_en, "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.en.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    vector<int> Index = KeywordIndex(
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.keywordindex.txt");

    unordered_map<int, unordered_map<int, double>> unordered_map_count_p, unordered_map_count_q;
    init_t2(vv_ch, vv_en, unordered_map_count_p, cnt_en);
    init_cnt(unordered_map_count_p);
    init_t2(vv_ch, vv_en, unordered_map_count_q, cnt_en);
    init_cnt(unordered_map_count_q);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_LOO(vv_ch, vv_en, single_cnt);

    start = time(NULL);

    for (int it = 1; it != 6; ++it) {
        EMWithLOOforKeyword(
                "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.model1.align",
                vv_ch, vv_en,
                unordered_map_count_p, unordered_map_count_q, single_cnt, cnt_ch, cnt_en, it, Index);
        MapCntQ2MapCntP(unordered_map_count_p, unordered_map_count_q);
        stop = time(NULL);
        printf("model1_LOOforKeyword use time:%ld\n", (stop - start));
    }

}

#endif //MODEL1_1_LOOFORKEYWORD_H
