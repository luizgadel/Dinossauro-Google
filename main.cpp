#include "DinoRN.cpp"
#include "GenAlgorithm.cpp"

int main(int argc, char *args[])
{
    DinoRN dinoneural = DinoRN(std::make_unique<GenAlgorithm>());
    dinoneural.startGame();

    return 0;
}
