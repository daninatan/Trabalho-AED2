/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


#include "TreeManager.h"
#include "Node.h"
#include <fstream>
#include <vector>
#include <string>

TreeManager::TreeManager(int m, int root) : M(m) {
    updateRoot(root);
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

    int currentP_Index = result.p; 
    int insertion_i = result.i;     
    int parentIndex = 0;            
    int mid = 0;                   
    int indexQ = 0;                 

    while(true) {
        Node p(M);
        binFile.seekg((currentP_Index - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&p), sizeof(Node));

        // Move elementos para a direita
        for (int j = p.n; j > insertion_i; j--){
            p.K[j + 1] = p.K[j];
            p.A[j + 1] = p.A[j]; 
            p.B[j + 1] = p.B[j];
        }

        p.K[insertion_i + 1] = K;
        p.A[insertion_i + 1] = A; // A é o índice do novo nó Q ou 0 se folha
        p.B[insertion_i + 1] = B;
        p.n++;

        if(p.n < M){ // Cabe a inserção
            binFile.seekp((currentP_Index - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&p), sizeof(Node));
            return 1;
        }

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
        indexQ = getNextIndex(binFile); 
        binFile.seekp(0, ios::end);
        binFile.write((const char*)(&q), sizeof(Node));

        K = p.K[mid];
        B = p.B[mid];
        A = indexQ; 
        
        parentIndex = getParentIndex(binFile, currentP_Index);
        
        if(parentIndex == 0) break; // Chegou na raiz

        currentP_Index = parentIndex;
        
        Node parent(M);
        binFile.seekg((parentIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&parent), sizeof(Node));
        
        insertion_i = 0;
        for(int i = 1; i <= parent.n; i++){
            if(K < parent.K[i]){
                insertion_i = i - 1;
                break;
            }
            insertion_i = i; 
        }
    }

    if(parentIndex == 0) { 
        int oldRootIndex = getRoot();
        int newRootIndex = getNextIndex(binFile);
        
        Node root(M);
        root.n = 1;
        root.K[1] = K;
        root.A[0] = oldRootIndex; 
        root.A[1] = A; 
        root.B[1] = B;

        binFile.seekp(0, ios::end);
        binFile.write((const char*)(&root), sizeof(Node));
        
        updateRoot(newRootIndex);
    }
    return 1;
}

int TreeManager::deleteB(fstream& binFile, int x, int &b){
    Result result = mSearch(binFile, x);

    if(!result.success){
        return 0;     //não achou
    }

    int currentIndex = result.p;
    int i = result.i;

    Node node(M);

    binFile.seekg((currentIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char *)(&node), sizeof(Node));
    b = node.B[i]; //atualiza a raíz para alterar o txt

    //chave na folha
    if (node.A[0] == 0) {
        for (int j = i; j < node.n; j++) {
            node.K[j] = node.K[j + 1];
            node.B[j] = node.B[j + 1];
        }
        node.n--;

        binFile.seekp((currentIndex - 1) * sizeof(Node), ios::beg);
        binFile.write((const char*)(&node), sizeof(Node));

        balanceAfterRemove(binFile, currentIndex); // balanceia se necessário
        return 1;
    }

    //Chave no nó interno
    int predIndex = node.A[i - 1];
    Node predNode(M);

    while (true) {
        binFile.seekg((predIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&predNode), sizeof(Node));
        if (predNode.A[predNode.n] == 0) break; // folha
        predIndex = predNode.A[predNode.n];
    }

    //Substitui chave
    node.K[i] = predNode.K[predNode.n];
    node.B[i] = predNode.B[predNode.n];

    binFile.seekp((currentIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&node), sizeof(Node));

    //Remove o predecessor
    removeFromNode(binFile, predIndex, predNode.K[predNode.n]);
    return 1;
}

void TreeManager::removeFromNode(fstream& binFile, int nodeIndex, int key) {
    Node node(M);
    binFile.seekg((nodeIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&node), sizeof(Node));

    int i = 1;
    while (i <= node.n && node.K[i] != key) i++;

    if (i > node.n) return; //não encontrado
    for (int j = i; j < node.n; j++) {
        node.K[j] = node.K[j + 1];
        node.B[j] = node.B[j + 1];
    }
    node.n--;

    binFile.seekp((nodeIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&node), sizeof(Node));

    balanceAfterRemove(binFile, nodeIndex);
}

void TreeManager::balanceAfterRemove(fstream& binFile, int nodeIndex) {
    Node node(M);
    binFile.seekg((nodeIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&node), sizeof(Node));

    int minKeys = (M / 2) - 1; // número mínimo de chaves
    if (node.n >= minKeys) return; // já está balanceado

    int parentIndex = getParentIndex(binFile, nodeIndex);
    if (parentIndex == 0) {
        // se for a raiz
        if (node.n == 0 && node.A[0] != 0) {
            updateRoot(node.A[0]); // reduz altura
        }
        return;
    }

    //lê o nó pai
    Node parent(M);
    binFile.seekg((parentIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&parent), sizeof(Node));

    //descobre posição do ponteiro no pai
    int pos = 0;
    for (int i = 0; i <= parent.n; i++) {
        if (parent.A[i] == nodeIndex) { pos = i; break; }
    }

    //Tenta redistribuir com o irmão esquerdo
    if (pos > 0) {
        int leftIndex = parent.A[pos - 1];
        Node left(M);
        binFile.seekg((leftIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&left), sizeof(Node));

        if (left.n > minKeys) {
            //desloca chaves e ponteiros do nó atual para abrir espaço à esquerda
            for (int j = node.n - 1; j >= 0; j--) {
                node.K[j + 1] = node.K[j];
                node.B[j + 1] = node.B[j];
                node.A[j + 1] = node.A[j];
            }

            //move chave do pai para o nó atual
            node.K[0] = parent.K[pos - 1];
            node.B[0] = parent.B[pos - 1];
            node.A[0] = left.A[left.n]; //ponteiro do irmão esquerdo

            //atualiza pai
            parent.K[pos - 1] = left.K[left.n - 1];
            parent.B[pos - 1] = left.B[left.n - 1];

            //atualiza contadores
            left.n--;
            node.n++;

            //grava tudo de volta
            binFile.seekp((leftIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&left), sizeof(Node));

            binFile.seekp((nodeIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&node), sizeof(Node));

            binFile.seekp((parentIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&parent), sizeof(Node));

            return;
        }
    }

    //Tenta redistribuir com o irmão direito
    if (pos < parent.n) {
        int rightIndex = parent.A[pos + 1];
        Node right(M);
        binFile.seekg((rightIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&right), sizeof(Node));

        if (right.n > minKeys) {
            //move chave do pai para o nó atual
            node.K[node.n] = parent.K[pos];
            node.B[node.n] = parent.B[pos];
            node.A[node.n + 1] = right.A[0];
            node.n++;

            //move primeira chave do irmão direito para o pai
            parent.K[pos] = right.K[0];
            parent.B[pos] = right.B[0];

            //desloca chaves e ponteiros do irmão direito para a esquerda
            for (int j = 0; j < right.n - 1; j++) {
                right.K[j] = right.K[j + 1];
                right.B[j] = right.B[j + 1];
                right.A[j] = right.A[j + 1];
            }
            right.A[right.n - 1] = right.A[right.n];// último ponteiro
            right.n--;

            // grava tudo
            binFile.seekp((rightIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&right), sizeof(Node));

            binFile.seekp((nodeIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&node), sizeof(Node));

            binFile.seekp((parentIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&parent), sizeof(Node));

            return;
        }
    }

    //Caso contrário, realiza fusão
    if (pos > 0) {
        fuseNodes(binFile, parentIndex, pos - 1, pos); //funde com o irmão esquerdo
    } else {
        fuseNodes(binFile, parentIndex, pos, pos + 1); //funde com o irmão direito
    }
}

void TreeManager::fuseNodes(fstream& binFile, int parentIndex, int leftPos, int rightPos) {
    Node parent(M);
    binFile.seekg((parentIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&parent), sizeof(Node));

    int leftIndex = parent.A[leftPos];
    int rightIndex = parent.A[rightPos];

    Node left(M), right(M);
    binFile.seekg((leftIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&left), sizeof(Node));
    binFile.seekg((rightIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&right), sizeof(Node));

    left.K[++left.n] = parent.K[rightPos];
    left.B[left.n] = parent.B[rightPos];
    left.A[left.n] = right.A[0];

    for (int j = 1; j <= right.n; j++) {
        left.K[++left.n] = right.K[j];
        left.B[left.n] = right.B[j];
        left.A[left.n] = right.A[j];
    }

    //remove chave do pai
    for (int j = rightPos; j < parent.n; j++) {
        parent.K[j] = parent.K[j + 1];
        parent.B[j] = parent.B[j + 1];
        parent.A[j] = parent.A[j + 1];
    }
    parent.n--;

    //grava
    binFile.seekp((leftIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&left), sizeof(Node));
    binFile.seekp((parentIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&parent), sizeof(Node));

    //Se o pai ficou vazio e era raiz → atualiza
    if (parentIndex == getRoot() && parent.n == 0) {
        updateRoot(leftIndex);
    }

    balanceAfterRemove(binFile, parentIndex);
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
        if(p.n != 0){
            cout << index << "  ";
            cout << p.n << "  ";
            cout << p.A[0] << ",    ";
            for(int i = 1; i <= p.n; i++){
                cout << "(" << p.K[i] << ",    " << p.A[i] << ",    " << p.B[i] << ")  ";
            }
            cout << endl;
        }
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
    fstream rootFile("treeInfo.txt", ios::out | ios::trunc);
    rootFile << newRoot << " " << M;
    this->root = newRoot;
    rootFile.close();
}

int TreeManager::getNextIndex(fstream& binFile) {
    binFile.seekp(0, ios::end);
    long pos = binFile.tellp();
    return (int)(pos / sizeof(Node)) + 1;
}

