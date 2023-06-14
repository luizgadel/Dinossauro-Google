#include <vector>

using namespace std;

class MutationByMultiplication : public MutationStrategy
{
public:
    MutationByMultiplication(double mutationProb = 0.03) : MutationStrategy(mutationProb)
    {
        sprintf(_args, "%.0f-mul", mutationProb*100);
    }

    double Mutate(double gene)
    {
        double factor = randn() + 0.5;
        return gene * factor;
    }
};
