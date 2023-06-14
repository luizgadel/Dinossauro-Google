#include <vector>

using namespace std;

class MutationByRM : public MutationStrategy
{
private:
    unique_ptr<MutationStrategy> mutationTypeOne_, mutationTypeTwo_, mutationTypeThree_;
public:
    MutationByRM(double mutationProb = 0.03) : MutationStrategy(mutationProb)
    {
        mutationTypeOne_ = make_unique<MutationBySubstitution>();
        mutationTypeTwo_ = make_unique<MutationByMultiplication>();
        mutationTypeThree_ = make_unique<MutationBySum>();
        sprintf(_args, "%.0f-rm", mutationProb*100);
    }

    double Mutate(double gene)
    {
        int choice = rand() % 3;
        double newGene;
        switch(choice)
        {
            case 0: 
                newGene = mutationTypeOne_->Mutate(gene);
                break;
            case 1: 
                newGene = mutationTypeTwo_->Mutate(gene);
                break;
            case 2: 
                newGene = mutationTypeThree_->Mutate(gene);
                break;
        }
        return newGene;
    }
};
