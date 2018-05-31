//
// Created by wangqinglong on 2017/8/23.
//

#ifndef MODEL1_1_IEMMETHOD_H
#define MODEL1_1_IEMMETHOD_H

#include "common.h"


void init_single_cnt_iEM(vector<vector<int>>& ch, vector<vector<int>>& en,
                         vector<unordered_map<int, unordered_map<int, double>>>& single_cnt) {
    single_cnt.resize(ch.size());
    for (int i = 0; i != ch.size(); ++i) {
        for (auto& c: ch[i]) {
            if (single_cnt[i].find(c) == single_cnt[i].end()) {
                single_cnt[i].insert({c, unordered_map<int, double>()});
            }
            for (auto& e:en[i]) {
                if (single_cnt[i][c].find(e) == single_cnt[i][c].end()) {
                    single_cnt[i][c].insert({e, 1.0 / en[i].size()});
                }
            }
        }
    }
}

void init_cnt_iEM(unordered_map<int, unordered_map<int, double>>& cnt,
                  vector<unordered_map<int, unordered_map<int, double>>>& single_cnt) {
    for (auto i = 0; i != single_cnt.size(); ++i) {
        for (auto c = single_cnt[i].begin(); c != single_cnt[i].end(); ++c) {
            if (cnt.find(c->first) == cnt.end()) {
                cnt.insert({c->first, unordered_map<int, double>()});
            }
            for (auto e = (c->second).begin(); e != (c->second).end(); ++e) {
                if (cnt[c->first].find(e->first) == cnt[c->first].end()) {
                    cnt[c->first].insert({e->first, e->second});
                }
                cnt[c->first][e->first] += e->second;
            }
        }
    }
}

unordered_map<int, unordered_map<int, double >> getiEMt(unordered_map<int, unordered_map<int, double >>& t_cnt,
                                                        vector<unordered_map<int, unordered_map<int, double>>>& single_cnt,
                                                        const int cnt_ch, const int cnt_en, int i) {

    auto map_t = single_cnt[i];
    for (auto it1 = single_cnt[i].begin(); it1 != single_cnt[i].end(); ++it1) {
        for (auto it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2) {
            map_t[it1->first][it2->first] = t_cnt[it1->first][it2->first];
            t_cnt[it1->first][it2->first] -= it2->second;//减去上一轮的
            it2->second = 0.0;//上一轮置零
        }
    }
    vector<int> nCh;
    vector<int> nEn;
    normalizeTable(map_t, nCh, nEn, cnt_ch, cnt_en);
    return map_t;

}

void EM_incremental(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
                    unordered_map<int, unordered_map<int, double>>& t_cnt,
                    vector<unordered_map<int, unordered_map<int, double>>>& single_cnt, const int cnt_ch,
                    const int cnt_en, int it, string align_op) {
    double uniform = double(1) / cnt_en;
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {

        auto l = (int) ch[line].size();
        auto m = (int) en[line].size();

        unordered_map<int, unordered_map<int, double >> map_t;
        map_t = getiEMt(t_cnt, single_cnt, cnt_ch, cnt_en, line);
        if (it == 5) {
            PrintAlignLOO(fout, ch[line], en[line], map_t, align_op);
        }

        for (auto& e : en[line]) {
            double denom = 0.0;
            vector<double*> sPtrCache(l, 0);
            double** sPtrCachePtr;
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
            if (denom > 0) {
                double val = double(1) / denom;
                sPtrCachePtr = &sPtrCache[0];
                for (auto& c : ch[line]) {
                    double ee = 0.0;
                    if (*(*sPtrCachePtr) > PROB_SMOOTH) {
                        ee = *(*sPtrCachePtr);
                    } else {
                        ee = PROB_SMOOTH;
                    }
                    ++sPtrCachePtr;
                    double x = ee * val;
                    t_cnt[c][e] += x;
                    single_cnt[line][c][e] += x;
                }
            } else {
                //不会发生
            }
        }
    }
    fout.close();
}

void iEMMethod() {
    time_t start, stop;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.ch", vv_ch, cnt_ch, max_line_ch, "/Users/wangqinglong/OneDrive/WordAlign/491/corpus.ch.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.en", vv_en, cnt_en, max_line_en,"/Users/wangqinglong/OneDrive/WordAlign/491/corpus.en.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    unordered_map<int, unordered_map<int, double>> unordered_map_cnt;
    init_t2(vv_ch, vv_en, unordered_map_cnt, cnt_en);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_iEM(vv_ch, vv_en, single_cnt);

    start = time(NULL);

    vector<int> nCh;
    vector<int> nEn;
    for (int it = 1; it != 6; ++it) {
        EM_incremental("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.align", vv_ch, vv_en,
                       unordered_map_cnt, single_cnt, cnt_ch, cnt_en, it, "ch_en");
        stop = time(NULL);
        printf("model1_iEM use time:%ld\n", (stop - start));
    }

}

#endif //MODEL1_1_IEMMETHOD_H
