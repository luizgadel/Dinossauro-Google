
#define PASSARO_CODIGO_TIPO 5
#define ESPINHO_CODIGO_TIPO 6

#define MODO_JOGO 0 /// 0 = TREINANDO   - OBS: Aumentar tamanho da populacao para 2000
                    /// 1 = JOGAVEL     - OBS: Diminuir tamanho da populacao para 1

#define POPULACAO_TAMANHO 2000

#define DINO_BRAIN_QTD_LAYERS 1 /// Quantidade de camadas escondidas na rede neural
#define DINO_BRAIN_QTD_INPUT 6  /// Quantidade de neuronios na camada de entrada
#define DINO_BRAIN_QTD_HIDE 13   /// Quantidade de neuronios nas camadas escondidas
#define DINO_BRAIN_QTD_OUTPUT 3 /// Quantidade de neuronios na camada de saida

#include "PIG.h"        ///   Biblioteca Grafica
#include "Sprites.h"    ///   Todos os c�digos sobre sprite
#include "redeNeural.c" ///   C�digo da rede neural
#include "Tipos.h"      ///   Defini��es de structs
#include "Variaveis.h"  ///   Variaveis globais
#include "FuncoesAuxiliares.h"
#include "DNAs.h"
#include "Desenhar.h"
#include "GeradorObstaculos.h"
#include "Inicializar.h"
#include "Alocacoes.h"
#include "Colisao.h"
#include "Movimentar.h"
#include "Atualizar.h"
#include "InputsRedeNeural.h" /// Fun��es que captam a informa��o para entregar para a rede neural

#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>

#define SIM_RENDER_INTERVAL_FRAMES 3

#include "ThreadPool.h"
#include "utils.cpp"
#include "EvolutionaryStrategy.cpp"
#include "RandMutations.cpp"
#include "RechenbergMuLambdaES.cpp"
#include "OnePointCrossover.cpp"
#include "NPointCrossover.cpp"
#include "TopNElitism.cpp"
#include "MutationStrategy.cpp"
#include "MutationBySubstitution.cpp"
#include "MutationByMultiplication.cpp"
#include "MutationBySum.cpp"
#include "MutationByRM.cpp"
#include <iostream>

///////////////////////////////////////////////////
Dinossauro lastGenBestDino;

char evoMethodName[100];
char evoMethodArgs[100];
int lastGenSavedToCSV = 0;
vector<Dinossauro> topN;
vector<int> topNPositions;

std::mutex gameStateMutex;

std::unique_ptr<ThreadPool> nnThreadPool;
std::atomic<int> mortesNoTick{0};

int ResolverNumNNWorkerThreads()
{
    if (NUM_NN_WORKER_THREADS > 0)
    {
        return NUM_NN_WORKER_THREADS;
    }

    unsigned int hw = std::thread::hardware_concurrency();
    if (hw <= 2)
    {
        return 1;
    }
    return static_cast<int>(hw) - 2;
}

bool VerificaCondicaoFim()
{
    return (Geracao == 300);
}
void AplicarGravidade()
{
    for (int i = 0; i < QuantidadeDinossauros; i++)
    {
        if (Dinossauros[i].Y > 15)
        {
            if (Dinossauros[i].Estado != 4) /// VOANDO
            {
                Dinossauros[i].VelocidadeY = Dinossauros[i].VelocidadeY - (0.08);
            }
            else
            {
                if (Dinossauros[i].VelocidadeY <= 0)
                {
                    Dinossauros[i].VelocidadeY = 0;
                }
                else
                {
                    Dinossauros[i].VelocidadeY = Dinossauros[i].VelocidadeY - (0.08);
                }
            }

            Dinossauros[i].Y = Dinossauros[i].Y + Dinossauros[i].VelocidadeY;
        }
        else
        {
            Dinossauros[i].VelocidadeY = 0;
            Dinossauros[i].Y = 15;
            if (Dinossauros[i].Estado == 2)
                Dinossauros[i].Estado = 0;
        }
    }
}

void ProcessarDinossauroNN(int i)
{
    if (Dinossauros[i].Estado == 3)
    {
        return;
    }

    int Abaixar = 0, Pular = 0, Aviao = 0;
    double Saida[10];
    double Entrada[10];

    int indiceObstaculo = ProcurarProximoObstaculo(Dinossauros[i].X);
    Obstaculo &obs = obstaculo[indiceObstaculo];

    Entrada[0] = obs.X - Dinossauros[i].X;
    Entrada[1] = obs.sprite[obs.FrameAtual]->Largura;
    Entrada[2] = obs.Y;
    Entrada[3] = obs.sprite[obs.FrameAtual]->Altura;
    Entrada[4] = fabs(VELOCIDADE);
    Entrada[5] = Dinossauros[i].Y;

    RNA_CopiarParaEntrada(Dinossauros[i].Cerebro, Entrada);
    RNA_CalcularSaida(Dinossauros[i].Cerebro);
    RNA_CopiarDaSaida(Dinossauros[i].Cerebro, Saida);

    if (Saida[0] == 0.0)
        Pular = 0;
    else
        Pular = 1;

    if (Saida[1] == 0.0)
        Abaixar = 0;
    else
        Abaixar = 1;

    if (Saida[2] == 0.0)
        Aviao = 0;
    else
        Aviao = 1;

    if (MODO_JOGO == 1 && i == 1)
    {
        Pular = 0;
        Abaixar = 0;
        Aviao = 0;

        if (PIG_teclado[TECLA_CIMA] == 1)
        {
            Pular = 1;
        }
        if (PIG_teclado[TECLA_BAIXO] == 1)
        {
            Abaixar = 1;
        }
        if (PIG_teclado[TECLA_BARRAESPACO] == 1)
        {
            Aviao = 1;
        }

        Saida[0] = Abaixar;
        Saida[1] = Pular;
        Saida[2] = Aviao;
    }

    if (DINO_BRAIN_QTD_OUTPUT == 2)
        Aviao = 0;

    if (Dinossauros[i].Estado != 4) /// Voando
    {
        if (Dinossauros[i].Estado != 2)
        {
            Dinossauros[i].Estado = 0;
        }
        if (Abaixar && Dinossauros[i].Estado != 2)
        {
            Dinossauros[i].Estado = 1;
        }
        if (Abaixar && Dinossauros[i].Estado == 2)
        {
            if (Dinossauros[i].VelocidadeY > 0)
                Dinossauros[i].VelocidadeY = 0;
            Dinossauros[i].Y = Dinossauros[i].Y - 2;
        }
        if (Pular && Dinossauros[i].Estado != 2)
        {
            Dinossauros[i].Estado = 2;
            Dinossauros[i].Y = Dinossauros[i].Y + 1;

            Dinossauros[i].VelocidadeY = Dinossauros[i].VelocidadeY + 4.0;
        }
        if (Aviao && Dinossauros[i].AviaoCooldown <= 0)
        {
            Dinossauros[i].Estado = 4;
            Dinossauros[i].Y = Dinossauros[i].Y + 1;
            if (Dinossauros[i].VelocidadeY <= 0.5 && Dinossauros[i].Y < 25)
            {
                Dinossauros[i].VelocidadeY = Dinossauros[i].VelocidadeY + 4.0;
            }
            Dinossauros[i].AviaoCooldown = 4000.0;
        }
    }
    else
    {
        if (Dinossauros[i].AviaoDeslocamento >= 820.0)
        {
            Dinossauros[i].AviaoDeslocamento = 0;
            Dinossauros[i].Estado = 2;
        }
        else
        {
            Dinossauros[i].AviaoDeslocamento = Dinossauros[i].AviaoDeslocamento + fabs(VELOCIDADE);
        }
    }
    Dinossauros[i].AviaoCooldown = Dinossauros[i].AviaoCooldown - fabs(VELOCIDADE);

    if (MODO_JOGO == 1)
    {
        if (Dinossauros[i].Estado == 0) /// Em pé
            Dinossauros[i].SpriteAtual = 0 + Dinossauros[i].Frame;
        if (Dinossauros[i].Estado == 1) /// Deitado
            Dinossauros[i].SpriteAtual = 2 + Dinossauros[i].Frame;
        if (Dinossauros[i].Estado == 2) /// Pulando
            Dinossauros[i].SpriteAtual = 4 + Dinossauros[i].Frame;
        if (Dinossauros[i].Estado == 3) /// Muerto
            Dinossauros[i].SpriteAtual = 6 + Dinossauros[i].Frame;
        if (Dinossauros[i].Estado == 4) /// Voando
            Dinossauros[i].SpriteAtual = 8 + Dinossauros[i].Frame;
    }
}

void ProcessarColisaoDino(int i)
{
    if (Dinossauros[i].Estado == 3)
        return;

    int IndiceObstaculo = ProcurarProximoObstaculo(Dinossauros[i].X);
    int FatorDeCorrecaoHorizontal = 7;
    int FatorDeCorrecaoVertical = 5;

    double XObstaculo = obstaculo[IndiceObstaculo].X;
    double YObstaculo = obstaculo[IndiceObstaculo].Y;
    double AlturaObstaculo = obstaculo[IndiceObstaculo].sprite[obstaculo[IndiceObstaculo].FrameAtual]->Altura;
    double LarguraObstaculo = obstaculo[IndiceObstaculo].sprite[obstaculo[IndiceObstaculo].FrameAtual]->Largura;

    double DinoX = Dinossauros[i].X + FatorDeCorrecaoHorizontal;
    double DinoY = Dinossauros[i].Y + FatorDeCorrecaoVertical;
    double DinoLarg = Dinossauros[i].sprite[Dinossauros[i].SpriteAtual].Largura - 2 * FatorDeCorrecaoHorizontal;
    double DinoAlt = Dinossauros[i].sprite[Dinossauros[i].SpriteAtual].Altura - 2 * FatorDeCorrecaoVertical;

    if (verificarColisao(DinoX, DinoY, DinoLarg, DinoAlt,
                         XObstaculo, YObstaculo, LarguraObstaculo, AlturaObstaculo) == 1)
    {
        Dinossauros[i].Estado = 3;
        mortesNoTick.fetch_add(1, std::memory_order_relaxed);
    }
}

void AplicarColisaoParalelo()
{
    mortesNoTick.store(0);
    nnThreadPool->parallelFor(0, QuantidadeDinossauros, ProcessarColisaoDino);
    DinossaurosMortos += mortesNoTick.load();
}

void ControlarEstadoDinossauros()
{
    nnThreadPool->parallelFor(0, QuantidadeDinossauros, ProcessarDinossauroNN);
}

void InicializarNovaPartida()
{
    GerarListaObstaculos();
    CarregarListaObstaculos();

    DistanciaAtual = 0;
    VELOCIDADE = -3;
    DinossaurosMortos = 0;

    InicializarObstaculos();

    for (int i = 0; i < POPULACAO_TAMANHO; i++)
    {
        InicializarDinossauro(i, DNADaVez[i], 300 + (rand() % 200 - 100), 15);
    }
}

void EncerrarPartida()
{
    if (DistanciaAtual > DistanciaRecorde)
    {
        DistanciaRecorde = DistanciaAtual;
        SalvarRedeArquivo();
    }
}

void CarregarRede()
{
    FILE *f = fopen("rede", "rb");
    fread(&Dinossauros[0].TamanhoDNA, 1, sizeof(int), f);
    fread(DNADaVez[0], Dinossauros[0].TamanhoDNA, sizeof(double), f);
    fclose(f);
}

void ConfiguracoesIniciais()
{
    CriarJanela("Google Dinossaur", 0);
    InicializarSprites();

    InicializarChao();
    InicializarMontanhas();
    InicializarNuvens();

    AlocarDinossauros();
    AlocarObstaculos();
    CarregarListaObstaculos();
    InicializarGrafico();

    TimerGeral = CriarTimer();
    TimerSimTPS = CriarTimer();
    Fonte = CriarFonteNormal("fontes/arial.ttf", 15, PRETO, 0, PRETO);
    FonteVermelha = CriarFonteNormal("fontes/arial.ttf", 15, VERMELHO, 0, PRETO);
    FonteAzul = CriarFonteNormal("fontes/arial.ttf", 15, AZUL, 0, PRETO);
    DistanciaRecorde = 0;
    Geracao = 0;
    MelhorDinossauro = &Dinossauros[0];

    InicializarDNA();
    InicializarNovaPartida();
}



using namespace std;

void VerificarFimDePartida(EvolutionaryStrategy &strategy)
{
    if (DinossaurosMortos == POPULACAO_TAMANHO)
    {
        EncerrarPartida();
        if (MODO_JOGO == 0)
        {
            /* Atualiza o gráfico a cada fim da partida */
            if (Geracao < LARG_GRAFICO)
            {
                GeracaoCompleta = Geracao + 1;
                BestFitnessPopulacao[Geracao] = BestFitnessGeracao();
                MediaFitnessPopulacao[Geracao] = MediaFitnessGeracao();
            }
            else
            {
                for (int i = 0; i < LARG_GRAFICO - 1; i++)
                {
                    BestFitnessPopulacao[i] = BestFitnessPopulacao[i + 1];
                    MediaFitnessPopulacao[i] = MediaFitnessPopulacao[i + 1];
                }
                BestFitnessPopulacao[GeracaoCompleta] = BestFitnessGeracao();
                MediaFitnessPopulacao[GeracaoCompleta] = MediaFitnessGeracao();
            }

            vector<dinossauro> d = arrayToVector(Dinossauros);

            int tamDNA = (*d.begin()).TamanhoDNA;
            vector<vector<double>> DNAs = matrixToVector(DNADaVez, tamDNA);

            strategy.Evolve(d, DNAs);
            lastGenBestDino = strategy.getLastGenBestDino();
        }
        InicializarNovaPartida();
    }
}

void SimulacaoThread(EvolutionaryStrategy *strategy)/// Thread da simulacao (tick do jogo, independente do render)
{
    int simTickCount = 0;
    while (PIG_jogoRodando() == 1 && !VerificaCondicaoFim())
    {
        if (Periodo <= 0.0 || TempoDecorrido(TimerGeral) >= Periodo)
        {
            simTickCount++;
            std::lock_guard<std::mutex> lock(gameStateMutex);
            MovimentarChao();
            MovimentarMontanhas();
            MovimentarNuvem();
            MovimentarObstaculos();
            MovimentarDinossauros();

            if (MODO_JOGO == 1)
            {
                AtualizarFramePassaro();
                AtualizarFrameDinossauro();
                AtualizarFrameAviao();
            }
            AtualizarMelhorDinossauro();
            AplicarGravidade();
            AplicarColisaoParalelo();
            ControlarEstadoDinossauros();

            if (fabs(VELOCIDADE) < 8)
            {
                VELOCIDADE = VELOCIDADE - 0.0005;
            }

            DistanciaAtual = DistanciaAtual + fabs(VELOCIDADE);
            if (DistanciaAtual > 1000000 && DistanciaAtual > DistanciaRecorde)
            {
                DinossaurosMortos = POPULACAO_TAMANHO;
            }

            VerificarFimDePartida(*strategy);
            ReiniciarTimer(TimerGeral);
        }

        double simElapsed = TempoDecorrido(TimerSimTPS);
        if (simElapsed >= 0.25)
        {
            SimTPS = simTickCount / simElapsed;
            simTickCount = 0;
            ReiniciarTimer(TimerSimTPS);
        }
    }
}

using std::make_unique;
using std::move;

class DinoRN
{
private:
    unique_ptr<EvolutionaryStrategy> strategy_;

public:
    void set_strategy(unique_ptr<EvolutionaryStrategy> &&strategy)
    {
        strategy_ = move(strategy);
    }

    DinoRN(unique_ptr<EvolutionaryStrategy> &&strategy = make_unique<RandMutations>()) : strategy_(move(strategy))
    {
        strcpy(evoMethodName, strategy_->getName());
        strcpy(evoMethodArgs, strategy_->getArgs());
    }

    void startGame()
    {
        ConfiguracoesIniciais();
        nnThreadPool = std::make_unique<ThreadPool>(ResolverNumNNWorkerThreads());

        std::thread simThread(SimulacaoThread, strategy_.get());

        int renderFrame = 0;

        while (PIG_jogoRodando() == 1 && !VerificaCondicaoFim())
        {
            AtualizarJanela();
            VerificarTeclas();
            renderFrame++;

            if (DesenharTela == 1 && renderFrame % SIM_RENDER_INTERVAL_FRAMES == 0)
            {
                vector<Dinossauro> d;
                vector<int> positions;
                Dinossauro bestDinoCopy;
                char methodNameCopy[100];

                {
                    std::lock_guard<std::mutex> lock(gameStateMutex);
                    d = arrayToVector(Dinossauros);
                    tie(topN, topNPositions) = getTopN(d, 10);
                    positions = topNPositions;
                    d = topN;
                    bestDinoCopy = lastGenBestDino;
                    strcpy(methodNameCopy, evoMethodName);
                }

                Desenhar(d, positions, bestDinoCopy, methodNameCopy);
            }
        }

        simThread.join();
        nnThreadPool.reset();
        FinalizarJanela();
    }
};