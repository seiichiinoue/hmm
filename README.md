# Hidden Markov Model

the implementation of hidden markov model and viterbi algorithm with C++

explanation of this implementation(myblog): [The implementation of Hidden Markov Model](https://seiichiinoue.github.io/post/hmm/)


## Environment

- C++ 11+
- clang++ 10.0
- boost 1.71.0
- glog 0.4.0
- gflag 2.2.2


## Usage

- if you want to use your additional data for training(**optional**, **NOTE**: if you want to use text processing script, you must install `mecab-python3` package.)

```
$ python utils/text.py --tar_path data/[filename].txt --wakati_save_path data/[filename]-wakati.txt --pos_save_path data/[filename]-pos.txt
```

- training and predict pos with viterbi algorithm

```
$ make
$ ./hmm -ITER=200 > data/wiki-result.txt
```

- valuation (mapping minimizing error rate)

you can choose perl(original) script or python(program myself) script

```
$ ./utils/grade-bayes-hmm.pl data/wiki-pos.txt data/wiki-result.txt
```

or 

```
$ python utils/valuation.py --ref data/wiki-pos.txt --test data/wiki-result.txt
```

- accuracy = 83.53%

## Reference

- [A Revealing Introduction to Hidden Markov Models](https://www.cs.sjsu.edu/~stamp/RUA/HMM.pdf)
- [neural networks for nlp](http://www.phontron.com/teaching.php)
- [position tagging using hidden markov model](http://www.phontron.com/slides/nlp-programming-ja-04-hmm.pdf)
- [introduction to nonparametric bayes](http://www.phontron.com/slides/nonparametric-tutorial-ja-1.pdf)
