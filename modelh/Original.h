//
// Created by wangqinglong on 2017/8/25.
//

#ifndef MODELH_ORIGINAL_H
#define MODELH_ORIGINAL_H

#include "Commons.h"


void Original(){
    time_t start, stop;

    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile_ch("/Users/wangql/wordaligntest/corpus.ch", vv_ch, cnt_ch, max_line_ch, "/Users/wangql/wordaligntest/corpus.ch.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("/Users/wangql/wordaligntest/corpus.en", vv_en, cnt_en, max_line_en, "/Users/wangql/wordaligntest/corpus.en.vcb");


    unordered_map<int, unordered_map<int, double>> unordered_map_t, unordered_map_t_count;//t用来存概率，t_count用来存储中间计算用来更新概率的结果
    init_t(vv_ch, vv_en, unordered_map_t, unordered_map_t_count, cnt_en);

    double **net_n;
    cre_net_n(net_n, max_line_ch, max_line_en);

    double **net_e;
    cre_net_e(net_e, max_line_ch);

    vector<double> gamma;//gamma每个句对都不一样，因此需要在每个句对前面进行构造一下
    vector<double> E;//E每个句对都不一样，因此同样需要在每个句对前面进行构造一下
    vector<int> vit;
    vector<int> viterbi_alignment;

    vector<int> nCh;//每个ch对应en的个数
    vector<int> nEn;

    unordered_map<int, double> AlCount, AlCount_count;//用来保存net_e变化的中间结果
    cre_AlCount(max_line_ch, AlCount);
    cre_AlCount(max_line_ch, AlCount_count);//存放中间结果，类似unordered_map_t_count的作用

    unordered_map<int, vector<double>> ai, bi, ai_count, bi_count;//分别用来存放alpha，beta初始化
    cre_ai(max_line_ch, ai);
    cre_ai(max_line_ch, bi);
    cre_ai(max_line_ch, ai_count);//存放中间结果，类似unordered_map_t_count的作用
    cre_ai(max_line_ch, bi_count);

    ofstream fout("/Users/wangql/wordaligntest/modelh.ch.en.align");

    start = time(NULL);
    for (int i = 0; i < vv_ch.size(); ++i) {
        makeHMMNetwork(fout, vv_ch[i], vv_en[i], net_n, unordered_map_t, net_e, AlCount, 1);

        const int I = 2 * ((int) vv_ch[i].size() - 1);
        vector<double> betainit_global(I, 0.0);

        ForwardBackwordTraining(fout, vv_ch[i], vv_en[i], net_n, net_e, gamma, E, unordered_map_t_count, ai, bi, 1,
                                betainit_global);//因为下面没有用到gamma，E,所以这些结果或许仅仅用于更新第二遍以后的概率，而下面的HMMRealViterbi
        //HMMRealViterbi(fout, viterbi_alignment, vit, vv_ch[i], vv_en[i], net_n, net_e, ai, 1, betainit_global);
        addAlCount(fout, vv_ch[i], vv_en[i], E, AlCount_count);
        addAiBi(fout, vv_ch[i], gamma, ai_count, bi_count);

    }//


    normalizeTable(unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en);
//    normalizeTableMy(unordered_map_t_count);

    stop = time(NULL);
    cout << "hmm ori corpus use time:" << stop - start << endl;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    for (int loop = 0; loop != 4; ++loop) {
        unordered_map_t_count2map_t(unordered_map_t, unordered_map_t_count);
        AlCount_count2AlCount(AlCount, AlCount_count);
        ai_count2ai(ai, ai_count);
        ai_count2ai(bi, bi_count);


        if (3 == loop) {
            print_t("/Users/wangql/wordaligntest/modelh.ch.en.t", unordered_map_t);
            PrintAlCount("/Users/wangql/wordaligntest/modelh.ch.en.AlCount", AlCount);
            Printai("/Users/wangql/wordaligntest/modelh.ch.en.ai", ai);
            Printbi("/Users/wangql/wordaligntest/modelh.ch.en.bi", bi);
        }


        for (int i = 0; i < vv_ch.size(); ++i) {
            makeHMMNetwork(fout, vv_ch[i], vv_en[i], net_n, unordered_map_t, net_e, AlCount, 0);
            const int I = 2 * ((int) vv_ch[i].size() - 1);
            vector<double> betainit_global(I, 0.0);
            ForwardBackwordTraining(fout, vv_ch[i], vv_en[i], net_n, net_e, gamma, E, unordered_map_t_count, ai, bi, 0,
                                    betainit_global);
            if (loop == 3) {


                HMMRealViterbi(word_freq_ch, word_freq_en, fout, viterbi_alignment, vit, vv_ch[i], vv_en[i], net_n,
                               net_e, ai, 0, betainit_global, "ch_en");
            }
            addAlCount(fout, vv_ch[i], vv_en[i], E, AlCount_count);
            addAiBi(fout, vv_ch[i], gamma, ai_count, bi_count);
        }
        normalizeTable(unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en);
//        normalizeTableMy(unordered_map_t_count);
        stop = time(NULL);
        cout << "hmm ori corpus use time:" << stop - start << endl;

    }
    fout.close();

    stop = time(NULL);
    printf("hmm ori corpus use time:%ld\n", (stop - start));
}

#endif //MODELH_ORIGINAL_H
