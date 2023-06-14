#include "DinoRN.cpp"
#include "GenAlgorithm.cpp"

DinoRN DRNGeneticAlgorithm() {
    double crossoverProb, mutationProb, elitismProb;
    cout << "Qual a probabilidade de crossover? ";
    cin >> crossoverProb;
    cout << "Qual a probabilidade de mutação? ";
    cin >> mutationProb;
    cout << "Qual a probabilidade de elitismo? ";
    cin >> elitismProb;

    return DinoRN(std::make_unique<GenAlgorithm>(crossoverProb, mutationProb, elitismProb));
}

int main(int argc, char *args[])
{
    DinoRN dinoneural = DRNGeneticAlgorithm();
    dinoneural.startGame();

    return 0;
}
