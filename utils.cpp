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
        v[i].Fitness = v[i].Fitness + 1;
    }
    return v;
}

vector<int> getRouletteWheel(vector<Dinossauro> d)
{
    vector<Dinossauro>::iterator di;

    vector<int> rouletteWheel = {0};

    int previous = 0;
    for (di = d.begin(); di != d.end(); ++di)
    {
        int actual = di->Fitness;
        int newPos = previous + actual;
        rouletteWheel.push_back(newPos);

        previous = newPos;
    }

    return rouletteWheel;
}