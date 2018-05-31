//
// Created by 王青龙 on 2017/10/19.
//

#ifndef MODEL2_PRIOREM_H
#define MODEL2_PRIOREM_H

#include "common.h"

void prior_ut2t(unordered_map<int, unordered_map<int, double>>& unordered_map_t,
                unordered_map<int, unordered_map<int, double>>& unordered_map_t_count,
                unordered_map<int, int>& word_freq_ch){
    for (auto it_c = unordered_map_t.begin(); it_c != unordered_map_t.end(); ++it_c) {
        if(word_freq_ch[it_c->first] < 30){
            for (auto it_e = (it_c->second).begin(); it_e != (it_c->second).end(); ++it_e) {
                it_e->second = unordered_map_t_count[it_c->first][it_e->first] * word_freq_ch[it_c->first] * 0.1;
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


void Prior() {
    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/corpus.en", vv_ch, cnt_ch, max_line_ch, "../data/corpus.model2.en.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/corpus.ch", vv_en, cnt_en, max_line_en, "../data/corpus.model2.ch.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    WordCnt(vv_ch, word_freq_ch);

    unordered_map<int, unordered_map<int, double>> unmap_t;
    init_t(vv_ch, vv_en, unmap_t, cnt_en);

    unordered_map<int, unordered_map<int, double>> unmap_t_cnt;
    init_t_cnt(vv_ch, vv_en, unmap_t_cnt);

    vector<int> nCh;
    vector<int> nEn;

    unordered_map<int, unordered_map<int, unordered_map<int, double>>> uuua;

    unordered_map<int, unordered_map<int, unordered_map<int, double>>> uuua_cnt;

    initA2(uuua);
    initA2_cnt(uuua_cnt);//这两步没有执行貌似也没有错误，好奇。。

    start = time(NULL);
    for (int it = 1; it != 6; ++it) {

        em_4(vv_ch, vv_en, unmap_t_cnt, unmap_t, uuua, uuua_cnt, cnt_en, it);

        normalizeTTable(unmap_t_cnt, nCh, nEn, cnt_ch, cnt_en);
        normalizeATable(uuua_cnt);
//        un_map_t_cnt2un_map_t(unmap_t, unmap_t_cnt);
        prior_ut2t(unmap_t, unmap_t_cnt, word_freq_ch);
        uuu_a_cnt2uuu_a(uuua, uuua_cnt);

        stop = time(NULL);
        cout <<"model2_prior use time:"  <<stop - start << endl;
    }
    print_em3_align("../data/corpus.model2.prior.en.ch.align", vv_ch, vv_en, uuua, unmap_t);
}

#endif //MODEL2_PRIOREM_H
