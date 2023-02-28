#include <vector>
#include <bits/stdc++.h>

using namespace std;

class GenAlgorithm : public EvolutionaryStrategy
{
private:
    double crossoverProbability_;
    double mutationProbability_;
    vector<int> rouletteWheel = {};

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

public:
    GenAlgorithm(double crossoverProbability = 0.8, double mutationProbability = 0.03)
    {
        crossoverProbability_ = crossoverProbability;
        mutationProbability_ = mutationProbability;
    }

    void Evolve(vector<Dinossauro> &d, vector<vector<double>> &DNAs)
    {
        
        int dSize = DNAs.size();
        int dnaSize = (*DNAs.begin()).size();
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
        }


    }
};