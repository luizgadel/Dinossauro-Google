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
    }
    return v;
}

double *vectorToPointerArray(vector<double> v)
{
    vector<double>::iterator vi;
    int dnaSize = v.size();
    double *ptr = (double *)malloc(dnaSize * sizeof(double));
    int i = 0;
    for (vi = v.begin(); vi != v.end(); ++vi)
    {
        ptr[i] = *vi;
        i++;
    }
    return ptr;
}

vector<double> pointerArrayToVector(double *arr, int len)
{
    vector<double> v = {};
    for (int i = 0; i < len; i++)
    {
        v.push_back(arr[i]);
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

template <typename T>
void printVector(vector<T> vect)
{
    typename vector<T>::iterator it;
    cout << "[";
    for (it = vect.begin(); it != vect.begin() + 5; ++it)
    {
        cout << " " << *it;
    }
    printf(" ...");
    for (it = vect.end() - 5; it != vect.end(); ++it)
    {
        cout << " " << *it;
    }
    cout << "]" << endl;
}

void printDinoData(int k, Dinossauro dino, vector<double> genes)
{
    cout << "Dinossauro " << k << ": "
         << "\t" << dino.Fitness << " ";
    printVector(genes);
}

void printGenerationData(vector<Dinossauro> d, vector<vector<double>> dnas)
{
    Dinossauro dino;
    vector<double> genes;

    vector<vector<double>>::iterator dnaIt = dnas.begin();
    vector<Dinossauro>::iterator dinoIt = d.begin();
    int k = 0;
    for (dnaIt = dnas.begin(); dnaIt != dnas.end(); ++dnaIt)
    {

        dino = *dinoIt;
        genes = *dnaIt;

        printDinoData(k, dino, genes);

        ++dinoIt;
        k++;
    }
}

void printGenerationData(vector<Dinossauro> d)
{
    Dinossauro dino;
    vector<double> genes;

    vector<Dinossauro>::iterator dinoIt;
    int k = 0;
    for (dinoIt = d.begin(); dinoIt != d.end(); ++dinoIt)
    {

        dino = *dinoIt;
        genes = pointerArrayToVector(dino.DNA, dino.TamanhoDNA);

        printDinoData(k, dino, genes);
        k++;
    }
}

template <typename T>
vector<T> sliceV(vector<T> v, int start, int end)
{
    vector<T> slicedVector = {};

    T vi;
    int i = 0;
    if (start <= end)
    {

        for (i = start; i < end; ++i)
        {
            vi = v.at(i);
            slicedVector.push_back(vi);
        }
    }
    else
    {
        cout << "Erro na função slice: início (" << start << ") é maior que o fim (" << end << ")." << endl;
    }

    return slicedVector;
}

vector<Dinossauro> getTopFive(vector<Dinossauro> d)
{
    vector<Dinossauro>::iterator di = d.begin();
    int numberOfDinos = d.size();
    Dinossauro dinoAux;

    for (int i = 0; i < numberOfDinos; i++)
    {
        for (int j = 0; j < numberOfDinos - 1; j++)
        {
            if (d.at(j).Fitness < d.at(j + 1).Fitness)
            {
                dinoAux = d.at(j);
                d.at(j) = d.at(j + 1);
                d.at(j + 1) = dinoAux;
            }
        }
    }

    vector<Dinossauro> topFive = sliceV(d, 0, 5);
    return topFive;
}