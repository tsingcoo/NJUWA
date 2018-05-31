#include "pre_process.h"
#include "distance.h"
#include "infer_align.h"
#include "infer_align2.h"
#include "infer_alignh.h"
#include "vocab.h"
#include "two_vocab_infer_align.h"
#include "two_vocab_infer_align.h"
#include "two_index_infer_align.h"

#include <iostream>
#include <string>
#include <cstring>
#include<sstream>


std::vector<std::string> ReadConfig(std::string input_file) {
    std::ifstream fin(input_file);
    std::string line;
    std::string path;
    std::vector<std::string> vs;
    while (getline(fin, line)) {
        std::istringstream iss(line);
        iss >> path;
        vs.push_back(path);
    }
    return vs;
}

int main(int argc, const char* argv[]) {

//    std::cout << "请输入推理的模型代码(1:model1  2:model2  h:modelh)以及参数文件的路径：" << std::endl;

    std::vector<std::string> vs = ReadConfig(argv[2]);

    std::cout << "模型：" << argv[1] << std::endl;

    if (0 == strcmp("1", argv[1])) {

        PrePro p_ch;
        p_ch.readFile(vs[0]);
        p_ch.corpus2Index(vs[1], vs[2]);

        PrePro p_en;
        p_en.readFile(vs[3]);
        p_en.corpus2Index(vs[4], vs[5]);

        std::cout<<"ll"<<std::endl;

        InferAlign ia;//model1推理
//        ia.ReadGizaVcbCh(vs[6]);
//        ia.ReadGizaVcbEn(vs[7]);

        ia.ReadMyVcbCh(vs[6]);
        ia.ReadMyVcbEn(vs[7]);
        ia.ReadTTable(vs[8]);
        ia.ReadCorpusIndexCh(vs[9]);
        ia.ReadCorpusIndexEn(vs[10]);
        ia.OutputAlign(vs[11]);

        std::cout<<"ll"<<std::endl;

    } else if (0 == strcmp("2", argv[1])) {

        PrePro p_ch;
        p_ch.readFile(vs[0]);
        p_ch.corpus2Index(vs[1], vs[2]);

        PrePro p_en;
        p_en.readFile(vs[3]);
        p_en.corpus2Index(vs[4], vs[5]);

        InferAlign2 ia2;//model2推理
//        ia2.ReadGizaVcbCh(vs[6]);
//        ia2.ReadGizaVcbEn(vs[7]);
        ia2.ReadMyVcbCh(vs[6]);
        ia2.ReadMyVcbEn(vs[7]);
        ia2.ReadTTable(vs[8]);
        ia2.ReadMyATable(vs[9]);
        ia2.ReadCorpusIndexCh(vs[10]);
        ia2.ReadCorpusIndexEn(vs[11]);
        ia2.OutputAlign(vs[12]);

    } else if (0 == strcmp("h", argv[1])) {

        PrePro p_ch;
        p_ch.readFile(vs[0]);
        p_ch.corpus2Index(vs[1], vs[2]);

        PrePro p_en;
        p_en.readFile(vs[3]);
        p_en.corpus2Index(vs[4], vs[5]);

        InferAlignh iah;//modelh推理
//        iah.ReadGizaVcbCh(vs[6]);
//        iah.ReadGizaVcbEn(vs[7]);
        iah.ReadMyVcbCh(vs[6]);
        iah.ReadMyVcbEn(vs[7]);
        iah.ReadTTable(vs[8]);
        iah.ReadCorpusIndexCh(vs[9]);
        iah.ReadCorpusIndexEn(vs[10]);
        iah.ConstructAlCount_();
        iah.ReadBackgroundAlCount_(vs[11]);
        iah.ReadBackgroundai_(vs[12]);
        iah.ReadBackgroundbi_(vs[13]);
        iah.ConstructNetN();
        iah.ConstructNetE();
        iah.EMLoop(vs[14]);

    } else if (0 == strcmp("y", argv[1])) {//演示用

        std::cout << "输入中文：" << std::endl;
        std::string s_ch;
        std::getline(std::cin, s_ch);

        std::cout << "输入英文：" << std::endl;
        std::string s_en;
        std::getline(std::cin, s_en);


        PrePro p_ch;
        p_ch.readFile(vs[0]);
        p_ch.sentence2Index(s_ch, vs[2]);

        PrePro p_en;
        p_en.readFile(vs[3]);
        p_en.sentence2Index(s_en, vs[5]);

        InferAlignh iah;//modelh推理
//        iah.ReadGizaVcbCh(vs[6]);
//        iah.ReadGizaVcbEn(vs[7]);
        iah.ReadMyVcbCh(vs[6]);
        iah.ReadMyVcbEn(vs[7]);
        iah.ReadTTable(vs[8]);
        iah.ReadCorpusIndexCh(vs[9]);
        iah.ReadCorpusIndexEn(vs[10]);
        iah.ConstructAlCount_();
        iah.ReadBackgroundAlCount_(vs[11]);
        iah.ReadBackgroundai_(vs[12]);
        iah.ReadBackgroundbi_(vs[13]);
        iah.ConstructNetN();
        iah.ConstructNetE();
        iah.EMLoop(vs[14]);
    } else if( 0 == strcmp("e", argv[1])){
        PrePro p_en;
//        p_en.readFile(vs[0]);
//        p_en.wordDis2IndexDis(vs[1], vs[2]);
        p_en.ReadTTableOri("../data/vec4align/corpus.modelh.ch.en.t");
        p_en.ReadIndexDis("../data/vec4align/corpus.26.en.dis.index.vec");
        p_en.ConstructTTableVec();
        p_en.PrintTTableVec("../data/vec4align/corpus.modelh.ch.en.dis.mean.t");


//        PrePro p_ch;
//        p_ch.readFile(vs[0]);
//        p_ch.corpus2Index(vs[1], vs[2]);
//
//        PrePro p_en;
//        p_en.readFile(vs[3]);
//        p_en.corpus2Index(vs[4], vs[5]);
//
//
//        InferAlignh iah;
//        iah.ReadWordDis("/Users/wangql/Windows/corpus.en.index.vec");



    }
    else {
        std::cout << "输入的模型代码有误！" << std::endl;
    }

    return 0;
}