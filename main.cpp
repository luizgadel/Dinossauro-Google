#include "DinoRN.cpp"
#include "GenAlgorithm.cpp"
#include <cstring>

DinoRN DRNGeneticAlgorithm(char *indice = "0")
{
    int indice_ = atoi(indice);
    /*double crossoverProb, mutationProb, elitismProb;
    cout << "Qual a probabilidade de crossover? ";
    cin >> crossoverProb;
    cout << "Qual a probabilidade de mutação? ";
    cin >> mutationProb;
    cout << "Qual a probabilidade de elitismo? ";
    cin >> elitismProb;*/
    return DinoRN(std::make_unique<GenAlgorithm>(0.85, 0.1, 0.04, indice_));
}

DinoRN DRNRechenbergES(char *indice = "0")
{
    int indice_ = atoi(indice);
    return DinoRN(std::make_unique<RechenbergMuLambdaES>(POPULACAO_TAMANHO / 10, POPULACAO_TAMANHO, 0.1, indice_));
}

int main(int argc, char *args[])
{
    char *indice = (argc > 1) ? args[1] : (char *)"0";
    bool useES = false;
    unsigned int seed = 0;

    if (argc > 2 && strcmp(args[2], "ES") == 0)
    {
        useES = true;
        if (argc > 3)
            seed = (unsigned int)atoi(args[3]);
    }
    else if (argc > 2)
    {
        seed = (unsigned int)atoi(args[2]);
    }

    obstaculosSeedFixa = seed;

    DinoRN dinoneural;

    if (useES)
        dinoneural = DRNRechenbergES(indice);
    else
        dinoneural = DRNGeneticAlgorithm(indice);

    dinoneural.startGame();

    return 0;
}
