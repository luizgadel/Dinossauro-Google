# Como executar

## Passo 1: A biblioteca PIG

### Baixando

Esse projeto utiliza a biblioteca PIG originalmente escrita por JVictorDias, ajustada com as alterações de brccabral e, posteriormente, por alterações minhas também. Dessa forma, é preciso utilizar a versão fork da biblioteca PIG disponível no meu perfil. Baixe-a pelo git.

### Ajustando o arquivo Makefile

Após baixada, verifique em qual pasta ela está contida e ajuste o caminho definido no arquivo Makefile deste projeto para a biblioteca. Os trechos que precisarão de ajustes são:

#### Caso execute em Windows:
```
	INCLUDE_PATHS := -IC:\CPP_Projects\pig-brccabral\include
	INCLUDE_PATHS += -IC:\CPP_Projects\pig-brccabral\include\SDL
	INCLUDE_PATHS += -IC:\CPP_Projects\pig-brccabral\include\SDL_image
```
#### No caso de Linux:
```
	INCLUDE_PATHS := -I$(HOME)/Documentos/repos/PIG/include
	INCLUDE_PATHS += -I$(HOME)/Documentos/repos/PIG/include/SDL
	INCLUDE_PATHS += -I$(HOME)/Documentos/repos/PIG/include/SDL_image
```

### Branch correta

Após esse ajuste, dentro da pasta do Dinossauro_Google, ao executar ```make``` no terminal você deve receber vários erros relacionados com:
- funções redefinidas em vários arquivos; e
- conversões proibidas de variáveis do tipo char*.

Já existem correções para esses erros na branch `luizgadel_patch_1` do repositório PIG, sendo assim, navegue até o caminho da biblioteca no seu computador e troque para a branch com as correções utilizando o comando `git checkout luizgadel_patch_1`.

Agora, ao executar `make` outra vez você deve receber os seguintes erros:
```
/usr/bin/ld: não foi possível localizar -lSDL2: Arquivo ou diretório inexistente
/usr/bin/ld: não foi possível localizar -lSDL2_ttf: Arquivo ou diretório inexistente
/usr/bin/ld: não foi possível localizar -lSDL2_image: Arquivo ou diretório inexistente
collect2: error: ld returned 1 exit status
make: *** [Makefile:50: all] Erro 1
```

Esse erro indica que a biblioteca SDL2 e suas demais versões estão faltando, o que é esperado, pois não são incluídas por padrão. O próxima passo é buscá-las!

## Passo 2: as bibliotecas de desenho SDL2

