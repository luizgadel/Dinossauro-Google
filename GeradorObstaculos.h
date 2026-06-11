#include <stdlib.h>
#include <time.h>
#include <chrono>

/// srand() afeta todo rand() do processo (obstáculos, AG, posição inicial dos dinos).
void DefinirSeedObstaculos()
{
    unsigned int seed;

    if (obstaculosSeedFixa > 0)
        seed = obstaculosSeedFixa;
    else
        seed = (unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count();

    srand(seed);
}

int TipoObstaculoAleatorio()
{
    return OBSTACULO_TIPO_MIN + rand() % OBSTACULO_TIPO_COUNT;
}

void GerarListaObstaculos()
{
    DefinirSeedObstaculos();

    int Largura;

    obstaculosModelo[0].X = 1250;
    obstaculosModelo[0].Tipo = TipoObstaculoAleatorio();

    if (obstaculosModelo[0].Tipo == PASSARO_CODIGO_TIPO)
        obstaculosModelo[0].Y = 20 + rand() % 65;
    else
        obstaculosModelo[0].Y = 15;

    for (int i = 1; i < 20000; i++)
    {
        obstaculosModelo[i].Tipo = TipoObstaculoAleatorio();

        /// ---------------

        Sprite *sprit = getObstaculosSprite(obstaculosModelo[i - 1].Tipo, 0);
        Largura = sprit->Largura;

        obstaculosModelo[i].X = obstaculosModelo[i - 1].X + Largura + 500 + ((rand() % 200) - 100);

        /// ---------------

        if (obstaculosModelo[i].Tipo == PASSARO_CODIGO_TIPO)
        {
            obstaculosModelo[i].Y = 20 + rand() % 65;
        }
        else
        {
            obstaculosModelo[i].Y = 15;
        }
    }

    FILE *f = fopen("obstaculos.dat", "wb");
    fwrite(obstaculosModelo, 20000, sizeof(Obstaculo), f);
    fclose(f);
}

/// Legado / debug — não usada no fluxo de treino atual. Ver GerarListaObstaculos().
void GerarListaObstaculosTreinoSemEspinho()
{
    int Largura;

    obstaculosModelo[0].X = 1250;
    obstaculosModelo[0].Y = 15;
    obstaculosModelo[0].Tipo = 0;

    for (int i = 1; i < 10000; i++)
    {
        if (i < 600)
        {
            obstaculosModelo[i].Tipo = 5;
        }
        else
        {
            obstaculosModelo[i].Tipo = (i - 600) / 300;
            if (obstaculosModelo[i].Tipo > 5)
            {
                obstaculosModelo[i].Tipo = 5;
            }
        }
        // obstaculosModelo[i].Tipo = 0;

        /// ---------------

        Sprite *sprit = getObstaculosSprite(obstaculosModelo[i - 1].Tipo, 0);
        Largura = sprit->Largura;

        obstaculosModelo[i].X = obstaculosModelo[i - 1].X + Largura + 400 + ((rand() % 200) - 100);

        /// ---------------

        if (obstaculosModelo[i].Tipo == PASSARO_CODIGO_TIPO)
        {
            obstaculosModelo[i].Y = 20 + i % 65;
        }
        else
        {
            obstaculosModelo[i].Y = 15;
        }
    }

    FILE *f = fopen("obstaculos.dat", "wb");
    fwrite(obstaculosModelo, 10000, sizeof(Obstaculo), f);
    fclose(f);

    f = fopen("obstaculos.txt", "w");
    for (int i = 0; i < 10000; i++)
    {
        fprintf(f, "%d %f %f\n", obstaculosModelo[i].Tipo, obstaculosModelo[i].X, obstaculosModelo[i].Y);
    }
    fclose(f);
}

void CarregarListaObstaculos()
{
    FILE *f = fopen("obstaculos.dat", "rb");
    fread(obstaculosModelo, 20000, sizeof(Obstaculo), f);
    fclose(f);
}
