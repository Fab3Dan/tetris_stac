#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

/* Compatibilidade para leitura segura no MSVC sem quebrar outros compiladores */
#if defined(_MSC_VER)
#define SCANF_INT(fmt, ptr) scanf_s(fmt, ptr)
#else
#define SCANF_INT(fmt, ptr) scanf(fmt, ptr)
#endif

/* Representa uma peca do Tetris Stack */
typedef struct {
    char nome;  /* Tipo: 'I', 'O', 'T', 'L' */
    int id;     /* Identificador sequencial unico */
} Peca;

/* Fila circular de pecas futuras */
typedef struct {
    Peca itens[MAX_FILA];
    int inicio; /* indice do primeiro elemento */
    int fim;    /* indice logo apos o ultimo elemento */
    int total;  /* quantidade atual de elementos */
} Fila;

/* Pilha de reserva */
typedef struct {
    Peca itens[MAX_PILHA];
    int topo; /* indice do topo; -1 indica pilha vazia */
} Pilha;

/* ------------- Utilidades ------------- */
static int proximoId = 0;

char tipoAleatorio(void) {
    const char tipos[4] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca(void) {
    Peca p;
    p.nome = tipoAleatorio();
    p.id = proximoId++;
    return p;
}

/* ------------- Operacoes de Fila ------------- */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        return 0; /* falha: fila cheia */
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
    return 1; /* sucesso */
}

int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        return 0; /* falha: fila vazia */
    }
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return 1; /* sucesso */
}

Peca frenteFila(Fila *f, int *ok) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) {
        if (ok) *ok = 0;
        return vazio;
    }
    if (ok) *ok = 1;
    return f->itens[f->inicio];
}

void mostrarFila(Fila *f) {
    printf("Fila de pecas ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

/* ------------- Operacoes de Pilha ------------- */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == (MAX_PILHA - 1);
}

int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->itens[p->topo] = x;
    return 1;
}

int pop(Pilha *p, Peca *x) {
    if (pilhaVazia(p)) return 0;
    *x = p->itens[p->topo];
    p->topo--;
    return 1;
}

int peek(Pilha *p, Peca *x) {
    if (pilhaVazia(p)) return 0;
    *x = p->itens[p->topo];
    return 1;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

/* ------------- Funcoes de jogo ------------- */
/* Mantem a fila cheia (se possivel) gerando novas pecas ate atingir MAX_FILA */
void reabastecerFila(Fila *f) {
    while (!filaCheia(f)) {
        enfileirar(f, gerarPeca());
    }
}

/* Troca a peca da frente da fila com o topo da pilha */
int trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) return 0;
    int idxFrente = f->inicio;
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    return 1;
}

/* Troca em bloco: primeiros 3 da fila com 3 do topo da pilha */
int trocarBloco3(Fila *f, Pilha *p) {
    if (f->total < 3 || p->topo + 1 < 3) return 0;
    /* indices da fila: inicio, inicio+1, inicio+2 (mod circular) */
    int idx0 = f->inicio;
    int idx1 = (f->inicio + 1) % MAX_FILA;
    int idx2 = (f->inicio + 2) % MAX_FILA;
    /* indices da pilha: topo, topo-1, topo-2 */
    int t0 = p->topo;
    int t1 = p->topo - 1;
    int t2 = p->topo - 2;

    Peca tmp0 = f->itens[idx0];
    Peca tmp1 = f->itens[idx1];
    Peca tmp2 = f->itens[idx2];

    f->itens[idx0] = p->itens[t0];
    f->itens[idx1] = p->itens[t1];
    f->itens[idx2] = p->itens[t2];

    p->itens[t0] = tmp0;
    p->itens[t1] = tmp1;
    p->itens[t2] = tmp2;

    return 1;
}

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    mostrarFila(f);
    mostrarPilha(p);
}

void menu(void) {
    printf("\nOpcoes:\n");
    printf("1 - Jogar peca (remover da frente da fila)\n");
    printf("2 - Reservar peca (mover frente da fila para pilha)\n");
    printf("3 - Usar peca reservada (remover topo da pilha)\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Troca multipla (3 primeiros da fila com 3 da pilha)\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Preenche fila inicialmente com 5 pecas */
    reabastecerFila(&fila);

    int opcao;
    int executando = 1;

    while (executando) {
        mostrarEstado(&fila, &pilha);
        menu();
        if (SCANF_INT("%d", &opcao) != 1) {
            /* limpa entrada invalida */
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        switch (opcao) {
            case 1: {
                /* Jogar peca (dequeue) */
                Peca jogada;
                if (desenfileirar(&fila, &jogada)) {
                    printf("Jogada: [%c %d]\n", jogada.nome, jogada.id);
                    /* Mantem fila cheia */
                    reabastecerFila(&fila);
                } else {
                    printf("Fila vazia. Nao ha peca para jogar.\n");
                }
                break;
            }
            case 2: {
                /* Reservar peca (mover frente da fila para pilha) */
                if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia. Nao e possivel reservar.\n");
                    break;
                }
                if (filaVazia(&fila)) {
                    printf("Fila vazia. Nao ha peca para reservar.\n");
                    break;
                }
                /* remove da fila e empilha */
                Peca movida;
                desenfileirar(&fila, &movida);
                push(&pilha, movida);
                printf("Reservada: [%c %d]\n", movida.nome, movida.id);
                /* Mantem fila cheia */
                reabastecerFila(&fila);
                break;
            }
            case 3: {
                /* Usar peca reservada (pop da pilha) */
                Peca usada;
                if (pop(&pilha, &usada)) {
                    printf("Usada da reserva: [%c %d]\n", usada.nome, usada.id);
                    /* Mantem fila cheia */
                    reabastecerFila(&fila);
                } else {
                    printf("Pilha vazia. Nao ha peca reservada para usar.\n");
                }
                break;
            }
            case 4: {
                /* Trocar frente da fila com topo da pilha */
                if (trocarFrenteComTopo(&fila, &pilha)) {
                    printf("Troca realizada entre frente da fila e topo da pilha.\n");
                } else {
                    printf("Nao foi possivel trocar (verifique se fila e pilha possuem elementos).\n");
                }
                break;
            }
            case 5: {
                /* Troca multipla de 3 elementos */
                if (trocarBloco3(&fila, &pilha)) {
                    printf("Troca multipla realizada (3 da fila <-> 3 da pilha).\n");
                } else {
                    printf("Nao foi possivel realizar a troca multipla (exigem-se 3 elementos em cada estrutura).\n");
                }
                break;
            }
            case 0: {
                executando = 0;
                break;
            }
            default: {
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }
    }

    printf("Encerrando...\n");
    return 0;
}
