

double *DNADaVez[POPULACAO_TAMANHO];

double MediaFitnessPopulacao[LARG_GRAFICO];
double MediaFitnessFilhos[LARG_GRAFICO];
double BestFitnessPopulacao[LARG_GRAFICO];

int GeracaoCompleta = 0;

double BestFitnessGeracao()
{
    double Maior = 0;
    for (int i = 0; i < POPULACAO_TAMANHO; i++)
    {
        if (Dinossauros[i].Fitness > Maior)
        {
            Maior = Dinossauros[i].Fitness;
        }
    }
    return Maior;
}

double BestFitnessGeracaoParcial()
{
    int partidasContadas = partidaAtual + 1;
    double Maior = 0;

    for (int i = 0; i < POPULACAO_TAMANHO; i++)
    {
        double mediaParcial = (FitnessSoma[i] + Dinossauros[i].Fitness) / partidasContadas;
        if (mediaParcial > Maior)
            Maior = mediaParcial;
    }
    return Maior;
}

double MediaFitnessGeracao()
{
    double Media = 0;
    for (int i = 0; i < POPULACAO_TAMANHO; i++)
    {
        Media = Media + Dinossauros[i].Fitness;
    }
    Media = Media / (double)POPULACAO_TAMANHO;
    return Media;
}

double BestFitnessEver()
{
    double Maior = 0;
    for (int i = 0; i < GeracaoCompleta; i++)
    {
        if (BestFitnessPopulacao[i] > Maior)
        {
            Maior = BestFitnessPopulacao[i];
        }
    }
    return Maior;
}

#define GRAPH_YMAX_INICIAL 15000.0

double FitnessMaximoGrafico()
{
    if (GeracaoCompleta == 0)
        return GRAPH_YMAX_INICIAL;

    double ref = BestFitnessPopulacao[GeracaoCompleta - 1];
    if (ref <= 0)
        return GRAPH_YMAX_INICIAL;

    return ref * 3.0;
}

void InicializarDNA()
{
    int TamanhoDNA = Dinossauros[0].TamanhoDNA;

    for (int i = 0; i < POPULACAO_TAMANHO; i++)
    {
        DNADaVez[i] = (double *)malloc(TamanhoDNA * sizeof(double));
        for (int j = 0; j < TamanhoDNA; j++)
        {
            DNADaVez[i][j] = getRandomValue();
        }
    }
}
