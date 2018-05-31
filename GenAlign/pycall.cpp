//
// Created by 王青龙 on 2018/1/31.
//

#include "pre_process.h"
//#include "distance.h"
#include "infer_align.h"
#include "infer_align2.h"
#include "infer_alignh.h"
//#include "vocab.h"
#include "two_vocab_infer_align.h"
#include "two_vocab_infer_align.h"
#include "two_index_infer_align.h"

extern "C" {

PrePro p_ch;

void ReadFileCh(std::string inputfile) {
    p_ch.readFile(inputfile);
}
void Sentence2IndexCh(std::string s, std::string inputfile) {
    p_ch.sentence2Index(s, inputfile);
}

PrePro p_en;

void ReadFileEn(std::string inputfile) {
    p_en.readFile(inputfile);
}
void Sentence2IndexEn(std::string s, std::string inputfile) {
    p_en.sentence2Index(s, inputfile);
}

InferAlignh iah;//modelh推理

void ReadMyVcbCh(std::string inputfile) {
    iah.ReadMyVcbCh(inputfile);
}

void ReadMyVcbEn(std::string inputfile) {
    iah.ReadMyVcbEn(inputfile);
}

void ReadTTable(std::string inputfile) {
    iah.ReadTTable(inputfile);
}

void ReadCorpusIndexCh(std::string inputfile) {
    iah.ReadCorpusIndexCh(inputfile);
}

void ReadCorpusIndexEn(std::string inputfile) {
    iah.ReadCorpusIndexEn(inputfile);
}

void ConstructAlCount_() {
    iah.ConstructAlCount_();
}

void ReadBackgroundAlCount_(std::string inputfile) {
    iah.ReadBackgroundAlCount_(inputfile);
}

void ReadBackgroundai_(std::string inputfile) {
    iah.ReadBackgroundai_(inputfile);
}

void ReadBackgroundbi_(std::string inputfile) {
    iah.ReadBackgroundbi_(inputfile);
}

void ConstructNetN() {
    iah.ConstructNetN();
}

void ConstructNetE() {
    iah.ConstructNetE();
}

void EMLoop(std::string inputfile) {
    iah.EMLoop(inputfile);
}
}