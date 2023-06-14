#include<vector>
#include <fstream>

using namespace std;

class EvolutionaryStrategy
{
private:
    void WriteBestFinessToCSV(vector<Dinossauro> &d)
    {
        ofstream scoresFile;
        char String[1000];
        char msg[] = "%s, %d, %.0f, %.0f, %d,%s\n";
        char filename[1000];
        char filenameTemplate[] = "data/scores-%s.csv";

        
        vector<Dinossauro> topN;
        vector<int> topNPositions;
        tie(topN, topNPositions) = getTopN(d, 10);
        vector<Dinossauro>::iterator dinoIt = topN.begin();
        Dinossauro bestDino = *dinoIt;
        double bestFitness = bestDino.Fitness;

        sprintf(filename, filenameTemplate, _name);
        scoresFile.open(filename, ios::app);

        sprintf(String, msg, _name, Geracao, DistanciaRecorde, bestFitness, POPULACAO_TAMANHO, _args);
        scoresFile << String;
        scoresFile.close();
    }

protected:
    char _name[100] = "Estratégia de Evolução";
    char _args[100] = "\"\"";

public:
    virtual void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        WriteBestFinessToCSV(d);
    }

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