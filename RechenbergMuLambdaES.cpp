#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

class RechenbergMuLambdaES : public EvolutionaryStrategy
{
private:
    int mu_;
    int lambda_;
    double sigma_;
    vector<Dinossauro> lastGen_;
    int lastGenBestPos_;

    vector<double> mutateGaussian(const vector<double> &parentDNA)
    {
        int dnaSize = parentDNA.size();
        vector<double> childDNA(dnaSize);

        for (int g = 0; g < dnaSize; g++)
            childDNA[g] = clampGene(parentDNA[g] + gaussianRandom(0.0, sigma_));

        return childDNA;
    }

public:
    RechenbergMuLambdaES(int mu = 5, int lambda = POPULACAO_TAMANHO, double sigma = 0.1, int indice = 0)
    {
        if (lambda < mu || lambda % mu != 0)
        {
            printf("Erro: lambda (%d) deve ser >= mu (%d) e divisivel por mu.\n", lambda, mu);
            exit(1);
        }

        mu_ = mu;
        lambda_ = lambda;
        sigma_ = sigma;
        lastGenBestPos_ = 0;
        _indice = indice;

        strcpy(_name, "ES-ML");
        sprintf(_args, "mu%d-lambda%d-sigma%.2f", mu_, lambda_, sigma_);
    }

    void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        EvolutionaryStrategy::Evolve(d, DNAs);

        cout << "--- Evolucao EE (mu,lambda) ---" << endl;
        cout << "Geracao " << Geracao << endl;

        vector<Dinossauro> topDinos;
        vector<int> topPositions;
        tie(topDinos, topPositions) = getTopN(d, mu_);

        lastGen_ = topDinos;
        lastGenBestPos_ = 0;

        vector<vector<double>> parentDNAs;

        for (int i = 0; i < mu_; i++)
        {
            parentDNAs.push_back(DNAs[topPositions[i]]);
            printf("Progenitor %d selecionado (pos %d, fitness %.0f)\n", i, topPositions[i], d[topPositions[i]].Fitness);
        }

        int offspringPerParent = lambda_ / mu_;
        int offspringIndex = 0;
        for (int parentIdx = 0; parentIdx < mu_; parentIdx++)
        {
            for (int j = 0; j < offspringPerParent; j++)
            {
                vector<double> childDNA = mutateGaussian(parentDNAs[parentIdx]);
                updateDNADaVezByDinoId(childDNA, offspringIndex);
                Dinossauros[offspringIndex].ResetarFitness = 1;
                offspringIndex++;
            }
        }

        Geracao++;
    }

    Dinossauro getLastGenBestDino()
    {
        if (lastGen_.size() > 0)
            return lastGen_.at(lastGenBestPos_);

        Dinossauro d;
        return d;
    }
};
