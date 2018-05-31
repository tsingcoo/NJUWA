//
// Created by wangqinglong on 2017/8/24.
//

#ifndef MODEL2_EMWITHLOO_H
#define MODEL2_EMWITHLOO_H

#include "common.h"
#include "EMwithLOOlex.h"




unordered_map<int, unordered_map<int, double >>
getLOOa(unordered_map<int, unordered_map<int, double>>& uu_a,
        unordered_map<int, unordered_map<int, double>>& single_a_cnt) {
    auto map_a = uu_a;
//    for (auto it1 = single_a_cnt.begin(); it1 != single_a_cnt.end(); ++it1) {
//        for (auto it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2) {
//            map_a[it1->first][it2->first] -= it2->second;
//            it2->second = 0.0;//single_a_cnt清零
//        }
//    }
//    for (auto it1 = map_a.begin(); it1 != map_a.end(); ++it1) {
//        double total = 0;
//        for (auto it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2) {
//            total += it2->second;
//        }
//        if (total) {
//            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
//                it2->second /= total;
//            }
//        }
//    }
    return map_a;
}


void ACntQ2ACntP(unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a_cnt_p,
                 unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a_cnt_q) {
    for (auto it1 = a_cnt_q.begin(); it1 != a_cnt_q.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                a_cnt_p[it1->first][it2->first][it3->first] = it3->second;
                it3->second = 0.0;
            }
        }
    }

}


void init_single_a_cnt_LOO(vector<vector<int>>& ch, vector<vector<int>>& en,
                           vector<unordered_map<int, unordered_map<int, double>>>& single_a_cnt) {
    single_a_cnt.resize(ch.size());
    for (int i = 0; i != ch.size(); ++i) {
        for (int j = 0; j != en.size(); ++j) {
            single_a_cnt[i].insert({j, unordered_map<int, double>()});
            for (int k = 0; k != ch.size(); ++k) {
                single_a_cnt[i][j].insert({k, 0});
            }
        }
    }
}



void
em_4_LOO(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
         unordered_map<int, unordered_map<int, double>>& uu_cnt_p,
         unordered_map<int, unordered_map<int, double>>& uu_cnt_q,
         unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a_cnt_p,
         unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a_cnt_q,
         vector<unordered_map<int, unordered_map<int, double>>>& single_cnt,
         vector<unordered_map<int, unordered_map<int, double>>>& single_a_cnt, const int cnt_ch,
         const int cnt_en, int it, string align_op) {
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {
        int l = (int) ch[line].size();
        int j = 0;
        unordered_map<int, unordered_map<int, double>> uu_t = getLOOt(uu_cnt_p, single_cnt, cnt_ch, cnt_en, line);
        unordered_map<int, unordered_map<int, double >> a = getLOOa(a_cnt_p[l - 1], single_a_cnt[line]);
        if (it == 5) {
            PrintAlignLOO(fout, ch[line], en[line], uu_t, a, align_op);
        }
        for (auto& e : en[line]) {
            double denom = 0.0;
            vector<double*> sPtrCache(l, 0);
            int i = 0;
            for (auto& c : ch[line]) {
                sPtrCache[i] = &uu_t[c][e];
                denom += max(*sPtrCache[i], kProbSmooth) * max(a[j][i], kProbSmooth);
                ++i;
            }
            if (denom > 0) {
                double val = double(1) / denom;
                int i = 0;
                for (auto& c : ch[line]) {
                    double ee = max(*sPtrCache[i], kProbSmooth) * max(a[j][i], kProbSmooth);
                    double temp = ee * val;
                    uu_cnt_q[c][e] += temp;
                    single_cnt[line][c][e] += temp;
                    a_cnt_q[l - 1][j][i] += temp;
                    single_a_cnt[line][j][i] += temp;
                    ++i;
                }
            }
            ++j;
        }
    }
}

void LeaveOneOut() {

    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.ch", vv_ch, cnt_ch, max_line_ch,
             "/Users/wangqinglong/OneDrive/WordAlign/491/corpus.ch.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.en", vv_en, cnt_en, max_line_en,
                "/Users/wangqinglong/OneDrive/WordAlign/491/corpus.en.vcb");


//    unordered_map<int, unordered_map<int, double>> unmap_t;
//    init_t(vv_ch, vv_en, unmap_t, cnt_en);

    unordered_map<int, unordered_map<int, double>> unmap_t_cnt_p;
    init_t(vv_ch, vv_en, unmap_t_cnt_p, cnt_en);

    unordered_map<int, unordered_map<int, double>> unmap_t_cnt_q;
    init_t_cnt(vv_ch, vv_en, unmap_t_cnt_q);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_LOO(vv_ch, vv_en, single_cnt);


    vector<int> nCh;
    vector<int> nEn;

    max_line_ch += 1;
    max_line_en += 1;


    unordered_map<int, unordered_map<int, unordered_map<int, double>>> a_cnt_p;
    initA2(a_cnt_p);

    unordered_map<int, unordered_map<int, unordered_map<int, double>>> a_cnt_q;
    initA2_cnt(a_cnt_q);

    vector<unordered_map<int, unordered_map<int, double>>> single_a_cnt;
    init_single_a_cnt_LOO(vv_ch, vv_en, single_a_cnt);

    start = time(NULL);
    for (int it = 1; it != 6; ++it) {

        em_4_LOO("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.align", vv_ch, vv_en, unmap_t_cnt_p, unmap_t_cnt_q,
                 a_cnt_p, a_cnt_q, single_cnt, single_a_cnt, cnt_ch, cnt_en, it, "ch_en");

        MapCntQ2MapCntP(unmap_t_cnt_p, unmap_t_cnt_q);

//        normalizeATable(uuua_cnt);
        ACntQ2ACntP(a_cnt_p, a_cnt_q);

//        uuu_a_cnt2uuu_a(uuua, uuua_cnt);
        stop = time(NULL);
        cout << "model2_LOO use time:" << stop - start << endl;
    }
//    print_t("/Users/wangqinglong/Windows/26/corpus.model2.t", unmap_t);
//    print_a_3("/Users/wangqinglong/Windows/26/corpus.model2.a", uuua);

}

#endif //MODEL2_EMWITHLOO_H
