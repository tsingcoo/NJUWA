//
// Created by 王青龙 on 2018/1/31.
//

#include "infer_alignh.h"
#include "pre_process.h"


extern "C" {

PrePro p_ch;

int ReadFileCh(char* inputfile) {
    return p_ch.readFile(inputfile);
}
int Sentence2IndexCh(char* s, char* inputfile) {
    return p_ch.sentence2Index(s, inputfile);
}

PrePro p_en;

int ReadFileEn(char* inputfile) {
    return p_en.readFile(inputfile);
}
int Sentence2IndexEn(char* s, char* inputfile) {
    return p_en.sentence2Index(s, inputfile);
}

InferAlignh iah;//modelh推理

int ReadMyVcbCh(char* inputfile) {
    return iah.ReadMyVcbCh(inputfile);
}

int ReadMyVcbEn(char* inputfile) {
    return iah.ReadMyVcbEn(inputfile);
}

int ReadTTable(char* inputfile) {
    return iah.ReadTTable(inputfile);
}

int ReadCorpusIndexCh(char* inputfile) {
    return iah.ReadCorpusIndexCh(inputfile);
}

int ReadCorpusIndexEn(char* inputfile) {
    return iah.ReadCorpusIndexEn(inputfile);
}

int ConstructAlCount_() {
    return iah.ConstructAlCount_();
}

int ReadBackgroundAlCount_(char* inputfile) {
    return iah.ReadBackgroundAlCount_(inputfile);
}

int ReadBackgroundai_(char* inputfile) {
    return iah.ReadBackgroundai_(inputfile);
}

int ReadBackgroundbi_(char* inputfile) {
    return iah.ReadBackgroundbi_(inputfile);
}

void ConstructNetN() {
    iah.ConstructNetN();
}

void ConstructNetE() {
    iah.ConstructNetE();
}

const char* EMLoop(char* inputfile) {
    return iah.EMLoop(inputfile);
}

}