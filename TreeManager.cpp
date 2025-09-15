#include "TreeManager.h"
#include <fstream>

void TreeManager::readAndWrite(string txtFile, string binFile){
    ifstream inFile;
    ofstream outFile;
    int n;

    inFile.open(txtFile, ios::in);
    outFile.open(binFile, ios::binary);

    while(inFile >> n){
        Node treeNode{};
        treeNode.n = n;
        inFile >> treeNode.A[0]; //Le a primeira subarvore

        for(int i = 1; i <= treeNode.n; i++){ //Le as respectivas chaves e as respectivas subarvores, de acordo com o valor N
            inFile >> treeNode.K[i - 1];
            inFile >> treeNode.A[i];
        }  
        cout << treeNode.n << " " << treeNode.A[0] << " ";
        outFile.write((const char *)(&treeNode), sizeof(Node)); 
    }
    inFile.close();
    outFile.close();
}

void TreeManager::readBin(string binFile){

    Node node;
    ifstream inFile;
    inFile.open(binFile, ios::binary);

    while(inFile.read((char*) (&node), sizeof(Node))){ 
        cout << node.n << " " << node.A[0] << " ";
    }
}

TreeManager::Result TreeManager::mSearch(Node* T, int x){
    TreeManager::Node* p = T;
    TreeManager::Node* q = NULL;

    while(p != NULL){
        int i = 0;
        for(int num : p->K){
            if(num == x){
                return {p, i, true};
            }
            i++;
        }
        q = p;
        p = p->A[i];
    }
    return {NULL, -1, false}; //nao encontrou
}

