# Hidden Markov Model

the implementation of hidden markov model with C++

## Environment

- C++ 11+
- clang++ 10.0
- boost 1.71.0
- glog 0.4.0
- gflag 2.2.2


## Usage

- training and backtrack

```
$ make
$ ./hmm -ITER=200 > data/result.txt
```

- valuation

```
$ cd data/
$ ./grade-bayes-hmm.pl reference.txt result.txt
```

## Reslut

experimental setting as follows:

- num of hidden states = 30
- num of iteration = 200
- training data and valuation data are same data(wikipedia)

accuracy = 67.18%


## Reference

- [A Revealing Introduction to Hidden Markov Models](https://www.cs.sjsu.edu/~stamp/RUA/HMM.pdf)
- [neural networks for nlp](http://www.phontron.com/teaching.php)
- [position tagging using hidden markov model](http://www.phontron.com/slides/nlp-programming-ja-04-hmm.pdf)
