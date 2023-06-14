#include <vector>
#include <bits/stdc++.h>

using namespace std;

class OnePointCrossover
{
private:
    double crossoverProb_;
    vector<int> rouletteWheel_ = {};
    int limiteRouletteWheel_ = 0;
    int lastRouletteGenIndex_ = -1;

    vector<int> getRandomParents(int numParents = 2)
    {
        vector<int> parentPositions = {};
        for (int i = 0; i < numParents; i++)
        {
            int randValue = (rand() % limiteRouletteWheel_) + 1;
            vector<int>::iterator rsi = rouletteWheel_.begin();

            while (randValue > *rsi)
                ++rsi;

            int index = rsi - rouletteWheel_.begin() - 1;
            parentPositions.push_back(index);
        }

        return parentPositions;
    }

    virtual vector<double> Crossover(vector<int> parentPositions, vector<vector<double>> DNAs)
    {

        vector<double> childDNA = {};
        
        int firstParentPos = parentPositions.at(0);
        int secondParentPos = parentPositions.at(1);
        vector<double> firstParentDNA = DNAs.at(firstParentPos);
        vector<double> secondParentDNA = DNAs.at(secondParentPos);
        int dnaSize = firstParentDNA.size();
        int crossoverPoint = rand() % (dnaSize - 2) + 1;

        childDNA = ConcatVector(childDNA, firstParentDNA, 0, crossoverPoint);
        childDNA = ConcatVector(childDNA, secondParentDNA, crossoverPoint, dnaSize);

        return childDNA;
    }

    vector<double> Clone(vector<int> parentPositions, vector<vector<double>> DNAs)
    {
        /* Clonagem */
        int pos = rand() % 2;
        
        vector<double> childDNA = {};

        int parentPos = parentPositions.at(pos);
        vector<double> parentDNA = DNAs.at(parentPos);

        vector<double>::iterator it;
        for (it = parentDNA.begin(); it != parentDNA.end(); ++it)
        {
            childDNA.push_back(*it);
        }

        return childDNA;
    }

protected:
    char _args[100] = "\"\"";

public:
    OnePointCrossover(double crossoverProb = 0.8)
    {
        crossoverProb_ = crossoverProb;
        sprintf(_args, "%.0f-1", crossoverProb*100);
    }

    void UpdateRouletteWheel(vector<Dinossauro> d)
    {
        rouletteWheel_ = getRouletteWheel(d);
        limiteRouletteWheel_ = *max_element(rouletteWheel_.begin(), rouletteWheel_.end());
        lastRouletteGenIndex_ = Geracao;
    }

    vector<double> NewChildDNA(vector<vector<double>> DNAs)
    {
        vector<double> childDNA;

        vector<int> parentPositions = getRandomParents();

        double p = randn();
        if (p < crossoverProb_)
        {
            childDNA = Crossover(parentPositions, DNAs);
        }
        else
        {
            childDNA = Clone(parentPositions, DNAs);
        }

        return childDNA;
    }

    char* GetArgs()
    {
        return _args;
    }
};