#include <vector>
#include <bits/stdc++.h>

using namespace std;

class GenAlgorithm : public EvolutionaryStrategy
{
private:
    int crossoverProbability_;
    int mutationProbability_;

    vector<int> getRandomParents(vector<Dinossauro> d, int numParents = 2)
    {
        vector<int> roletaSorte = getRouletteWheel(d);

        int limite = *max_element(roletaSorte.begin(), roletaSorte.end());

        vector<int> parentPositions = {};
        for (int i = 0; i < numParents; i++)
        {
            int randValue = rand() % limite;
            cout << "Valor sorteado: " << randValue << endl;

            vector<int>::iterator rsi = roletaSorte.begin(); 
            while(randValue > *rsi) ++rsi;
            int index = rsi - roletaSorte.begin();
            parentPositions.push_back(index);

            cout << "Posição encontrada: " << index << ", intervalo [" << *(rsi - 1) << ", " << *rsi << "]" << endl;
            cout << "-----";
        }

        return parentPositions;
    }

public:
    GenAlgorithm(int crossoverProbability = 0.8, int mutationProbability = 0.03)
    {
        crossoverProbability_ = crossoverProbability;
        mutationProbability_ = mutationProbability;
    }

    void Evolve(vector<Dinossauro> &d)
    {
        int dSize = d.size();
        vector<Dinossauro> novaGer;

        for (int i = 0; i < dSize; i++)
        {
            vector<int> parentPositions = getRandomParents(d);
        }

        RandMutations().Evolve(d);
    }
};