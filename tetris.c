#include <stdio.h>
#include <stdlib.h> // Para funções de propósito geral (rand, srand)
#include <time.h>   // Para inicialização do gerador de números aleatórios

// --- DEFINIÇÕES E CONSTANTES ---
#define CAPACIDADE_FILA 5 // Tamanho fixo da fila de peças futuras

// --- ESTRUTURA DA PEÇA ---
/**
 * @brief Estrutura para representar uma Peça do Tetris.
 * * Agrupa o tipo da peça (char) e um ID numérico único.
 */
struct Peca {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', 'J', 'S', 'Z')
    int id;    // Identificador único da peça
};

// --- VARIÁVEIS GLOBAIS DA FILA ---
struct Peca fila[CAPACIDADE_FILA]; // O vetor que armazena as peças (Fila Circular)
int frente = 0;                    // Índice da frente da fila (elemento a ser removido)
int tras = -1;                     // Índice da parte de trás da fila (posição para nova inserção)
int contador = 0;                  // Número atual de elementos na fila
int proximo_id = 0;                // Contador global para gerar IDs únicos

// --- PROTÓTIPOS DAS FUNÇÕES ---
struct Peca gerarPeca();
void inicializarFila();
void exibirFila();
void enqueue(struct Peca novaPeca);
struct Peca dequeue();
void menuPrincipal();

// =========================================================================
// FUNÇÕES DE MANIPULAÇÃO DA FILA
// =========================================================================

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 * * Requisito: Peças geradas automaticamente.
 * @return Retorna a nova struct Peca criada.
 */
struct Peca gerarPeca() {
    struct Peca nova;
    // Tipos de peças disponíveis no Tetris (I, O, T, L, J, S, Z)
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    // Define o tipo da peça aleatoriamente
    nova.nome = tipos[rand() % num_tipos];
    
    // Define um ID único e incrementa o contador global
    nova.id = proximo_id++;
    
    return nova;
}

/**
 * @brief Inicializa a fila preenchendo-a com peças geradas aleatoriamente.
 * * Requisito: Inicializar a fila com um número fixo de elementos.
 */
void inicializarFila() {
    // Preenche a fila até a capacidade máxima
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(gerarPeca());
    }
    printf("\n✅ Fila de peças inicializada com %d peças.\n", CAPACIDADE_FILA);
}

/**
 * @brief Insere uma nova peça no final da fila (Enqueue).
 * * Requisito: Inserir nova peça ao final da fila.
 * @param novaPeca A peça a ser inserida.
 */
void enqueue(struct Peca novaPeca) {
    if (contador == CAPACIDADE_FILA) {
        printf("\n❌ Fila Cheia! Não é possível inserir a peça '%c' ID: %d.\n", novaPeca.nome, novaPeca.id);
        return;
    }

    // Lógica da Fila Circular: calcula o novo índice de trás (resto da divisão)
    tras = (tras + 1) % CAPACIDADE_FILA;
    fila[tras] = novaPeca;
    contador++;
    
    // printf("➕ ENQUEUE: Peça '%c' ID: %d inserida na posição %d.\n", novaPeca.nome, novaPeca.id, tras);
}

/**
 * @brief Remove a peça da frente da fila (Dequeue).
 * * Requisito: Jogar uma peça (remove da frente).
 * @return Retorna a peça removida, ou uma peça vazia se a fila estiver vazia.
 */
struct Peca dequeue() {
    struct Peca pecaRemovida = {'\0', -1}; // Peça nula para erro

    if (contador == 0) {
        printf("\n❌ Fila Vazia! Nenhuma peça para jogar.\n");
        return pecaRemovida;
    }

    // Pega a peça na frente
    pecaRemovida = fila[frente];
    
    // Lógica da Fila Circular: calcula o novo índice da frente
    frente = (frente + 1) % CAPACIDADE_FILA;
    contador--;
    
    printf("\n➖ DEQUEUE: Peça jogada -> Tipo: '%c', ID: %d.\n", pecaRemovida.nome, pecaRemovida.id);
    return pecaRemovida;
}

/**
 * @brief Exibe o estado atual da fila, mostrando o tipo e o ID de cada peça.
 * * Requisito: Exibir o estado atual da fila após cada ação.
 */
void exibirFila() {
    printf("\n--- 👁️ ESTADO ATUAL DA FILA DE PEÇAS FUTURAS ---\n");
    
    if (contador == 0) {
        printf("Fila Vazia.\n");
        return;
    }

    // Cabeçalho e informações
    printf("Capacidade: %d | Total de Peças: %d\n", CAPACIDADE_FILA, contador);
    printf("Frente (Dequeue): %d | Trás (Enqueue): %d\n", frente, tras);
    
    // Exibição da fila
    printf("Fila de Peças: ");
    
    int indice = frente;
    for (int i = 0; i < contador; i++) {
        // Exibe o elemento na posição atual do índice
        printf("[%c %d]", fila[indice].nome, fila[indice].id);
        
        // Move para a próxima posição na Fila Circular
        indice = (indice + 1) % CAPACIDADE_FILA;
    }
    printf("\n");
}

// =========================================================================
// FUNÇÃO DE INTERFACE
// =========================================================================

/**
 * @brief Exibe o menu principal e gerencia as ações do jogador.
 * * Requisito: Usabilidade e menu simples.
 */
void menuPrincipal() {
    int escolha;
    
    do {
        printf("\n============================================\n");
        printf("        🧱 TETRIS STACK - FILA DE PEÇAS 🧱\n");
        printf("============================================\n");
        exibirFila(); // Exibe o estado da fila antes do menu
        
        printf("\nOpções de Ação:\n");
        printf("1. Jogar Peça (Dequeue)\n");
        printf("2. Inserir Nova Peça (Enqueue)\n");
        printf("0. Sair\n");
        printf("--------------------------------------------\n");
        printf("Opção: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("\n❌ Entrada inválida. Digite um número inteiro de 0 a 2.\n");
            while (getchar() != '\n'); // Limpa o buffer em caso de entrada não numérica
            continue;
        }
        
        // Limpa o buffer após a leitura do inteiro
        while (getchar() != '\n'); 
        
        switch (escolha) {
            case 1: {
                dequeue();
                break;
            }
            case 2: {
                struct Peca nova = gerarPeca();
                enqueue(nova);
                printf("\n➕ Nova peça gerada -> Tipo: '%c', ID: %d.\n", nova.nome, nova.id);
                break;
            }
            case 0:
                printf("\n👋 Fechando o Tetris Stack. Até logo!\n");
                exit(0);
                break;
            default:
                printf("\n❌ Opção inválida. Por favor, escolha 0, 1 ou 2.\n");
        }
    } while (1);
}

// =========================================================================
// FUNÇÃO PRINCIPAL (MAIN)
// =========================================================================

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // 1. Inicializa a fila com 5 peças
    inicializarFila();
    
    // 2. Inicia o loop do menu principal
    menuPrincipal();

    return 0;
}