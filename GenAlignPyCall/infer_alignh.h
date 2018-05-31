//
// Created by 王青龙 on 2018/1/31.
//

#ifndef GENALIGNPYCALL_INFER_ALIGNH_H
#define GENALIGNPYCALL_INFER_ALIGNH_H

#include <algorithm>
#include "infer_align.h"


const int kLongSentence = 101;
const double kProbSmooth = 1e-4;
const double kHMMAlignmentModelSmoothFactor = 0.2;
const double kGLOBALProbabilityForEmpty = 0.4;

class InferAlignh : public InferAlign {
public:
    int ConstructAlCount_() {
        AlCount_.insert({0, 0.0});
        for (int i = 1; i != kLongSentence; ++i) {
            AlCount_.insert({i, 0.0});
            AlCount_.insert({-i, 0.0});
        }
        return 0;
    }

    int ReadBackgroundAlCount_(char* filename1) {
        std::string filename = filename1;
        std::ifstream fin(filename);
        std::string line;
        int key;
        double value;
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            iss >> key;
            iss >> value;
            AlCount_[key] = value;
        }
        fin.close();
        return 0;
    }

    int ReadBackgroundai_(char* filename1) {
        std::string filename = filename1;
        std::ifstream fin(filename);
        std::string line;
        int key;
        double value;
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            iss >> key;
            iss >> value;
            ai_[key].push_back(value);
        }
//        std::cout<<"ai大小"<<ai_.size()<<std::endl;
        fin.close();
        return 0;
    }

    int ReadBackgroundbi_(char* filename1) {
        std::string filename = filename1;
        std::ifstream fin(filename);
        std::string line;
        int key;
        double value;
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            iss >> key;
            iss >> value;
            bi_[key].push_back(value);
        }
        fin.close();
//        std::cout << "bi" << std::endl;
        return 0;
    }

    int ConstructNetN() {
        int I = 2 * (kLongSentence - 1);
        int J = kLongSentence - 1;
        for (int i = 0; i != I; ++i) {
            net_n_.push_back(std::vector<double>(J, 0.0));
        }
        return 0;
    }

    void InitNetN(std::vector<int>& v_ch, std::vector<int>& v_en, const int& cnt_line_ch, const int& cnt_line_en) {
        int I = 2 * (cnt_line_ch - 1);
        int J = cnt_line_en;
        double sum = 0.0;
//        std::cout<<"进来N"<<std::endl;
        for (int j = 0; j != J; ++j) {
            sum = 0;
            for (int i = 0; i != I / 2; ++i) {

                if ((t_table_.find(v_ch[i + 1]) != t_table_.end()) &&
                    (t_table_[v_ch[i + 1]].find(v_en[j]) !=
                     t_table_[v_ch[i + 1]].end())) {//稀疏词阈值15
                    net_n_[i][j] = std::max(t_table_[v_ch[i + 1]][v_en[j]], kProbSmooth);
                }
//            else if ((t_table_.find(v_ch[i + 1]) != t_table_.end()) &&
//                       (t_table_[v_ch[i + 1]].find(v_en[j]) !=
//                        t_table_[v_ch[i + 1]].end()) && word_freq_ch_[v_ch[i + 1]] <= 15 &&
//                       word_freq_en_[v_en[j]] <= 15) {
//                net_n_[i][j] = 1e-3;
//            }
                else {//在merged里面这一项应该是不存在的
                    net_n_[i][j] = kProbSmooth;
                }

                sum += net_n_[i][j];
            }
            for (int i = I / 2; i != I; ++i) {
                net_n_[i][j] = std::max(t_table_[v_ch[0]][v_en[j]], kProbSmooth);
                sum += net_n_[i][j];
            }
            if (sum > 0.0) {
                for (int i = 0; i != I; ++i) {
                    net_n_[i][j] /= sum;
                }
            }
        }
//        std::cout<<"出来N"<<std::endl;
    }

    int ConstructNetE() {
        int I = 2 * (kLongSentence - 1);
        for (int i = 0; i != I; ++i) {
            net_e_.push_back(std::vector<double>(I, 0.0));
        }
        return 0;
    }

    void InitNetE(bool doInit, const int& cnt_line_ch) {
        int l = cnt_line_ch - 1;
        int I = 2 * l;
        double sum = 0.0;

//        std::cout << "进入E" << std::endl;

        if (doInit) {//在增量里应该不会用到这个了
            for (int i = 0; i != I; ++i) {
                for (int j = 0; j != l; ++j) {
                    net_e_[i][j] = 1.0 / cnt_line_ch;
                }
                for (int j = l; j != I; ++j) {
                    if (i % l == j % l) {
                        net_e_[i][j] = 1.0 / cnt_line_ch;
                    } else {
                        net_e_[i][j] = 0.0;
                    }
                }

            }
            for (int i = 0; i != I; ++i) {
                sum = 0.0;
                for (int j = 0; j != I; ++j) {
                    sum += net_e_[i][j];
                }
                for (int j = 0; j != I; ++j) {
                    net_e_[i][j] /= sum;
                }
            }
        } else {
            std::vector<double> al(l, 0.0);
            for (int i = 0; i != I; ++i) {
                sum = 0.0;
                for (int j = 0; j != l; ++j) {
                    al[j] = AlCount_[i % l - j];
                    sum += al[j];
                }
                for (int j = 0; j != l; ++j) {
                    al[j] /= sum;
                }
                double p = kHMMAlignmentModelSmoothFactor;
                double pp = p / l;
                for (int j = 0; j != l; ++j) {
                    al[j] = (1.0 - p) * al[j] + pp;
                }
                sum = 0.0;
                for (int j = 0; j != I; ++j) {
                    net_e_[i][j] = al[j % l];

                    if (j >= l) {
                        if ((i % l) != (j % l)) {
                            net_e_[i][j] = 0.0;
                        } else {
                            net_e_[i][j] = kGLOBALProbabilityForEmpty;
                        }
                    }
                    sum += net_e_[i][j];
                }
                if (sum) {
                    for (int j = 0; j != I; ++j) {
                        net_e_[i][j] /= sum;
                    }
                } else {
                    for (int j = 0; j != I; ++j) {
                        net_e_[i][j] = 1.0 / l;
                    }
                }
            }
        }
//        std::cout << "出来E" << std::endl;
    }

    const char* EMLoop(char* output_f1) {
        std::string output_f = output_f1;
        static std::string line_align_prior = "";
        line_align_prior = "";
        for (int i = 0; i != corpus_index_ch_.size(); ++i) {
            MakeHMMNetwork(corpus_index_ch_[i], corpus_index_en_[i], 0);
            const int l = (int) corpus_index_ch_[i].size();
            const int I = 2 * (l - 1);
            std::vector<double> betainit_global(I, 0.0);
//            std::cout<<"pf"<<std::endl;
            ForwardBackwardTraining(corpus_index_ch_[i], corpus_index_en_[i], betainit_global, 0);
//            std::cout<<"hf"<<std::endl;
            line_align_prior += HMMRealViterbi(output_f, corpus_index_ch_[i], corpus_index_en_[i], betainit_global, 0);

        }

        std::cout << line_align_prior << std::endl;

        corpus_index_ch_.clear();
        corpus_index_ch_.shrink_to_fit();
        corpus_index_en_.clear();
        corpus_index_en_.shrink_to_fit();

        return line_align_prior.c_str();
    }



    void MakeHMMNetwork(std::vector<int>& v_ch, std::vector<int>& v_en, bool doInit) {
        int cnt_line_ch = (int) v_ch.size();
        int cnt_line_en = (int) v_en.size();

//        std::cout << "中文长度：" << cnt_line_ch << std::endl;
//        std::cout << "英文长度：" << cnt_line_en << std::endl;

        InitNetE(doInit, cnt_line_ch);

        InitNetN(v_ch, v_en, cnt_line_ch, cnt_line_en);
    }

    void ForwardBackwardTraining(std::vector<int>& v_ch, std::vector<int>& v_en, std::vector<double>& betainit_global,
                                 bool doInit) {
        const int I = 2 * ((int) v_ch.size() - 1);
        const int J = (int) v_en.size();
        const int N = I * J;

        std::vector<double> alphainit(I, 1.0);//真正的初始化在下面
        std::vector<double> betainit(I, 1.0);
        std::vector<double> alpha(N, 0.0);
        std::vector<double> beta(N, 0.0);

        if (doInit) {//这个在推理里面用不到
            double sum_alphainit = 0.0;
            for (int i = 0; i != I; ++i) {
                alphainit[i] = (i < I / 2) ? 1 : (2.0 / I);
                sum_alphainit += alphainit[i];
            }
            for (int i = 0; i != I; ++i) {
                alphainit[i] /= sum_alphainit;
            }

            double sum_betainit = 0.0;
            for (int i = 0; i != I; ++i) {
                sum_betainit += betainit[i];
            }
            for (int i = 0; i != I; ++i) {
                betainit[i] /= sum_betainit;
            }

            std::transform(betainit.begin(), betainit.end(), betainit.begin(),
                           std::bind1st(std::multiplies<double>(), I));

        } else {

//            std::cout<<"ppf"<<std::endl;

            double sum_alphainit = 0.0;
            for (int i = 0; i != I / 2 + 1; ++i) {
//                std::cout<<i<<std::endl;
//                std::cout<<I<<std::endl;
//                std::cout<<ai_.size()<<std::endl;
                alphainit[i] = ai_[I / 2][i];
                sum_alphainit += alphainit[i];
            }
//            std::cout<<"ppf1"<<std::endl;


            for (int i = I / 2 + 1; i != I; ++i) {
                alphainit[i] = 0.0;
            }

//            std::cout<<"ppf2"<<std::endl;

            for (int i = 0; i != I; ++i) {
                alphainit[i] /= sum_alphainit;
            }

//            std::cout<<"ppf3"<<std::endl;

            double sum_betainit = 0.0;
            for (int i = 0; i != I; ++i) {
                betainit[i] = bi_[I / 2][i];
                sum_betainit += betainit[i];
            }

//            std::cout<<"ppf4"<<std::endl;

            for (int i = 0; i != I; ++i) {
                betainit[i] /= sum_betainit;
            }


            std::transform(betainit.begin(), betainit.end(), betainit.begin(),
                           std::bind1st(std::multiplies<double>(), I));

        }

        for (int i = 0; i != I; ++i) {
            beta[N - I + i] = betainit[i];
            betainit_global[i] = betainit[i];
        }
    }

    std::string
    HMMRealViterbi(std::string output_file, std::vector<int>& v_ch, std::vector<int>& v_en,
                   std::vector<double>& betainit_global, bool doInit) {

        std::string res_align = "";
        const int l = (int) v_ch.size() - 1;
        const int I = 2 * l;
        const int J = (int) v_en.size();
        const int N = I * J;
        std::vector<double> alpha(N, -1);
        std::vector<double*> bp(N, (double*) 0);
        vit_.resize(J);

        std::vector<double> alphainit(I, 1.0);

        if (doInit) {
            double sum_alphainit = 0.0;
            for (int i = 0; i != I; ++i) {
                alphainit[i] = (i < l) ? 1.0 : (1.0 / l);
                sum_alphainit += alphainit[i];
            }
            for (int i = 0; i != I; ++i) {
                alphainit[i] /= sum_alphainit;
            }
        } else {
            double sum_alphainit = 0.0;
            for (int i = 0; i != I; ++i) {
                alphainit[i] = ai_[l][i];
                sum_alphainit += alphainit[i];
            }
            for (int i = 0; i != I; ++i) {
                alphainit[i] /= sum_alphainit;
            }
        }

        for (int i = 0; i != I; ++i) {
            alpha[i] = alphainit[i] * net_n_[i][0];
            if (i > l) {
                alpha[i] = 0;
            }
            bp[i] = 0;
        }

        auto cur_alpha = alpha.begin() + I;
        double** cur_bp = (&*bp.begin()) + I;
        for (int j = 1; j < J; ++j) {
            for (int ti = 0; ti != I; ++ti, ++cur_alpha, ++cur_bp) {
                double* prev_alpha = &*(alpha.begin() + I * (j - 1));
                double this_node = net_n_[ti][j];
                for (int pi = 0; pi != I; ++pi, ++prev_alpha) {
                    double alpha_increment = *prev_alpha * net_e_[pi][ti] * this_node;
                    if (alpha_increment > *cur_alpha) {
                        (*cur_alpha) = alpha_increment;
                        (*cur_bp) = prev_alpha;
                    }
                    //在这里有可能控制一下对空>I/2
                    //但是从这里看，这是一个越来越小的过程

//                if (pi >= I / 2) {
//                    if (alpha_increment != 0) {
//                        if (*cur_alpha < 5 * alpha_increment) {
//                            (*cur_alpha) = alpha_increment;
//                            (*cur_bp) = prev_alpha;
//                        }
//                    }
//                }

                }

            }
        }

        for (int i = 0; i != I; ++i) {
            alpha[N - I + i] *= betainit_global[i];
        }

        int j = J - 1;
        cur_alpha = alpha.begin() + j * I;
        vit_[J - 1] = int(std::max_element(cur_alpha, cur_alpha + I) - cur_alpha);
        while (bp[vit_[j] + j * I]) {
            cur_alpha -= I;
            vit_[j - 1] = int(bp[vit_[j] + j * I] - (&*cur_alpha));
            --j;
        }


        //在找完对齐之后，利用词频来过滤，第一次过滤
//    for (int j = 1; j <= J; ++j) {
//        if (word_freq_ch_[v_ch[vit_[j - 1]]] < 15) {
//            vit_[j - 1] = 0;
//        }
//    }


        viterbi_alignment_.resize(J + 1);
        for (int j = 1; j <= J; ++j) {
            viterbi_alignment_[j] = vit_[j - 1] + 1;


            if (viterbi_alignment_[j] > l) {
                viterbi_alignment_[j] = 0;

                //在这里可以考虑利用词典的信息
//            if (defs_.find(v_en[j - 1]) != defs_.end()) {
//                double max_prop = 0.0;
//                int max_index = 0;
//                for (int p = 0; p != v_ch.size(); ++p) {
//                    auto it = std::find(defs_[v_en[j - 1]].begin(), defs_[v_en[j - 1]].end(),
//                                        v_ch[p]);
//                    if (it != defs_[v_en[j - 1]].end()) {
//                        if (net_n_[p][j - 1] > max_prop) {
//                            max_prop = net_n_[p][j - 1];
//                            max_index = p;
//                        }
//                    }
//                }
//                if (net_n_[max_index][j - 1] > 0.09) {
//                    viterbi_alignment_[j] = max_index + 1;//阈值
//                }
//            }
            }
//        else{
//            if(defs_.find(v_en[j - 1]) != defs_.end()){
//                auto it = std::find(defs_[v_en[j - 1]].begin(), defs_[v_en[j - 1]].end(),
//                                    v_ch[viterbi_alignment_[j]-1]);
//                if(it == defs_[v_en[j-1]].end()){
//                    viterbi_alignment_[j] = 0;//找不到，对0
//                }
//            }
//
//        }
//        else if (net_n_[viterbi_alignment_[j] - 1][j - 1] < 0.15) {//设置一个阈值
//            viterbi_alignment_[j] = 0;
//        }
        }


        std::ofstream fout(output_file, std::ios_base::app);//接着写
        bool firstword = true;

        std::cout << "演示结果：" << std::endl;
        for (int j = 1; j <= J; ++j) {
            if (viterbi_alignment_[j]) {
                if (firstword) {
                    firstword = false;
                } else {
                    fout << " ";
                    std::cout << " ";
                    res_align += " ";
                }
//                中英输出格式
                fout << viterbi_alignment_[j] - 1 << "-" << j - 1;

                std::cout << viterbi_alignment_[j] - 1 << "-" << j - 1;

                res_align += std::to_string(viterbi_alignment_[j] - 1);
                res_align += "-";
                res_align += std::to_string(j - 1);

                //英中输出格式
//                fout << j - 1 << "-" << viterbi_alignment_[j] - 1;
            }
        }
        fout << std::endl;
        fout.close();
        std::cout << std::endl << "have a nice day!" << std::endl;
        return res_align;
    }

public:
    std::vector<std::vector<double >> net_n_;
    std::vector<std::vector<double >> net_e_;

    std::vector<double> gamma_;
    std::vector<double> E_;
    std::vector<int> vit_;
    std::vector<int> viterbi_alignment_;

    std::unordered_map<int, double> AlCount_;

    std::unordered_map<int, std::vector<double >> ai_;
    std::unordered_map<int, std::vector<double >> bi_;
};

#endif //GENALIGNPYCALL_INFER_ALIGNH_H
