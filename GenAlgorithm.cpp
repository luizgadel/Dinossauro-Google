#include <vector>
#include <bits/stdc++.h>

using namespace std;

class GenAlgorithm : public EvolutionaryStrategy
{
private:
    vector<Dinossauro> lastGen = {};
    int lastGenBestDinoPos = -1;
    int bestFitnessDidntChange = 0;
    int maxPatiente = 50;
    int topNElitismParam_;
    TopNElitism topFiveElitism_;
    unique_ptr<OnePointCrossover> crossoverStrategy_;
    unique_ptr<MutationStrategy> mutationStrategy_;

    int findBestDinoPos(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        vector<Dinossauro>::iterator di = d.begin();
        int i = 0;

        double bestFitness = di->Fitness;
        int bestDinoPos = 0;
        ++di;
        ++i;
        for (; di != d.end(); ++di)
        {
            if (bestFitness < di->Fitness)
            {
                bestFitness = di->Fitness;
                bestDinoPos = i;
            }
            ++i;
        }

        cout << endl
             << "O melhor fitness encontrado está na posição " << bestDinoPos << " e seu valor é " << bestFitness << "." << endl;
        printDinoData(bestDinoPos, d[bestDinoPos], DNAs[bestDinoPos]);

        return bestDinoPos;
    }

    void saveLastGenBestDino(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        int bestDinoPos = findBestDinoPos(d, DNAs);

        if (Geracao > 0)
        {
            if (d[bestDinoPos].Fitness == lastGen[lastGenBestDinoPos].Fitness)
            {
                bestFitnessDidntChange += 1;
            }
            else
            {
                bestFitnessDidntChange = 0;
            }

            if (bestFitnessDidntChange > 10)
                cout << "Melhor fitness não muda há 10 gerações: " << d[bestDinoPos].Fitness << endl;
        }

        lastGenBestDinoPos = bestDinoPos;
    }

    void updateLastGenVector(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        vector<Dinossauro>::iterator di;

        saveLastGenBestDino(d, DNAs);

        lastGen = {};
        int i = 0;
        for (di = d.begin(); di != d.end(); ++di)
        {
            Dinossauro dino = *di;
            lastGen.push_back(dino);
            lastGen.at(i).DNA = vectorToPointerArray(DNAs[i]);
            i++;
        }
    }

    int findWorstDinoPos(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        vector<Dinossauro>::iterator di = d.begin();
        int i = 0;

        double worstFitness = di->Fitness;
        int worstDinoPos = 0;
        ++di;
        ++i;
        for (; di != d.end(); ++di)
        {
            if (worstFitness > di->Fitness)
            {
                worstFitness = di->Fitness;
                worstDinoPos = i;
            }
            ++i;
        }

        cout << endl
             << "O pior fitness encontrado está na posição " << worstDinoPos << " e seu valor é " << worstFitness << "." << endl;
        printDinoData(worstDinoPos, d[worstDinoPos], DNAs[worstDinoPos]);

        return worstDinoPos;
    }

public:
    GenAlgorithm(double crossoverProbability = 0.8, double mutationProbability = 0.03, double elitismPercent = 0.05)
    {
        strcpy(_name, "AG");
        sprintf(_args, "%0.f-%0.f-%0.f", crossoverProbability*100, mutationProbability*100, elitismPercent*100);
        crossoverStrategy_ = make_unique<OnePointCrossover>(crossoverProbability);
        topNElitismParam_ = round(POPULACAO_TAMANHO * elitismPercent);
        topFiveElitism_ = TopNElitism(topNElitismParam_);
        mutationStrategy_ = make_unique<MutationBySubstitution>(mutationProbability);
    }

    void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        EvolutionaryStrategy::Evolve(d, DNAs);

        cout << "--- Evolução ---" << endl;
        cout << "Geração " << Geracao << endl;

        int dSize = DNAs.size();
        int dnaSize = (*DNAs.begin()).size();

        updateLastGenVector(d, DNAs);

        cout << endl
             << "Criando novos genes..." << endl;

        /* Elitismo */
        topFiveElitism_.Apply(d);

        /* Crossover */
        crossoverStrategy_->UpdateRouletteWheel(d);

        for (int i = topNElitismParam_; i < dSize; i++)
        {
            vector<double> childDNA = crossoverStrategy_->NewChildDNA(DNAs);

            /* Mutação */
            childDNA = mutationStrategy_->Apply(childDNA);

            updateDNADaVezByDinoId(childDNA, i);

            Dinossauros[i].ResetarFitness = 1;
        }

        Geracao++;
    }

    Dinossauro getLastGenBestDino()
    {
        if (lastGen.size() > 0)
        {
            return lastGen.at(lastGenBestDinoPos);
        }
        else
        {
            Dinossauro d;
            return d;
        }
    }
};