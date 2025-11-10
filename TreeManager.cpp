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

    if(!result.success){ //nao achou
        return 0;
    }

    int currentIndex = result.p;
    int i = result.i;

    Node node(M);

    binFile.seekg((currentIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char *)(&node), sizeof(Node));

    b = node.B[i];

    //chave em nó folha
    if (node.A[0] == 0) {
        // shift left: copiar de i+1 .. n para i .. n-1 (evita ler K[n+1])
        for (int j = i; j < node.n; j++) {
            node.K[j] = node.K[j + 1];
            node.B[j] = node.B[j + 1];
        }
        // limpa última posição
        node.K[node.n] = 0;
        node.B[node.n] = 0;
        node.n--;

        binFile.seekp((currentIndex - 1) * sizeof(Node), ios::beg);
        binFile.write((const char*)(&node), sizeof(Node));
        binFile.flush();

        // Se não é raiz E ficou com poucas chaves, balanceia
        if (currentIndex != getRoot()) {
            balanceAfterRemove(binFile, currentIndex);
        }
        
        // Verifica raiz após balanceamento
        checkAndUpdateRoot(binFile);
        return 1;
    }

    //chave em nó interno - busca predecessor (maior chave da subárvore esquerda)
    int predIndex = node.A[i - 1];
    Node predNode(M);

    // Vai sempre para o filho mais à direita até encontrar uma folha
    while (true) {
        binFile.seekg((predIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&predNode), sizeof(Node));
        // folha: A[0] == 0
        if (predNode.A[0] == 0) break; 
        // segue para o ponteiro mais à direita
        predIndex = predNode.A[predNode.n];
    }

    //substitui a chave pelo predecessor
    int predKey = predNode.K[predNode.n];
    int predB = predNode.B[predNode.n];
    
    node.K[i] = predKey;
    node.B[i] = predB;

    binFile.seekp((currentIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&node), sizeof(Node));
    binFile.flush();

    //remove do predecessor (folha)
    removeFromNode(binFile, predIndex, predKey);
    
    // Verifica raiz após toda a operação
    checkAndUpdateRoot(binFile);

    return 1;
}

void TreeManager::removeFromNode(fstream& binFile, int nodeIndex, int key) {
    Node node(M);
    binFile.seekg((nodeIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&node), sizeof(Node));

    int i = 1;
    while (i <= node.n && node.K[i] != key) i++;

    if (i > node.n) {
        return; // Chave não encontrada
    }

    //remove a chave - desloca as chaves seguintes (corrige off-by-one)
    for (int j = i; j < node.n; j++) {
        node.K[j] = node.K[j + 1];
        node.B[j] = node.B[j + 1];
    }
    
    // Para nós internos, também ajusta os ponteiros
    if (node.A[0] != 0) {
        // existem node.n+1 ponteiros antes da remoção; mover A[i+1..n+1] para A[i..n]
        for (int j = i; j <= node.n; j++) {
            node.A[j] = node.A[j + 1];
        }
    }
    
    node.K[node.n] = 0;
    node.B[node.n] = 0;
    node.A[node.n + 1] = 0; // limpa possível ponteiro extra
    node.n--;

    //escreve no arquivo
    binFile.seekp((nodeIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&node), sizeof(Node));
    binFile.flush();

    // Se não é raiz, balanceia
    if (nodeIndex != getRoot()) {
        balanceAfterRemove(binFile, nodeIndex);
    }
}

// Verifica e atualiza a raiz se necessário
void TreeManager::checkAndUpdateRoot(fstream& binFile) {
    int rootIndex = getRoot();
    Node root(M);
    
    binFile.seekg((rootIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&root), sizeof(Node));
    
    // Se a raiz está vazia E tem filhos, promove o único filho
    if (root.n == 0 && root.A[0] != 0) {
        updateRoot(root.A[0]);
    }
}

void TreeManager::balanceAfterRemove(fstream& binFile, int nodeIndex) {
    // Relê o nó para garantir dados atualizados
    Node node(M);
    binFile.seekg((nodeIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&node), sizeof(Node));

    int minKeys = M / 2;
    
    // Se é raiz, não balanceia (será tratado por checkAndUpdateRoot)
    if (nodeIndex == getRoot()) {
        return;
    }

    // Se tem chaves suficientes, não precisa balancear
    if (node.n >= minKeys) return;

    //lê o nó pai
    int parentIndex = getParentIndex(binFile, nodeIndex);
    
    if (parentIndex == 0) {
        return;
    }

    Node parent(M);
    binFile.seekg((parentIndex - 1) * sizeof(Node), ios::beg);
    binFile.read((char*)(&parent), sizeof(Node));

    //descobre posição do ponteiro no pai
    int pos = -1;
    for (int i = 0; i <= parent.n; i++) {
        if (parent.A[i] == nodeIndex) { 
            pos = i; 
            break; 
        }
    }

    if (pos == -1) {
        return;
    }

    //tenta redistribuir com o irmão esquerdo
    if (pos > 0) {
        int leftIndex = parent.A[pos - 1];
        Node left(M);
        binFile.seekg((leftIndex - 1) * sizeof(Node), ios::beg);
        binFile.read((char*)(&left), sizeof(Node));

        if (left.n > minKeys) {
            //desloca chaves do nó atual para abrir espaço à esquerda
            for (int j = node.n; j >= 1; j--) {
                node.K[j + 1] = node.K[j];
                node.B[j + 1] = node.B[j];
            }
            for (int j = node.n; j >= 0; j--) {
                node.A[j + 1] = node.A[j];
            }

            // A chave K[pos] separa o irmão esquerdo do nó atual
            node.K[1] = parent.K[pos];
            node.B[1] = parent.B[pos];
            node.A[0] = left.A[left.n];
            node.n++;

            // A última chave do irmão esquerdo sobe para o pai
            parent.K[pos] = left.K[left.n];
            parent.B[pos] = left.B[left.n];

            //limpa última posição do irmão esquerdo
            left.K[left.n] = 0;
            left.B[left.n] = 0;
            left.A[left.n] = 0;
            left.n--;

            //grava tudo de volta
            binFile.seekp((leftIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&left), sizeof(Node));

            binFile.seekp((nodeIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&node), sizeof(Node));

            binFile.seekp((parentIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&parent), sizeof(Node));

            binFile.flush();
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
            // A chave K[pos+1] separa o nó atual do irmão direito
            node.K[node.n + 1] = parent.K[pos + 1];
            node.B[node.n + 1] = parent.B[pos + 1];
            node.A[node.n + 1] = right.A[0];
            node.n++;

            // A primeira chave do irmão direito sobe para o pai
            parent.K[pos + 1] = right.K[1];
            parent.B[pos + 1] = right.B[1];

            //desloca chaves e ponteiros do irmão direito para a esquerda
            for (int j = 1; j <= right.n; j++) {
                right.K[j] = right.K[j + 1];
                right.B[j] = right.B[j + 1];
            }
            for (int j = 0; j <= right.n; j++) {
                right.A[j] = right.A[j + 1];
            }
            
            //limpa última posição
            right.K[right.n] = 0;
            right.B[right.n] = 0;
            right.A[right.n] = 0;
            right.n--;

            // grava tudo
            binFile.seekp((rightIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&right), sizeof(Node));

            binFile.seekp((nodeIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&node), sizeof(Node));

            binFile.seekp((parentIndex - 1) * sizeof(Node), ios::beg);
            binFile.write((const char*)(&parent), sizeof(Node));

            binFile.flush();
            return;
        }
    }

    //Fusão: não conseguiu redistribuir
    if (pos > 0) {
        fuseNodes(binFile, parentIndex, pos - 1, pos);
    } else {
        fuseNodes(binFile, parentIndex, pos, pos + 1);
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

    // A chave separadora entre leftPos e rightPos
    int separatorIndex = rightPos;
    
    //move chave separadora do pai para o nó esquerdo
    left.n++;
    left.K[left.n] = parent.K[separatorIndex];
    left.B[left.n] = parent.B[separatorIndex];
    left.A[left.n] = right.A[0];

    //copia todas as chaves do nó direito para o esquerdo
    for (int j = 1; j <= right.n; j++) {
        left.n++;
        left.K[left.n] = right.K[j];
        left.B[left.n] = right.B[j];
        left.A[left.n] = right.A[j];
    }

    //remove chave separadora e ponteiro rightPos do pai
    for (int j = separatorIndex; j < parent.n; j++) {
        parent.K[j] = parent.K[j + 1];
        parent.B[j] = parent.B[j + 1];
    }
    for (int j = rightPos; j < parent.n; j++) {
        parent.A[j] = parent.A[j + 1];
    }
    
    //limpa última posição do pai
    parent.K[parent.n] = 0;
    parent.B[parent.n] = 0;
    parent.A[parent.n] = 0;
    parent.n--;

    //grava os nós modificados
    binFile.seekp((leftIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&left), sizeof(Node));
    
    binFile.seekp((parentIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&parent), sizeof(Node));

    // LIMPEZA: zera o nó direito (já não é referenciado) e grava para evitar "fantasmas"
    right.n = 0;
    for (int j = 0; j <= M; j++) { // limpa arrays completamente
        right.K[j] = 0;
        right.B[j] = 0;
        right.A[j] = 0;
    }
    binFile.seekp((rightIndex - 1) * sizeof(Node), ios::beg);
    binFile.write((const char*)(&right), sizeof(Node));

    binFile.flush();

    // Se o pai não é a raiz, propaga o balanceamento
    if (parentIndex != getRoot()) {
        balanceAfterRemove(binFile, parentIndex);
    }
}

int TreeManager::getParentIndex(fstream& binFile, int child){
    int rootIndex = getRoot();
    
    if(child == rootIndex)
        return 0;

    Node node(M);
    binFile.seekg(0, ios::beg);
    binFile.clear();

    int index = 1;
    while(binFile.read((char*)&node, sizeof(Node))) {
        for(int i = 0; i <= node.n; i++) {
            if(node.A[i] == child) {
                binFile.clear();
                return index;
            }
        }
        index++;
    }
    
    binFile.clear();
    return 0;
}

//funcao para printar o arquivo completo na tela
void TreeManager::printTree(fstream& file){
    system("clear || cls");
    cout << "Raiz: " << getRoot() << "\n\n";
    Node p(M);
    int index = 1;

    cout << "=========================================================" << endl;
    cout << "No n, A[0], (K[1], A[1], B[1]), ... , (K[n], A[n], B[n]) \n\n";
    
    file.clear();
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
    rootFile.clear();
    rootFile << newRoot << " " << M;
    this->root = newRoot;

    rootFile.close();
}

int TreeManager::getNextIndex(fstream& binFile) {
    binFile.seekp(0, ios::end);
    long pos = binFile.tellp();
    return (int)(pos / sizeof(Node)) + 1;
}