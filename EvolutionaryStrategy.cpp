#include<vector>

using namespace std;

class EvolutionaryStrategy
{
protected:
    char _name[100] = "Estratégia de Evolução";
    char _args[100] = "\"\"";

public:
    virtual void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs) = 0;

    virtual Dinossauro getLastGenBestDino() {
        Dinossauro d;
        return d;
    }

    virtual char* getName() {
        return _name;
    }

    virtual char* getArgs() {
        return _args;
    }
};