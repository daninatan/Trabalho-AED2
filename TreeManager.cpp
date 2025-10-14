/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


#include "TreeManager.h"
#include "Node.h"
#include <fstream>
#include <vector>
#include <string>

TreeManager::TreeManager(int m) : M(m) {}

TreeManager::Result TreeManager::mSearch(fstream& file, int x, int* b){
    Node p(M); //utilizada para fazer as leitutas
    int q = 1; //utilizada para guardar em qual indice do arquivo o registro esta
    if(file.tellg() == 0) {
        return {0, 0, false}; 
    }
    file.seekg(0, ios::beg);
    file.read((char*)(&p), sizeof(Node));

    while(true){
        int index = 0;

        for(int i = 0; i <= p.n; i++){
            if(p.K[i] <= x && x < p.K[i+1]){
                index = i;
            }
        }

        if(x == p.K[index]){
            if(b!=nullptr){
                *b = p.B[index];
            }
            return {q, index, true};
            break;
        }else{     
            if(p.A[index] == 0){
                return {q, index, false};
                break;
            }else{
                q = p.A[index];
                file.seekg((p.A[index] - 1) * sizeof(Node), ios::beg);
                file.read((char*)(&p), sizeof(Node));
            }
           
        }
    }
    return {0, 0, false};
}

int TreeManager::insertB(fstream& binFile, int x, int b) {
    int K = x;
    int A = 0; 
    int B = b;

    Result result = mSearch(binFile, x), result2;


    if(result.success){
       return 0; //já está inserido
    }

    if (result.p == 0) {
        // Cria a primeira raiz no índice 1 
        Node root(M);
        root.n = 1;
        root.K[1] = x;
        root.B[1] = b;
        root.A[0] = 0; // Ponteiro para o nó (filho) 0
        root.A[1] = 0; // Ponteiro para o nó (filho) A
        
        binFile.seekp(0, ios::beg); 
        binFile.write((const char *) (&root), sizeof(Node));
        return 1;
    }

    int index = result.p;

    while(true) {  // Sempre tenta inserir
        Node p(M);
        binFile.seekg((index - 1) * sizeof(Node), ios::beg);  // Ler o nó correto
        binFile.read((char*)(&p), sizeof(Node));

        for (int j = p.n; j > result.i; j--){ //move todos os elementos para a direita, se a nova chave estiver em um espaço ja ocupado
            p.K[j + 1] = p.K[j];
            p.A[j+1] = p.A[j];
            p.B[j+1] = p.B[j];
        }

        p.K[result.i + 1] = K; //insere o valor
        p.A[result.i + 1] = A;
        p.B[result.i + 1] = B;
        p.n++; //aumenta o número de elementos no nó

        if(p.n < M){ //cabe a inserção
            binFile.seekp((index - 1) * sizeof(Node), ios::beg);
            binFile.write((const char *)(&p), sizeof(Node)); //escreve no arquivo
            return 1;
        }

        int mid = (p.n + 1) / 2;
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

        binFile.seekp((index - 1) * sizeof(Node), ios::beg);
        binFile.write((const char *)(&p), sizeof(Node)); //escreve o P

        binFile.seekp(0, ios::end);
        int indexQ = (int)(binFile.tellp() / sizeof(Node)) + 1; // Corrigir cálculo do índice
        binFile.write((const char *)(&q), sizeof(Node)); //escreve o q

        K = p.K[mid];
        B = p.B[mid];
        A = indexQ;
        
        index = getParentIndex(binFile, index);
        if(index == 0) break; // Se chegou na raiz, sair do loop
        
        result2 = mSearch(binFile, K);
        result.i = result2.i;
    }

    // Só cria nova raiz se saiu do loop por split na raiz
    if(index == 0) {
        Node root(M);
        root.n = 1;
        root.K[1] = K;
        root.A[0] = 1;
        root.A[1] = A;
        root.B[1] = B;

        binFile.seekp(0, ios::end);
        binFile.write((const char *) (&root), sizeof(Node));
    }
    return 1;
}

int TreeManager::getParentIndex(fstream& binFile, int child){
    if(child == 1) // rootIndex é o índice da raiz da árvore
        return 0;

    Node node(M);
    binFile.seekg(0, ios::beg);

    int index = 1; // assumindo índices começam em 1
    while(binFile.read((char*)&node, sizeof(Node))) {
        for(int i = 0; i <= node.n; i++) { // percorre todos os ponteiros do nó
            if(node.A[i] == child) {
                return index; // achou o pai
            }
        }
        index++;
    }

    return 1; 
}

//funcao para printar o arquivo completo na tela
void TreeManager::printTree(fstream& file){
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

