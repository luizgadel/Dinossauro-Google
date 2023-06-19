#include<vector>

using namespace std;

class MutationStrategy
{
protected:
    double mutationProb_;
    char _args[100] = "\"\"";

public:
    MutationStrategy(double mutationProb = 0.03)
    {
        mutationProb_ = mutationProb;
    }

    virtual double Mutate(double gene) = 0;
    
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

    char* GetArgs()
    {
        return _args;
    }
};