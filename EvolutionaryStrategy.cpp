#include<vector>

using namespace std;

class EvolutionaryStrategy
{
protected:
    char _name[100] = "Estratégia de Evolução";

public:
    virtual void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs) = 0;

    virtual Dinossauro getLastGenBestDino() {
        Dinossauro d;
        return d;
    }

    virtual char* getName() {
        return _name;
    }
};