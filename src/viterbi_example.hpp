#ifndef MODEL_EXAMPLE_HPP
#define MODEL_EXAMPLE_HPP
#include "hmm.hpp"

class SampleModel : public HMM {
public:
    SampleModel();
    void run();
};

#endif