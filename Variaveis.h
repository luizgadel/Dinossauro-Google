#define NUM_NN_WORKER_THREADS 0 /// 0 = auto: max(1, hardware_concurrency() - 2)

PIG_Cor Cores[8] = {CINZA, AMARELO, VERDE, VERMELHO, AZUL, CIANO, LARANJA, ROXO};

Dinossauro Dinossauros[POPULACAO_TAMANHO];
int QuantidadeDinossauros = 0;

Chao chao[CHAO_QUANTIDADE];
Montanha montanha[MONTANHA_QUANTIDADE];
Nuvem nuvem[NUVEM_QUANTIDADE];
Grafico grafico;
Dinossauro *MelhorDinossauro;

Obstaculo obstaculo[MAX_OBSTACULOS];
Obstaculo obstaculosModelo[OBSTACULOS_MODELO_TAMANHO];

int Fonte, FonteVermelha, FonteAzul;
double VELOCIDADE;
int TimerGeral = 0;
int TimerSimTPS = 0;
double TempoExecucao = 0.0;
char HoraInicioExecucao[16] = "00:00:00";
double SimTPS = 0.0;
double Periodo = 0.0005;
double DistanciaRecorde, DistanciaAtual;

int DinossaurosMortos;
int ObstaculoDaVez = 1;
int Geracao;
int DesenharTela = 1;

unsigned int obstaculosSeedFixa = 0; /// 0 = seed automática independente por partida; >0 = seed fixa via CLI

int partidaAtual = 0;
double FitnessSoma[POPULACAO_TAMANHO];
