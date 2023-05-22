#include<vector>

using namespace std;

class EvolutionaryStrategy
{
public:
    virtual void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs) = 0;

    virtual Dinossauro getLastGenBestDino() {
        Dinossauro d;
        return d;
    }
};