// Trabalho de:
// Pedro Henrique da Silva Martins Benevides - Matricula: 202502498616
// Enrique Augusto Angelo Pires e Silva - Matricula: 202503198861


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 

// definicao de limites
#define MAX 20 
#define CHAVE_MESTRA "chave mestra" // define o nome para a chave mestra

// criacao da struct Componente
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;


// funcao para remover espaços em branco no início e no fim da string
void trim_whitespace(char *str) {
    int i;
    int inicio = 0;
    int fim = strlen(str) - 1;

    // remove espaços do inicio
    while (inicio <= fim && isspace((unsigned char)str[inicio])) {
        inicio++;
    }

    // remove espaços do fim
    while (fim > inicio && isspace((unsigned char)str[fim])) {
        fim--;
    }

    // move os caracteres para o início do buffer
    for (i = inicio; i <= fim; i++) {
        str[i - inicio] = str[i];
    }
    str[i - inicio] = '\0'; // finaliza a string
}


// funcao para medir tempo
double medirTempoExecucao(int (*algoritmo)(Componente[], int), Componente v[], int n, int *comparacoes) {
    clock_t inicio = clock();
    *comparacoes = algoritmo(v, n);
    clock_t fim = clock();
    
    // retorna o tempo em milissegundos 
    return (double)(fim - inicio) / CLOCKS_PER_SEC * 1000; 
}



// prototipos de funcoes
void mostrarComponentes(Componente v[], int n);
int bubbleSortNome(Componente v[], int n);
int insertionSortTipo(Componente v[], int n);
int selectionSortPrioridade(Componente v[], int n);
int buscaBinariaPorNome(Componente v[], int n, char chave[], int *comparacoes);


// funcao para mostrar componentes
void mostrarComponentes(Componente v[], int n) {
    if (n == 0) {
        printf("\nNenhum componente na mochila.\n");
        return;
    }
    printf("\n==== INVENTARIO DE COMPONENTES ====");
    printf("\n| %-20s | %-15s | %-10s |", "NOME", "TIPO", "PRIORIDADE");
    printf("\n|----------------------|-----------------|------------|");
    for (int i = 0; i < n; i++) {
        v[i].nome[strcspn(v[i].nome, "\n")] = 0;
        v[i].tipo[strcspn(v[i].tipo, "\n")] = 0;
        
        printf("\n| %-20s | %-15s | %10d |", v[i].nome, v[i].tipo, v[i].prioridade);
    }
    printf("\n=======================================================\n");
}

// ordenacao por nome
int bubbleSortNome(Componente v[], int n) {
    int comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0) {
                Componente aux = v[j];
                v[j] = v[j+1];
                v[j+1] = aux;
            }
        }
    }
    return comparacoes;
}

// ordenacao por tipo
int insertionSortTipo(Componente v[], int n) {
    int comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente atual = v[i];
        int j = i - 1;
        while (j >= 0) {
            comparacoes++;
            if (strcmp(v[j].tipo, atual.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else break;
        }
        v[j+1] = atual;
    }
    return comparacoes;
}

// ordenacao por prioridade
int selectionSortPrioridade(Componente v[], int n) {
    int comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (v[j].prioridade < v[menor].prioridade)
                menor = j;
        }
        if (menor != i) {
            Componente aux = v[i];
            v[i] = v[menor];
            v[menor] = aux;
        }
    }
    return comparacoes;
}

// busca binaria por nome
int buscaBinariaPorNome(Componente v[], int n, char chave[], int *comparacoes) {
    int ini = 0, fim = n - 1;
    *comparacoes = 0;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        
        (*comparacoes)++; 
        int r = strcmp(chave, v[meio].nome);

        if (r == 0) return meio;
        else if (r < 0) fim = meio - 1;
        else ini = meio + 1;
    }
    return -1;
}



// funcao principal (main)
int main() {
    Componente componentes[MAX];
    int n = 0; 
    int opcao;
    
    // Variáveis de Estado
    int ordenadoPorNome = 0;
    int chaveMestraEncontrada = 0;
    
    // Variáveis de Medição
    int comparacoes = 0;
    double tempoExec = 0.0;

    while (1) {
        
        // interface de interacao com o usuario
        printf("\n============================================================\n");
        printf("                     PLANO DE FUGA\n");
        printf("============================================================\n");
        printf("Itens na Mochila: %d/%d\n", n, MAX);
        printf("Status da Ordenacao por Nome: %s\n",
                 ordenadoPorNome ? "ORDENADO" : "NAO ORDENADO");
        printf("------------------------------------------------------------\n");
        printf("1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Busca Binaria por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("------------------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        // tratamento para que apenas numeros sejam lidos 
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        getchar();

        // ativar a fuga
        if (opcao == 0) {
            if (chaveMestraEncontrada) {
                printf("\n>>>  CHAVE MESTRA OK! Torre de Fuga ativada! Missão cumprida!\n");
                break;
            } else {
                printf("\n>>>  ERRO! Ativação Bloqueada. Encontre o Componente-Chave ('%s') primeiro (Opção 5).\n", CHAVE_MESTRA);
                continue;
            }
        }
        
        // bloqueia opcoes caso a mochila esteja vazia
        if (n == 0 && (opcao > 2 && opcao < 6)) {
            printf("\n--- ERRO: A mochila está vazia. Adicione componentes primeiro (Opção 1).\n");
            continue;
        }

        switch (opcao) {
            // adiciona componentes ao inventario
            case 1: {
                if (n >= MAX) {
                    printf("\n--- MOCHILA CHEIA! Descarte um item (Opção 2) antes de adicionar.\n");
                    break;
                }
                
                printf("\n--- ADICIONAR COMPONENTE %d ---\n", n + 1);
                
                printf("Nome (ex: %s): ", CHAVE_MESTRA); 
                fgets(componentes[n].nome, 30, stdin);
                componentes[n].nome[strcspn(componentes[n].nome, "\n")] = 0; 
                trim_whitespace(componentes[n].nome); // limpa espacos em branco
                
                printf("Tipo (ex: Suporte, Energia, Controle): "); 
                fgets(componentes[n].tipo, 20, stdin);
                componentes[n].tipo[strcspn(componentes[n].tipo, "\n")] = 0;
                trim_whitespace(componentes[n].tipo); // limnpa espacoes em branco
                
                printf("Prioridade (1 a 10): "); 
                scanf("%d", &componentes[n].prioridade);
                getchar();
                
                n++; 
                ordenadoPorNome = 0; 
                chaveMestraEncontrada = 0;
                printf("\n Componente adicionado com sucesso.\n");
            } break;

            // descarta componentes do inventario
            case 2: {
                 if (n == 0) {
                    printf("\nNenhum componente para descartar!\n");
                    break;
                }
                mostrarComponentes(componentes, n);
                printf("Digite o índice do item a descartar (1 a %d): ", n);
                int idx;
                if (scanf("%d", &idx) != 1 || idx < 1 || idx > n) {
                    printf("Índice inválido ou entrada incorreta!\n");
                    break;
                }
                getchar();

                for (int i = idx - 1; i < n - 1; i++)
                    componentes[i] = componentes[i + 1];

                n--; 
                ordenadoPorNome = 0;
                chaveMestraEncontrada = 0;
                printf("\n Componente %d descartado com sucesso.\n", idx);
            } break;

            // mostra os componentes no inventario 
            case 3:
                mostrarComponentes(componentes, n);
                break;

            // organiza a mochila de acordo com a opcao escolhida pelo usuario 
            case 4: {
                printf("\nEscolha o tipo de ordenação:\n");
                printf("1. Bubble Sort (nome)\n");
                printf("2. Insertion Sort (tipo)\n");
                printf("3. Selection Sort (prioridade)\n");
                printf("Escolha: ");
                int tipo;
                if (scanf("%d", &tipo) != 1) tipo = 0;
                getchar();

                ordenadoPorNome = 0; 

                if (tipo == 1) {
                    tempoExec = medirTempoExecucao(bubbleSortNome, componentes, n, &comparacoes);
                    ordenadoPorNome = 1; 
                    printf("\n Ordenado por NOME!\n");
                }
                else if (tipo == 2) {
                    tempoExec = medirTempoExecucao(insertionSortTipo, componentes, n, &comparacoes);
                    printf("\n Ordenado por TIPO!\n");
                }
                else if (tipo == 3) {
                    tempoExec = medirTempoExecucao(selectionSortPrioridade, componentes, n, &comparacoes);
                    printf("\n Ordenado por PRIORIDADE!\n");
                } else {
                    printf("Opcao de ordenação inválida!\n");
                    break;
                }
                
                mostrarComponentes(componentes, n);
                printf("\n--- Desempenho ---\nComparações: %d\nTempo: %f ms\n", comparacoes, tempoExec);
            } break;

            // opcao de busca binaria
            case 5: {
                if (!ordenadoPorNome) {
                    printf("\nERRO: A Busca Binária requer ordenação por NOME (Opção 4, escolha 1)!\n");
                    break;
                }
                
                char chave_busca[30];
                strcpy(chave_busca, CHAVE_MESTRA); 

                int comp_busca = 0;
                clock_t inicio_busca = clock();
                int pos = buscaBinariaPorNome(componentes, n, chave_busca, &comp_busca);
                clock_t fim_busca = clock();
                double tempo_busca = (double)(fim_busca - inicio_busca) / CLOCKS_PER_SEC * 1000;
                
                printf("\n---  Resultado da Busca pelo Componente-Chave ('%s') ---\n", CHAVE_MESTRA);
                
                if (pos >= 0) {
                    printf("\n>>>  CHAVE MESTRA ENCONTRADA! Ativação Liberada!\n");
                    chaveMestraEncontrada = 1; 
                    printf("Localização: Posição %d\n", pos + 1);
                } else {
                    printf("\n>>>  CHAVE MESTRA NÃO ENCONTRADA. Continue buscando!\n");
                    chaveMestraEncontrada = 0; 
                }
                
                printf("\n--- Desempenho da Busca ---\nComparações: %d\nTempo: %f ms\n", comp_busca, tempo_busca);
            } break;
            
            default:
                printf("Opção inválida!\n");
        }
    }

    printf("\nEncerrando o sistema de organização de componentes.\n");
    return 0;
}