#include <vector>

using namespace std;

class MutationBySum : public MutationStrategy
{
public:
    MutationBySum(double mutationProb = 0.03) : MutationStrategy(mutationProb)
    {
        sprintf(_args, "%.0f-sum", mutationProb*100);
    }

    double Mutate(double gene)
    {
        double factor = getRandomBetweenThousandMinusThousand() / 100.0;
        return gene + factor;
    }
};
