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
    int successesInWindow_;
    int attemptsInWindow_;
    vector<double> lastGenParentsFitness_;
    vector<int> offspringParentIndex_;
    vector<Dinossauro> lastGen_;
    int lastGenBestPos_;

    static constexpr double SIGMA_INCREASE = 1.2;
    static constexpr double SIGMA_DECREASE = 0.82;
    static constexpr double SUCCESS_RATE_TARGET = 0.2;

    void adaptSigma()
    {
        if (attemptsInWindow_ == 0)
            return;

        double successRate = static_cast<double>(successesInWindow_) / attemptsInWindow_;

        if (successRate > SUCCESS_RATE_TARGET)
            sigma_ *= SIGMA_INCREASE;
        else if (successRate < SUCCESS_RATE_TARGET)
            sigma_ *= SIGMA_DECREASE;

        printf("EE (mu,lambda): taxa de sucesso = %.3f, sigma = %.6f\n", successRate, sigma_);

        successesInWindow_ = 0;
        attemptsInWindow_ = 0;
    }

    void countSuccessesForOneFifthRule(vector<Dinossauro> &d)
    {
        if (offspringParentIndex_.empty())
            return;

        for (int i = 0; i < lambda_; i++)
        {
            int parentIdx = offspringParentIndex_[i];
            if (d[i].Fitness > lastGenParentsFitness_[parentIdx])
                successesInWindow_++;

            attemptsInWindow_++;
        }

        adaptSigma();
    }

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
        successesInWindow_ = 0;
        attemptsInWindow_ = 0;
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

        countSuccessesForOneFifthRule(d);

        vector<Dinossauro> topDinos;
        vector<int> topPositions;
        tie(topDinos, topPositions) = getTopN(d, mu_);

        lastGen_ = topDinos;
        lastGenBestPos_ = 0;

        vector<vector<double>> parentDNAs;
        vector<double> parentFitness;

        for (int i = 0; i < mu_; i++)
        {
            parentDNAs.push_back(DNAs[topPositions[i]]);
            parentFitness.push_back(d[topPositions[i]].Fitness);
            printf("Progenitor %d selecionado (pos %d, fitness %.0f)\n", i, topPositions[i], parentFitness[i]);
        }

        lastGenParentsFitness_ = parentFitness;

        int offspringPerParent = lambda_ / mu_;
        offspringParentIndex_.clear();
        offspringParentIndex_.reserve(lambda_);

        int offspringIndex = 0;
        for (int parentIdx = 0; parentIdx < mu_; parentIdx++)
        {
            for (int j = 0; j < offspringPerParent; j++)
            {
                vector<double> childDNA = mutateGaussian(parentDNAs[parentIdx]);
                updateDNADaVezByDinoId(childDNA, offspringIndex);
                Dinossauros[offspringIndex].ResetarFitness = 1;
                offspringParentIndex_.push_back(parentIdx);
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
