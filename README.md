# Hidden Markov Model

the implementation of hidden markov model and viterbi algorithm with C++

explanation of this implementation(myblog): [The implementation of Hidden Markov Model](https://seiichiinoue.github.io/post/hmm/)

## Todo 

- implementation of Infinit Hidden Markov Model
- Sampling of Hyper Parameters
- Beam Sampling


## Environment

- C++ 11+
- clang++ 10.0
- boost 1.71.0
- glog 0.4.0
- gflag 2.2.2


## Usage

- if you want to use your additional data for training(**optional, NOTE: if you want to use text processing script, you must install **mecab-python3 package.**)

```
$ python utils/text.py --tar_path data/[filename].txt --wakati_save_path data/[filename]-wakati.txt --pos_save_path data/[filename]-pos.txt
```

- training and predict pos with viterbi algorithm

```
$ make
$ ./hmm -ITER=200 > data/wiki-result.txt
```

- valuation

```
$ ./utils/grade-bayes-hmm.pl data/wiki-pos.txt data/wiki-result.txt
```

## Experimental Result

experimental setting as follows:

- num of hidden states = 30
- num of iteration = 200

### wikipedia corpus random sampled

```
回転/16 数/25 の/5 制御/29 が/3 事実/16 上/25 無/11 段階/28 で/6 可能/23 で/6 あ/2 る/21 ため/3 、/11 加速/13 ・/5 減速/16 時/25 の/5 衝動/29 を/6 軽減/14 でき/2 る/21 。/12 ハートフル/0 コミュニケーション/9 。/12 （/22 １９９０/0 年/9 １０/1 月/8 　/20 －/24 　/20 １９９２/0 年/9 ８/1 月/8 ）/22 １０８７/0 年/9 （/22 寛治/0 元年/9 １１/1 月/8 １４/1 日/8 ）/22 　/20
－/24 　/20 後/25 三/1 年/8 の/26 役/23 が/6 終結/27 する/29 。/12
１９３３/0 年/9 　/20 －/24 　/20 昭和/1 三/1 陸/8 地震/22 発生/25 、/11 ３００８/28 人/15 の/26 死者/23 を/6 出/27 す/29 。/12
１８０２/0 年/9 　/20 －/24 　/20 イギリス/28 の/26 ウィリアム/7 ・/5 マードック/16 が/25 石炭/11 ガス/28 を/6 照明/23 に/6 利用/17 。/12
スタンド/0 名/9 の/26 由来/27 は/13 Ｎｅｉｌ/4 　/20 Ｙｏｕｎｇ/0 の/5 アルバム/13 「/11 Ｈａｒｖｅｓｔ/28 」/15
１８９５/0 年/9 　/20 高橋/24 義雄/15 が/6 理事/23 に/6 就任/17 。/12
経営/15 の/26 大/27 改革/29 に/6 着手/17 。/12 公式/15 の/26 英語/7 表記/7 は/25 、/11 。/11 日本/28 語/15 の/26 表記/7 は/25 、/11 セルビア/13 ・/5 モンテネグロ/29 。/12
俗称/15 と/6 し/10 て/19 、/11 セルビア/13 ・/11 モンテネグロ/28 連邦/15 と/6 呼/14 ば/18 れ/10 る/21 こと/3 も/6 あ/2 る/21 。/12
死者/0 の/5 日/8 　/20 キリスト/24 教/20 （/22 カトリック/1 ）/8 の/26 記念/1 日/8 。/12
死者/0 の/5 霊魂/29 の/26 ため/7 に/6 祈り/23 を/6 捧げ/2 る/21 日/3 。/12
第/0 １１/1 条/8 ：/22 俘虜/16 は/25 宣誓/11 解放/28 の/26 受諾/23 を/6 強制/14 さ/18 れ/10 る/21 こと/3 な/5 く/13 、/11 また/16 敵/25 の/5 政府/16 は/25 宣誓/11 解放/28 を/6 求め/2 る/21 俘虜/3 の/6 請願/23 に/6 応/14 ずる/17 義務/12 は/14 な/2 い/21 。/12
```

- accuracy = 67.18%

### wagahai wa nekodearu

```
吾輩/12 は/20 猫/3 で/5 ある/13 。/11 
名前/12 は/20 まだ/3 無い/13 。/11
どこ/3 で/5 生れ/17 た/0 か/26 とんと/14 見当/9 が/23 つか/27 ぬ/0 。/11
何/12 でも/20 薄暗い/14 じめじめ/3 し/17 た/0 所/18 で/5 ニャーニャー/17 泣い/17 て/19 いた事/10 だけ/29 は/20 記憶/3 し/17 て/19 いる/10 。/11
吾輩/12 は/20 ここ/3 で/5 始め/17 て/19 人間/4 という/7 もの/18 を/23 見/27 た/0 。/11
しかも/28 あと/3 で/5 聞く/13 と/24 それ/12 は/20 書生/4 という/7 人間/8 中/18 で/5 一番/20 獰悪/4 な/7 種族/18 で/5 あっ/17 た/0 そう/29 だ/25 。/11
この/28 書生/4 という/7 の/29 は/20 時々/14 我々/9 を/23 捕え/27 て/19 煮/27 て/19 食う/4 という/7 話/18 で/5 ある/13 。/11
しかし/28 その/1 当時/12 は/20 何/4 という/7 考/18 も/23 なかっ/27 た/0 から/26 別段/14 恐し/6 いとも/24 思わ/22 なかっ/17 た/0 。/11
ただ/28 彼/4 の/7 掌/18 に/5 載せ/17 られ/17 て/19 スー/16 と/24 持ち上げ/22 られ/17 た/0 時/26 何だか/14 フワフワ/3 し/17 た/0 感じ/18 が/23 あっ/27 た/0 ばかり/2 で/5 ある/13 。/11
```

- accuracy = 78.79%

## Reference

- [A Revealing Introduction to Hidden Markov Models](https://www.cs.sjsu.edu/~stamp/RUA/HMM.pdf)
- [neural networks for nlp](http://www.phontron.com/teaching.php)
- [position tagging using hidden markov model](http://www.phontron.com/slides/nlp-programming-ja-04-hmm.pdf)
