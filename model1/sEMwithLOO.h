//
// Created by wangqinglong on 2017/8/28.
//

#ifndef MODEL1_1_SEMWITHLOO_H
#define MODEL1_1_SEMWITHLOO_H

#include "sEM.h"
#include "LOO.h"

void sEMwithLOO(string str, vector<vector<int>> &ch, vector<vector<int>> &en,
                unordered_map<int, unordered_map<int, double>> &unordered_map_t,
                vector<unordered_map<int, unordered_map<int, double>>> &single_cnt, const int cnt_ch, const int cnt_en,
                int it,
                double alpha, double pi_eta, string align_op) {
    double uniform = double(1) / cnt_en;
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {

        unordered_map<int, unordered_map<int, double >> map_t_LOO;
        map_t_LOO = getLOOt(unordered_map_t, single_cnt, cnt_ch, cnt_en, line);
        if (it == 5) {
            PrintAlignLOO(fout, ch[line], en[line], map_t_LOO, align_op);
        }

        int l = (int) ch[line].size();
        int m = (int) en[line].size();
        double num = eta((it - 1) * ch.size() + line, alpha);
        pi_eta *= (1 - num);
        for (auto &e : en[line]) {
            double denom = 0.0;
            vector<double *> sPtrCache(l, 0);
            double **sPtrCachePtr;
            if (it == 1) {
                denom = uniform * l;
            } else {
                sPtrCachePtr = &sPtrCache[0];
                for (auto &c : ch[line]) {
                    *sPtrCachePtr = &map_t_LOO[c][e];
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
                for (auto &c : ch[line]) {
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
                    unordered_map_t[c][e] += num / pi_eta * x;
                    single_cnt[line][c][e] += num / pi_eta * x;
                }
            } else {
                //不会发生
            }
        }
    }
}

void sEMwithLOO() {
    time_t start, stop;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("/Users/wangql/OneDrive/WordAlign/491/corpus.ch", vv_ch, cnt_ch, max_line_ch, "/Users/wangql/OneDrive/WordAlign/491/corpus.ch.vcb");

    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;
    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("/Users/wangql/OneDrive/WordAlign/491/corpus.en", vv_en, cnt_en, max_line_en, "/Users/wangql/OneDrive/WordAlign/491/corpus.en.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;


    unordered_map<int, unordered_map<int, double>> unordered_map_t;
    init_t2(vv_ch, vv_en, unordered_map_t, cnt_en);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_LOO(vv_ch, vv_en, single_cnt);

    vector<int> nCh;
    vector<int> nEn;

    start = time(NULL);


    double pi_eta = 1.0;
    for (int it = 1; it != 6; ++it) {
        sEMwithLOO("/Users/wangql/OneDrive/WordAlign/491/corpus.align", vv_ch, vv_en, unordered_map_t, single_cnt,
                   cnt_ch, cnt_en, it, -0.8, pi_eta, "ch_en");
        normalizeTable(unordered_map_t, nCh, nEn, cnt_ch, cnt_en);
        stop = time(NULL);
        printf("model1_sem use time:%ld\n", (stop - start));
    }


    stop = time(NULL);
    printf("model1_sem use time:%ld\n", (stop - start));

}


#endif //MODEL1_1_SEMWITHLOO_H
