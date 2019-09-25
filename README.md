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
公立/8 高校/1 ・/8 公立/26 大学/7 へ/15 の/26 入学/7 に/6 は/25 それぞれ/5 ＬＧＳ/29 ・/6 Ö/11 ＳＳ/28 の/26 受験/23 を/6 必要/23 と/6 し/10 、/11 成績/28 順/15 で/6 入/14 学校/23 を/6 決定/27 する/29 。/12
トルコ/15 に/6 も/14 受験/23 競争/6 は/14 存在/18 し/10 、/11 高校/16 入試/25 ・/5 大学/16 入試/25 の/5 ため/29 に/6 塾/25 　/20 （/24 ）/22 　/20 に/6 通/2 う/21 こと/3 も/6 珍し/2 く/19 な/2 い/21 。/12
この/16 ため/25 、/11 ディレクトリ/28 型/15 検索/26 エンジン/23 で/6 は/25 、/11 検索/28 に/6 ヒット/27 する/29 サイト/29 が/6 無/14 かっ/2 た/21 場合/3 、/11 ロボット/28 型/15 検索/26 エンジン/23 を/6 用い/2 て/19 結果/23 を/6 表示/27 する/29 よう/3 な/6 、/11 併用/28 型/15 の/26 もの/23 が/6 多/2 い/21 。/12
```

- accuracy = 67.18%

### wagahai wa nekodearu

```
吾輩/12 は/20 猫/3 で/5 ある/13 。/11 名前/12 は/20 まだ/3 無い/13 。/11
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
