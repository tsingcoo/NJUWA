//
// Created by wangqinglong on 2017/8/31.
//

#ifndef MODEL2_LOOFORKEYWORD_H
#define MODEL2_LOOFORKEYWORD_H

#include "common.h"
#include "EMwithLOOlex.h"

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


void
PrintAlignLOOforKeyword(ofstream& fout, vector<int>& ch, vector<int>& en,
                        unordered_map<int, unordered_map<int, double>>& uu_t,
                        unordered_map<int, unordered_map<int, double>>& uu_a, int index) {
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
    if (viterbi_alignment[index]) {
        fout << viterbi_alignment[index] - 1 << "-" << index;
    }
    fout << endl;
}

void
em_4_LOO_lex_keyword(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
                     unordered_map<int, unordered_map<int, double>>& uu_cnt_p,
                     unordered_map<int, unordered_map<int, double>>& uu_cnt_q,
                     unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a,
                     unordered_map<int, unordered_map<int, unordered_map<int, double>>>& a_cnt,
                     vector<unordered_map<int, unordered_map<int, double>>>& single_cnt, const int cnt_ch,
                     const int cnt_en, int it, vector<int>& index) {
    ofstream fout(str);
    for (int line = 0; line != ch.size(); ++line) {
        int l = (int) ch[line].size();
        int j = 0;
        unordered_map<int, unordered_map<int, double>> uu_t = getLOOt(uu_cnt_p, single_cnt, cnt_ch, cnt_en, line);
        if (it == 5) {
            PrintAlignLOOforKeyword(fout, ch[line], en[line], uu_t, a[l - 1], index[line]);
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


void LOOLexforKeyword() {

    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.ch",
             vv_ch, cnt_ch, max_line_ch,
             "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.ch.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.en",
                vv_en, cnt_en, max_line_en,
                "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.en.vcb");


    vector<int> Index = KeywordIndex(
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.keywordindex.txt");


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

        em_4_LOO_lex_keyword(
                "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.model2.align",
                vv_ch, vv_en, unmap_t_cnt_p, unmap_t_cnt_q,
                a, a_cnt, single_cnt, cnt_ch, cnt_en, it, Index);

        MapCntQ2MapCntP(unmap_t_cnt_p, unmap_t_cnt_q);

        normalizeATable(a_cnt);

        uuu_a_cnt2uuu_a(a, a_cnt);
        stop = time(NULL);
        cout << "model2_LOO use time:" << stop - start << endl;
    }
//    print_t("/Users/wangqinglong/Windows/26/corpus.model2.t", unmap_t);
//    print_a_3("/Users/wangqinglong/Windows/26/corpus.model2.a", uuua);

}


#endif //MODEL2_LOOFORKEYWORD_H
