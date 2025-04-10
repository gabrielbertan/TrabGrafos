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

Tipo_Grafo *iniciar_grafo(){
    Tipo_Grafo *g = (Tipo_Grafo*)malloc(sizeof(Tipo_Grafo));

    g->qtd_colunas = 0;//usamos a quantidade de linhas e colunas, representando o grafo como uma matriz esparsa para realizar a busca pelo grafo posteriormente
    g->qtd_linhas = 0;
    g->inicio = NULL;
 
    return g;
}
int verificar_grafo(Tipo_Grafo *g){//verifica se existe pelo menos um vertice no grafo
    if(g->inicio == NULL){
        return 1;
    }
    else{
        return 0;
    }
}
void inserir_vertice_grafo(Tipo_Grafo *g, int id){

    if(verificar_grafo(g)){//se o grafo não existir por nao possuir vertices insere-se o primeiro
        tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
        novo->ID_Coluna = id;
        novo->proximo = NULL;
        novo->inicio = NULL;
        g->inicio = novo;
        if((id>=g->qtd_colunas && id>0)){
            g->qtd_colunas = id;
        }
    }
    else{
        tipo_linha *aux = g->inicio;

        if(aux->ID_Coluna > id){//caso o id do vertice seja menor do que o primeiro vertice da lista, deixando-a ordenada
            tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
            novo->proximo = aux;
            novo->inicio = NULL;
            novo->ID_Coluna = id;
            g->inicio = novo;

            if((id>g->qtd_colunas)){
                g->qtd_colunas = id; 
            }
        }
        else{//caso o vertice a ser adicionado seja colocado entre dois vertices ou no final da lista
            while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna < id)){
                aux = aux->proximo;
            }
            if((aux->ID_Coluna != id && aux->proximo == NULL)||((aux->ID_Coluna != id) && (aux->proximo->ID_Coluna > id) && (aux->proximo != NULL))){
                tipo_linha *novo = (tipo_linha*)malloc(sizeof(tipo_linha));
                novo->ID_Coluna = id;
                novo->proximo = aux->proximo;
                novo->inicio = NULL;

                aux->proximo = novo;
                if((id>g->qtd_colunas)){
                    g->qtd_colunas = id;
                }
            }
        }
    }
}

int busca_Grafo(Tipo_Grafo *g,int v1, int v2){//busca o vertice v2 que forma a aresta com o vertice v1

    if(!verificar_grafo(g)){//se o grafo existir(possui vertices)
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
        printf("Sem vertices e arestas");//grafo nao existe
        return -1;
    }
    
    return 0;
}

void inserir_aresta(int v1, int v2, Tipo_Grafo *g){
 
    if(!verificar_grafo(g)){//verifica se existe pelo menos um vertice

        tipo_linha *aux = g->inicio;
        
        while((aux->proximo != NULL)&&(aux->proximo->ID_Coluna <= v2)){ 
            aux = aux->proximo;
        }
        if((aux == NULL)||(aux->ID_Coluna != v2)){//caso o vertice fornecido não exista, inserimos o mesmo para a inserção da aresta
            inserir_vertice_grafo(g, v2);//inserimos o vertice e atualizamos o aux para o id desse vertice inserido
            if(v2 > aux->ID_Coluna){
                aux = aux->proximo;
            }
        }

        if(aux->inicio == NULL){ //se o vertice nao tiver uma aresta com nenhum outro vertice, é realizado a ligação
            tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
            aux3->ID_linha = v1;
            aux3->valor = v1;
            aux3->proximo = NULL;
            aux->inicio = aux3;
            if((v1 > g->qtd_linhas) && v1>0){
                g->qtd_linhas = v1;
            }
        }
        else{//caso o vertice ja tenha ligações formando arestas devemos verificar onde o vertice sera colocado visando a ordenação das listas
            tipo_elemento *aux2;
            aux2 = aux->inicio;

            if(aux2->ID_linha > v1){//caso o id do vertice seja menor do que o primeiro vertice da lista, deixando-a ordenada
                tipo_elemento *aux3 = (tipo_elemento*)malloc(sizeof(tipo_elemento));
                aux3->ID_linha = v1;
                aux3->valor = v1;
                aux3->proximo = aux2;
                aux->inicio = aux3;
                if((v1 > g->qtd_linhas)){
                    g->qtd_linhas = v1;
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
                    if((v1 > g->qtd_linhas)){
                        g->qtd_linhas = v1;
                    }
                }
            }
        }
        if(busca_Grafo(g,v2,v1) == 0){//se a busca retornar 0, significa que precisamos inserir o outro vertice na outra lista tambem
            inserir_aresta(v2,v1,g);
            //como criamos a aresta v1-v2, temos que criar a respectiva v2-v1
        }
    }
    else{
        //caso não exista vertice ainda, criamos os mesmos e então inserimos a aresta
        inserir_vertice_grafo(g,v2);//inserimos o primeiro vertice com esta função para criar o grafo e então conseguirmos inserir a aresta
        inserir_aresta(v1,v2,g);//com o primeiro vertice criado, nessa função ela criará o outro vertice necessario
    }
}

void ver_grafo(Tipo_Grafo *g){

    if(!verificar_grafo(g)){

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

    if(!verificar_grafo(grafo)){
        tipo_linha *aux = grafo->inicio;

        while((aux->ID_Coluna!=v2)&&(aux!=NULL)&&(v2>aux->ID_Coluna)){
            aux = aux->proximo;
        }
        if(aux->ID_Coluna == v2){
            tipo_elemento *aux2 = aux->inicio;//criamos dois ponteiros auxiliares, um para alançar o vertice desejado e o outro para permanecer no elemento anterior 
            tipo_elemento *auxA = aux->inicio;

            while((aux2->ID_linha!=v1)&&(aux2!=NULL)&&(v1>aux2->ID_linha)){
                if(aux2 != aux->inicio){//o auxA que é o auxiliar anterior ao aux2 só começara a se locomover quando o aux2 não for o primeiro elemento da lista assim como ele
                    auxA = auxA->proximo;
                }
                aux2 = aux2->proximo;
            }
            if(aux2->ID_linha == v1){

                if(aux->inicio->ID_linha == v1){//no caso de só existir uma aresta conectada a esse vertice, liberamos a aresta ligada entre os vertices
                    if(aux2->proximo == NULL){
                        aux->inicio = NULL;
                        free(aux2);
                    }
                    else{//a derivação do caso acima é que o vertice que forma a resta não é o unico elemento da lista encadeada, então a variavel auxiliar guarda todo o restante da lista e o primeiro elemento é liberado, desfazendo a aresta
                        tipo_elemento *aux3 = aux2->proximo;
                        aux->inicio = aux3;//recolocando o restante das ligações(arestas)
                        free(aux2);
                    }
                }
                else if(aux2->proximo == NULL){//caso o vertice responsavel pela aresta seja o ultimo elemento da lista encadeada
                    auxA->proximo = NULL;//o auxiliar anterior apontará para nulo e a aresta sera liberada
                    free(aux2);
                }
                else{//caso o elemento desejado esteja no meio da lista, guardamos o restante da lista a partir dele, e fazemos o auxiliar anterior se conectar com esse restante de lista, liberando então, o elemento que estava no meio da lista encadeada
                    tipo_elemento *aux3 = aux2->proximo;
                    auxA->proximo = aux3;
                    free(aux2);
                }
                if(busca_Grafo(grafo,v2,v1)==v2){//como excluimos a aresta, temos que remover os elementos correspondetes v1 em v2 e v2 em v1
                    remover_aresta(grafo,v2,v1);//se no elemento v1 da primeira lista encadeada tiver o elemento v2 na segunda lista encadeada, ele tambem deve ser liberado como dito acima
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

    if(!verificar_grafo(grafo)){
        tipo_linha *aux = grafo->inicio;//da mesma forma que na liberação de arestas, vamos precisar de dois auxiliares, aux e auxA que é o anterior a aux
        tipo_linha *auxA = grafo->inicio;

        while((aux->ID_Coluna!=v)&&(aux!=NULL)&&(v>aux->ID_Coluna)){//fazendo a busca
            if(aux != grafo->inicio){
                auxA = auxA->proximo;
            }
            aux = aux->proximo;
        }
        tipo_linha *aux2 = aux->proximo;
        if((aux->ID_Coluna == v)&&(aux->proximo != NULL)&&(aux != grafo->inicio)){

            if(aux->inicio == NULL){//se o vertice não tiver nenhuma ligação formando arestas ele é liberado
                auxA->proximo = aux2;
                free(aux);
            }
            else{
                while(aux->inicio != NULL){//se o vertice tiver ligações formando arestas, liberamos todas elas e então excluimos o vertice
                    tipo_elemento *aux3 = aux->inicio;//a cada remoção, o inicio da lista é atualizado, e vai sendo excluido ate que o inicio seja nulo
                    remover_aresta(grafo,aux3->ID_linha,aux->ID_Coluna);
                }
                auxA->proximo = aux2;
                free(aux);
            }
        }
        else if((aux->ID_Coluna == v)&&(aux == grafo->inicio)){//se o vertice a ser removido for o primeiro da lista, fazemos toda verifiação e liberação se necessario
            if(aux->inicio == NULL){
                grafo->inicio = aux2;
                free(aux);
            }
            else{
                while(aux->inicio != NULL){
                    tipo_elemento *aux3 = aux->inicio;
                    remover_aresta(grafo,aux3->ID_linha,aux->ID_Coluna);
                }
                grafo->inicio = aux2;//a diferença se da que o inicio do grafo sera atualizado, grafo->inicio recebera tudo a partir do primeiro elemento
                free(aux);
            }
        }
        else if((aux->ID_Coluna == v)&&(aux->proximo == NULL)){//caso o vertice a ser removido seja o ultimo da lista
            if(aux->inicio == NULL){
                auxA->proximo = NULL;
                free(aux);
            }
            else{
                while(aux->inicio != NULL){
                    tipo_elemento *aux3 = aux->inicio;
                    remover_aresta(grafo,aux3->ID_linha,aux->ID_Coluna);
                }
                auxA->proximo = NULL;//a diferença é que o penultimo elemento da lista apontara para nulo agora
                free(aux);
            }
        }
        else{
            printf("Vertice não existente");
        }
    }
}

void grauVertice(Tipo_Grafo *grafo, int v){
    if(!verificar_grafo(grafo)){
        tipo_linha *aux = grafo->inicio;
        while(aux != NULL && aux->ID_Coluna < v){
            aux = aux->proximo;
        }
        if(aux != NULL && aux->ID_Coluna == v){
            tipo_elemento *aux2 = aux->inicio;
            int grau = 0;
            while(aux2 != NULL){
                grau++;
                aux2 = aux2->proximo;
            }
            printf("O grau do vertice %d e: %d\n", v, grau);
        }else{
            printf("Vertice %d nao encontrado!\n", v);
        }
    }else{
        printf("Nao existem vertices no grafo!\n");
    }
}
        
int dfs(tipo_linha *vertice, int *verticesVisitados, Tipo_Grafo *grafo){
    if (vertice == NULL || verticesVisitados[vertice->ID_Coluna]) {//caso o vertice não exista, ou o vertice ja tenha sido visitado retornamos 1
        return 1;
    }
    else{
        verticesVisitados[vertice->ID_Coluna] = 1; //caso o vertice nao tenha sido visitado, agora ele foi por isso atualizamos o vetor na posição do id do vertice
        tipo_elemento *aux = vertice->inicio; //utilizamos um auxiliar para percorrer a lista da lista
        
        while (aux != NULL) {
            tipo_linha *busca = grafo->inicio; //utilizamos a variavel busca para percorrer a lista de vertices do grafo
            while (busca != NULL && busca->ID_Coluna != aux->ID_linha){
                busca = busca->proximo;
            }
            dfs(busca, verticesVisitados, grafo);
            //printf("\nid coluna %d / id linha %d -> ", busca->ID_Coluna, aux->ID_linha);
            aux = aux->proximo;//quando na chamada da função recursiva a primeira condicional for satisfeita, o aux irá para o próximo elemento da lista de arestas
            //printf(" id coluna %d / id linha %d\n", busca->ID_Coluna, aux->ID_linha);
        }
        return 0;
    }
}

int buscarVertice(Tipo_Grafo *grafo, int idVertice){
    //esta função foi implementada com a finalidade de caso removermos um vertice do grafo conexo com V=[1,2,3,4], e mesmo assim ele continue sendo conexo por exemplo V=[1,2,4]
    //como o grafo agora com os vertices V[1,2,4] ainda é conexo, utilizamos o buscar vertice para verificar se ele esta presente ou nao no grafo, como ele foi removido, o grafo continua conexo
    //porém caso ele esteja no grafo e não possa ser acessado por nenhum outro vertice que forme uma aresta com ele, o grafo não é conexo!

    if(verificar_grafo(grafo)){
        return -1;
    }
    else{
        tipo_linha *aux = grafo->inicio;
        while(aux->ID_Coluna < idVertice && aux->proximo != NULL){
            aux = aux->proximo;
        }
        if(idVertice == aux->ID_Coluna){
            return 1;
        }
        else{
            return 0;
        }
    }
}

int grafoConexo(Tipo_Grafo *grafo){
    if (!verificar_grafo(grafo)) {
        int *verticesVisitados = (int *)calloc(grafo->qtd_colunas, sizeof(int));//inicializamos o vetor dos vertices visitados todos com 0 para que possamos atualiza-lo posteriormente

        dfs(grafo->inicio, verticesVisitados, grafo);//realizamos a busca para verificar se todos os vertices serão visitados
        for (int i = 1; i <= grafo->qtd_colunas; i++) {
            if (verticesVisitados[i] == 0 && buscarVertice(grafo, i) == 1) {//caso algum vertice nao tenha sido visitado, o grafo não é conexo
                printf("O grafo nao e conexo!\n\n");
                free(verticesVisitados);
                return 1;
            }
        }
        
        printf("O grafo e conexo!\n\n");//caso contrario, todos foram visitados portanto o grafo é conexo
        free(verticesVisitados);

        return 0;
    } else{
        printf("O grafo esta vazio!\n");
        return -1;
    }
}

void converterGrafo(Tipo_Grafo *g){
    if(!verificar_grafo(g)){
        int **matriz = (int**)malloc((g->qtd_colunas+1) * sizeof(int*));//inicializamos a matriz de adjacência utilizando calloc para preenche-la com 0s
        for(int i = 1; i <= g->qtd_colunas; i++){
            matriz[i] = (int*)calloc((g->qtd_colunas+1), sizeof(int));
        }
        
        // Preenchimento da matriz
        tipo_linha *aux = g->inicio;
        while(aux != NULL){//percorremos todo o grafo para que o preenchimento da matriz com os 1s seja correto
            tipo_elemento *aux2 = aux->inicio;
            while(aux2 != NULL){
                matriz[aux->ID_Coluna][aux2->ID_linha] = 1;//nas posições [id_coluna][id_linha] que se referem às arestas do grafo será atribuido o valor 1
                aux2 = aux2->proximo;
            }
            aux = aux->proximo;
        }
       
        printf("\nMatriz de Adjacencia:\n");
        printf("   ");
        for(int i = 1; i <= g->qtd_colunas; i++){//indicamos as posições da matriz
            printf("%2d ", i);
        }
        printf("\n");
        
        for(int i = 1; i <= g->qtd_colunas; i++){
            printf("%2d ", i);
            for(int j = 1; j <= g->qtd_colunas; j++){//impressão da matriz
                printf("%2d ", matriz[i][j]);
            }
            printf("\n");
        }
        
        // Liberação da memória alocada
        for(int i = 0; i <= g->qtd_colunas; i++){
            free(matriz[i]);
        }
        free(matriz);
        printf("\n");
    }
    else{
        printf("Grafo vazio!\n");
    }
}

void buscaEmLargura(Tipo_Grafo *grafo, int vertice_inicial) {
    if (verificar_grafo(grafo)) {
        printf("Grafo vazio! Não é possível realizar a busca.\n");
        return;
    }
    
    // Verifica se o vértice inicial existe no grafo
    tipo_linha *aux = grafo->inicio;
    while (aux != NULL && aux->ID_Coluna != vertice_inicial) {
        aux = aux->proximo;
    }
    
    if (aux == NULL) {
        printf("Vértice inicial %d não encontrado no grafo!\n", vertice_inicial);
        return;
    }
    
    // Estrutura simplificada para a fila
    int *fila = (int*)malloc((grafo->qtd_colunas + 1) * sizeof(int));
    int inicio = 0, fim = 0;
    
    // Vetor de visitados e pais
    int *visitados = (int*)calloc(grafo->qtd_colunas + 1, sizeof(int));
    int *pais = (int*)malloc((grafo->qtd_colunas + 1) * sizeof(int));
    
    // Inicialização
    for (int i = 0; i <= grafo->qtd_colunas; i++) {
        pais[i] = -1;
    }
    
    // Enfileira o vértice inicial
    fila[fim++] = vertice_inicial;
    visitados[vertice_inicial] = 1;
    
    printf("Caminhamento em Largura a partir do vertice %d:\n", vertice_inicial);
    printf("%d ", vertice_inicial);
    
    // Enquanto houver vértices na fila
    while (inicio < fim) {
        // Desenfileira um vértice
        int v_atual = fila[inicio++];
        
        // Encontra este vértice no grafo
        aux = grafo->inicio;
        while (aux != NULL && aux->ID_Coluna != v_atual) {
            aux = aux->proximo;
        }
        
        if (aux != NULL) {
            // Percorre todos os vizinhos do vértice atual
            tipo_elemento *vizinho = aux->inicio;
            while (vizinho != NULL) {
                int id_vizinho = vizinho->ID_linha;
                
                // Se o vizinho ainda não foi visitado
                if (!visitados[id_vizinho]) {
                    visitados[id_vizinho] = 1;  // Marca como visitado
                    pais[id_vizinho] = v_atual;  // Atualiza o pai
                    fila[fim++] = id_vizinho;  // Enfileira para visitar depois
                    printf("%d ", id_vizinho);  // Imprime o vértice visitado
                }
                
                vizinho = vizinho->proximo;
            }
        }
    }
    
    //impressão
    printf("\n\nCaminhos encontrados a partir do vertice %d:\n", vertice_inicial);
    for (int i = 1; i <= grafo->qtd_colunas; i++) {
        // Verifica se o vértice existe no grafo
        aux = grafo->inicio;
        while (aux != NULL && aux->ID_Coluna != i) {
            aux = aux->proximo;
        }
        
        if (aux != NULL && i != vertice_inicial && pais[i] != -1) {
            printf("%d para %d: ", vertice_inicial, i);
            
            // Guarda o caminho em um vetor temporário
            int temp[10];  // Tamanho arbitrário, mas suficiente
            int pos = 0;
            int atual = i;
            
            // Percorre do destino até a origem
            while (atual != -1) {
                temp[pos++] = atual;
                atual = pais[atual];
            }
            
            // Imprime o caminho na ordem correta
            for (int j = pos - 1; j >= 0; j--) {
                printf("%d", temp[j]);
                if (j > 0) printf(" -> ");
            }
            printf("\n");
        }
    }
    
    // Libera a memória alocada
    free(fila);
    free(visitados);
    free(pais);
}

int main(){
    Tipo_Grafo *grafo1 = iniciar_grafo();

     int op=0;

    while(op!=9){
        printf("Escolha uma das opcoes: \n");
        printf("1 - Inserir Vertice\n");
        printf("2 - Inserir Aresta\n");
        printf("3 - Visualizar grafo\n");
        printf("4 - Remover Vertice\n");
        printf("5 - Remover Aresta\n");
        printf("6 - Grau do Vertice\n");
        printf("7 - Verificar Grafo Conexo\n");
        printf("8 - Converter em Matriz de Adjacencia\n");
        printf("9 - Busca em Largura\n");
        printf("10 - Sair do Programa\n");
        printf("Opcao: ");
        scanf("%d", &op);
        printf("\n");
        int n, m;
        
        switch(op){
            case 1:
                printf("\nQual vertice que deseja inserir: ");
                scanf("%d", &n);
                inserir_vertice_grafo(grafo1,n);
                break;
            
            case 2: 
                printf("\nQual aresta deseja inserir, informe os vertices correspondentes:");
                scanf("%d %d",&n ,&m);
                inserir_aresta(n,m,grafo1);
                break;
            
            case 3:
                ver_grafo(grafo1);
                break;
            
            case 4: 
                printf("\nId do vertice que deseja remover: ");
                scanf("%d", &n);
                remover_vertice(grafo1,n);
                break;
            
            case 5: 
                printf("\ninforme o id dos vertices correspondentes da aresta");
                scanf("%d %d", &n, &m);
                remover_aresta(grafo1,n,m);
                break;
            
            case 6: 
                printf("Informe o vertice:\n");
                scanf("%d", &n);
                grauVertice(grafo1, n);
                break;

            case 7:
                grafoConexo(grafo1);
                break;

            case 8:
                converterGrafo(grafo1);
                break;
            
            case 9:
                printf("Informe o vertice:\n");
                scanf("%d", &n);
                buscaEmLargura(grafo1,n);
                break;

            case 10:
                printf("Encerrando Programa\n");
                break;

            default:
                printf("Opcao Invalida!\n");
                break;
        }
    }
    return 0;
}
