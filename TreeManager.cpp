/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


#include "TreeManager.h"
#include "Node.h"
#include <fstream>
#include <vector>
#include <string>

TreeManager::TreeManager(int m) : M(m) {
    fstream rootFile("root.txt", ios::in);
    int rootValue;
    rootFile >> rootValue;
    this->root = rootValue;
    rootFile.close();
}

TreeManager::Result TreeManager::mSearch(fstream& file, int x, int* b){
    Node p(M);
    int q = getRoot();
    
    if(q == 0) {
        return {0, 0, false};
    }
    
    file.seekg((q - 1) * sizeof(Node), ios::beg);
    file.read((char*)(&p), sizeof(Node));

    while(true){
        int index = p.n; 

        for(int i = 1; i <= p.n; i++){
            if(x == p.K[i]){ //encontrou a chave
                if(b!=nullptr){
                    *b = p.B[i];
                }
                return {q, i, true}; 
            }
            if(x < p.K[i]){
                index = i - 1; //chave à esquerda
                break;
            }
        }
        
        
        if(p.A[index] == 0){
            return {q, index, false}; 
        }else{
            q = p.A[index];
            file.seekg((q - 1) * sizeof(Node), ios::beg);
            file.read((char*)(&p), sizeof(Node));
        }
    }
}

int TreeManager::insertB(fstream& binFile, int x, int b) {
    int K = x;
    int A = 0; // Ponteiro para o novo nó Q (lado direito do split)
    int B = b;

    Result result = mSearch(binFile, x);

    if(result.success){
       return 0; // Já está inserido
    }

    if (result.p == 0) {
        // Cria a primeira raiz
        Node root(M);
        root.n = 1;
        root.K[1] = x;
        root.B[1] = b;
        root.A[0] = 0;
        root.A[1] = 0;
        
        binFile.seekp(0, ios::beg); 
        binFile.write((const char*)(&root), sizeof(Node));
        
        updateRoot(1);
        return 1;
    }

    int currentP_Index = result.p; // Índice do nó P atual
    int insertion_i = result.i;     // Índice do ponteiro (A[i])
    int parentIndex = 0;            // CORREÇÃO: Escopo de parentIndex
    int mid = 0;                    // CORREÇÃO: Escopo de mid
    int indexQ = 0;                 // CORREÇÃO: Escopo de indexQ

    while(true) {
        Node p(M);
        binFile.seekg((currentP_Index - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&p), sizeof(Node));

        // 1. Inserção na Posição Encontrada
        // Move elementos para a direita
        for (int j = p.n; j > insertion_i; j--){
            p.K[j + 1] = p.K[j];
            p.A[j + 1] = p.A[j]; 
            p.B[j + 1] = p.B[j];
        }

        // Insere o valor promovido (K) e o ponteiro para o novo nó (A)
        p.K[insertion_i + 1] = K;
        p.A[insertion_i + 1] = A; // A é o índice do novo nó Q ou 0 se folha
        p.B[insertion_i + 1] = B;
        p.n++;

        if(p.n < M){ // Cabe a inserção
            binFile.seekp((currentP_Index - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&p), sizeof(Node));
            return 1;
        }

        // 2. Split do nó
        if (M == 2 && p.n == 2) {
            mid = 2; 
        } else {
            mid = (p.n + 1) / 2;
        } 
        Node q(M);

        int j = 1;
        for(int k = mid + 1; k <= p.n; k++){
            q.K[j] = p.K[k];
            q.A[j] = p.A[k];
            q.B[j] = p.B[k];
            j++;
        }

        q.A[0] = p.A[mid];
        q.n = p.n - mid;
        p.n = mid - 1;

        // Escreve P atualizado
        binFile.seekp((currentP_Index - 1) * sizeof(Node), ios::beg);
        binFile.write((const char*)(&p), sizeof(Node));

        // Escreve Q no final 
        indexQ = getNextIndex(binFile); // Usa a variável indexQ no escopo correto
        binFile.seekp(0, ios::end);
        binFile.write((const char*)(&q), sizeof(Node));

        // 3. Promove a chave do meio e busca o pai
        K = p.K[mid];
        B = p.B[mid];
        A = indexQ; // A é o ponteiro para o novo nó Q (lado direito do split)
        
        parentIndex = getParentIndex(binFile, currentP_Index);
        
        if(parentIndex == 0) break; // Chegou na raiz

        // 4. Configura para a próxima iteração no nó pai (parentIndex)
        currentP_Index = parentIndex;
        
        // Lê o nó pai (parent) para determinar a posição de inserção do K promovido
        Node parent(M);
        binFile.seekg((parentIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&parent), sizeof(Node));
        
        // Encontra a posição de inserção (insertion_i) da chave K no nó pai
        // ATENÇÃO: Fazemos a busca local no nó pai para o 'K' promovido!
        insertion_i = 0;
        for(int i = 1; i <= parent.n; i++){
            if(K < parent.K[i]){
                insertion_i = i - 1;
                break;
            }
            // Se o loop terminar, insertion_i será o índice do último ponteiro (parent.n)
            insertion_i = i; 
        }
    }

    // 5. Cria nova raiz (Se parentIndex == 0)
    if(parentIndex == 0) { 
        int oldRootIndex = getRoot();
        int newRootIndex = getNextIndex(binFile);
        
        Node root(M);
        root.n = 1;
        root.K[1] = K;
        root.A[0] = oldRootIndex; // P original (lado esquerdo)
        root.A[1] = A; // Q novo (lado direito)
        root.B[1] = B;

        binFile.seekp(0, ios::end);
        binFile.write((const char*)(&root), sizeof(Node));
        
        updateRoot(newRootIndex);
    }
    return 1;
}

int TreeManager::getParentIndex(fstream& binFile, int child){
    int rootIndex = getRoot();
    
    if(child == rootIndex)
        return 0;

    Node node(M);
    binFile.seekg(0, ios::beg);

    int index = 1;
    while(binFile.read((char*)&node, sizeof(Node))) {
        for(int i = 0; i <= node.n; i++) {
            if(node.A[i] == child) {
                return index;
            }
        }
        index++;
    }

    return 0; // Não encontrou (erro)
}

//funcao para printar o arquivo completo na tela
void TreeManager::printTree(fstream& file){
    system("clear || cls");
    cout << "Raiz: " << getRoot() << "\n\n";
    Node p(M);
    int index = 1;

    cout << "=========================================================" << endl;
    cout << "No n, A[0], (K[1], A[1], B[1]), ... , (K[n], A[n], B[n]) \n\n";
    
    file.seekg(0, ios::beg);
    
    while(file.read((char*)(&p), sizeof(Node))){
        
        cout << index << "  ";
        cout << p.n << "  ";
        cout << p.A[0] << ",    ";
        for(int i = 1; i <= p.n; i++){
            cout << "(" << p.K[i] << ",    " << p.A[i] << ",    " << p.B[i] << ")  ";
        }
        cout << endl;
        index++;
    }
    cout << "=========================================================\n\n";
    
    file.clear(); //limpar flags de erro
    file.seekg(0, ios::beg); //voltar o ponteiro no começo do arquivo
}

int TreeManager::getRoot(){
    return this->root;
}


void TreeManager::updateRoot(int newRoot){
    fstream rootFile("root.txt", ios::out | ios::trunc);
    rootFile << newRoot;
    this->root = newRoot;
    rootFile.close();
}

int TreeManager::getNextIndex(fstream& binFile) {
    binFile.seekp(0, ios::end);
    long pos = binFile.tellp();
    return (int)(pos / sizeof(Node)) + 1;
}

