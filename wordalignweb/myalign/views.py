

from . import globalvar as gl

from wordcloud import WordCloud

from django.http import JsonResponse
from django.shortcuts import render

from ctypes import *

import jieba

import nltk
from nltk.tokenize import word_tokenize

jieba.load_userdict("corpus.ch.dict.txt")
jieba.del_word('计算机犯罪')
jieba.del_word('数十亿美元')
jieba.del_word('数十')
jieba.del_word('十亿美元')
jieba.del_word('官方语言')
jieba.del_word('传奇人物')
jieba.del_word('基本定律')
jieba.del_word('需在')
jieba.del_word('出生日期')
jieba.del_word('南京大学')
jieba.del_word('201')
jieba.suggest_freq(('柏柏尔语', '数十亿','2018'), True)


def index(request):
    return render(request, 'myalign/ajaxhome.html')


def batchPage(request):
    return render(request, 'myalign/ajaxhome2.html')

def interPage(request):
    return render(request, 'myalign/ajaxhome3.html')


def add(request):
    a_ori = request.GET['a']
    b_ori = request.GET['b']

    zh_list = jieba.cut(a_ori)
    a = ' '.join(zh_list)
    zh_list = a.split()
    # a = a_ori
    b = b_ori.lower()
    en_list = b.split()

    dict_align = ""
    firstalign = True
    for j in range(len(en_list)):
        if en_list[j] in gl.en_zh_dict:
            zh_word_set = gl.en_zh_dict[en_list[j]]
            zh_target = [i for i, x in enumerate(zh_list) if x in zh_word_set]
            for i in zh_target:
                if firstalign:
                    firstalign = False
                else:
                    dict_align += " "
                dict_align += str(i)
                dict_align += "-"
                dict_align += str(j)
    print("词典读完")

    s2 = a
    s3 = "../26491/merged.26.ch.index"  # write path,s10需跟这个保持一致
    s5 = b
    s6 = "../26491/merged.26.en.index"  # write path,s11需跟这个保持一致
    s10 = "../26491/merged.26.ch.index"
    s11 = "../26491/merged.26.en.index"
    s15 = "../26491/merged.26.ch.en.align"

    bs2 = s2.encode("utf-8")
    bs3 = s3.encode("utf-8")
    bs5 = s5.encode("utf-8")
    bs6 = s6.encode("utf-8")
    bs10 = s10.encode("utf-8")
    bs11 = s11.encode("utf-8")
    bs15 = s15.encode("utf-8")

    gl.lib_global.Sentence2IndexCh(bs2, bs3)
    gl.lib_global.Sentence2IndexEn(bs5, bs6)
    gl.lib_global.ReadCorpusIndexCh(bs10)
    gl.lib_global.ReadCorpusIndexEn(bs11)
    gl.lib_global.ConstructNetN()
    gl.lib_global.ConstructNetE()
    gl.lib_global.EMLoop.restype = c_char_p
    align = gl.lib_global.EMLoop(bs15)

    s2_prior = a
    s3_prior = "../26491/merged.26.ch.prior.index"  # write path,s10需跟这个保持一致
    s5_prior = b
    s6_prior = "../26491/merged.26.en.prior.index"  # write path,s11需跟这个保持一致
    s10_prior = "../26491/merged.26.ch.prior.index"
    s11_prior = "../26491/merged.26.en.prior.index"
    s15_prior = "../26491/merged.26.ch.en.prior.align"

    bs2_prior = s2_prior.encode("utf-8")
    bs3_prior = s3_prior.encode("utf-8")
    bs5_prior = s5_prior.encode("utf-8")
    bs6_prior = s6_prior.encode("utf-8")
    bs10_prior = s10_prior.encode("utf-8")
    bs11_prior = s11_prior.encode("utf-8")
    bs15_prior = s15_prior.encode("utf-8")

    gl.lib_global_prior.Sentence2IndexCh(bs2_prior, bs3_prior)
    gl.lib_global_prior.Sentence2IndexEn(bs5_prior, bs6_prior)
    gl.lib_global_prior.ReadCorpusIndexCh(bs10_prior)
    gl.lib_global_prior.ReadCorpusIndexEn(bs11_prior)
    gl.lib_global_prior.ConstructNetN()
    gl.lib_global_prior.ConstructNetE()
    gl.lib_global_prior.EMLoop.restype = c_char_p
    align_prior = gl.lib_global_prior.EMLoop(bs15_prior)

    dict_align_list = dict_align.split()
    align_prior_list = align_prior.decode('utf-8').split()

    deleted_ap_list = []

    for da in dict_align_list:
        rda = da.split('-')[1]
        for ap in align_prior_list:
            rap = ap.split('-')[1]
            if rda == rap:
                deleted_ap_list.append(str(ap))
    print("align_prior")
    all_ap = ""
    for ap in align_prior_list:
        all_ap += ap
        all_ap += " "
    print(all_ap)
    print("deleted_ap_list")
    all_dap = ""
    for dap in deleted_ap_list:
        all_dap += dap
        all_dap += " "
    print(all_dap)
    print("united_align_list")
    united_align_list = list(set(align_prior_list) - set(deleted_ap_list))
    all_u = ""
    for u in united_align_list:
        all_u += u
        all_u += " "
    print(all_u)

    united_align = ""
    firstalign = True
    for i in range(len(united_align_list)):
        if firstalign:
            firstalign = False
        else:
            united_align += " "
        united_align += united_align_list[i]

    for i in range(len(dict_align_list)):
        united_align += " "
        united_align += dict_align_list[i]

    res_dict = {'ch': a, 'en': b_ori, 'align': align.decode('utf-8'), 'align_prior': united_align}
    return JsonResponse(res_dict)


def batchadd(request):
    ch_ori = request.GET['ch']
    en_ori = request.GET['en']

    ch_ori_list = ch_ori.split('\n')
    en_ori_list = en_ori.split('\n')
    print(len(ch_ori_list))
    print(len(en_ori_list))
    if len(ch_ori_list) != len(en_ori_list):
        res_dict = {'batchalign': 'the lines of chinese and english is inequal.'}
        return JsonResponse(res_dict)

    batchalign = ""
    cloud_text_zh = ""
    cloud_text_en = ""
    for i in range(len(ch_ori_list)):
        a_ori = ch_ori_list[i]
        b_ori = en_ori_list[i]
        zh_list = jieba.cut(a_ori)
        a = ' '.join(zh_list)
        zh_list = a.split()
        # a = a_ori
        b = b_ori.lower()
        en_list = b.split()


        cloud_text_zh += ','.join(zh_list)
        cloud_text_zh += ','
        cloud_text_en += ','.join(en_list)
        cloud_text_en += ','

        s2 = a
        s3 = "../26491/merged.26.ch.index"  # write path,s10需跟这个保持一致
        s5 = b
        s6 = "../26491/merged.26.en.index"  # write path,s11需跟这个保持一致
        s10 = "../26491/merged.26.ch.index"
        s11 = "../26491/merged.26.en.index"
        s15 = "../26491/merged.26.ch.en.align"

        bs2 = s2.encode("utf-8")
        bs3 = s3.encode("utf-8")
        bs5 = s5.encode("utf-8")
        bs6 = s6.encode("utf-8")
        bs10 = s10.encode("utf-8")
        bs11 = s11.encode("utf-8")
        bs15 = s15.encode("utf-8")

        gl.lib_global.Sentence2IndexCh(bs2, bs3)
        gl.lib_global.Sentence2IndexEn(bs5, bs6)
        gl.lib_global.ReadCorpusIndexCh(bs10)
        gl.lib_global.ReadCorpusIndexEn(bs11)
        gl.lib_global.ConstructNetN()
        gl.lib_global.ConstructNetE()
        gl.lib_global.EMLoop.restype = c_char_p
        align = gl.lib_global.EMLoop(bs15).decode('utf-8')

        batchalign += align
        batchalign += '<br/>'
        batchalign += '<br/>'


    wc = WordCloud(
        background_color="white",  # 背景颜色
        max_words=200,  # 显示最大词数
        font_path="AdobeFangsongStd-Regular.otf",  # 使用字体
        min_font_size=15,
        max_font_size=50,
        width=500  # 图幅宽度
    )

    wc.generate(cloud_text_zh)
    wc.to_file("static/images/piczh.png")
    wc.generate(cloud_text_en)
    wc.to_file("static/images/picen.png")


    res_dict = {'batchalign': batchalign,'pic_zh':'../static/images/piczh.png', 'pic_en':'../static/images/picen.png'}
    return JsonResponse(res_dict)


def interadd(request):
    a_ori = request.GET['a']
    b_ori = request.GET['b']

    zh_list = jieba.cut(a_ori)
    a = ' '.join(zh_list)
    zh_list = a.split()
    # a = a_ori

    b = b_ori.lower()
    en_list = word_tokenize(b)
    b = ' '.join(en_list)

    # 加入词典

    dict_align = ""
    firstalign = True
    for j in range(len(en_list)):
        if en_list[j] in gl.en_zh_dict:
            zh_word_set = gl.en_zh_dict[en_list[j]]
            zh_target = [i for i, x in enumerate(zh_list) if x in zh_word_set]
            for i in zh_target:
                if firstalign:
                    firstalign = False
                else:
                    dict_align += " "
                dict_align += str(i)
                dict_align += "-"
                dict_align += str(j)
    print("词典读完")

    s2_inter = a
    s3_inter = "../26491/merged.26.ch.prior.index"  # write path,s10需跟这个保持一致
    s5_inter = b
    s6_inter = "../26491/merged.26.en.prior.index"  # write path,s11需跟这个保持一致
    s10_inter = "../26491/merged.26.ch.prior.index"
    s11_inter = "../26491/merged.26.en.prior.index"
    s15_inter = "../26491/merged.26.ch.en.prior.align"

    bs2_inter = s2_inter.encode("utf-8")
    bs3_inter = s3_inter.encode("utf-8")
    bs5_inter = s5_inter.encode("utf-8")
    bs6_inter = s6_inter.encode("utf-8")
    bs10_inter = s10_inter.encode("utf-8")
    bs11_inter = s11_inter.encode("utf-8")
    bs15_inter = s15_inter.encode("utf-8")

    gl.lib_global_inter.Sentence2IndexCh(bs2_inter, bs3_inter)
    gl.lib_global_inter.Sentence2IndexEn(bs5_inter, bs6_inter)
    gl.lib_global_inter.ReadCorpusIndexCh(bs10_inter)
    gl.lib_global_inter.ReadCorpusIndexEn(bs11_inter)
    gl.lib_global_inter.ConstructNetN()
    gl.lib_global_inter.ConstructNetE()
    gl.lib_global_inter.EMLoop.restype = c_char_p
    align_prior = gl.lib_global_inter.EMLoop(bs15_inter)

    dict_align_list = dict_align.split()
    align_prior_list = align_prior.decode('utf-8').split()

    deleted_ap_list = []

    for da in dict_align_list:
        rda = da.split('-')[1]
        for ap in align_prior_list:
            rap = ap.split('-')[1]
            if rda == rap:
                deleted_ap_list.append(str(ap))
    print("align_prior")
    all_ap = ""
    for ap in align_prior_list:
        all_ap += ap
        all_ap += " "
    print(all_ap)
    print("deleted_ap_list")
    all_dap = ""
    for dap in deleted_ap_list:
        all_dap += dap
        all_dap += " "
    print(all_dap)
    print("united_align_list")
    united_align_list = list(set(align_prior_list) - set(deleted_ap_list))
    all_u = ""
    for u in united_align_list:
        all_u += u
        all_u += " "
    print(all_u)

    united_align = ""
    firstalign = True
    for i in range(len(united_align_list)):
        if firstalign:
            firstalign = False
        else:
            united_align += " "
        united_align += united_align_list[i]

    for i in range(len(dict_align_list)):
        united_align += " "
        united_align += dict_align_list[i]

    res_dict = {'ch': a, 'en': b, 'align_prior': united_align}
    return JsonResponse(res_dict)


def interaddword(request):
    a_word = request.GET['a']
    b_word = request.GET['b'].lower()
    gl.en_zh_dict[b_word].add(a_word)
    hint = "已成功加入词对:"+a_word+"-"+b_word
    res_dict = {'hint':hint}
    return JsonResponse(res_dict)
