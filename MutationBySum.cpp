#include <vector>

using namespace std;

class MutationBySum : public MutationStrategy
{
public:
    using MutationStrategy::MutationStrategy;

    double Mutate(double gene)
    {
        double factor = getRandomBetweenThousandMinusThousand() / 100.0;
        return gene + factor;
    }
};
