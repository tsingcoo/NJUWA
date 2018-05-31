//
// Created by 王青龙 on 26/12/2016.
//

#include "incre_modelh.h"

void IncreModelh::EMLoop(int loop) {

    for (int i = 0; i != corpus_index_ch_.size(); ++i) {
        MakeHMMNetwork(corpus_index_ch_[i], corpus_index_en_[i], 0);
        const int l = (int) corpus_index_ch_[i].size();
        const int I = 2 * (l - 1);
        std::vector<double> betainit_global(I, 0.0);
        ForwardBackwardTraining(corpus_index_ch_[i], corpus_index_en_[i], betainit_global, 0);
        //没有执行到这里
        if (3 == loop) {
            HMMRealViterbi("/Users/wangql/Desktop/hmm_5.infer.26+491.align", corpus_index_ch_[i], corpus_index_en_[i],
                           betainit_global, 0);
        }
        AddAlCount_cnt_(corpus_index_ch_[i], corpus_index_en_[i]);
        Addai_cnt_bi_cnt_(l);
    }
    NormalizeTTableCntMy();
}

void IncreModelh::ConstructNetN() {
    int I = 2 * (kLongSentence - 1);
    int J = kLongSentence - 1;
    for (int i = 0; i != I; ++i) {
        net_n.push_back(std::vector<double>(J, 0.0));
    }
}

void
IncreModelh::InitNetN(std::vector<int> &v_ch, std::vector<int> &v_en, const int &cnt_line_ch, const int &cnt_line_en) {
    int I = 2 * (cnt_line_ch - 1);
    int J = cnt_line_en;
    double sum = 0.0;

    for (int j = 0; j != J; ++j) {
        sum = 0;
        for (int i = 0; i != I / 2; ++i) {
//            net_n[i][j] = std::max(new_table_t_[v_ch[i + 1]][v_en[j]], kProbSmooth);

            if ((new_table_t_.find(v_ch[i + 1]) != new_table_t_.end()) &&
                (new_table_t_[v_ch[i + 1]].find(v_en[j]) != new_table_t_[v_ch[i + 1]].end())) {
                net_n[i][j] = std::max(new_table_t_[v_ch[i + 1]][v_en[j]], kProbSmooth);
            } else {
                net_n[i][j] = kProbSmooth;
            }

            sum += net_n[i][j];
        }
        for (int i = I / 2; i != I; ++i) {
//            net_n[i][j] = std::max(new_table_t_[v_ch[0]][v_en[j]], kProbSmooth);
            net_n[i][j] = std::max(new_table_t_[v_ch[0]][v_en[j]], kProbSmooth);
            sum += net_n[i][j];
        }
        if (sum > 0.0) {
            for (int i = 0; i != I; ++i) {
                net_n[i][j] /= sum;
            }
        }
    }
}

void IncreModelh::ConstructNetE() {
    int I = 2 * (kLongSentence - 1);
    for (int i = 0; i != I; ++i) {
        net_e.push_back(std::vector<double>(I, 0.0));
    }
}

void IncreModelh::InitNetE(bool doInit, const int &cnt_line_ch) {
    int l = cnt_line_ch - 1;
    int I = 2 * l;
    double sum = 0.0;

    if (doInit) {//在增量里应该不会用到这个了
        for (int i = 0; i != I; ++i) {
            for (int j = 0; j != l; ++j) {
                net_e[i][j] = 1.0 / cnt_line_ch;
            }
            for (int j = l; j != I; ++j) {
                if (i % l == j % l) {
                    net_e[i][j] = 1.0 / cnt_line_ch;
                } else {
                    net_e[i][j] = 0.0;
                }
            }

        }
        for (int i = 0; i != I; ++i) {
            sum = 0.0;
            for (int j = 0; j != I; ++j) {
                sum += net_e[i][j];
            }
            for (int j = 0; j != I; ++j) {
                net_e[i][j] /= sum;
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
                net_e[i][j] = al[j % l];

                if (j >= l) {
                    if ((i % l) != (j % l)) {
                        net_e[i][j] = 0.0;
                    } else {
                        net_e[i][j] = kGLOBALProbabilityForEmpty;
                    }
                }
                sum += net_e[i][j];
            }
            if (sum) {
                for (int j = 0; j != I; ++j) {
                    net_e[i][j] /= sum;
                }
            } else {
                for (int j = 0; j != I; ++j) {
                    net_e[i][j] = 1.0 / l;
                }
            }
        }
    }

}

void IncreModelh::MakeHMMNetwork(std::vector<int> &v_ch, std::vector<int> &v_en, bool doInit) {
    int cnt_line_ch = (int) v_ch.size();
    int cnt_line_en = (int) v_en.size();

    InitNetE(doInit, cnt_line_ch);

    InitNetN(v_ch, v_en, cnt_line_ch, cnt_line_en);
}

void IncreModelh::ForwardBackwardTraining(std::vector<int> &v_ch, std::vector<int> &v_en,
                                          std::vector<double> &betainit_global, bool doInit) {

    const int I = 2 * ((int) v_ch.size() - 1);
    const int J = (int) v_en.size();
    const int N = I * J;

    std::vector<double> alphainit(I, 1.0);//真正的初始化在下面
    std::vector<double> betainit(I, 1.0);
    std::vector<double> alpha(N, 0.0);
    std::vector<double> beta(N, 0.0);

    if (doInit) {
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

        std::transform(betainit.begin(), betainit.end(), betainit.begin(), std::bind1st(std::multiplies<double>(), I));

    } else {
        double sum_alphainit = 0.0;
        for (int i = 0; i != I / 2 + 1; ++i) {
            alphainit[i] = ai_[I / 2][i];
            sum_alphainit += alphainit[i];
        }
        for (int i = I / 2 + 1; i != I; ++i) {
            alphainit[i] = 0.0;
        }
        for (int i = 0; i != I; ++i) {
            alphainit[i] /= sum_alphainit;
        }

        double sum_betainit = 0.0;
        for (int i = 0; i != I; ++i) {
            betainit[i] = bi_[I / 2][i];
            sum_betainit += betainit[i];
        }
        for (int i = 0; i != I; ++i) {
            betainit[i] /= sum_betainit;
        }

        std::transform(betainit.begin(), betainit.end(), betainit.begin(), std::bind1st(std::multiplies<double>(), I));

    }

    for (int i = 0; i != I; ++i) {
        beta[N - I + i] = betainit[i];
        betainit_global[i] = betainit[i];
    }

    int NN = N - I - 1;

    for (int j = J - 2; j >= 0; --j) {
        for (int ti = I - 1; ti >= 0; --ti, --NN) {
            auto next_beta = beta.begin() + (j + 1) * I;
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

    E_.resize(I * I);
    std::fill(E_.begin(), E_.end(), 0.0);

    for (int j = 1; j < J; ++j) {
        for (int ti = 0; ti < I; ++ti, ++cur_alpha, ++cur_beta) {
            auto prev_alpha = alpha.begin() + I * (j - 1);
            auto this_node = net_n[ti][j];

            for (int pi = 0; pi < I; ++pi, ++prev_alpha) {
                auto alpha_increment = *prev_alpha * net_e[pi][ti] * this_node;
                (*cur_alpha) += alpha_increment;
                E_[I * ti + pi] += alpha_increment * (*cur_beta);
            }
        }
    }
    gamma_.resize(N);
    std::transform(alpha.begin(), alpha.end(), beta.begin(), gamma_.begin(), std::multiplies<double>());

    auto ge = gamma_.end();
    for (auto gp = gamma_.begin(); gp != ge; gp += I) {
        double sum = 0.0;
        for (auto gval = gp; gval != gp + I; ++gval) {
            sum += *gval;
        }
        if (sum) {
            for (auto gval = gp; gval != gp + I; ++gval) {
                *gval /= sum;
            }
        } else {
            std::fill(gp, gp + I, 1.0 / I);
        }
    }

    auto gp = gamma_.begin();
    for (int j = 0; j != J; ++j) {
        for (int i = 0; i != I; ++i, ++gp) {
            if (*gp > kProbSmooth) {
                if (i >= I / 2) {
                    new_table_t_cnt_[v_ch[0]][v_en[j]] += *gp;
                } else {
                    new_table_t_cnt_[v_ch[i + 1]][v_en[j]] += *gp;
                }
            }
        }
    }
    double E_sum = 0.0;
    for (auto E_p = E_.begin(); E_p != E_.end(); ++E_p) {
        E_sum += *E_p;
    }
    if (E_sum) {
        for (auto E_p = E_.begin(); E_p != E_.end(); ++E_p) {
            *E_p /= E_sum;
        }
    } else {
        for (auto E_p = E_.begin(); E_p != E_.end(); ++E_p) {
            *E_p = 1.0 / (I * I);
        }
    }
}

void IncreModelh::AddAlCount_cnt_(std::vector<int> &v_ch, std::vector<int> &v_en) {
    const int l = (int) v_ch.size() - 1;
    const int I = 2 * l;
    auto ep = E_.begin();
    double mult = 1.0;
    mult *= l;
    int i_bef_real = 0;
    double np0c = 0.0;
    for (int i = 0; i != l; ++i) {
        for (int i_bef = 0; i_bef != I; ++i_bef, ++ep) {
            i_bef_real = (i_bef % l);
            AlCount_cnt_[i_bef_real - i] += *ep * mult;
        }
    }
}

void IncreModelh::Addai_cnt_bi_cnt_(const int &cnt_line_ch) {
    const int I = 2 * (cnt_line_ch - 1);
    auto gp1 = gamma_.begin();
    auto gp2 = gamma_.end() - I;
    for (int i = 0; i != I; ++i, ++gp1, ++gp2) {
        ai_cnt_[I / 2][i] += *gp1;
        bi_cnt_[I / 2][i] += *gp2;
    }
}

void IncreModelh::ai_cnt_2ai_() {
    for (auto i = ai_.begin(); i != ai_.end(); ++i) {
        ai_[i->first] = ai_cnt_[i->first];
        ai_cnt_[i->first] = std::vector<double>(2 * (i->first), 0.0);
    }
}

void IncreModelh::bi_cnt_2bi_() {
    for (auto i = bi_.begin(); i != bi_.end(); ++i) {
        bi_[i->first] = bi_cnt_[i->first];
        bi_cnt_[i->first] = std::vector<double>(2 * (i->first), 0.0);
    }
}

void IncreModelh::AlCount_cnt_2AlCount_() {
    for (auto i = AlCount_.begin(); i != AlCount_.end(); ++i) {
        AlCount_[i->first] = AlCount_cnt_[i->first];
        AlCount_cnt_[i->first] = 0.0;
    }
}

void IncreModelh::Constructai_() {
    for (int i = 1; i != kLongSentence; ++i) {
//        ai_[i] = std::vector<double>(2 * i, 0.0);
        ai_.insert({i, std::vector<double>(2 * i, 0.0)});
    }

}

void IncreModelh::Constructbi_() {
    for (int i = 1; i != kLongSentence; ++i) {
//        bi_[i] = std::vector<double>(2 * i, 0.0);
        bi_.insert({i, std::vector<double>(2 * i, 0.0)});
    }
}

void IncreModelh::Constructai_cnt_() {
    for (int i = 1; i != kLongSentence; ++i) {
//        ai_cnt_[i] = std::vector<double>(2 * i, 0.0);
        ai_cnt_.insert({i, std::vector<double>(2 * i, 0.0)});
    }
}

void IncreModelh::Constructbi_cnt_() {
    for (int i = 1; i != kLongSentence; ++i) {
//        bi_cnt_[i] = std::vector<double>(2 * i, 0.0);
        bi_cnt_.insert({i, std::vector<double>(2 * i, 0.0)});
    }
}

void IncreModelh::ConstructAlCount_() {
    AlCount_.insert({0, 0.0});
    for (int i = 1; i != kLongSentence; ++i) {
        AlCount_.insert({i, 0.0});
        AlCount_.insert({-i, 0.0});
    }
}

void IncreModelh::ConstructAlCount_cnt_() {
    AlCount_cnt_.insert({0, 0.0});
    for (int i = 1; i != kLongSentence; ++i) {
        AlCount_cnt_.insert({i, 0.0});
        AlCount_cnt_.insert({-i, 0.0});
    }
}

void IncreModelh::HMMRealViterbi(std::string output_file, std::vector<int> &v_ch, std::vector<int> &v_en,
                                 std::vector<double> &betainit_global, bool doInit) {
    const int l = (int) v_ch.size() - 1;
    const int I = 2 * l;
    const int J = (int) v_en.size();
    const int N = I * J;
    std::vector<double> alpha(N, -1);
    std::vector<double *> bp(N, (double *) 0);
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
        alpha[i] = alphainit[i] * net_n[i][0];
        if (i > l) {
            alpha[i] = 0;
        }
        bp[i] = 0;
    }

    auto cur_alpha = alpha.begin() + I;
    double **cur_bp = (&*bp.begin()) + I;
    for (int j = 1; j < J; ++j) {
        for (int ti = 0; ti != I; ++ti, ++cur_alpha, ++cur_bp) {
            double *prev_alpha = &*(alpha.begin() + I * (j - 1));
            double this_node = net_n[ti][j];
            for (int pi = 0; pi != I; ++pi, ++prev_alpha) {
                double alpha_increment = *prev_alpha * net_e[pi][ti] * this_node;
                if (alpha_increment > *cur_alpha) {
                    (*cur_alpha) = alpha_increment;
                    (*cur_bp) = prev_alpha;
                }
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

    viterbi_alignment_.resize(J + 1);
    for (int j = 1; j <= J; ++j) {
        viterbi_alignment_[j] = vit_[j - 1] + 1;
        if (viterbi_alignment_[j] > l) {
            viterbi_alignment_[j] = 0;
        }
    }

    std::ofstream fout(output_file, std::ios_base::app);//接着写
    bool firstword = true;
    for (int j = 1; j <= J; ++j) {
        if (viterbi_alignment_[j]) {
            if (firstword) {
                firstword = false;
            } else {
                fout << " ";
            }
            fout << viterbi_alignment_[j] - 1 << "-" << j - 1;
        }
    }
    fout << std::endl;
}


void IncreModelh::ReadBackgroundAlCount_(std::string filename) {
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
}

void IncreModelh::ReadBackgroundai_(std::string filename) {
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
    fin.close();
}

void IncreModelh::ReadBackgroundbi_(std::string filename) {
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
}

void IncreModelh::PrintAlCount_(std::string output_file) {
    std::ofstream fout(output_file);
    for (auto it = AlCount_.begin(); it != AlCount_.end(); ++it) {
        fout << it->first << " " << it->second << std::endl;
    }
    fout.close();
}

void IncreModelh::Printai_(std::string output_file) {
    std::ofstream fout(output_file);
    for (auto it1 = ai_.begin(); it1 != ai_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            fout << it1->first << " " << *it2 << std::endl;
        }
    }
    fout.close();
}

void IncreModelh::Printbi_(std::string output_file) {
    std::ofstream fout(output_file);
    for (auto it1 = bi_.begin(); it1 != bi_.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            fout << it1->first << " " << *it2 << std::endl;
        }
    }
    fout.close();
}







