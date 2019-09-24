#include <set>
#include <algorithm>
#include <boost/foreach.hpp>
#include "hmm.hpp"

DEFINE_int32(N, 30, "Number of hidden stete");
DEFINE_string(DATA, "data/wiki-wakati.txt", "Data file");

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(*argv);
    google::ParseCommandLineFlags(&argc, &argv, true);

    HMM hmm;
    HMM::Observation o;
    
    hmm.read(FLAGS_DATA, o,  true);
    hmm.train(FLAGS_N);
    hmm.forward_backward(o);
    hmm.viterbi(o);
    return 0;
}