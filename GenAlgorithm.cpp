#include <vector>

class GenAlgorithm : public EvolutionaryStrategy
{
private:
    int crossoverProbability_;
    int mutationProbability_;

public:
    GenAlgorithm(int crossoverProbability = 0.8, int mutationProbability = 0.03)
    {
        crossoverProbability_ = crossoverProbability;
        mutationProbability_ = mutationProbability;
    }
    
    void Evolve(std::vector<Dinossauro> &d)
    {
        Dinossauro *Vetor[POPULACAO_TAMANHO];

        /* Guarda a referência de cada dinossauro em Vetor */
        for (int i = 0; i < POPULACAO_TAMANHO; i++)
        {
            Vetor[i] = &Dinossauros[i];
        }

        
    }
};