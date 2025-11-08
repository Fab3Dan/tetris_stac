# Tetris Stack (Fila + Pilha em C)

Implementação em C do gerenciamento de peças do jogo Tetris Stack usando:
- Fila circular (capacidade 5) para peças futuras.
- Pilha (capacidade 3) para peças reservadas.

O programa oferece operações dos níveis novato, aventureiro e mestre em um único menu:
- 1: Jogar peça (dequeue da fila). Gera nova peça automaticamente para manter a fila cheia.
- 2: Reservar peça (move frente da fila para o topo da pilha). Gera nova peça automaticamente.
- 3: Usar peça reservada (pop da pilha). Gera nova peça automaticamente para manter a fila cheia.
- 4: Trocar peça atual (frente da fila) com o topo da pilha.
- 5: Trocar em bloco (3 primeiros da fila) com (3 da pilha), se possível.
- 0: Sair.

## Compilação (Windows)

Você pode compilar com MSVC (cl) ou MinGW-w64 (gcc).

### MSVC (Developer Command Prompt)
```
cl /Fe:tetris_stack.exe super_trunfo.c
```

### GCC (MinGW-w64)
```
gcc -std=c11 -O2 -Wall -Wextra -o tetris_stack.exe super_trunfo.c
```

Ou use o script:
```
build.bat
```

## Execução
```
./tetris_stack.exe
```

## Estrutura
- super_trunfo.c: código-fonte principal com fila circular, pilha e menu.
- build.bat: script de build automático (tenta MSVC e, se não disponível, GCC).
- .gitignore: ignora artefatos de build e IDE.

## Observações
- As peças são geradas automaticamente por `gerarPeca()` com tipos aleatórios entre {I, O, T, L} e `id` sequencial.
- A fila é mantida sempre cheia (5) sempre que possível após ações de remoção/movimentação.

## Créditos
- Projeto desenvolvido por: **Fabiane Scolari**
- Aluna de Ciência da Computação – 3º semestre
