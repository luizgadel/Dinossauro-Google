#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

class NPointCrossover : public OnePointCrossover
{
private: 
    int n_;

public:
    NPointCrossover(double crossoverProb = 0.80, int n = 2) : OnePointCrossover(crossoverProb)
    {
        n_ = n;
    }

    vector<double> Crossover(vector<int> parentPositions, vector<vector<double>> DNAs)
    {
        vector<double> childDNA = {};
        int firstParentPos = parentPositions.at(0);
        int secondParentPos = parentPositions.at(1);
        vector<double> firstParentDNA = DNAs.at(firstParentPos);
        vector<double> secondParentDNA = DNAs.at(secondParentPos);
        int dnaSize = firstParentDNA.size();
        int lastGenePos = dnaSize - 1;
        vector<int> crossoverPoints = GetRandomVector(n_, lastGenePos - 2, 1);
        vector<int> parentOrder(2);
        auto rng = default_random_engine {};
        
        sort(crossoverPoints.begin(), crossoverPoints.end());
        crossoverPoints.push_back(dnaSize);
        printVector(crossoverPoints);

        iota(parentOrder.begin(), parentOrder.end(), 0);
        shuffle(begin(parentOrder), end(parentOrder), rng);
        parentOrder = ConcatVector(parentOrder, GetRandomVector(n_ - 1, 2));

        vector<int>::iterator crossoverIt = crossoverPoints.begin();
        vector<int>::iterator parentIt = parentOrder.begin();
        int startIndex = 0, endIndex;
        for (; crossoverIt != crossoverPoints.end(); ++crossoverIt)
        {
            endIndex = *crossoverIt;
            int chosenParentPos = parentPositions.at(*parentIt);
            childDNA = ConcatVector(childDNA, DNAs.at(chosenParentPos), startIndex, endIndex);
            startIndex = endIndex;
            ++parentIt;
        }

        return childDNA;
    }
};