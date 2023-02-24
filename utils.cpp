#include <iostream>
#include <vector>

using namespace std;

template <typename T, size_t S>
vector<T> arrayToVector(T (&arr)[S])
{
    vector<T> v = {};
    for (size_t i = 0; i < S; i++)
    {
        v.push_back(arr[i]);
        v.at(i) += 1;
    }
    return v;
}

vector<Dinossauro> &getRodaDaFortuna(vector<Dinossauro> d)
{
    double fitnessTotal = 0.0;
    vector<Dinossauro>::iterator di;

    for (di = d.begin(); di != d.end(); ++di)
    {
        fitnessTotal += di->Fitness;
    }

    cout << "Fitnesses / Normas: " << endl;

    vector<Dinossauro> dnorm = {};
    for (di = d.begin(); di != d.end(); ++di)
    {
        cout << di->Fitness << " / ";
        di->Fitness = di->Fitness / fitnessTotal;
        cout << di->Fitness;
        dnorm.push_back(*di);
    }

    return dnorm;
}