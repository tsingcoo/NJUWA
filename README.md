# NJUWA

NJUWA is a general purpose expectation-maximization tool in C++ while word alignment is the main target task.

## Introduction

With the standardization of word alignment, an important task for researchers and research institutes in the direction of natural language processing is to develop an open source baseline system that can be learned and easily expanded by future researchers. Just like Gizapp, it is the most widely accepted open source system based on IBM model.  Nowadays, there are some open source codes with word alignment in github open source community, such as fastalign, anymalign, gizapp, Berkeley aligner, etc. These codes are mainly based on Linux platform and have been successfully applied to academic research.  However, the expansibility and usability of these systems are not good. take gizapp system as an example, this tool can only be used on Linux platform. although some researchers are currently expanding on windows platform, the usability is very poor, which greatly slows down the progress of scientific research.  In addition, due to the early development time of the code, C++ did not support some advanced structures in the early days. there are many data structures customized by authors in the code, which makes the code extremely readable. if users want to change the structure of the model, it will become very difficult.  Therefore, this chapter implements a word alignment open source system NJUWA based on C++11. 

## Requirements

- `c++11`

- `python (>=3.6)`

- `django (>=2.0)`

- `jieba (>=0.39)`

- `nltk (>=1.11)`

## Acknowledgments

The implementation is inspired by the following:

- [The Mathematics of Statistical Machine Translation: Parameter Estimation](http://www.aclweb.org/anthology/J93-2003)

- [HMM-Based Word Alignment in Statistical Translation](http://www.aclweb.org/anthology/C96-2141)

- [GIZA++: Training of statistical translation models.](http://www.fjoch.com/GIZA++.html)

- [Incremental Training and Intentional Over-fitting of Word Alignment](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/MT-Summit_Alignment.pdf)

- [Leave-one-out Word Alignment without Garbage Collector Effects](https://pdfs.semanticscholar.org/3ad5/aea3a98dbf00eb8800816040dbee88572f72.pdf)

