#include <iostream>
#include <vector>
#include <chrono>
#include <random>

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

vector<vector<double>> matrixToVector(double *arr[], int cols)
{
    vector<vector<double>> matr = {};
    for (int i = 0; i < POPULACAO_TAMANHO; i++)
    {
        vector<double> vect = {};
        for (int j = 0; j < cols; j++)
        {
            vect.push_back(arr[i][j]);
        }
        matr.push_back(vect);
    }

    return matr;
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

double randn()
{
    mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> unif(0, 1);
    return unif(rng);
}

void printDoubleVector(vector<double> vect)
{
    vector<double>::iterator it;
    printf("[");
    for (it = vect.begin(); it != vect.begin() + 5; ++it)
    {
        printf(" %.2f", *it);
    }
    printf(" ...");
    for (it = vect.end() - 6; it != vect.end(); ++it)
    {
        printf(" %.2f", *it);
    }
    printf("]\n");
}