//
// Created by wangqinglong on 2017/8/29.
//

#ifndef MODELH_LOOFORKEYWORD_H
#define MODELH_LOOFORKEYWORD_H


#include "Commons.h"
#include "LeaveOneOut.h"


vector<int> KeywordIndex(string str){
    vector<int> res;
    ifstream fin(str);
    string line;
    int index;
    while(getline(fin, line)){
        istringstream linestream(line);
        linestream>>index;
        res.push_back(index);
    }
    return res;
}

void
HMMRealViterbiForKeyword(unordered_map<int, int>& word_freq_ch, unordered_map<int, int>& word_freq_en, ofstream& fout,
                         vector<int>& viterbi_alignment, vector<int>& vitar, vector<int>& v_ch, vector<int>& v_en,
                         double** (& net_n), double** (& net_e), unordered_map<int, vector<double>>& ai, bool doInit,
                         vector<double>& betainit_global, int index) {
    const int l = (int) v_ch.size() - 1;
    const int I = 2 * l;
    const int J = (int) v_en.size();
    const int N = I * J;
    vector<double> alpha(N, -1);
    vector<double*> bp(N, (double*) 0);//用以存储当前节点取最大值时前一节点
    vitar.resize(J);

    vector<double> alphainit(I, 1.0);//这里只是占坑，真正的初始化在下面


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
    } else {
        double sum_alphainit = 0.0;
        for (int i = 0; i != I; ++i) {
            alphainit[i] = ai[I / 2][i];
            sum_alphainit += alphainit[i];
        }
        for (int i = 0; i != I; ++i) {
            alphainit[i] /= sum_alphainit;
        }
    }


    for (int i = 0; i < I; ++i) {
        alpha[i] = alphainit[i] * net_n[i][0];
        //        cout << I << ":" << J << "   " << i << "    beta    " << beta[i] <<"   betainit    "<<betainit[i]<<"    net_n    "<<net_n[i][0]<< endl;

        if (i > I / 2) {//only first empty word can be chosen
            alpha[i] = 0;
        }
//
//        fout<<alphainit[i]<<"*"<<net_n[i][0]<<endl;//由此发现查找到的不同之处出现在net_n[i][0]上面
//        fout<<I<<"   alpha["<<i<<"]   "<<alpha[i]<<endl;
        bp[i] = 0;
    }


    auto cur_alpha = alpha.begin() + I;
    //    auto cur_bp=bp.begin()+I;
    double** cur_bp = (&*bp.begin()) + I;
    for (int j = 1; j < J; ++j) {
        for (int ti = 0; ti < I; ++ti, ++cur_alpha, ++cur_bp) {
            double* prev_alpha = &*(alpha.begin() + I * (j - 1));

            //            auto prev_alpha=alpha.begin()+I*(j-1);
            double this_node = net_n[ti][j];//翻译概率

//            if(word_freq_ch[v_ch[ti]]>2 || ti >=l){//加了一条限制,但是我这个限制好像并没有起作用

            for (int pi = 0; pi < I; ++pi, ++prev_alpha) {
                //                cout<<*prev_alpha<<endl;
                //                double test=*prev_alpha;
                double alpha_increment =
                        *prev_alpha * net_e[pi][ti] * this_node;//之前在这里访问错误是因为上面的pi最后一部分忘记写++pi了，因此这里不能少
                if (alpha_increment > *cur_alpha) {
                    (*cur_alpha) = alpha_increment;
                    (*cur_bp) = prev_alpha;//存放是指针
                }
            }

//            }
        }
    }


//    vector<double> betainit(I, 1.0);
//
//
//    for (int i=0; i<I; ++i) {
//        alpha[N-I+i]*=betainit[i];
//    }
    //上面这一段没有任何意义，但是对于原始HMM算法,最后一列确实没有乘betainit
    //对于下面乘以betainit的情况，可以认为乘的是那个隐变量出现的概率

    for (int i = 0; i < I; ++i) {
        alpha[N - I + i] *= betainit_global[i];
        //cout<<betainit_global[i]<<endl;
    }


    int j = J - 1;
    cur_alpha = alpha.begin() + j * I;
    vitar[J - 1] = int(max_element(cur_alpha, cur_alpha + I) - cur_alpha);//max_element返回的是迭代器，不是元素，这里返回的是在这一列的索引
    while (bp[vitar[j] + j * I]) {//bp里面放的是此点的前一个点的指针，vitar[j]+j*I得到的是全局的索引
        cur_alpha -= I;
        vitar[j - 1] = int(bp[vitar[j] + j * I] - (&*cur_alpha));//这样减掉本列的初始值确实得到本列的偏移值
        --j;
    }

    viterbi_alignment.resize(J + 1);//注意J+1个长度
    for (int j = 1; j <= J; ++j) {
        viterbi_alignment[j] = vitar[j - 1] + 1;
        if (viterbi_alignment[j] > l) {//对的位置大于实际长度，就是对空了
            viterbi_alignment[j] = 0;
        }
    }


    if (viterbi_alignment[index + 1]) {
        fout << viterbi_alignment[index + 1] - 1 << "-" << index;
    }
    fout << endl;

}

void LOOforKeyword() {
    time_t start, stop;
    vector<vector<int>> vv_ch;
    unsigned int cnt_ch;
    unsigned int max_line_ch;
    readfile_ch(
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.ch",
            vv_ch, cnt_ch, max_line_ch,
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.ch.vcb");

    vector<vector<int>> vv_en;
    unsigned int cnt_en;
    unsigned int max_line_en;
    readfile_en(
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.en",
            vv_en, cnt_en, max_line_en,
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.en.vcb");


    vector<int> Index = KeywordIndex("/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.keywordindex.txt");


    unordered_map<int, unordered_map<int, double>> t_cnt_p, t_cnt_q;//t用来存概率，t_count用来存储中间计算用来更新概率的结果
    init_t(vv_ch, vv_en, t_cnt_p, t_cnt_q, cnt_en);

    vector<unordered_map<int, unordered_map<int, double >>> single_cnt;
    init_single_cnt_LOO(vv_ch, vv_en, single_cnt);

    double** net_n;
    cre_net_n(net_n, max_line_ch, max_line_en);

    double** net_e;
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

    ofstream fout(
            "/Users/wangqinglong/Library/Mobile Documents/com~apple~CloudDocs/Shanbay/keyword_align/examples_OALD/examples_OALD.modelh.align");

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
    cout << "hmm LOOforKeyword corpus use time:" << stop - start << endl;

    unordered_map<int, int> word_freq_ch;
    unordered_map<int, int> word_freq_en;

    for (int loop = 0; loop != 4; ++loop) {
        MapCntQ2MapCntP(t_cnt_p, t_cnt_q);
        AlCount_count2AlCount(AlCount, AlCount_count);
        ai_count2ai(ai, ai_count);
        ai_count2ai(bi, bi_count);

//
//        if (3 == loop) {
//            print_t("/Users/wangqinglong/Windows/26/corpus.modelh.t", t_cnt_p);
//            PrintAlCount("/Users/wangqinglong/Windows/26/corpus.modelh.AlCount", AlCount);
//            Printai("/Users/wangqinglong/Windows/26/corpus.modelh.ai", ai);
//            Printbi("/Users/wangqinglong/Windows/26/corpus.modelh.bi", bi);
//        }


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


                HMMRealViterbiForKeyword(word_freq_ch, word_freq_en, fout, viterbi_alignment, vit, vv_ch[i], vv_en[i], net_n,
                               net_e, ai, 0, betainit_global, Index[i]);
            }
            addAlCount(fout, vv_ch[i], vv_en[i], E, AlCount_count);
            addAiBi(fout, vv_ch[i], gamma, ai_count, bi_count);
        }
//        normalizeTable(t_cnt_q, nCh, nEn, cnt_ch, cnt_en);
        stop = time(NULL);
        cout << "hmm LOOforKeyword corpus use time:" << stop - start << endl;

    }
    fout.close();

    stop = time(NULL);
    cout << "hmm LOOforKeyword corpus use time:" << stop - start << endl;
}


#endif //MODELH_LOOFORKEYWORD_H
