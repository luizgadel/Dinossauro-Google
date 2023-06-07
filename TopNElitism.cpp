
#include <vector>

using namespace std;

class TopNElitism
{
private:
    int topN_;

public:
    TopNElitism(int topN = 1)
    {
        topN_ = topN;
    }

    void Apply(vector<Dinossauro> d)
    {
        if (topN_ > 0)
        {
            vector<Dinossauro> topDinos;
            vector<int> topDinosPos;
            tie(topDinos, topDinosPos) = getTopN(d, topN_);
    
            vector<Dinossauro>::iterator it = topDinos.begin();
            Dinossauro dino = *it;
            int i = 0;
            for (; it != topDinos.end(); ++it)
            {
                dino = *it;
    
                vector<double> lastGenBestDNA = pointerArrayToVector(dino.DNA, dino.TamanhoDNA);
                updateDNADaVezByDinoId(lastGenBestDNA, i++);
            }
        }
    }
};