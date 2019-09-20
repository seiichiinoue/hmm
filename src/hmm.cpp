// with referenced to
// "A Revealing Introduction to Hidden Markov Models"
// https://www.cs.sjsu.edu/~stamp/RUA/HMM.pdf

#include "hmm.hpp"
DEFINE_int32(ITER, 10, "Number of iteration");

double map_sum(double sum, const std::pair<int, double> pair) {
    return sum + pair.second;
};

double log_sum(double sum, const double d) {
    return sum + log(d);
};

HMM::HMM() : _N(0), _M(0) {
};

HMM::~HMM() {
};

void HMM::train(const int N) {
    _N = N;
    for (int i=0; i<_N; ++i) {
        _a.push_back(std::vector<double>(_N));
        _b.push_back(std::vector<double>(_N));
        _pi.push_back(0.0);
    }
};

int HMM::getId(const std::string &str, const bool train) {
    std::map<std::string, int>::const_iterator it = _word2id.find(str);
    if (it != _word2id.end()) {
        return it->second;
    } else if (train) {
        int newID = static_cast<int>(_word2id.size());
        _word2id[str] = newID;
        _id2word.push_back(str);
        return newID;
    } else {
        return _M;
    }
};

void HMM::read(const std::string &filename, Observation &o, const bool train) {
    std::ifstream ifs(filename.c_str());
    if (!ifs.is_open()) {
        LOG(FATAL) << filename << ": cannnot open" << std::endl;
    } else {
        LOG(INFO) << "Start reading: " << filename << std::endl;
    }
    std::string line;
    while (getline(ifs, line)) {
        std::istringstream istrs((char*)line.c_str());
        std::string word;
        while (istrs >> word) {
            o.push_back(getId(word, train));
        }
    }
    _M = static_cast<int>(_word2id.size());
};

void HMM::init_alpha_and_c(const Observation &o) {
    const int T = o.size();  // length of sequence
    _alpha.clear();
    _c.clear();
    _c.resize(T);

    std::vector<double> init;
    _c[0] = 0.0;
    for (int i=0; i<_N; ++i) {
        double tmp = _pi[i] * _b[i][o[0]];
        _c[0] += tmp;
        init.push_back(tmp);
    }
    _c[0] = 1.0 / _c[0];

    for (int i=0; i<_N; ++i) {
        init[i] *= _c[0];
    }

    _alpha.push_back(init);

    for (int i=0; i<T-1; ++i) {
        std::vector<double> tmp(_N);
        _alpha.push_back(tmp);
    }
};

void HMM::init_beta(const Observation &o) {
    int T = o.size();   // length of sequence
    _beta.clear();
    
    for (int i=0; i<T-1; ++i) {
        std::vector<double> tmp(_N);
        _beta.push_back(tmp);
    }

    std::vector<double> init;
    for (int i=0; i<_N; ++i) {
        init.push_back(_c[T-1]);
    }
    _beta.push_back(init);
};

void HMM::set_a(TransitionMatrix &a) {
    _a = a;
};

void HMM::set_b(EmissionMatrix &b) {
    _b = b;
    _M = _b[0].size();
};

void HMM::forward(const Observation &o, bool init) {
    init_alpha_and_c(o);
    int T = o.size();

    for (int t=1; t<T; ++t) {
        _c[t] = 0.0;
        for (int i=0; i<_N; ++i) {
            _alpha[t][i] = 0.0;
            for (int j=0; j<_N; ++j) {
                _alpha[t][i] += _alpha[t-1][j] * _a[j][i];
            }
            _alpha[t][i] *= _b[i][o[t]];
            _c[t] += _alpha[t][i];
        }
        // scale _alpha[t][i]
        _c[t] = 1.0 / _c[t];
        for (int i=0; i<_N; ++i) {
            _alpha[t][i] *= _c[t];
        }
    }
};

void HMM::backward(const Observation &o, bool init) {
    init_beta(o);
    const int T = o.size();
    for (int t=T-2; t>=0; --t) {
        for (int i=0; i<_N; ++i) {
            _beta[t][i] = 0.0;
            for (int j=0; j<_N; ++j) {
                _beta[t][i] += (_a[i][j] * _b[j][o[t+1]] * _beta[t+1][j]);
            }
            _beta[t][i] *= _c[t];
        }
    }
};

void HMM::forward_backward(const Observation &o) {
    const int T = o.size();
    LOG(WARNING) << "Start randomizing matrices" << std::endl;
    randomize();
    LOG(WARNING) << "Finished randomizing matrices" << std::endl;

    // initialization
    std::vector<std::vector<std::vector<double>>> gamma(_N);
    _tmp = std::vector<std::vector<std::tr1::unordered_map<int, double>>>(_N);
    std::vector<std::vector<double>> sum_of_gamma(_N);

    for (int i=0; i<_N; ++i) {
        gamma[i] = std::vector<std::vector<double>>(_N);
        _tmp[i] = std::vector<std::tr1::unordered_map<int, double>>(_M);
        sum_of_gamma[i] = std::vector<double>(T-1);
        for (int j=0; j<_N; ++j) {
            gamma[i][j] = std::vector<double>(T-1);
        }
    }

    for (int iter=0; iter<FLAGS_ITER; ++iter) {
        LOG(WARNING) << "Start " << iter << "th E-Step" << std::endl;
        EStep(o, gamma, sum_of_gamma);
        LOG(WARNING) << "Finished " << iter << "th E-Step" << std::endl;
        LOG(WARNING) << "Start " << iter << "th M-Step" << std::endl;
        MStep(o, gamma, sum_of_gamma);
        LOG(WARNING) << "Finished " << iter << "th M-Step" << std::endl;
        LOG(WARNING) << "After " << iter << "th iteration: " << log_likelihood(o) << std::endl;
    }
};

void HMM::EStep(const Observation &o,
                std::vector<std::vector<std::vector<double>>> &gamma,
                std::vector<std::vector<double>> &sum_of_gamma) {
    const int T = o.size();
    LOG(WARNING) << "Start forward" << std::endl;
    forward(o);
    LOG(WARNING) << "Finished forward" << std::endl;
    LOG(WARNING) << "Start backward" << std::endl;
    backward(o);
    LOG(WARNING) << "Finished backward" << std::endl;

    for (int t=0; t<T-1; ++t) {
        double denom = 0.0;
        for (int i=0; i<_N; ++i) {
            for (int j=0; i<_N; ++j) {
                gamma[i][j][t] = (_alpha[t][i] * _a[i][j] * _b[j][o[t + 1]] * _beta[t + 1][j]) / denom;
                sum_of_gamma[i][t] += gamma[i][j][t];
            }
            _tmp[i][o[t]][t] = sum_of_gamma[i][t];
        }
    }
};

void HMM::MStep(const Observation &o,
                const std::vector<std::vector<std::vector<double>>> &gamma,
                const std::vector<std::vector<double>> &sum_of_gamma) {
    // re-estimate pi
    LOG(INFO) << "Start re-estimation of pi" << std::endl;
    for (int i=0; i<_N; ++i) {
        _pi[i] = sum_of_gamma[i][0];
    }
    LOG(INFO) << "Finished re-estimation of pi" << std::endl;

    // re-estimate A
    LOG(INFO) << "Start re-estimation of A" << std::endl;
    for(int i = 0; i < _N; i++) {
        double denom = accumulate(sum_of_gamma[i].begin(), sum_of_gamma[i].end(), 0.0);
        for(int j = 0; j < _N; j++) {
            double nemer = accumulate(gamma[i][j].begin(), gamma[i][j].end(), 0.0);
            _a[i][j] = (nemer / denom);
        }
    }
    LOG(INFO) << "Finish re-estimation of A" << std::endl;
};