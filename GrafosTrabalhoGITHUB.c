#include <stdio.h>
#include <stdlib.h>

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
}Tipo_Grafo;

Tipo_Grafo *criar_matriz_esparsa(){
    Tipo_Grafo *mat = (Tipo_Grafo*)malloc(sizeof(Tipo_Grafo));

    mat->qtd_colunas = 0;
    mat->qtd_linhas = 0;
    mat->inicio = NULL;
 
    return mat;
}
int verificar_mat_vazia(Tipo_Grafo *mat){
    if(mat->inicio == NULL){
        return 1;
    }
    else{
        return 0;
    }
}
void inserir_vertice_grafo(Tipo_Grafo *mat, int id){

    if(verificar_mat_vazia(mat)){//se o grafo não possuir vertices insere-se o primeiro
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

        if(aux->ID_Coluna > id){//caso o id do vertice seja menor do que o primeiro vertice da lista, deixando-a ordenada
            tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
            novo->proximo = aux;
            novo->inicio = NULL;
            novo->ID_Coluna = id;
            mat->inicio = novo;

            if((id>=mat->qtd_colunas)){
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
                if((id>=mat->qtd_colunas)){
                    mat->qtd_colunas = id + 1;
                }
            }
        }
    }
}

int busca_Grafo(Tipo_Grafo *g,int v1, int v2){

    if(!verificar_mat_vazia(g)){
        tipo_linha *aux = g->inicio;

        if(g->qtd_colunas >= v2){
            while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna <= v2)){
                aux = aux->proximo;
            }
            if(aux->ID_Coluna == v2){
                tipo_elemento *aux2 = aux->inicio;

                if(g->qtd_linhas >= v1){
                    if(aux2 != NULL){
                        while((aux2->proximo != NULL)&&(aux2->proximo->ID_linha <= v1)){
                            aux2 = aux2->proximo;
                        }
                        if(aux->ID_Coluna == v2 && aux2->ID_linha == v1){
                            return aux2->valor;
                        }
                        else{
                            return 0;
                        }
                    }
                }
            }
        }
    }
    else{
        printf("Sem vertices e arestas");
        return -1;
    }
    
    return 0;
}

void inserir_aresta(int v1, int v2, Tipo_Grafo *mat){
 
    if(!verificar_mat_vazia(mat)){

        tipo_linha *aux = mat->inicio;
        
        while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna <= v2)){ 
            aux = aux->proximo;
        }
        if((aux == NULL)||(aux->ID_Coluna != v2)){//caso o vertice fornecido não exista, inserimos o mesmo para a inserção da aresta
            inserir_vertice_grafo(mat, v2);
            if(v2 > aux->ID_Coluna){
                aux = aux->proximo;
            }
        }

        if(aux->inicio == NULL){ 
            tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
            aux3->ID_linha = v1;
            aux3->valor = v1;
            aux3->proximo = NULL;
            aux->inicio = aux3;
            if((v1 >= mat->qtd_linhas) || (v1 == 0 && mat->qtd_linhas == 0)){
                mat->qtd_linhas = v1 + 1;
            }
        }
        else{
            tipo_elemento *aux2;
            aux2 = aux->inicio;

            if(aux2->ID_linha > v1){//caso o id do vertice seja menor do que o primeiro vertice da lista, deixando-a ordenada
                tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
                aux3->ID_linha = v1;
                aux3->valor = v1;
                aux3->proximo = aux2;
                aux->inicio = aux3;
                if((v1 >= mat->qtd_linhas)){
                    mat->qtd_linhas = v1 + 1;
                }
            }
            else{
                while((aux2->proximo != NULL)&&(aux2->proximo->ID_linha <= v1)){
                    aux2 = aux2->proximo;
                }
                if((aux2->ID_linha != v1 && aux2->proximo == NULL)||((aux2->ID_linha != v1) && (aux2->proximo->ID_linha > v1) && (aux2->proximo != NULL))){
                    //inserção ordenada
                    tipo_elemento *novol = (tipo_elemento*)malloc(sizeof(tipo_elemento));
                    novol->ID_linha = v1;
                    novol->proximo = aux2->proximo;
                    novol->valor = v1;
                    aux2->proximo = novol;
                    if((v1 >= mat->qtd_linhas)){
                        mat->qtd_linhas = v1 + 1;
                    }
                }
            }
        }
        if(busca_Grafo(mat,v2,v1) == 0){
            inserir_aresta(v2,v1,mat);
            //como criamos a aresta v1-v2, temos que criar a respectiva v2-v1
        }
    }
    else{
        //caso não exista vertice ainda, criamos os mesmos e então inserimos a aresta
        inserir_vertice_grafo(mat,v2);//inserimos o primeiro vertice com esta função para criar o grafo e então conseguirmos inserir a aresta
        inserir_aresta(v1,v2,mat);
    }
}

void imprimir_matrizES(Tipo_Grafo *mat){
    int l = mat->qtd_linhas, c = mat->qtd_colunas;
    int **matrizAux;
    matrizAux = (int**)calloc(l,sizeof(int));

    for(int i=0; i<l; i++){
        matrizAux[i] = (int*)calloc(c,sizeof(int));
    }

    for(int i = 0; i < l; i++){
        for(int j = 0; j < c; j++){
            matrizAux[i][j] = busca_Grafo(mat,i,j);
            printf("%d\t", matrizAux[i][j]);
        }
        printf("\n");
    }

    for(int i=0; i<l; i++){
        free(matrizAux[i]);
    }
    free(matrizAux);
}

void ver_grafo(Tipo_Grafo *g){

    if(!verificar_mat_vazia(g)){

        tipo_linha *aux = g->inicio;
        tipo_elemento *aux2;
        
        while(aux != NULL){
            aux2 = aux->inicio;

            if(aux!=NULL){
                printf("(%d)->", aux->ID_Coluna);
                while(aux2!=NULL){

                    if(aux2->proximo!=NULL){
                        printf("%d->", aux2->valor);
                        aux2 = aux2->proximo;
                    }
                    else{
                        printf("%d", aux2->valor);
                        aux2 = aux2->proximo;
                    }
                }
                printf("\n");
            }       
            aux = aux->proximo;
        }
    }
    else{
        printf("Nao existem vertices, portanto nao ha grafo!");
    }
}
void remover_aresta(Tipo_Grafo *grafo, int v1, int v2){

    if(!verificar_mat_vazia(grafo)){
        tipo_linha *aux = grafo->inicio;

        while((aux->ID_Coluna!=v2)&&(aux!=NULL)&&(v2>aux->ID_Coluna)){
            aux = aux->proximo;
        }
        if(aux->ID_Coluna == v2){
            tipo_elemento *aux2 = aux->inicio;
            tipo_elemento *auxA = aux->inicio;

            while((aux2->ID_linha!=v1)&&(aux2!=NULL)&&(v1>aux2->ID_linha)){
                if(aux2 != aux->inicio){
                    auxA = auxA->proximo;
                }
                aux2 = aux2->proximo;
            }
            if(aux2->ID_linha == v1){

                if(aux->inicio->ID_linha == v1){
                    if(aux2->proximo == NULL){
                        aux->inicio = NULL;
                        free(aux2);
                    }
                    else{
                        tipo_elemento *aux3 = aux2->proximo;
                        aux->inicio = aux3;
                        free(aux2);
                    }
                }
                else if(aux2->proximo == NULL){
                    auxA->proximo = NULL;
                    free(aux2);
                }
                else{
                    tipo_elemento *aux3 = aux2->proximo;
                    auxA->proximo = aux3;
                    free(aux2);
                }
                if(busca_Grafo(grafo,v2,v1)==v2){
                    remover_aresta(grafo,v2,v1);
                }
            }
            else{
                printf("Aresta v2-v1 não existente");
            }

        }
        else{
            printf("Vertice v2 não existe!");
        }
    }
}

void remover_vertice(Tipo_Grafo *grafo, int v){

    if(!verificar_mat_vazia(grafo)){
        tipo_linha *aux = grafo->inicio;
        tipo_linha *auxA = grafo->inicio;

        while((aux->ID_Coluna!=v)&&(aux!=NULL)&&(v>aux->ID_Coluna)){
            if(aux != grafo->inicio){
                auxA = auxA->proximo;
            }
            aux = aux->proximo;
        }
        tipo_linha *aux2 = aux->proximo;
        if((aux->ID_Coluna == v)&&(aux->proximo != NULL)&&(aux != grafo->inicio)){

            if(aux->inicio == NULL){
                auxA->proximo = aux2;
                free(aux);
            }
            else{
                while(aux->inicio != NULL){
                    tipo_elemento *aux3 = aux->inicio;
                    remover_aresta(grafo,aux3->ID_linha,aux->ID_Coluna);
                }
                auxA->proximo = aux2;
                free(aux);
            }
        }
        else if((aux->ID_Coluna == v)&&(aux == grafo->inicio)){
            if(aux->inicio == NULL){
                grafo->inicio = aux2;
                free(aux);
            }
            else{
                while(aux->inicio != NULL){
                    tipo_elemento *aux3 = aux->inicio;
                    remover_aresta(grafo,aux3->ID_linha,aux->ID_Coluna);
                }
                grafo->inicio = aux2;
                free(aux);
            }
        }
        else if((aux->ID_Coluna == v)&&(aux->proximo == NULL)){
            if(aux->inicio == NULL){
                auxA->proximo = NULL;
                free(aux);
            }
            else{
                while(aux->inicio != NULL){
                    tipo_elemento *aux3 = aux->inicio;
                    remover_aresta(grafo,aux3->ID_linha,aux->ID_Coluna);
                }
                auxA->proximo = NULL;
                free(aux);
            }
        }
        else{
            printf("Vertice não existente");
        }
    }
}


int main(){
    Tipo_Grafo *grafo1 = criar_matriz_esparsa();
    
    inserir_aresta(2,1,grafo1);
    inserir_aresta(6,1,grafo1);
    inserir_aresta(2,4,grafo1);
    inserir_aresta(2,3,grafo1);
    inserir_aresta(3,5,grafo1);
    inserir_aresta(4,6,grafo1);
    inserir_aresta(4,5,grafo1);
    inserir_aresta(4,7,grafo1);
    inserir_aresta(7,8,grafo1);
    inserir_aresta(7,9,grafo1);
    printf("GRAFO ANTES:\n");
    ver_grafo(grafo1);
    printf("\n\n");

    //remover_aresta(grafo1,1,2);
    remover_aresta(grafo1,1,2);
    remover_aresta(grafo1,7,8);
    remover_vertice(grafo1,3);
    remover_vertice(grafo1,4);
    //remover_vertice(grafo1,1);
    printf("GRAFO DEPOIS DAS OPERACOES:\n");
    ver_grafo(grafo1);//(n):vertices ->n->:arestas

    return 0;
}
