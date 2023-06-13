#include <vector>

using namespace std;

class MutationByMultiplication : public MutationStrategy
{
public:
    using MutationStrategy::MutationStrategy;

    double Mutate(double gene)
    {
        double factor = randn() + 0.5;
        return gene * factor;
    }
};
