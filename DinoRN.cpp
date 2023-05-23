
#define PASSARO_CODIGO_TIPO 5
#define ESPINHO_CODIGO_TIPO 6

#define MODO_JOGO 0 /// 0 = TREINANDO   - OBS: Aumentar tamanho da populacao para 2000
                    /// 1 = JOGAVEL     - OBS: Diminuir tamanho da populacao para 1

#define POPULACAO_TAMANHO 500

#define DINO_BRAIN_QTD_LAYERS 1 /// Quantidade de camadas escondidas na rede neural
#define DINO_BRAIN_QTD_INPUT 6  /// Quantidade de neuronios na camada de entrada
#define DINO_BRAIN_QTD_HIDE 6   /// Quantidade de neuronios nas camadas escondidas
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

#include "EvolutionaryStrategy.cpp"
#include "RandMutations.cpp"
#include "utils.cpp"

///////////////////////////////////////////////////
Dinossauro lastGenBestDino;

char evoMethodName[100];

void DesenharThread() /// Fun��o chamada pela Thread responsavel por desenhar na tela
{
    while (PIG_jogoRodando() == 1)
    {
        vector<Dinossauro> d = arrayToVector(Dinossauros);
        vector<Dinossauro> topFive = getTopFive(d);
        Desenhar(topFive, lastGenBestDino, evoMethodName);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
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

void ControlarEstadoDinossauros() /// Fun��o responsavel por calcular a decis�o da rede neural e aplicar no dinossauro (ou seja, � a fun��o que faz ele pular, abaixar ou usar o aviao)
{
    int Abaixar = 0, Pular = 0, Aviao = 0;
    double Saida[10];
    double Entrada[10];

    for (int i = 0; i < QuantidadeDinossauros; i++)
    {
        if (Dinossauros[i].Estado != 3)
        {
            Entrada[0] = DistanciaProximoObstaculo(Dinossauros[i].X);
            Entrada[1] = LarguraProximoObstaculo(Dinossauros[i].X);
            Entrada[2] = AlturaProximoObstaculo(Dinossauros[i].X);
            Entrada[3] = ComprimentoProximoObstaculo(Dinossauros[i].X);
            Entrada[4] = fabs(VELOCIDADE);
            Entrada[5] = Dinossauros[i].Y;

            RNA_CopiarParaEntrada(Dinossauros[i].Cerebro, Entrada); /// Enviando informa��es para a rede neural
            RNA_CalcularSaida(Dinossauros[i].Cerebro);              /// Calculando a decis�o da rede
            RNA_CopiarDaSaida(Dinossauros[i].Cerebro, Saida);       /// Extraindo a decis�o para vetor ''saida''

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

            if (Dinossauros[i].Estado == 0) /// Em p�
            {
                Dinossauros[i].SpriteAtual = 0 + Dinossauros[i].Frame;
            }
            if (Dinossauros[i].Estado == 1) /// Deitado
            {
                Dinossauros[i].SpriteAtual = 2 + Dinossauros[i].Frame;
            }
            if (Dinossauros[i].Estado == 2) /// Pulando
            {
                Dinossauros[i].SpriteAtual = 4 + Dinossauros[i].Frame;
            }
            if (Dinossauros[i].Estado == 3) /// Muerto
            {
                Dinossauros[i].SpriteAtual = 6 + Dinossauros[i].Frame;
            }
            if (Dinossauros[i].Estado == 4) /// Voando
            {
                Dinossauros[i].SpriteAtual = 8 + Dinossauros[i].Frame;
            }
        }
    }
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

void VerificarFimDePartida(unique_ptr<EvolutionaryStrategy> &&Strategy)
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

            Strategy->Evolve(d, DNAs);
            lastGenBestDino = Strategy->getLastGenBestDino();
        }
        InicializarNovaPartida();
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
    }

    void startGame()
    {
        ConfiguracoesIniciais();

        std::thread Desenho(DesenharThread);

        while (PIG_jogoRodando() == 1)
        {
            AtualizarJanela();
            VerificarTeclas();

            if (TempoDecorrido(TimerGeral) >= Periodo)
            {
                MovimentarChao();
                MovimentarMontanhas();
                MovimentarNuvem();
                MovimentarObstaculos();
                MovimentarDinossauros();

                AtualizarFramePassaro();
                AtualizarFrameDinossauro();
                AtualizarFrameAviao();
                AtualizarMelhorDinossauro();
                AplicarGravidade();
                AplicarColisao();
                ControlarEstadoDinossauros();

                if (fabs(VELOCIDADE) < 8)
                {
                    VELOCIDADE = VELOCIDADE - 0.0005;
                }

                DistanciaAtual = DistanciaAtual + fabs(VELOCIDADE);
                if (DistanciaAtual > 1000000 && DistanciaAtual > DistanciaRecorde)
                {
                    // SalvarRedeArquivo();
                    DinossaurosMortos = POPULACAO_TAMANHO;
                }

                VerificarFimDePartida(move(strategy_));
                ReiniciarTimer(TimerGeral);
            }
        }
        FinalizarJanela();
    }
};