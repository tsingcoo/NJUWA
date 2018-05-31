//
// Created by wangqinglong on 2017/8/25.
//

#ifndef MODELH_LEAVEONEOUT_H
#define MODELH_LEAVEONEOUT_H

#include "Commons.h"

void init_single_cnt_LOO(vector<vector<int>> &ch, vector<vector<int>> &en,
                         vector<unordered_map<int, unordered_map<int, double>>> &single_cnt) {
    single_cnt.resize(ch.size());
    for (int i = 0; i != ch.size(); ++i) {
        for (auto &c: ch[i]) {
            if (single_cnt[i].find(c) == single_cnt[i].end()) {
                single_cnt[i].insert({c, unordered_map<int, double>()});
            }
            for (auto &e:en[i]) {
                if (single_cnt[i][c].find(e) == single_cnt[i][c].end()) {
                    single_cnt[i][c][e] = 0.0;
                }
            }
        }
    }
}

void MapCntQ2MapCntP(unordered_map<int, unordered_map<int, double>> &map_cnt_p,
                     unordered_map<int, unordered_map<int, double>> &map_cnt_q) {
    for (auto c = map_cnt_q.begin(); c != map_cnt_q.end(); ++c) {
        for (auto e = (c->second).begin(); e != (c->second).end(); ++e) {
            map_cnt_p[c->first][e->first] = e->second;
            e->second = 0.0;
        }
    }
}

void ForwardBackwordTrainingLOO(vector<int> &v_ch, vector<int> &v_en, double **(&net_n), double **(&net_e),
                                vector<double> &gamma, vector<double> &E,
                                unordered_map<int, unordered_map<int, double>> &t_count,
                                unordered_map<int, vector<double>> &ai, unordered_map<int, vector<double>> &bi,
                                bool doInit, vector<double> &betainit_global,
                                vector<unordered_map<int, unordered_map<int, double>>> &single_cnt, int line) {
    const int I = 2 * ((int) v_ch.size() - 1);
    const int J = (int) v_en.size();
    const int N = I * J;
    vector<double> alphainit(I, 1.0);//这里只是占坑，真正的初始化在下面
    vector<double> betainit(I, 1.0);
    vector<double> alpha(N, 0);
    vector<double> beta(N, 0);


    if (doInit) {
        //这里的初始化情况仅仅对于第一次有效
        double sum_alphainit = 0.0;
        for (int i = 0; i < I; ++i) {
            alphainit[i] = (i < I / 2) ? 1 : (2.0 / I);//第一遍初始化分为前半段和后半段
            sum_alphainit += alphainit[i];
        }
        for (int i = 0; i < I; ++i) {
            alphainit[i] /= sum_alphainit;
        }

        double sum_betainit = 0.0;
        for (int i = 0; i < I; ++i) {
            sum_betainit += betainit[i];
        }
        for (int i = 0; i != I; ++i) {
            betainit[i] /= sum_betainit;
        }
        transform(betainit.begin(), betainit.end(), betainit.begin(), bind1st(multiplies<double>(), I));


    } else {
        double sum_alphainit = 0.0;
        for (int i = 0; i != I / 2 + 1; ++i) {//only first empty word can be chosen
            alphainit[i] = ai[I / 2][i];
            sum_alphainit += alphainit[i];
        }

        for (int i = I / 2 + 1; i != I; ++i) {//后半段除了第一个空其余都为0
            alphainit[i] = 0.0;
        }

        for (int i = 0; i != I; ++i) {
            alphainit[i] /= sum_alphainit;
        }

        double sum_betainit = 0.0;
        for (int i = 0; i != I; ++i) {
            betainit[i] = bi[I / 2][i];
            sum_betainit += betainit[i];
        }
        for (int i = 0; i != I; ++i) {
            betainit[i] /= sum_betainit;
        }
        transform(betainit.begin(), betainit.end(), betainit.begin(), bind1st(multiplies<double>(), I));
        //这里的目的是让每个元素乘以I，这样所有元素的和为I（2*l）

    }


    for (int i = 0; i < I; ++i) {
        beta[N - I + i] = betainit[i];
        betainit_global[i] = betainit[i];
    }

    int NN = N - I - 1;

    for (int j = J - 2; j >= 0; --j) {
        for (int ti = I - 1; ti >= 0; --ti, --NN) {
            auto next_beta = beta.begin() + (j + 1) * I;//后一列beta值最上面一个
            for (int ni = 0; ni < I; ++ni) {
                beta[NN] += (*next_beta++) * (net_e[ti][ni]) * (net_n[ni][j + 1]);
            }
        }
    }


    for (int i = 0; i < I; ++i) {
        alpha[i] = alphainit[i] * net_n[i][0];
    }

    auto cur_alpha = alpha.begin() + I;

    auto cur_beta = beta.begin() + I;

    E.resize(I * I);
    fill(E.begin(), E.end(), 0.0);

    for (int j = 1; j < J; ++j) {

        for (int ti = 0; ti < I; ++ti, ++cur_alpha, ++cur_beta) {
            auto prev_alpha = alpha.begin() + I * (j - 1);//迭代器
            auto this_node = net_n[ti][j];//double类型的翻译概率
            for (int pi = 0; pi < I; ++pi, ++prev_alpha) {
                auto alpha_increment = *prev_alpha * net_e[pi][ti] * this_node;
                (*cur_alpha) += alpha_increment;
                E[I * ti + pi] += alpha_increment * (*cur_beta);
            }
        }
    }

    gamma.resize(N);//这个resize很管用
    transform(alpha.begin(), alpha.end(), beta.begin(), gamma.begin(), multiplies<double>());//这个gamma是针对一句话的

    auto ge = gamma.end();
    for (auto gp = gamma.begin(); gp != ge; gp += I) {
        double sum = 0;
        for (auto gval = gp; gval != gp + I; ++gval) {
            sum += *gval;
        }
        if (sum) {
            for (auto gval = gp; gval != gp + I; ++gval) {
                *gval /= sum;
            }
        } else {
            fill(gp, gp + I, 1.0 / I);
        }
    }

    auto gp = gamma.begin();
    for (int j = 0; j != J; ++j) {
        for (int i = 0; i != I; ++i, ++gp) {
            if (*gp > PROB_SMOOTH) {
                if (i >= I / 2) {//i>=I/2的部分表示空
                    t_count[v_ch[0]][v_en[j]] += *gp;
                    single_cnt[line][v_ch[0]][v_en[j]] += *gp;
                } else {
                    t_count[v_ch[i + 1]][v_en[j]] += *gp;//v_ch下标加1用来跳过开头的空
                    single_cnt[line][v_ch[0]][v_en[j]] += *gp;
                }
            }
        }
    }

    double Esum = 0.0;

    for (auto Ep = E.begin(); Ep != E.end(); ++Ep) {
        Esum += *Ep;
    }
    if (Esum) {
        for (auto Ep = E.begin(); Ep != E.end(); ++Ep) {
            *Ep /= Esum;
            //            fout << "e：" << I << ":" << J << "  " << kk++ << "  " << *Ep << endl;
        }
    } else {//这一句应该不会执行
        for (auto Ep = E.begin(); Ep != E.end(); ++Ep) {
            *Ep = 1.0 / (I * I);
            //            fout << "e：" << I << ":" << J << "  " << kk++ << "  " << *Ep << endl;
//            cout<<"执行了我不认为会执行的一句！"<<endl;
        }
    }


}


unordered_map<int, unordered_map<int, double >> getLOOt(unordered_map<int, unordered_map<int, double >> &cnt_p,
                                                        vector<unordered_map<int, unordered_map<int, double>>> &single_cnt,
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
    normalizeTable(map_t, nCh, nEn, cnt_ch, cnt_en);
    return map_t;

}

void LeaveOneOut() {
    time_t start, stop;
    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile_ch("../data/26491/merged.26.ch", vv_ch, cnt_ch, max_line_ch, "../data/26491/merged.26.ch.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en("../data/26491/merged.26.en", vv_en, cnt_en, max_line_en, "../data/26491/merged.26.en.vcb");


    unordered_map<int, unordered_map<int, double>> t_cnt_p, t_cnt_q;//t用来存概率，t_count用来存储中间计算用来更新概率的结果
    init_t(vv_ch, vv_en, t_cnt_p, t_cnt_q, cnt_en);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_LOO(vv_ch, vv_en, single_cnt);

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

    ofstream fout("../data/26491/merged.26.modelh.ch.en.loo.align");

    start = time(NULL);
    for (int i = 0; i < vv_ch.size(); ++i) {

        makeHMMNetwork(fout, vv_ch[i], vv_en[i], net_n, t_cnt_p, net_e, AlCount, 1);

        const int I = 2 * ((int) vv_ch[i].size() - 1);
        vector<double> betainit_global(I, 0.0);

        ForwardBackwordTrainingLOO(vv_ch[i], vv_en[i], net_n, net_e, gamma, E, t_cnt_q, ai, bi, 1,
                                   betainit_global, single_cnt,
                                   i);//因为下面没有用到gamma，E,所以这些结果或许仅仅用于更新第二遍以后的概率，而下面的HMMRealViterbi
        addAlCount(fout, vv_ch[i], vv_en[i], E, AlCount_count);
        addAiBi(fout, vv_ch[i], gamma, ai_count, bi_count);

    }//


//    normalizeTable(fout, unordered_map_t_count, nCh, nEn, cnt_ch, cnt_en);

    stop = time(NULL);
    cout << "hmm LOO corpus use time:" << stop - start << endl;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    for (int loop = 0; loop != 4; ++loop) {
        MapCntQ2MapCntP(t_cnt_p, t_cnt_q);
        AlCount_count2AlCount(AlCount, AlCount_count);
        ai_count2ai(ai, ai_count);
        ai_count2ai(bi, bi_count);


        if (3 == loop) {
            print_t("../data/26491/merged.26.modelh.ch.en.loo.t", t_cnt_p);
            PrintAlCount("../data/26491/merged.26.modelh.ch.en.loo.AlCount", AlCount);
            Printai("../data/26491/merged.26.modelh.ch.en.loo.ai", ai);
            Printbi("../data/26491/merged.26.modelh.ch.en.loo.bi", bi);
        }


        for (int i = 0; i < vv_ch.size(); ++i) {
            //在这里加一个对概率表的leave one out操作
            unordered_map<int, unordered_map<int, double >> map_t;
            map_t = getLOOt(t_cnt_p, single_cnt, cnt_ch, cnt_en, i);
            makeHMMNetwork(fout, vv_ch[i], vv_en[i], net_n, map_t, net_e, AlCount, 0);
            const int I = 2 * ((int) vv_ch[i].size() - 1);
            vector<double> betainit_global(I, 0.0);
            ForwardBackwordTrainingLOO(vv_ch[i], vv_en[i], net_n, net_e, gamma, E, t_cnt_q, ai, bi, 0,
                                       betainit_global, single_cnt, i);
            if (loop == 3) {
                HMMRealViterbi(word_freq_ch, word_freq_en, fout, viterbi_alignment, vit, vv_ch[i], vv_en[i], net_n,
                               net_e, ai, 0, betainit_global, "ch_en");
            }
            addAlCount(fout, vv_ch[i], vv_en[i], E, AlCount_count);
            addAiBi(fout, vv_ch[i], gamma, ai_count, bi_count);
        }
//        normalizeTable(t_cnt_q, nCh, nEn, cnt_ch, cnt_en);
        stop = time(NULL);
        cout << "hmm LOO corpus use time:" << stop - start << endl;

    }
    fout.close();

    stop = time(NULL);
    cout << "hmm LOO corpus use time:" << stop - start << endl;
}

#endif //MODELH_LEAVEONEOUT_H
