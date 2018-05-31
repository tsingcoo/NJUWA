//
// Created by wangqinglong on 2017/8/24.
//

#ifndef MODEL2_ORIGINAL_H
#define MODEL2_ORIGINAL_H

#include "common.h"



void un_map_t_cnt2un_map_t(unordered_map<int, unordered_map<int, double>> &un_map_t,
                           unordered_map<int, unordered_map<int, double>> &un_map_t_cnt) {
    for (auto it1 = un_map_t.begin(); it1 != un_map_t.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            it2->second = un_map_t_cnt[it1->first][it2->first];
            un_map_t_cnt[it1->first][it2->first] = 0.0;
        }
    }
}






void em4main() {

    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/134w/zh.shuf", vv_ch, cnt_ch, max_line_ch, "../data/134w/model2.zh.shuf.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/134w/en.shuf", vv_en, cnt_en, max_line_en, "../data/134w/model2.en.shuf.vcb");


    unordered_map<int, unordered_map<int, double>> unmap_t;
    init_t(vv_ch, vv_en, unmap_t, cnt_en);

    unordered_map<int, unordered_map<int, double>> unmap_t_cnt;
    init_t_cnt(vv_ch, vv_en, unmap_t_cnt);

    vector<int> nCh;
    vector<int> nEn;

//    max_line_ch += 1;
//    max_line_en += 1;


    unordered_map<int, unordered_map<int, unordered_map<int, double>>> uuua;

    unordered_map<int, unordered_map<int, unordered_map<int, double>>> uuua_cnt;

    initA2(uuua);
    initA2_cnt(uuua_cnt);//这两步没有执行貌似也没有错误，好奇。。

    start = time(NULL);
    for (int it = 1; it != 6; ++it) {

        em_4(vv_ch, vv_en, unmap_t_cnt, unmap_t, uuua, uuua_cnt, cnt_en, it);

        normalizeTTable(unmap_t_cnt, nCh, nEn, cnt_ch, cnt_en);
        normalizeATable(uuua_cnt);
        un_map_t_cnt2un_map_t(unmap_t, unmap_t_cnt);
        uuu_a_cnt2uuu_a(uuua, uuua_cnt);
//        if (it == 4) {
//            print_em3_align("/root/model2/merged.model2.4.align",
//                            vv_ch, vv_en, uuua, unmap_t);
//        }
        stop = time(NULL);
        cout <<"model2_ori use time:"  <<stop - start << endl;
    }
//    print_em3_align("../data/corpus.model2.en.ch.ori.align", vv_ch, vv_en, uuua, unmap_t);
    print_t("../data/134w/model2.zh.en.shuf.t", unmap_t);
    print_a_3("../data/134w/model2.zh.en.shuf.a", uuua);

    stop = time(NULL);
    printf("model2_ori use time:%ld\n", (stop - start));

}

#endif //MODEL2_ORIGINAL_H
