#include<vector>

using namespace std;

class MutationStrategy
{
protected:
    double mutationProb_;

public:
    MutationStrategy(double mutationProb = 0.03)
    {
        mutationProb_ = mutationProb;
    }

    virtual double Mutate(double gene)
    {
        return getRandomBetweenThousandMinusThousand();
    }

    vector<double> Apply(vector<double> dna)
    {
        int dnaSize = dna.size();
        double p;
        for (int j = 0; j < dnaSize; j++)
        {
            p = randn();
            if (p <= mutationProb_)
            {
                dna.at(j) = Mutate(dna.at(j));
            }
        }

        return dna;
    }
};