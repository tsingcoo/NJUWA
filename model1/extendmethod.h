//
// Created by wangqinglong on 2017/8/22.
//

#ifndef MODEL1_1_EXTENDMETHOD_H
#define MODEL1_1_EXTENDMETHOD_H

#include "common.h"


int ExternVV(vector<vector<int>>& vv, int length, int startNull) {
    int cnt = 0;
    for (auto i = 0; i < vv.size(); ++i) {
        for (auto j = vv[i].size(); j < length; ++j) {
//            cout << "i:" << i << "  j:" << j << endl;
            vv[i].push_back(startNull);
            startNull += 1;
            cnt += 1;
        }
    }
    return cnt;
}

void print_align_extend(string str, vector<vector<int>>& ch, vector<vector<int>>& en,
                        unordered_map<int, unordered_map<int, double>>& t, int cnt_en, int cnt_ch) {
    ofstream fout(str);
    double tmp_max = 0.0;
    int tmp_k = 0;
    bool firstword = true;
    for (auto i = 0; i < ch.size(); ++i) {
        firstword = true;
        vector<int> viterbi_alignment(en[i].size());//存储每个目标语言所对应的源语言号码
        for (auto j = 0; j < en[i].size() && en[i][j] < cnt_en + 2; ++j) {
            tmp_max = 0.0;
            tmp_k = 0;
            for (auto k = 0; k < ch[i].size() && ch[i][k] < cnt_ch + 1; ++k) {//k=0代表对空
                if (tmp_max < t[ch[i][k]][en[i][j]]) {
                    tmp_max = t[ch[i][k]][en[i][j]];
                    tmp_k = k;
                }
            }

            if (tmp_max < 0.01) {//去掉较小的不可信的对齐
                tmp_k = 0;
            }

            viterbi_alignment[j] = tmp_k;//也就是说，在这里已经得到了每个目标语言应该对哪个源语言
        }

        for (auto j = 0; j != en[i].size(); ++j) {
            if (viterbi_alignment[j]) {//跳过k=0的对空
                if (firstword) {
                    firstword = false;
                } else {
                    fout << " ";
                }
                fout << viterbi_alignment[j] - 1 << "-" << j;//因为源语言开头有个空，所以这里需要-1
            }
        }
        fout << endl;
    }

    fout.close();
}

void ExtendMethod() {
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
    readfile_en("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.en", vv_en, cnt_en, max_line_en, "/Users/wangqinglong/OneDrive/WordAlign/491/corpus.en.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    int cnt_ch_patch = ExternVV(vv_ch, 150, cnt_ch + 1);
    int cnt_en_patch = ExternVV(vv_en, 150, cnt_en + 2);
    cnt_ch += cnt_ch_patch;
    cnt_en += cnt_en_patch;

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
        printf("model1_em4 use time:%ld\n", (stop - start));
    }

    print_align_extend("/Users/wangqinglong/OneDrive/WordAlign/491/corpus.align", vv_ch, vv_en, unordered_map_t,
                       cnt_en - cnt_en_patch, cnt_ch - cnt_ch_patch);

    stop = time(NULL);
    printf("model1_em4 use time:%ld\n", (stop - start));

}

#endif //MODEL1_1_EXTENDMETHOD_H
