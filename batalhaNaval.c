C

#include <stdio.h>
#include <stdbool.h> // Para usar 'bool'
#include <stdlib.h>  // Para abs()

// --- Definição das Constantes ---
#define TAMANHO_TABULEIRO 10 // Tamanho do tabuleiro principal (10x10)
#define TAMANHO_NAVIO 3      // Tamanho fixo dos navios

// Tamanho das matrizes de habilidade (deve ser ímpar para ter um centro claro)
#define TAMANHO_HABILIDADE 7 
#define CENTRO_HABILIDADE (TAMANHO_HABILIDADE / 2) // O centro da matriz de habilidade (e.g., 3 para 7x7)

// --- Valores para representação no tabuleiro ---
#define AGUA 0
#define NAVIO 3
#define HABILIDADE_AFETADA 5 // Valor para posições afetadas por habilidade

// --- Enum para representar as orientações dos navios ---
typedef enum {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_PRINCIPAL,    // Linha e coluna aumentam (ex: (0,0), (1,1), (2,2))
    DIAGONAL_SECUNDARIA    // Linha aumenta, coluna diminui (ex: (0,9), (1,8), (2,7))
} OrientacaoNavio;

// --- Funções Auxiliares (do nível anterior, mantidas) ---

// Função para inicializar o tabuleiro com água (0)
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA; // 0 representa água
        }
    }
}

// Função para tentar posicionar um navio no tabuleiro
// Retorna true se o navio foi posicionado com sucesso, false caso contrário
bool posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                     int linha_inicio, int coluna_inicio, OrientacaoNavio orientacao) {

    // 1. Validação de Limites e Verificação de Sobreposição
    bool pode_posicionar = true;

    if (orientacao == HORIZONTAL) {
        if (coluna_inicio < 0 || coluna_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO ||
            linha_inicio < 0 || linha_inicio >= TAMANHO_TABULEIRO) {
            return false; // Fora dos limites
        }
        for (int j = 0; j < TAMANHO_NAVIO; j++) {
            if (tabuleiro[linha_inicio][coluna_inicio + j] != AGUA) {
                pode_posicionar = false;
                break;
            }
        }
    } else if (orientacao == VERTICAL) {
        if (linha_inicio < 0 || linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO ||
            coluna_inicio < 0 || coluna_inicio >= TAMANHO_TABULEIRO) {
            return false; // Fora dos limites
        }
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            if (tabuleiro[linha_inicio + i][coluna_inicio] != AGUA) {
                pode_posicionar = false;
                break;
            }
        }
    } else if (orientacao == DIAGONAL_PRINCIPAL) {
        if (linha_inicio < 0 || linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO ||
            coluna_inicio < 0 || coluna_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO) {
            return false; // Fora dos limites
        }
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            if (tabuleiro[linha_inicio + i][coluna_inicio + i] != AGUA) {
                pode_posicionar = false;
                break;
            }
        }
    } else if (orientacao == DIAGONAL_SECUNDARIA) {
        if (linha_inicio < 0 || linha_inicio + TAMANHO_NAVIO > TAMANHO_TABULEIRO ||
            coluna_inicio < TAMANHO_NAVIO - 1 || coluna_inicio >= TAMANHO_TABULEIRO) {
            return false; // Fora dos limites (coluna_inicio deve permitir decremento)
        }
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            if (tabuleiro[linha_inicio + i][coluna_inicio - i] != AGUA) {
                pode_posicionar = false;
                break;
            }
        }
    }

    if (!pode_posicionar) {
        return false;
    }

    // 2. Posicionamento do Navio
    if (orientacao == HORIZONTAL) {
        for (int j = 0; j < TAMANHO_NAVIO; j++) {
            tabuleiro[linha_inicio][coluna_inicio + j] = NAVIO;
        }
    } else if (orientacao == VERTICAL) {
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            tabuleiro[linha_inicio + i][coluna_inicio] = NAVIO;
        }
    } else if (orientacao == DIAGONAL_PRINCIPAL) {
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            tabuleiro[linha_inicio + i][coluna_inicio + i] = NAVIO;
        }
    } else if (orientacao == DIAGONAL_SECUNDARIA) {
        for (int i = 0; i < TAMANHO_NAVIO; i++) {
            tabuleiro[linha_inicio + i][coluna_inicio - i] = NAVIO;
        }
    }
    return true; // Navio posicionado com sucesso
}

// --- Funções para Criar Matrizes de Habilidade ---

// Cria uma matriz para a habilidade Cone (ponto de origem no topo, expande para baixo)
void criarHabilidadeCone(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // A largura do cone aumenta com a linha (i).
            // A linha central é j = CENTRO_HABILIDADE.
            // abs(j - CENTRO_HABILIDADE) dá a distância horizontal do centro.
            // Para um cone, a largura máxima em cada linha é proporcional à linha atual.
            // A lógica é que, para a linha 'i', o cone se estende 'i' posições para cada lado do centro.
            if (abs(j - CENTRO_HABILIDADE) <= i) {
                habilidade[i][j] = 1;
            } else {
                habilidade[i][j] = 0;
            }
        }
    }
}

// Cria uma matriz para a habilidade Cruz (ponto de origem no centro)
void criarHabilidadeCruz(int habilidade[TAMANO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (i == CENTRO_HABILIDADE || j == CENTRO_HABILIDADE) {
                habilidade[i][j] = 1;
            } else {
                habilidade[i][j] = 0;
            }
        }
    }
}

// Cria uma matriz para a habilidade Octaedro (losango, ponto de origem no centro)
void criarHabilidadeOctaedro(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // A condição para um losango é baseada na distância de Manhattan (abs(dx) + abs(dy))
            // do centro. Para um losango centrado no (CENTRO_HABILIDADE, CENTRO_HABILIDADE),
            // a soma das distâncias absolutas ao centro deve ser menor ou igual a uma constante (raio).
            // Aqui, usamos CENTRO_HABILIDADE como o "raio" máximo.
            if (abs(i - CENTRO_HABILIDADE) + abs(j - CENTRO_HABILIDADE) <= CENTRO_HABILIDADE) {
                habilidade[i][j] = 1;
            } else {
                habilidade[i][j] = 0;
            }
        }
    }
}

// --- Função para Sobrepor Habilidade no Tabuleiro ---

// Aplica a matriz de habilidade no tabuleiro principal, centrada em (linha_alvo, coluna_alvo)
void aplicarHabilidadeNoTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                  const int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE],
                                  int linha_alvo, int coluna_alvo) {

    // Calcula a posição do canto superior esquerdo da matriz de habilidade no tabuleiro
    // para que seu centro coincida com (linha_alvo, coluna_alvo).
    int start_linha_habilidade_no_tabuleiro = linha_alvo - CENTRO_HABILIDADE;
    int start_coluna_habilidade_no_tabuleiro = coluna_alvo - CENTRO_HABILIDADE;

    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Calcula as coordenadas reais no tabuleiro
            int tabuleiro_linha = start_linha_habilidade_no_tabuleiro + i;
            int tabuleiro_coluna = start_coluna_habilidade_no_tabuleiro + j;

            // Verifica se a posição está dentro dos limites do tabuleiro
            if (tabuleiro_linha >= 0 && tabuleiro_linha < TAMANHO_TABULEIRO &&
                tabuleiro_coluna >= 0 && tabuleiro_coluna < TAMANHO_TABULEIRO) {
                
                // Se a posição na matriz de habilidade é '1' (afetada),
                // e a posição no tabuleiro não é um navio, marca como área de habilidade.
                // Priorizamos a visualização do navio sobre a habilidade se houver colisão.
                if (habilidade[i][j] == 1 && tabuleiro[tabuleiro_linha][tabuleiro_coluna] != NAVIO) {
                    tabuleiro[tabuleiro_linha][tabuleiro_coluna] = HABILIDADE_AFETADA;
                }
            }
        }
    }
}

// --- Função para Exibir o Tabuleiro (Modificada para caracteres) ---

// Exibe o tabuleiro no console com caracteres personalizados
void exibirTabuleiroComHabilidades(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("Tabuleiro de Batalha Naval com Habilidades:\n");
    // Imprime os cabeçalhos das colunas
    printf("   "); // Espaço para o cabeçalho das linhas
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%2d ", j); // Imprime números de coluna
    }
    printf("\n");

    // Imprime o tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i); // Imprime número da linha
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == AGUA) {
                printf("~  "); // Água
            } else if (tabuleiro[i][j] == NAVIO) {
                printf("N  "); // Navio
            } else if (tabuleiro[i][j] == HABILIDADE_AFETADA) {
                printf("X  "); // Área afetada pela habilidade
            } else {
                printf("%d  ", tabuleiro[i][j]); // Caso algum valor inesperado apareça
            }
        }
        printf("\n");
    }
}

// --- Função Principal ---
int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int habilidade_cone[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int habilidade_cruz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int habilidade_octaedro[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

    // 1. Inicializa o tabuleiro com água
    inicializarTabuleiro(tabuleiro);

    // 2. Posiciona alguns navios (do nível intermediário)
    printf("Posicionando navios...\n");
    if (posicionarNavio(tabuleiro, 2, 1, HORIZONTAL)) {
        printf("Navio 1 (Horizontal) posicionado em (2,1).\n");
    } else {
        printf("Falha ao posicionar Navio 1 (Horizontal) em (2,1).\n");
    }

    if (posicionarNavio(tabuleiro, 5, 7, VERTICAL)) {
        printf("Navio 2 (Vertical) posicionado em (5,7).\n");
    } else {
        printf("Falha ao posicionar Navio 2 (Vertical) em (5,7).\n");
    }

    if (posicionarNavio(tabuleiro, 0, 0, DIAGONAL_PRINCIPAL)) {
        printf("Navio 3 (Diagonal Principal) posicionado em (0,0).\n");
    } else {
        printf("Falha ao posicionar Navio 3 (Diagonal Principal) em (0,0).\n");
    }

    if (posicionarNavio(tabuleiro, 0, 9, DIAGONAL_SECUNDARIA)) {
        printf("Navio 4 (Diagonal Secundária) posicionado em (0,9).\n");
    } else {
        printf("Falha ao posicionar Navio 4 (Diagonal Secundária) em (0,9).\n");
    }

    printf("\n");

    // 3. Cria as matrizes de habilidade
    printf("Criando matrizes de habilidade...\n");
    criarHabilidadeCone(habilidade_cone);
    criarHabilidadeCruz(habilidade_cruz);
    criarHabilidadeOctaedro(habilidade_octaedro);
    printf("Matrizes de habilidade criadas.\n");

    // 4. Aplica as habilidades no tabuleiro em pontos de origem definidos
    printf("\nAplicando habilidades no tabuleiro...\n");

    // Habilidade Cone centrada em (5,5)
    aplicarHabilidadeNoTabuleiro(tabuleiro, habilidade_cone, 5, 5);
    printf("Habilidade Cone aplicada em (5,5).\n");

    // Habilidade Cruz centrada em (2,7)
    aplicarHabilidadeNoTabuleiro(tabuleiro, habilidade_cruz, 2, 7);
    printf("Habilidade Cruz aplicada em (2,7).\n");

    // Habilidade Octaedro centrada em (8,2)
    aplicarHabilidadeNoTabuleiro(tabuleiro, habilidade_octaedro, 8, 2);
    printf("Habilidade Octaedro aplicada em (8,2).\n");

    printf("\n");

    // 5. Exibe o tabuleiro final com navios e áreas de habilidade
    exibirTabuleiroComHabilidades(tabuleiro);

    return 0;
}
