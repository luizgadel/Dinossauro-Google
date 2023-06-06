#include <vector>
#include <bits/stdc++.h>

using namespace std;

class GenAlgorithm : public EvolutionaryStrategy
{
private:
    double crossoverProbability_;
    double mutationProbability_;
    vector<int> rouletteWheel = {};
    int limiteRouletteWheel = 0;
    int lastRouletteGenIndex = -1;
    vector<Dinossauro> lastGen = {};
    int lastGenBestDinoPos = -1;
    int bestFitnessDidntChange = 0;
    int maxPatiente = 50;
    int topNElitismParam_;
    TopNElitism topFiveElitism_;

    vector<int> getRandomParents(vector<Dinossauro> d, int numParents = 2)
    {
        bool isRouletteOutdated = (lastRouletteGenIndex != Geracao);
        if (isRouletteOutdated)
        {
            rouletteWheel = getRouletteWheel(d);
            limiteRouletteWheel = *max_element(rouletteWheel.begin(), rouletteWheel.end());
            lastRouletteGenIndex = Geracao;
        }

        vector<int> parentPositions = {};
        for (int i = 0; i < numParents; i++)
        {
            int randValue = (rand() % limiteRouletteWheel) + 1;
            vector<int>::iterator rsi = rouletteWheel.begin();

            while (randValue > *rsi)
                ++rsi;

            int index = rsi - rouletteWheel.begin() - 1;
            parentPositions.push_back(index);
        }

        return parentPositions;
    }

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

    int applyElitism(vector<Dinossauro> &d, vector<vector<double>> &DNAs, int dnaSize)
    {
        int worstDinoPos = findWorstDinoPos(d, DNAs);

        cout << endl
             << "Substituindo pelo melhor da última geração..." << endl;
        vector<double> lastGenBestDNA = pointerArrayToVector(lastGen[lastGenBestDinoPos].DNA, dnaSize);
        printDinoData(lastGenBestDinoPos, lastGen[lastGenBestDinoPos], lastGenBestDNA);

        d[worstDinoPos] = lastGen[lastGenBestDinoPos];
        DNAs[worstDinoPos] = lastGenBestDNA;

        printDinoData(worstDinoPos, d[worstDinoPos], DNAs[worstDinoPos]);

        return worstDinoPos;
    }

public:
    GenAlgorithm(double crossoverProbability = 0.8, double mutationProbability = 0.03, double elitismPercent = 0.05)
    {
        strcpy(_name, "AG");
        sprintf(_args, "\"%.2f,%.2f,%.2f\"", crossoverProbability, mutationProbability, elitismPercent);
        crossoverProbability_ = crossoverProbability;
        mutationProbability_ = mutationProbability;
        topNElitismParam_ = round(POPULACAO_TAMANHO * elitismPercent);
        topFiveElitism_ = TopNElitism(topNElitismParam_);
    }

    void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        cout << "--- Evolução ---" << endl;
        cout << "Geração " << Geracao << endl;

        int dSize = DNAs.size();
        int dnaSize = (*DNAs.begin()).size();

        updateLastGenVector(d, DNAs);

        cout << endl
             << "Criando novos genes..." << endl;

        /* Elitismo */
        topFiveElitism_.Apply(d);

        for (int i = topNElitismParam_; i < dSize; i++)
        {
            vector<int> parentPositions = getRandomParents(d);

            int firstParentPos = parentPositions.at(0);
            vector<double> firstParentDNA = DNAs.at(firstParentPos);

            int secondParentPos = parentPositions.at(1);
            vector<double> secondParentDNA = DNAs.at(secondParentPos);

            vector<double> childDNA = {};

            double p = randn();
            if (p <= crossoverProbability_)
            {
                /* Crossover */
                int crossoverPoint = rand() % (dnaSize - 2) + 1;

                for (int j = 0; j < crossoverPoint; j++)
                {
                    double gene = firstParentDNA.at(j);
                    childDNA.push_back(gene);
                }

                for (int j = crossoverPoint; j < dnaSize; j++)
                {
                    double gene = secondParentDNA.at(j);
                    childDNA.push_back(gene);
                }
            }
            else
            {
                /* Clonagem */
                int pos = rand() % 2;
                int parentPos = parentPositions.at(pos);
                vector<double> parentDNA = DNAs.at(parentPos);

                vector<double>::iterator it;
                for (it = parentDNA.begin(); it != parentDNA.end(); ++it)
                {
                    childDNA.push_back(*it);
                }
            }

            /* Mutação */
            for (int j = 0; j < dnaSize; j++)
            {
                p = randn();
                if (p <= mutationProbability_)
                {
                    double antigoValor = childDNA.at(j);
                    childDNA.at(j) = getRandomValue();
                }
            }

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