//
// Created by 王青龙 on 2017/9/18.
//

#ifndef MODEL1_1_EXT_H
#define MODEL1_1_EXT_H

#include "common.h"

void
readfile_ext(string str, vector<vector<int>>& vv, unsigned int& cnt, unsigned int& max_line, string output_vcb_file) {
    ifstream fin(str);
    string line;
    string word;
    int i = 0;
    int j = 0;
    unsigned int cnt_line = 0;
    max_line = 0;
    unordered_map<string, int> word2int;

    ++j;
//    word2int.insert({"null_wangql", j});
    word2int["null_wangql"] += j;
    ++j;//让j从2开始

    while (getline(fin, line)) {
        istringstream linestream(line);
        vv.push_back(vector<int>());

//        bool first_word = true;//

        cnt_line = 0;
        vv[i].push_back(word2int["null_wangql"]);//在句首加个1
        ++cnt_line;

        while (linestream >> word) {
            ++cnt_line;
            auto ret = word2int.insert({word, j});
            if (ret.second) {
                ++j;
            }
            vv[i].push_back(word2int[word]);
        }
        if (cnt_line > max_line) {
            max_line = cnt_line;
        }
        ++i;
    }
    cnt = j;//cnt数量包含空值
    fin.close();

    ofstream fout(output_vcb_file);
    for (auto it = word2int.begin(); it != word2int.end(); ++it) {
        fout << it->second << " " << it->first << endl;
    }
    fout.close();
}

void Ext() {

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile("../data/corpus.ch", vv_ch, cnt_ch, max_line_ch, "../data/corpus.model1.ch.vcb");
    cout << "中文句长: " << max_line_ch << endl;
    cout << "中文词数: " << cnt_ch << endl;

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/corpus.en", vv_en, cnt_en, max_line_en, "../data/corpus.model1.en.vcb");
    cout << "英文句长: " << max_line_en << endl;
    cout << "英文词数: " << cnt_en << endl;

    unordered_map<int, unordered_map<int, double>> unordered_map_t, unordered_map_t_count;

    init_t_ext(unordered_map_t, vv_ch, vv_en, cnt_ch, cnt_en);
    init_t_ext(unordered_map_t_count, vv_ch, vv_en, cnt_ch, cnt_en);


    vector<int> nCh;
    vector<int> nEn;

    auto start = chrono::system_clock::now();

    for (int it = 1; it != 3; ++it) {
        em4(word_freq_ch, word_freq_en, vv_ch, vv_en, unordered_map_t_count, unordered_map_t, cnt_en, it);
        normalizeTable(unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en);
        unordered_map_t_count2map_t(unordered_map_t, unordered_map_t_count);
        auto end = chrono::system_clock::now();
        auto duration = duration_cast<chrono::seconds>(end - start);
        cout << "model1_em4 use time:"
             << double(duration.count()) * chrono::seconds::period::num / chrono::seconds::period::den << endl;
    }

    print_align2("../data/corpus.model1.ch.en.align", vv_ch, vv_en, unordered_map_t);

}

#endif //MODEL1_1_EXT_H
