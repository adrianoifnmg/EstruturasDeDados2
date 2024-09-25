#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
    int valor;
    struct no *esquerdo, *direito;
    int altura;
}No;

No* novoNo(int x) {
    No* novo = malloc(sizeof(No));
    if(novo){
        novo->valor = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    }
    else
        printf("\nERRO ao alocar nó!\n");
    return novo;
}

int maior(int a, int b){ 
    return (a > b)? a: b;
}

int alturaDoNo (No* no) {
    if(no == NULL)
        return -1;
    else
        return no->altura;
}

short fatorDeBalanceamento (No* no) {
    if (no)
        return(alturaDoNo (no->esquerdo) - alturaDoNo (no->direito));
    else
        return 0;
}

No* rotacaoEsquerda (No *r) {
    No* y; 
    No* f;
    y = r->direito;
    f = y->esquerdo;
    y->esquerdo = r;
    r->direito = f;
    r->altura = maior (alturaDoNo (r->esquerdo), alturaDoNo (r->direito)) + 1;
    y->altura = maior (alturaDoNo (y->esquerdo), alturaDoNo(y->direito)) + 1;
    return y;
}

No* rotacaoDireita (No *r) {
    No* y; 
    No* f;
    y = r->esquerdo;
    f = y->direito;
    y->direito = r;
    r->esquerdo = f;
    r->altura = maior(alturaDoNo (r->esquerdo), alturaDoNo (r->direito)) + 1; 
    y->altura = maior (alturaDoNo (y->esquerdo), alturaDoNo(y->direito)) + 1;
    return y;
}

No* rotacaoDireitaEsquerda (No *r) {
    r->direito = rotacaoDireita (r->direito);
    return rotacaoEsquerda (r);
}

No* rotacaoEsquerdaDireita (No *r) {
    r->esquerdo = rotacaoEsquerda (r->esquerdo);
    return rotacaoDireita(r);
}

No* balancear(No* raiz){
    int fb = fatorDeBalanceamento(raiz);
    
    // Rotação à esquerda
    if(fb < -1 && fatorDeBalanceamento(raiz->direito) <= 0)
         raiz = rotacaoEsquerda(raiz);

    // Rotação à direita
    else if(fb > 1 && fatorDeBalanceamento(raiz->esquerdo) >= 0)
        raiz = rotacaoDireita(raiz);

    // Rotação dupla à esquerda
    else if(fb > 1 && fatorDeBalanceamento(raiz->esquerdo) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    // Rotação dupla à direita
    else if(fb < -1 && fatorDeBalanceamento(raiz->direito) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);
    
    return raiz;
}

No* inserir(No* raiz, int x) {
    if(raiz == NULL) // árvore vazia
        return novoNo(x);
    else{ // inserção será à esquerda ou à direita
        if(x < raiz->valor)
            raiz->esquerdo = inserir(raiz->esquerdo, x);
        else if (x > raiz->valor)
            raiz->direito = inserir(raiz->direito, x);
        else 
            printf("\nInsercao nao realizada!\n");
    }

    // Recalcula a altura de todos os nós entre a raiz e o novo no inserido
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito))+1;

    // verifica a necessidade de rebalancear a árvore raiz balancear(raiz);
    raiz = balancear(raiz);

    return raiz;
}

No* remover(No* raiz, int chave) {
    if(raiz == NULL) {
        printf("Valor nao encontrado!\n");
        return NULL;
    }else{ // procura o nó a remover
        if(raiz->valor == chave){
            // remove nós folhas (nós sem filhos)
            if(raiz->esquerdo == NULL && raiz->direito == NULL) {
                free(raiz);
                return NULL;
            }
            else{
                // remover nós que possuem 2 filhos
                if(raiz->esquerdo != NULL && raiz->direito != NULL) {
                No* aux = raiz->esquerdo;
                while(aux->direito != NULL)
                    aux = aux->direito;
                raiz->valor = aux->valor;
                aux->valor = chave;
                raiz->esquerdo = remover(raiz->esquerdo, chave);
                return raiz;
            }
            else{
                // remover nós que possuem apenas 1 filho
                No* aux;
                if(raiz->esquerdo != NULL)
                    aux = raiz->esquerdo;
                else
                    aux = raiz->direito;
                free(raiz);
                return aux;
            }
        }
        }else{
            if (chave < raiz->valor)
                raiz->esquerdo = remover(raiz->esquerdo, chave);
            else
                raiz->direito = remover(raiz->direito, chave);
        }
        // Recalcula a altura de todos os nós entre a raiz e o novo nó inserido
        raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;
        // verifica a necessidade de rebalancear a árvore raiz balancear(raiz);
        raiz = balancear(raiz);
        return raiz;
    }
}

void imprimir (No* raiz, int nivel) {
    if(raiz){
        imprimir(raiz->direito, nivel + 1); 
        printf("\n\n");

        for(int i = 0; i < nivel; i++)
            printf("\t");

        printf("%d", raiz->valor);
        imprimir(raiz->esquerdo, nivel + 1);
    }
}

int main(){
    int opcao, valor;
    No* raiz = NULL;
    do{
        printf("\n0 - Sair\n1 - Inserir\n2 - Remover\n3 - Imprimir\n\n"); 
        scanf("%d", &opcao);
        switch(opcao) {
            case 0:
                printf("Saindo!!!");
                break;
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
            case 3:
                imprimir(raiz, 1);
                break;
            default:
                printf("Opcao invalida!!!\n");
        }
    }while(opcao != 0);
    return 0;
}
