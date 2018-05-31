#include <iostream>

#include "incre_model1.h"

#include "incre_model2.h"

#include "incre_modelh.h"

void model1() {
    IncreModel1 increModel1;

    increModel1.ReadBackgroundVocab(
            "../data/16/r.corpus.h16.en.vcb", true);
    increModel1.ReadBackgroundVocab(
            "../data/16/r.corpus.h16.ch.vcb", false);
    increModel1.ReadBackgroundTTable(
            "../data/16/r.corpus.model1.ori.h16.en.ch.t");

//    increModel1.ReadBackgroundCorpus("/Users/wangql/windows/119/16/corpus.h16.ch", true);
//    increModel1.ReadBackgroundCorpus("/Users/wangql/windows/119/16/corpus.h16.en", false);

    increModel1.ReadCorpus("../data/10491/corpus.t10491.en", true);
    increModel1.ReadCorpus("../data/10491/corpus.t10491.ch", false);

    increModel1.ConstrucNewTTable();
    increModel1.ConstrucNewTTableCnt();

    increModel1.CalcuVocabNum(true);
    increModel1.CalcuVocabNum(false);

    std::vector<int> n_ch;
    std::vector<int> n_en;

    for (int it = 1; it < 6; ++it) {
        increModel1.EMLoop(it);
//        increModel1.NormalizeTTableCnt(n_ch, n_en);
        increModel1.NormalizeTTableCntMy();
        increModel1.NewTableCnt2NewTableT();
    }

    increModel1.PrintVocab("../data/10491/r.corpus.model1.incre.en.vcb", true);
    increModel1.PrintVocab("../data/10491/r.corpus.model1.incre.ch.vcb", false);

    increModel1.Interpolation2T(0.5);//这里是1，写0得到的数太差了,我猜这里写0太差的原因可能是忘了我这里仅仅保存了新的概率表

    increModel1.PrintTTable("../data/10491/r.corpus.model1.incre.en.ch.t");
}

void model2() {

    time_t start, stop;

    IncreModel2 increModel2;

    increModel2.ConstructATable();

    increModel2.ConstructATableCnt();

    increModel2.ReadBackgroundVocab("../data/16/r.corpus.h16.en.vcb", true);
    increModel2.ReadBackgroundVocab("../data/16/r.corpus.h16.ch.vcb", false);
    increModel2.ReadBackgroundTTable("../data/16/r.corpus.model2.h16.en.ch.t");
    increModel2.ReadBackgroundATable("../data/16/r.corpus.model2.h16.en.ch.a");
//    increModel2.ReadBackgroundMyATable("/Users/wangql/Desktop/incre/s2t64.incre.a2.h5.5");


    increModel2.ReadCorpus("../data/10491/corpus.t10491.en", true);
    increModel2.ReadCorpus("../data/10491/corpus.t10491.ch", false);


//    increModel2.ReadBackgroundVocab("/home/wangql/incremodel/corpus.h16.ch.vcb", true);
//    increModel2.ReadBackgroundVocab("/home/wangql/incremodel/corpus.h16.en.vcb", false);
//    increModel2.ReadBackgroundTTable("/home/wangql/incremodel/s2t64.t2.5");
//    increModel2.ReadBackgroundATable("/home/wangql/incremodel/s2t64.a2.5");
//
//
//    increModel2.ReadCorpus("/home/wangql/incremodel/corpus.t10.ch", true);
//    increModel2.ReadCorpus("/home/wangql/incremodel/corpus.t10.en", false);

    increModel2.ConstrucNewTTable();
    increModel2.ConstrucNewTTableCnt();


    increModel2.CalcuVocabNum(true);
    increModel2.CalcuVocabNum(false);

    std::vector<int> n_ch;
    std::vector<int> n_en;

    start = time(NULL);
    for (int it = 1; it < 6; ++it) {
        increModel2.EMLoop(it);
//        increModel2.NormalizeTTableCnt(n_ch, n_en);
        increModel2.NormalizeTTableCntMy();
        increModel2.NewTableCnt2NewTableT();
        increModel2.NormalizeATableCnt();
        increModel2.ATableCnt2ATable();
    }

    stop = time(NULL);
    printf("incremodel2 use time:%ld\n", (stop - start));

    increModel2.PrintVocab("../data/10491/r.corpus.model2.incre.en.vcb", true);
    increModel2.PrintVocab("../data/10491/r.corpus.model2.incre.ch.vcb", false);

    increModel2.Interpolation2T(0.5);//参数表示对给原有概率表的权重

    increModel2.PrintTTable("../data/10491/r.corpus.model2.incre.en.ch.t");
    increModel2.PrintATable("../data/10491/r.corpus.model2.incre.en.ch.a");

//    increModel2.PrintVocab("/home/wangql/incremodel/corpus.incre.ch.vcb", true);
//    increModel2.PrintVocab("/home/wangql/incremodel/corpus.incre.en.vcb", false);
//
//    increModel2.Interpolation2T(0);//参数表示对给原有概率表的权重
//
//    increModel2.PrintTTable("/home/wangql/incremodel/s2t64.incre.t2.5");
//    increModel2.PrintATable("/home/wangql/incremodel/s2t64.incre.a2.5");
}

void modelh() {
    time_t start, stop;
    IncreModelh increModelh;


    increModelh.ReadBackgroundVocab("../data/16/corpus.h16.ch.vcb", true);
    increModelh.ReadBackgroundVocab("../data/16/corpus.h16.en.vcb", false);

    increModelh.ReadBackgroundTTable("../data/16/corpus.modelh.h16.ch.en.t");


    increModelh.ConstructAlCount_();
    increModelh.ConstructAlCount_cnt_();

//    increModelh.Constructai_();//这个只有在不从文件中读入时才需要构造
    increModelh.Constructai_cnt_();

//    increModelh.Constructbi_();//这个只有在不从文件中读入时才需要构造
    increModelh.Constructbi_cnt_();

    increModelh.ReadBackgroundAlCount_("../data/16/corpus.modelh.h16.ch.en.AlCount");
    increModelh.ReadBackgroundai_("../data/16/corpus.modelh.h16.ch.en.ai");
    increModelh.ReadBackgroundbi_("../data/16/corpus.modelh.h16.ch.en.bi");

    increModelh.ReadCorpus("../data/10491/corpus.t10491.ch", true);
    increModelh.ReadCorpus("../data/10491/corpus.t10491.en", false);

    increModelh.PrintVocab("../data/10491/corpus.modelh.incre.ch.vcb", true);
    increModelh.PrintVocab("../data/10491/corpus.modelh.incre.en.vcb", false);

    increModelh.ConstrucNewTTable();
    increModelh.ConstrucNewTTableCnt();

    increModelh.CalcuVocabNum(true);
    increModelh.CalcuVocabNum(false);

    increModelh.ConstructNetE();
    increModelh.ConstructNetN();

//    increModelh.EMLoop(4);

    start = time(NULL);
    increModelh.EMLoop(0);
    //需要接着实验下面的步骤

    for (int loop = 0; loop != 4; ++loop) {
        increModelh.NewTableCnt2NewTableT();
        increModelh.AlCount_cnt_2AlCount_();
        increModelh.ai_cnt_2ai_();
        increModelh.bi_cnt_2bi_();

        if (3 == loop) {
            stop = time(NULL);
            printf("increhmm use time:%ld\n", (stop - start));
            increModelh.Interpolation2T(0.5);
            increModelh.PrintTTable("../data/10491/corpus.modelh.incre.ch.en.t");
            increModelh.PrintAlCount_("../data/10491/corpus.modelh.incre.ch.en.AlCount");
            increModelh.Printai_("../data/10491/corpus.modelh.incre.ch.en.ai");
            increModelh.Printbi_("../data/10491/corpus.modelh.incre.ch.en.bi");
        }

        increModelh.EMLoop(loop);
    }
}


int main() {

//    model1();
//    model2();
    modelh();

    return 0;
}