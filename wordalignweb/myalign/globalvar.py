import ctypes
from ctypes import *
import collections

def _init():
    global lib_global
    lib_global = ctypes.cdll.LoadLibrary("libgenalignpycall.so")


    s1_global = "../26491/merged.26.ch.vcb" #read path
    s4_global = "../26491/merged.26.en.vcb" #read path
    s7_global = "../26491/merged.26.ch.vcb" #read path
    s8_global = "../26491/merged.26.en.vcb" #read path
    s9_global = "../26491/merged.26.modelh.ch.en.t" #read path
    s12_global = "../26491/merged.26.modelh.ch.en.AlCount" #read path
    s13_global = "../26491/merged.26.modelh.ch.en.ai" #read path
    s14_global = "../26491/merged.26.modelh.ch.en.bi" #read path

    # s1_global = "/Users/wangql/wordaligntest/corpus.ch.vcb" #read path
    # s4_global = "/Users/wangql/wordaligntest/corpus.en.vcb" #read path
    # s7_global = "/Users/wangql/wordaligntest/corpus.ch.vcb" #read path
    # s8_global = "/Users/wangql/wordaligntest/corpus.en.vcb" #read path
    # s9_global = "/Users/wangql/wordaligntest/modelh.ch.en.t" #read path
    # s12_global = "/Users/wangql/wordaligntest/modelh.ch.en.AlCount" #read path
    # s13_global = "/Users/wangql/wordaligntest/modelh.ch.en.ai" #read path
    # s14_global = "/Users/wangql/wordaligntest/modelh.ch.en.bi" #read path

    bs1_global = s1_global.encode("utf-8")
    bs4_global = s4_global.encode("utf-8")
    bs7_global = s7_global.encode("utf-8")
    bs8_global = s8_global.encode("utf-8")
    bs9_global = s9_global.encode("utf-8")
    bs12_global = s12_global.encode("utf-8")
    bs13_global = s13_global.encode("utf-8")
    bs14_global = s14_global.encode("utf-8")
    

    lib_global.ReadFileCh(bs1_global)
    lib_global.ReadFileEn(bs4_global)
    lib_global.ReadMyVcbCh(bs7_global)
    lib_global.ReadMyVcbEn(bs8_global)
    lib_global.ReadTTable(bs9_global)
    lib_global.ConstructAlCount_()
    lib_global.ReadBackgroundAlCount_(bs12_global)
    lib_global.ReadBackgroundai_(bs13_global)
    lib_global.ReadBackgroundbi_(bs14_global)


    global lib_global_prior
    lib_global_prior = ctypes.cdll.LoadLibrary("libgenalignpycallloo.so")


    s1_global_prior = "../26491/merged.26.ch.vcb" #read path
    s4_global_prior = "../26491/merged.26.en.vcb" #read path
    s7_global_prior = "../26491/merged.26.ch.vcb" #read path
    s8_global_prior = "../26491/merged.26.en.vcb" #read path
    s9_global_prior = "../26491/merged.26.modelh.ch.en.loo.t" #read path
    s12_global_prior = "../26491/merged.26.modelh.ch.en.loo.AlCount" #read path
    s13_global_prior = "../26491/merged.26.modelh.ch.en.loo.ai" #read path
    s14_global_prior = "../26491/merged.26.modelh.ch.en.loo.bi" #read path

    # s1_global_prior = "/Users/wangql/wordaligntest/corpus.ch.vcb" #read path
    # s4_global_prior = "/Users/wangql/wordaligntest/corpus.en.vcb" #read path
    # s7_global_prior = "/Users/wangql/wordaligntest/corpus.ch.vcb" #read path
    # s8_global_prior = "/Users/wangql/wordaligntest/corpus.en.vcb" #read path
    # s9_global_prior = "/Users/wangql/wordaligntest/modelh.ch.en.t" #read path
    # s12_global_prior = "/Users/wangql/wordaligntest/modelh.ch.en.AlCount" #read path
    # s13_global_prior = "/Users/wangql/wordaligntest/modelh.ch.en.ai" #read path
    # s14_global_prior = "/Users/wangql/wordaligntest/modelh.ch.en.bi" #read path

    bs1_global_prior = s1_global_prior.encode("utf-8")
    bs4_global_prior = s4_global_prior.encode("utf-8")
    bs7_global_prior = s7_global_prior.encode("utf-8")
    bs8_global_prior = s8_global_prior.encode("utf-8")
    bs9_global_prior = s9_global_prior.encode("utf-8")
    bs12_global_prior = s12_global_prior.encode("utf-8")
    bs13_global_prior = s13_global_prior.encode("utf-8")
    bs14_global_prior = s14_global_prior.encode("utf-8")
    

    lib_global_prior.ReadFileCh(bs1_global_prior)
    lib_global_prior.ReadFileEn(bs4_global_prior)
    lib_global_prior.ReadMyVcbCh(bs7_global_prior)
    lib_global_prior.ReadMyVcbEn(bs8_global_prior)
    lib_global_prior.ReadTTable(bs9_global_prior)
    lib_global_prior.ConstructAlCount_()
    lib_global_prior.ReadBackgroundAlCount_(bs12_global_prior)
    lib_global_prior.ReadBackgroundai_(bs13_global_prior)
    lib_global_prior.ReadBackgroundbi_(bs14_global_prior)

    global lib_global_inter
    lib_global_inter = ctypes.cdll.LoadLibrary("libgenalignpycallinter.so")

    s1_global_inter = "../134w/modelh.zh.shuf.vcb"  # read path
    s4_global_inter = "../134w/modelh.en.shuf.vcb"  # read path
    s7_global_inter = "../134w/modelh.zh.shuf.vcb"  # read path
    s8_global_inter = "../134w/modelh.en.shuf.vcb"  # read path
    s9_global_inter = "../134w/modelh.zh.en.shuf.t"  # read path
    s12_global_inter = "../134w/modelh.zh.en.shuf.AlCount"  # read path
    s13_global_inter = "../134w/modelh.zh.en.shuf.ai"  # read path
    s14_global_inter = "../134w/modelh.zh.en.shuf.bi"  # read path

    bs1_global_inter = s1_global_inter.encode("utf-8")
    bs4_global_inter = s4_global_inter.encode("utf-8")
    bs7_global_inter = s7_global_inter.encode("utf-8")
    bs8_global_inter = s8_global_inter.encode("utf-8")
    bs9_global_inter = s9_global_inter.encode("utf-8")
    bs12_global_inter = s12_global_inter.encode("utf-8")
    bs13_global_inter = s13_global_inter.encode("utf-8")
    bs14_global_inter = s14_global_inter.encode("utf-8")

    lib_global_inter.ReadFileCh(bs1_global_inter)
    lib_global_inter.ReadFileEn(bs4_global_inter)
    lib_global_inter.ReadMyVcbCh(bs7_global_inter)
    lib_global_inter.ReadMyVcbEn(bs8_global_inter)
    lib_global_inter.ReadTTable(bs9_global_inter)
    lib_global_inter.ConstructAlCount_()
    lib_global_inter.ReadBackgroundAlCount_(bs12_global_inter)
    lib_global_inter.ReadBackgroundai_(bs13_global_inter)
    lib_global_inter.ReadBackgroundbi_(bs14_global_inter)


    global en_zh_dict
    en_zh_dict = collections.defaultdict(set)
    with open("defs.nopos.hand.txt", 'r', encoding='utf-8') as f1:
        for line in f1.readlines():
            wordlist = line.split('\t')
            en = wordlist[0].strip()
            zh_list = wordlist[1].strip().split()
            for zh in zh_list:
                en_zh_dict[en].add(zh)