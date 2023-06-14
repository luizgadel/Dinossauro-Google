#include <vector>

using namespace std;

class MutationBySubstitution : public MutationStrategy
{
public:
    MutationBySubstitution(double mutationProb = 0.03) : MutationStrategy(mutationProb)
    {
        sprintf(_args, "%.0f-sub", mutationProb*100);
    }
};
