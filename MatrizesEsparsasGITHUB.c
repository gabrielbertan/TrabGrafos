#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct tipo_elemento {
    int valor;
    int ID_linha;
    struct tipo_elemento *proximo;
}tipo_elemento;

typedef struct tipo_linha{
    int ID_Coluna;
    struct tipo_linha *proximo;
    struct tipo_elemento *inicio;
}tipo_linha;

typedef struct {
    int qtd_linhas;
    int qtd_colunas;
    struct tipo_linha *inicio;
}Tipo_Mat_Esparsa;



Tipo_Mat_Esparsa *criar_matriz_esparsa(){
    Tipo_Mat_Esparsa *mat = (Tipo_Mat_Esparsa*)malloc(sizeof(Tipo_Mat_Esparsa));

    
    mat->qtd_colunas = 0;
    mat->qtd_linhas = 0;

    mat->inicio = NULL;
 
    return mat;
}
int verificar_mat_vazia(Tipo_Mat_Esparsa *mat){
    if(mat->inicio == NULL){
        return 1;
    }
    else{
        return 0;
    }
}
void inserir_tipo_linha_mat(Tipo_Mat_Esparsa *mat, int id){

    if(verificar_mat_vazia(mat)){
        tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
        novo->ID_Coluna = id;
        novo->proximo = NULL;
        novo->inicio = NULL;
        mat->inicio = novo;
        if((mat->qtd_colunas == 0 && id == 0)||(id>=mat->qtd_colunas)){
            mat->qtd_colunas = id + 1;
        }
    }
    else{
        tipo_linha *aux = mat->inicio;

        if(aux->ID_Coluna > id){
            tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
            novo->proximo = aux;
            novo->inicio = NULL;
            novo->ID_Coluna = id;
            mat->inicio = novo;

            if((mat->qtd_colunas == 0 && id == 0)||(id>=mat->qtd_colunas)){
                mat->qtd_colunas = id + 1;
            }
        }
        else{
            while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna < id)){
                aux = aux->proximo;
            }
            if((aux->ID_Coluna != id && aux->proximo == NULL)||((aux->ID_Coluna != id) && (aux->proximo->ID_Coluna > id) && (aux->proximo != NULL))){
                tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
                novo->ID_Coluna = id;
                novo->proximo = aux->proximo;
                novo->inicio = NULL;

                aux->proximo = novo;
                if((mat->qtd_colunas == 0 && id == 0)||(id>=mat->qtd_colunas)){
                    mat->qtd_colunas = id + 1;
                }
            }
        }
    }
}

void inserir_elemento_mat(int linha, int coluna, int value, Tipo_Mat_Esparsa *mat){
 
    if(!verificar_mat_vazia(mat)){

        tipo_linha *aux = mat->inicio;
        
        while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna <= coluna)){ 
            aux = aux->proximo;
        }
        if((aux == NULL)||(aux->ID_Coluna != coluna)){
            inserir_tipo_linha_mat(mat, coluna);
 
            if(coluna > aux->ID_Coluna){
                aux = aux->proximo;
            }
            else{
                aux = mat->inicio;
            }
        }

        if(aux->inicio == NULL){ 
            tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
            aux3->ID_linha = linha;
            aux3->valor = value;
            aux3->proximo = NULL;
            aux->inicio = aux3;
            if((linha >= mat->qtd_linhas) || (linha == 0 && mat->qtd_linhas == 0)){
                mat->qtd_linhas = linha + 1;
            }
        }
        else{
            tipo_elemento *aux2;
            aux2 = aux->inicio;

            if(aux2->ID_linha > linha){
                tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
                aux3->ID_linha = linha;
                aux3->valor = value;
                aux3->proximo = aux2;
                aux->inicio = aux3;
                if((linha >= mat->qtd_linhas) || (linha == 0 && mat->qtd_linhas == 0)){
                    mat->qtd_linhas = linha + 1;
                }
            }
            else{
                while((aux2->proximo != NULL)&&(aux2->proximo->ID_linha <= linha)){
                    aux2 = aux2->proximo;
                }

                if(aux2->ID_linha == linha){
                    aux2->valor = value;
                }
                else if((aux2->ID_linha != linha && aux2->proximo == NULL)||((aux2->ID_linha != linha) && (aux2->proximo->ID_linha > linha) && (aux2->proximo != NULL))){
                    tipo_elemento *novol = (tipo_elemento*)malloc(sizeof(tipo_elemento));
                    novol->ID_linha = linha;
                    novol->proximo = aux2->proximo;
                    novol->valor = value;
                    aux2->proximo = novol;
                    if((linha >= mat->qtd_linhas) || (linha == 0 && mat->qtd_linhas == 0)){
                        mat->qtd_linhas = linha + 1;
                    }
                }
            }
        }
    }
    else{
        inserir_tipo_linha_mat(mat,coluna);
        inserir_elemento_mat(linha,coluna,value,mat);
    }
}

int busca_matrizES(Tipo_Mat_Esparsa *mat,int l, int c){

    if(!verificar_mat_vazia(mat)){
        tipo_linha *aux = mat->inicio;

        if(mat->qtd_colunas >= c){
            while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna <= c)){
                aux = aux->proximo;
            }
            if(aux->ID_Coluna == c){
                tipo_elemento *aux2 = aux->inicio;

                if(mat->qtd_linhas >= l){
                    if(aux2 != NULL){
                        while((aux2->proximo != NULL)&&(aux2->proximo->ID_linha <= l)){
                            aux2 = aux2->proximo;
                        }
                        if(aux->ID_Coluna == c && aux2->ID_linha == l){
                            return aux2->valor;
                        }
                        else{
                            return 0;
                        }
                    }
                }
            }
        }
        return 0;
    }
    else{
        printf("Matriz VAZIA");
        return -1;
    }
    
    return 0;
}

void imprimir_matrizES(Tipo_Mat_Esparsa *mat){
    int l = mat->qtd_linhas, c = mat->qtd_colunas;
    int **matrizAux;

    matrizAux = (int**)calloc(l,sizeof(int));

    for(int i=0; i<l; i++){
        matrizAux[i] = (int*)calloc(c,sizeof(int));
    }

    for(int i = 0; i < l; i++){
        for(int j = 0; j < c; j++){
            matrizAux[i][j] = busca_matrizES(mat,i,j);
            printf("%d\t", matrizAux[i][j]);
        }
        printf("\n");
    }

    for(int i=0; i<l; i++){
        free(matrizAux[i]);
    }
    free(matrizAux);
}

Tipo_Mat_Esparsa *multiplica_matrizes(Tipo_Mat_Esparsa *mat1, Tipo_Mat_Esparsa *mat2){
    Tipo_Mat_Esparsa *matResult = criar_matriz_esparsa();
    int soma;
    if(mat1->qtd_colunas == mat2->qtd_linhas){

        for(int i=0; i<mat1->qtd_linhas; i++){

            for(int k = 0; k<mat2->qtd_colunas; k++){
                soma = 0;
                for(int j=0; j<mat1->qtd_colunas; j++){
                    soma = soma + busca_matrizES(mat1,i,j) * busca_matrizES(mat2,j,k);
                }
                inserir_elemento_mat(i,k,soma,matResult);
            }
        }
    }
    return matResult;
}

Tipo_Mat_Esparsa *matriz_trasposta(Tipo_Mat_Esparsa *mat){

    if(!verificar_mat_vazia(mat)){
        Tipo_Mat_Esparsa *matT = criar_matriz_esparsa();

        for(int i=0; i<mat->qtd_linhas; i++){

            for(int j=0; j<mat->qtd_colunas; j++){
                if(busca_matrizES(mat,i,j)!=0){
                    inserir_elemento_mat(j,i,busca_matrizES(mat,i,j),matT);
                }
            }
        }
        imprimir_matrizES(matT);
        return matT;
    }
    return 0;
}

Tipo_Mat_Esparsa *soma_matriz(Tipo_Mat_Esparsa *mat1, Tipo_Mat_Esparsa *mat2){
    Tipo_Mat_Esparsa *matResult = criar_matriz_esparsa();
    int soma;

    if(mat1->qtd_colunas == mat2->qtd_colunas && mat1->qtd_linhas == mat2->qtd_linhas){
        for(int i=0; i<mat1->qtd_linhas; i++){
            soma = 0;
            for(int j = 0; j<mat2->qtd_colunas; j++){
                soma = busca_matrizES(mat1,i,j) + busca_matrizES(mat2,i,j);
                inserir_elemento_mat(i,j,soma,matResult);
            }
        }
    }
    return matResult;
}

void apagar_matrizES(Tipo_Mat_Esparsa *mat){

    if(mat != NULL){
        tipo_linha *aux;
        tipo_elemento *aux2;

        while(mat->inicio != NULL){
            aux = mat->inicio;

            while(aux->inicio != NULL){
                aux2 = aux->inicio;
                aux->inicio = aux2->proximo;
                free(aux2);
            }
            mat->inicio = aux->proximo;
            free(aux);
        }
        free(mat);
        free(aux);
        free(aux2);

        if(mat->inicio == NULL){
            printf("\n\nMATRIZ APAGADA!");
        }
        else{
            printf("\n\nERRO AO APAGAR MATRIZ!");
        }
    }

}

int main(){
    Tipo_Mat_Esparsa *matriz = criar_matriz_esparsa();
    Tipo_Mat_Esparsa *matriz2 = criar_matriz_esparsa();
    Tipo_Mat_Esparsa *matriz3 = criar_matriz_esparsa();
    Tipo_Mat_Esparsa *matriz4 = criar_matriz_esparsa();

    inserir_elemento_mat(0,0,1,matriz);
    inserir_elemento_mat(0,1,2,matriz);
    inserir_elemento_mat(1,0,4,matriz);
    inserir_elemento_mat(1,1,0,matriz);

    inserir_elemento_mat(0,0,1,matriz2);
    inserir_elemento_mat(0,1,2,matriz2);
    inserir_elemento_mat(1,0,3,matriz2);
    inserir_elemento_mat(1,1,4,matriz2);

    printf("Quantidade de colunas da Matriz A: %d\n", matriz->qtd_colunas);
    printf("Quantidade de linhas da Matriz A: %d\n\n", matriz->qtd_linhas);
    printf("Quantidade de colunas da Matriz B: %d\n", matriz2->qtd_colunas);
    printf("Quantidade de linhas da Matriz B: %d\n\n", matriz2->qtd_linhas);
    printf("Matriz A:\n\n");
    imprimir_matrizES(matriz);
    printf("\n\nMatriz B:\n\n");
    imprimir_matrizES(matriz2);

    printf("\n\nMultiplicacao de Matrizes:\n\n");
    matriz3 = multiplica_matrizes(matriz,matriz2);
    imprimir_matrizES(matriz3);

    printf("\n\nMatriz Transposta:\n\n");  
    matriz_trasposta(matriz);

    printf("\n\nSoma de Matrizes:\n\n");  
    matriz4 = soma_matriz(matriz,matriz2);
    imprimir_matrizES(matriz4);

    apagar_matrizES(matriz3);
    apagar_matrizES(matriz4);
    apagar_matrizES(matriz2);
    apagar_matrizES(matriz);

    return 0;
}