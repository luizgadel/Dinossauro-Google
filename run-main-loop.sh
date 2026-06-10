#!/bin/bash
# Uso: ./run-main-loop.sh <parametro>
# Executa ./main com o parâmetro informado, aguarda terminar, e repete 100 vezes.

set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "Uso: $0 <parametro>" >&2
    echo "Exemplo: $0 1" >&2
    exit 1
fi

PARAM="$1"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if [[ ! -f ./main ]]; then
    echo "Erro: ./main não encontrado. Compile com 'make' antes de executar." >&2
    exit 1
fi

if [[ ! -x ./main ]]; then
    chmod +x ./main
fi

TOTAL=15

for ((i = 1; i <= TOTAL; i++)); do
    echo "---- Execução $i de $TOTAL (parâmetro: $PARAM) ----"
    ./main "$PARAM"
    echo "---- Execução $i finalizada ----"
done

echo "Concluído: $TOTAL execuções de ./main com parâmetro '$PARAM'."
