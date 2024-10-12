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

        if(aux->inicio == NULL){ //aqui começa a verificação das linhas
            tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
            aux3->ID_linha = linha;
            aux3->valor = value;
            aux3->proximo = NULL;
            aux->inicio = aux3;
            //printf("o primeiro elemento tem valor %d\n", aux->inicio->valor);
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
                //printf("entrei aqui e tenho %d , %d\n", linha,coluna);
                if((linha >= mat->qtd_linhas) || (linha == 0 && mat->qtd_linhas == 0)){
                    mat->qtd_linhas = linha + 1;
                }
            }
            else{
                //printf("entrei aqui e tenho %d , %d\n", linha,coluna);
                while((aux2->proximo != NULL)&&(aux2->proximo->ID_linha <= linha)){
                    aux2 = aux2->proximo;
                }
                //printf("meu id apos entrar aqui e %d\n", aux2->ID_linha);

                if(aux2->ID_linha == linha){
                    //printf("troquei %d por ", aux2->valor);
                    aux2->valor = value;
                    //printf("%d \n", aux2->valor);
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
        //printf("A matriz se encontra vazia!");
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
                    else{
                        return 0;
                    }
                }
                else{
                    return 0;
                }
            }
            else{
                return 0;
            }
        }
    }
}

void imprimir_matrizES(Tipo_Mat_Esparsa *mat){
    int l = mat->qtd_linhas, c = mat->qtd_colunas;
    int **matrizAux;

    matrizAux = (int**)calloc(l,sizeof(int));

    for(int i=0; i<l; i++){
        matrizAux[i] = (int*)calloc(c,sizeof(int));
    }

    tipo_linha *aux = mat->inicio;
    tipo_elemento *aux2;

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

int main(){
    Tipo_Mat_Esparsa *matriz = criar_matriz_esparsa();

    inserir_elemento_mat(4,5,4,matriz);
    inserir_elemento_mat(0,0,4,matriz);
    inserir_elemento_mat(4,4,17,matriz);
    inserir_elemento_mat(4,4,18,matriz);
    inserir_elemento_mat(2,2,3,matriz);  
    inserir_elemento_mat(0,8,3,matriz); 
    inserir_elemento_mat(4,7,3,matriz);

    printf("\nQuantidade de colunas: %d\n", matriz->qtd_colunas);
    printf("Quantidade de linhas: %d\n\n", matriz->qtd_linhas);
    imprimir_matrizES(matriz);

    return 0;
}
    
