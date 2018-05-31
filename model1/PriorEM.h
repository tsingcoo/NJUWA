//
// Created by 王青龙 on 2017/10/18.
//

#ifndef MODEL1_PRIOREM_H
#define MODEL1_PRIOREM_H

#include "common.h"

void prior_ut2t(unordered_map<int, unordered_map<int, double>>& unordered_map_t,
                unordered_map<int, unordered_map<int, double>>& unordered_map_t_count,
                unordered_map<int, int>& word_freq_ch){
    for (auto it_c = unordered_map_t.begin(); it_c != unordered_map_t.end(); ++it_c) {
        if(word_freq_ch[it_c->first] < 30){
            for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
                it_e->second = unordered_map_t_count[it_c->first][it_e->first] * word_freq_ch[it_c->first] * 0.01;
                unordered_map_t_count[it_c->first][it_e->first] = 0.0;
            }
        } else{
            for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
                it_e->second = unordered_map_t_count[it_c->first][it_e->first];
                unordered_map_t_count[it_c->first][it_e->first] = 0.0;
            }
        }
    }
}

void Prior(){

    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/corpus.en", vv_ch, cnt_ch, max_line_ch, "../data/corpus.model1.prior.r.en.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/corpus.ch", vv_en, cnt_en, max_line_en, "../data/corpus.model1.prior.r.ch.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    WordCnt(vv_ch, word_freq_ch);

    unordered_map<int, unordered_map<int, double>> unordered_map_t, unordered_map_t_count;
    init_t2(vv_ch, vv_en, unordered_map_t, cnt_en);
    init_t2(vv_ch, vv_en, unordered_map_t_count, cnt_en);
    init_cnt(unordered_map_t_count);

    vector<int> nCh;
    vector<int> nEn;

    start = time(NULL);

    for (int it = 1; it != 6; ++it) {
        em4(word_freq_ch, word_freq_en, vv_ch, vv_en, unordered_map_t_count, unordered_map_t, cnt_en, it);
        normalizeTable(unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en);
        //可以考虑在这里加入对于概率的先验操作
//        unordered_map_t_count2map_t(unordered_map_t, unordered_map_t_count);

        if(it == 5){
            print_t("../data/corpus.model1.prior.en.ch.t", unordered_map_t);
        }

        prior_ut2t(unordered_map_t, unordered_map_t_count, word_freq_ch);
        stop = time(NULL);
        printf("model1 prior en-ch use time:%ld\n", (stop - start));
    }


    print_align2("../data/corpus.model1.prior.en.ch.align", vv_ch, vv_en, unordered_map_t);

    stop = time(NULL);
    printf("model1 prior en-ch use time:%ld\n", (stop - start));
}

#endif //MODEL1_PRIOREM_H
