//
// Created by wangqinglong on 2017/8/30.
//

#ifndef MODEL2_EMWITHLOOLEX_H
#define MODEL2_EMWITHLOOLEX_H

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
    normalizeTTable(map_t, nCh, nEn, cnt_ch, cnt_en);
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
                    single_cnt[i][c].insert({e, 0.0});//第一次初始化的概率不能减，这样会造成找不到概率的情况
                }
            }
        }
    }
}

void
PrintAlignLOO(ofstream& fout, vector<int>& ch, vector<int>& en, unordered_map<int, unordered_map<int, double>>& uu_t,
              unordered_map<int, unordered_map<int, double>>& uu_a, string align_op) {
    bool firstword = true;
    double tmp_max = 0.0;
    int tmp_i = 0;

    firstword = true;
    int lc = (int) ch.size() - 1;
    vector<int> viterbi_alignment(en.size());
    for (int j = 0; j < en.size(); ++j) {
        tmp_max = 0.0;
        tmp_i = 0;
        for (int i = 0; i < ch.size(); ++i) {
            double v = uu_t[ch[i]][en[j]] * uu_a[j][i];

            if (tmp_max < v) {
                tmp_max = v;
                tmp_i = i;
            }

        }
        viterbi_alignment[j] = tmp_i;
    }
    for (int j = 0; j < en.size(); ++j) {
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


void
em_4_LOO_lex(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
             unordered_map<int, unordered_map<int, double>>& uu_cnt_p,
             unordered_map<int, unordered_map<int, double>>& uu_cnt_q,
             unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a,
             unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a_cnt,
             vector<unordered_map<int, unordered_map<int, double>>>& single_cnt, const int cnt_ch,
             const int cnt_en, int it, string align_op) {
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {
        int l = (int) ch[line].size();
        int j = 0;
        unordered_map<int, unordered_map<int, double>> uu_t = getLOOt(uu_cnt_p, single_cnt, cnt_ch, cnt_en, line);
        if (it == 5) {
            PrintAlignLOO(fout, ch[line], en[line], uu_t, a[l - 1], align_op);
        }
        for (auto& e : en[line]) {
            double denom = 0.0;
            vector<double*> sPtrCache(l, 0);
            int i = 0;
            for (auto& c : ch[line]) {
                sPtrCache[i] = &uu_t[c][e];
                denom += max(*sPtrCache[i], kProbSmooth) * max(a[l - 1][j][i], kProbSmooth);
                ++i;
            }
            if (denom > 0) {
                double val = double(1) / denom;
                int i = 0;
                for (auto& c : ch[line]) {
                    double ee = max(*sPtrCache[i], kProbSmooth) * max(a[l - 1][j][i], kProbSmooth);
                    double temp = ee * val;
                    uu_cnt_q[c][e] += temp;
                    single_cnt[line][c][e] += temp;
                    a_cnt[l - 1][j][i] += temp;
                    ++i;
                }
            }
            ++j;
        }
    }
}


void LeaveOneOutLex() {

    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/corpus.ch", vv_ch, cnt_ch, max_line_ch,
             "../data/corpus.model2.ch.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/corpus.en", vv_en, cnt_en, max_line_en,
                "../data/corpus.model2.en.vcb");


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


    unordered_map<int, unordered_map<int, unordered_map<int, double>>> a;
    initA2(a);

    unordered_map<int, unordered_map<int, unordered_map<int, double>>> a_cnt;
    initA2_cnt(a_cnt);

    start = time(NULL);
    for (int it = 1; it != 6; ++it) {

        if (it == 5) {
            print_t("../data/corpus.model2.ch.en.t", unmap_t_cnt_p);
            print_a_3("../data/corpus.model2.ch.en.a", a);
        }
        em_4_LOO_lex("../data/corpus.model2.ch.en.align", vv_ch, vv_en, unmap_t_cnt_p, unmap_t_cnt_q,
                     a, a_cnt, single_cnt, cnt_ch, cnt_en, it, "ch_en");

        MapCntQ2MapCntP(unmap_t_cnt_p, unmap_t_cnt_q);//更改

        normalizeATable(a_cnt);

        uuu_a_cnt2uuu_a(a, a_cnt);
        stop = time(NULL);
        cout << "model2_LOO use time:" << stop - start << endl;
    }
//    print_t("/Users/wangqinglong/Windows/26/corpus.model2.t", unmap_t);
//    print_a_3("/Users/wangqinglong/Windows/26/corpus.model2.a", uuua);

}

#endif //MODEL2_EMWITHLOOLEX_H
