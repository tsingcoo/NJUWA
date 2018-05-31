//
// Created by wangqinglong on 2017/8/22.
//

#ifndef MODEL1_1_LEAVEONEOUT_H
#define MODEL1_1_LEAVEONEOUT_H

#include "common.h"


unordered_map<int, unordered_map<int, double >> getLOOt(unordered_map<int, unordered_map<int, double >>& cnt_p,
                                                        vector<unordered_map<int, unordered_map<int, double>>>& single_cnt,
                                                        const int cnt_ch, const int cnt_en, int i) {

    auto map_t = single_cnt[i];
    for (auto it1 = single_cnt[i].begin(); it1 != single_cnt[i].end(); ++it1) {
        for (auto it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2) {
            map_t[it1->first][it2->first] = cnt_p[it1->first][it2->first] - it2->second;
            it2->second = 0.0;//single_cnt置零
        }
    }
    vector<int> nCh;
    vector<int> nEn;
    normalizeTable(map_t, nCh, nEn, cnt_ch, cnt_en);
    return map_t;

}

void MapCntQ2MapCntP(unordered_map<int, unordered_map<int, double>>& map_cnt_p,
                     unordered_map<int, unordered_map<int, double>>& map_cnt_q) {
    for (auto c = map_cnt_q.begin(); c != map_cnt_q.end(); ++c) {
        for (auto e = (c->second).begin(); e != (c->second).end(); ++e) {
            map_cnt_p[c->first][e->first] = e->second;
            e->second = 0.0;
        }
    }
}

void PrintAlignLOO(ofstream& fout, vector<int>& ch, vector<int>& en, unordered_map<int, unordered_map<int, double>>& t,
                   string align_op) {
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

    for (auto j = 0; j != en.size(); ++j) {
        if (viterbi_alignment[j]) {
            if (firstword) {
                firstword = false;
            } else {
                fout << " ";
            }
            if (align_op == "ch_en") {
                fout << viterbi_alignment[j] - 1 << "-" << j;
            } else if (align_op == "en_ch") {
                fout << j << "-" << viterbi_alignment[j] - 1;
            }
        }
    }
    fout << endl;
}

void init_single_cnt_LOO(vector<vector<int>>& ch, vector<vector<int>>& en,
                         vector<unordered_map<int, unordered_map<int, double>>>& single_cnt) {
    single_cnt.resize(ch.size());
    for (int i = 0; i != ch.size(); ++i) {
        for (auto& c: ch[i]) {
            if (single_cnt[i].find(c) == single_cnt[i].end()) {
                single_cnt[i].insert({c, unordered_map<int, double>()});
            }
            for (auto& e:en[i]) {
                if (single_cnt[i][c].find(e) == single_cnt[i][c].end()) {
                    single_cnt[i][c].insert({e, 0.0});
                }
            }
        }
    }
}

void EMWithLeaveOneOut(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
                       unordered_map<int, unordered_map<int, double>>& unordered_map_count_p,
                       unordered_map<int, unordered_map<int, double>>& unordered_map_count_q,
                       vector<unordered_map<int, unordered_map<int, double>>>& single_cnt, const int cnt_ch,
                       const int cnt_en, int it, string align_op) {

    double uniform = double(1) / cnt_en;
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {

        auto l = (int) ch[line].size();
        auto m = (int) en[line].size();

        unordered_map<int, unordered_map<int, double >> map_t;
        map_t = getLOOt(unordered_map_count_p, single_cnt, cnt_ch, cnt_en, line);
        if (it == 5) {
            PrintAlignLOO(fout, ch[line], en[line], map_t, align_op);
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


void LeaveOneOut() {
    time_t start, stop;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/corpus.ch", vv_ch, cnt_ch, max_line_ch, "../data/corpus.model1.ch.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/corpus.en", vv_en, cnt_en, max_line_en, "../data/corpus.model1.en.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    unordered_map<int, unordered_map<int, double>> unordered_map_count_p, unordered_map_count_q;
    init_t2(vv_ch, vv_en, unordered_map_count_p, cnt_en);
    init_cnt(unordered_map_count_p);
    init_t2(vv_ch, vv_en, unordered_map_count_q, cnt_en);
    init_cnt(unordered_map_count_q);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_LOO(vv_ch, vv_en, single_cnt);

    start = time(NULL);

    for (int it = 1; it != 6; ++it) {
        if (it == 5) {
            print_t("../data/corpus.model1.ch.en.t", unordered_map_count_p);
        }
        EMWithLeaveOneOut("../data/corpus.model1.ch.en.loo.align", vv_ch, vv_en,
                          unordered_map_count_p, unordered_map_count_q, single_cnt, cnt_ch, cnt_en, it, "ch_en");
        MapCntQ2MapCntP(unordered_map_count_p, unordered_map_count_q);//这里可以更改，对于原来的概率不要完全舍弃，降低过拟合的风险
        stop = time(NULL);
        printf("model1_LOO use time:%ld\n", (stop - start));
    }

}

#endif //MODEL1_1_LEAVEONEOUT_H
