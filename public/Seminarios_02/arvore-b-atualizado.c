#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 4

struct BTreeNode {
    int num_chaves; //numero de chaves atualmente no nó
    int chaves[M - 1]; // Array de chaves
    struct BTreeNode *filhos[M]; //array de ponteiros para filhos do atual nó
    bool eh_folha; //booleano verificador se o nó é uma folha ou nao
};

// Função para criar um novo nó
struct BTreeNode *criarNo(bool eh_folha) {
	//alocacao dinamica
    struct BTreeNode *novoNo = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    
    if (novoNo == NULL) {
        perror("Falha na alocação de memória");
        exit(EXIT_FAILURE);
    }
    //cria o novo nó (que será uma folha)
    novoNo->num_chaves = 0;
    novoNo->eh_folha = eh_folha;
    for (int i = 0; i < M; i++) {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

// Função para dividir um filho cheio
void dividirFilho(struct BTreeNode *pai, int indice) {
    struct BTreeNode *filho = pai->filhos[indice];
    struct BTreeNode *novoNo = criarNo(filho->eh_folha);

	//movendo metade das chaves de um novo para o novoNo
    novoNo->num_chaves = M / 2 - 1;
    for (int i = 0; i < M / 2 - 1; i++) {
        novoNo->chaves[i] = filho->chaves[i + M / 2];
    }

	//se o filho n for folha, moveremos seus filhos também
    if (!filho->eh_folha) {
        for (int i = 0; i < M / 2; i++) {
            novoNo->filhos[i] = filho->filhos[i + M / 2];
        }
    }

    filho->num_chaves = M / 2 - 1;

    // Deslocar filhos do pai para abrir espaço para o novo nó
    for (int i = pai->num_chaves; i > indice; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[indice + 1] = novoNo;

    // Deslocar chaves do pai para inserir a chave do meio do filho
    for (int i = pai->num_chaves - 1; i >= indice; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }
	
	//chave do meio é promovida a pai
    pai->chaves[indice] = filho->chaves[M / 2 - 1];
    pai->num_chaves++;
}

// Função para inserir uma chave em um nó não cheio
void inserirNaoCheio(struct BTreeNode *no, int chave) {
    int i = no->num_chaves - 1;

    if (no->eh_folha) {
        // Inserir chave na ordem crescente
        while (i >= 0 && no->chaves[i] > chave) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->num_chaves++;
    } else {
        // Encontrar o filho onde a chave deve ser inserida
        while (i >= 0 && no->chaves[i] > chave) {
            i--;
        }
        i++;

        if (no->filhos[i]->num_chaves == M - 1) {
            // Dividir o filho se estiver cheio
            dividirFilho(no, i);

            // Determinar qual dos dois filhos é o novo
            if (no->chaves[i] < chave) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], chave);
    }
}

// Função para inserir uma chave na árvore B
void inserir(struct BTreeNode **raiz, int chave) {
    struct BTreeNode *no = *raiz;

    if (no == NULL) {
        // Criar um novo nó raiz
        *raiz = criarNo(true);
        (*raiz)->chaves[0] = chave;
        (*raiz)->num_chaves = 1;
    } else {
        if (no->num_chaves == M - 1) {
            // Dividir a raiz se estiver cheia
            struct BTreeNode *nova_raiz = criarNo(false);
            nova_raiz->filhos[0] = no;
            dividirFilho(nova_raiz, 0);
            *raiz = nova_raiz;
        }
        inserirNaoCheio(*raiz, chave);
    }
    system("clear");
    printf("[%d] inserido na árvore com sucesso!\n\n", chave);
}

// Função para calcular a altura da árvore
int altura(struct BTreeNode *raiz) {
	//Função recursiva que conta a altura da árvore (da raiz até a folha)
    if (raiz == NULL) return 0;
    return 1 + altura(raiz->filhos[0]);
}

// Função para imprimir a árvore B em níveis
void imprimirNivel(struct BTreeNode *raiz, int nivel) {
    if (raiz == NULL) return;

    if (nivel == 1) {
        for (int i = 0; i < raiz->num_chaves; i++) {
            printf("[%d]", raiz->chaves[i]);
        }
    } else { /*Se o nó estiver no nivel correta, imprima-o, caso contrario, 
		chama a função recursivamente (desce de nivel)*/
        for (int i = 0; i < raiz->num_chaves + 1; i++) {
            imprimirNivel(raiz->filhos[i], nivel - 1);
            if (i < raiz->num_chaves) {
                printf("    "); // Espaço entre os nós de diferentes filhos
            }
        }
    }
}

// Função para imprimir todos os níveis de uma vez
void imprimirTodosOsNiveis(struct BTreeNode *raiz) {
    int h = altura(raiz);
    system("clear");
    for (int i = 1; i <= h; i++) {
        if(i == 1)
            printf("RAIZ: ");
        else
            printf("NIVEL %d: ", i-1);

        // Cálculo de espaços para centralizar
        int espacos = (1 << (h - i + 1)) - 1; // 2^(h-i+1) - 1
        for (int j = 0; j < espacos; j++) {
            printf("      ");
        }

        imprimirNivel(raiz, i);
        printf("\n"); // Nova linha após cada nível
    }
}

//função para buscar um elemento na árvore B e contar comparações
bool buscar(struct BTreeNode *no, int chave, int *comparacoes) {
    if (no == NULL) {
        return false;
    }

	//este loop percorre as chaves do nó atual, comparando cada uma com a chave que a que estamos procurando
    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i]) { // garante que o loop nao ultrapasse o maximo de chaves no nó
        (*comparacoes)++;
        i++;
    }
	
	//se estivermos dentro do limite de chaves do nó e a chave a ser procurado for igual a chave que encontramos, entao returna true
    if (i < no->num_chaves && chave == no->chaves[i]) {
        (*comparacoes)++;
        return true; // Chave encontrada
    }
	
	//se nao for encontrada, e o nó for uma folha, significa que a chave nao foi encontrada
    if (no->eh_folha) {
        return false; // Chave não encontrada
    }

    (*comparacoes)++;
    return buscar(no->filhos[i], chave, comparacoes); // Busca no filho apropriado
}

// Função principal para testar a implementação da árvore B
int main() {
    struct BTreeNode *raiz = NULL;
    int valor, opcao;

    while (1) {
		printf("\n=========================\n");
		printf("\tÁRVORE B\n");
		printf("=========================\n");
        printf("\nEscolha uma opção:\n");
        printf("1 - Mostrar árvore\n");
        printf("2 - Adicionar elementos\n");
        printf("3 - Buscar elemento\n");
        printf("0 - Sair\n");
        scanf("%d", &opcao);

        if (opcao == 0) {
            break; // Sair do programa
        } else if (opcao == 1) {
            printf("\n");
            imprimirTodosOsNiveis(raiz);
        } else if (opcao == 2) {
            printf("Insira um valor para adicionar na árvore: ");
            scanf("%d", &valor);
            if (valor != 0) {
                inserir(&raiz, valor);
            }
        } else if (opcao == 3) {
            printf("Insira o valor a ser buscado: ");
            scanf("%d", &valor);
            int comparacoes = 0;
            system("clear");
            if (buscar(raiz, valor, &comparacoes)) {	
                printf("Elemento %d encontrado! \nTotal de %d comparações.\n", valor, comparacoes);
            } else {
                printf("Elemento %d não encontrado. \nTotal de %d comparações feitas.\n", valor, comparacoes);
            }
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}

//...
