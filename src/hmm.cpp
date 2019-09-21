// with referenced to
// "A Revealing Introduction to Hidden Markov Models"
// https://www.cs.sjsu.edu/~stamp/RUA/HMM.pdf

#include "hmm.hpp"
DEFINE_int32(ITER, 10, "Number of iteration");

double map_sum(double sum, const std::pair<int, double> pair) {
    return sum + pair.second;
}

double log_sum(double sum, const double d) {
    return sum + log(d);
}

HMM::HMM() : _N(0), _M(0) {
}

HMM::~HMM() {
}

void HMM::train(const int N) {
    _N = N;
    for (int i=0; i<_N; ++i) {
        _a.push_back(std::vector<double>(_N));
        _b.push_back(std::vector<double>(_N));
        _pi.push_back(0.0);
    }
}

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
}

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
}

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
}

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
}

void HMM::set_a(TransitionMatrix &a) {
    _a = a;
}

void HMM::set_b(EmissionMatrix &b) {
    _b = b;
    _M = _b[0].size();
}

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
}

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
}

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
}

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
}

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

    // re-estimate B
    LOG(INFO) << "Start re-estimation of B" << std::endl;
    int i = 0;
    std::vector<std::vector<double> >::const_iterator it_sum_of_gamma = sum_of_gamma.begin();
    for(std::vector<std::vector<double> >::iterator it_i = _b.begin(),
        it_end_i = _b.end(); it_i != it_end_i; ++it_i, ++it_sum_of_gamma, ++i) {
        double denom = accumulate(it_sum_of_gamma->begin(), it_sum_of_gamma->end(), 0.0);
        int j = 0;
        for(std::vector<double>::iterator it_j = it_i->begin(),
            it_end_j = it_i->end(); it_j != it_end_j; ++it_j, ++j) {
            double numer = accumulate(_tmp[i][j].begin(), _tmp[i][j].end(), 0.0, map_sum);
            *it_j = numer / denom;
        }
    }
    LOG(INFO) << "Finish re-estimation of B" << std::endl;
}

void HMM::check_params_consistency() {
    for (int i=0; i<_N; ++i) {
        double sum = 0.0;
        for (int j=0; j<_N; ++j) {
            CHECK_LE(0.0, _a[i][j]);
            CHECK_LE(_a[i][j], 1.0);
            sum += _a[i][j];
        }
        CHECK_DOUBLE_EQ(1.0, sum);
    }
    for (int i=0; i<_N; ++i) {
        double sum = 0.0;
        for (int k=0; k<_M; ++k) {
            sum += _b[i][k];
            CHECK_LE(0.0, _b[i][k]);
            CHECK_LE(_b[i][k], 1.0);
        }
    }
    double sum = 0.0;
    for (int i=0; i<_N; ++i) {
        sum += _pi[i];
    }
    CHECK_DOUBLE_EQ(1.0, sum);
}

double HMM::likelihood(const Observation &o) {
    return exp(log_likelihood(o));
}

double HMM::log_likelihood(const Observation &o) {
    return -accumulate(_c.begin(), _c.end(), 0.0, log_sum);
}

void HMM::viterbi(const Observation &o) {
    // initialization
    init_alpha_and_c(o);
    init_beta(o);

    const int T= o.size();

    // step1
    std::vector<double> init;
    std::vector<std::map<int, int>> psi;
    {
        std::map<int, int> tmp;
        psi.push_back(tmp);
    }

    for (int i=0; i<_N; ++i) {
        init.push_back(log(_pi[i]) + log(_b[i][o[0]]));
        std::map<int, int> tmp;
        psi.push_back(tmp);
        psi[0][i] = 0;
    }

    for (int t=0; t<T-1; ++t) {
        std::map<int, int> tmp;
        psi.push_back(tmp);
    }

    std::vector<std::vector<double>> log_delta;
    log_delta.push_back(init);
    for (int t=0; t<T; ++t) {
        std::vector<double> tmp(_N);
        log_delta.push_back(tmp);
    }

    // step2
    for (int t=0; t<T-1; ++t) {
        for (int j=0; j<_N; ++j) {
            double log_max = -std::numeric_limits<double>::max();
            for (int i=0; i<_N; ++i) {
                if (log_delta[t][i] + log(_a[i][j]) > log_max) {
                    log_max = log_delta[t][i] + log(_a[i][j]);
                    psi[t+1][j] = i;
                }
            }
            log_delta[t+1][j] = log_max + log(_b[j][o[t+1]]);
        }
    }

    // step3
    double log_p_hat = -std::numeric_limits<double>::max();
    int q_T = 0;
    for (int i=0; i<_N; ++i) {
        if (log_delta[_N-1][i] > log_p_hat) {
            log_p_hat = log_delta[_N-1][i];
            q_T = i;
        }
    }

    std::map<int, int> q_hat;
    q_hat[T] = q_T;

    // step4: backtrack
    for (int t=T-2; t>=0; --t) {
        q_hat[t] = psi[t+1][q_hat[t+1]];
    }

    for (int t=0; t<T; ++t) {
        std::cout << _id2word[o[t]] << "/" << q_hat[t] << " ";
    }
    std::cout << std::endl;
    LOG(ERROR) << "p_hat: " << exp(log_p_hat) << std::endl;
}

void HMM::randomize() {
    boost::mt19937 gen(static_cast<unsigned long>(time(0)));
    boost::uniform_real<> range(0, 1);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<>> random_value(gen, range);

    for (int i=0; i<_N; ++i) {
        double sum = 0.0;
        for (int j=0; j<_N; ++j) {
            double r = random_value();
            sum += r; _a[i][j] = r;
        }
        for (int j=0; j<_N; ++j) {
            _a[i][j] /= sum;
        }
    }

    for (int i=0; i<_N; ++i) {
        double sum = 0.0;
        for (int k=0; k<_M; ++k) {
            double r = random_value();
            sum += r; _b[i][k] = r;
        }
        for (int k=0; k<_M; ++k) {
            _b[i][k] /= sum;
        }
    }

    double sum = 0.0;
    for (int i=0; i<_N; ++i) {
        double r = random_value();
        sum += r; _pi[i] = r;
    }
    for (int i=0; i<_N; ++i) {
        _pi[i] /= sum;
    }
}