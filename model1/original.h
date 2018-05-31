//
// Created by wangqinglong on 2017/8/22.
//

#ifndef MODEL1_1_ORIGINAL_H
#define MODEL1_1_ORIGINAL_H

#include "common.h"



void Original() {
    time_t start, stop;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/134w/zh.shuf", vv_ch, cnt_ch, max_line_ch, "../data/134w/model1.zh.shuf.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/134w/en.shuf", vv_en, cnt_en, max_line_en, "../data/134w/model1.en.shuf.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

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
        unordered_map_t_count2map_t(unordered_map_t, unordered_map_t_count);
        stop = time(NULL);
        printf("model1 ori en-ch use time:%ld\n", (stop - start));
    }

    print_t("../data/134w/model1.zh.en.shuf.t", unordered_map_t);
//    print_align2("/Users/wangql/Downloads/491/corpus.model1.ori.ch.en.align", vv_ch, vv_en, unordered_map_t);

    stop = time(NULL);
    printf("model1 ori ch-en use time:%ld\n", (stop - start));
}

#endif //MODEL1_1_ORIGINAL_H
