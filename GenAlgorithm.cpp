#include <vector>
#include <bits/stdc++.h>

using namespace std;

class GenAlgorithm : public EvolutionaryStrategy
{
private:
    double crossoverProbability_;
    double mutationProbability_;
    vector<int> rouletteWheel = {};
    vector<Dinossauro> lastGen = {};
    int lastGenBestDinoPos = 0;
    int bestFitnessDidntChange = 0;
    int maxPatiente = 50;

    vector<int> getRandomParents(vector<Dinossauro> d, int numParents = 2)
    {
        if (rouletteWheel.size() == 0)
            rouletteWheel = getRouletteWheel(d);

        int limite = *max_element(rouletteWheel.begin(), rouletteWheel.end());

        vector<int> parentPositions = {};
        for (int i = 0; i < numParents; i++)
        {
            int randValue = rand() % limite;
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

        if (d[bestDinoPos].Fitness == lastGen[lastGenBestDinoPos].Fitness)
        {
            bestFitnessDidntChange += 1;
        }
        else
        {
            bestFitnessDidntChange = 0;
        }

        lastGenBestDinoPos = bestDinoPos;

        if (bestFitnessDidntChange > 10)
            cout << "Melhor fitness não muda há 10 gerações: " << d[bestDinoPos].Fitness << endl;
    }

    void updateLastGenVector(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        vector<Dinossauro>::iterator di;

        cout << "Atualizando última geração..." << endl;
        lastGen = {};
        lastGenBestDinoPos = 0;

        int i = 0;
        for (di = d.begin(); di != d.end(); ++di)
        {
            lastGen.push_back(*di);
        }

        saveLastGenBestDino(d, DNAs);
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

        d[worstDinoPos] = lastGen[lastGenBestDinoPos];
        vector<double> lastGenBestDNA = pointerArrayToVector(lastGen[lastGenBestDinoPos].DNA, dnaSize);
        DNAs[worstDinoPos] = lastGenBestDNA;

        printDinoData(worstDinoPos, d[worstDinoPos], DNAs[worstDinoPos]);

        return worstDinoPos;
    }

public:
    GenAlgorithm(double crossoverProbability = 0.8, double mutationProbability = 0.03)
    {
        crossoverProbability_ = crossoverProbability;
        mutationProbability_ = mutationProbability;
    }

    void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        cout << "--- Evolução ---" << endl;
        cout << "Geração " << Geracao << endl;
        printGenerationData(d, DNAs);

        int dSize = DNAs.size();
        int dnaSize = (*DNAs.begin()).size();

        if (Geracao > 0)
        {
            cout << "Aplicando elitismo..." << endl;
            applyElitism(d, DNAs, dnaSize);
        }
        else
        {
            cout << "Não é necessário aplicar elitismo." << endl;
        }

        updateLastGenVector(d, DNAs);

        cout << endl
             << "Criando novos genes..." << endl;

        vector<vector<double>> newGenerationDNA;
        for (int i = 0; i < dSize; i++)
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

            newGenerationDNA.push_back(childDNA);

            for (int j = 0; j < dnaSize; j++)
            {
                Dinossauros[i].DNA[j] = childDNA.at(j);
            }

            Dinossauros[i].ResetarFitness = 1;
        }

        Geracao++;
    }
};