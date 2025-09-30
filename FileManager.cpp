#include <iostream>
#include <fstream>
#include "FileManager.h"
#include "Node.h"
using namespace std;

FileManager::FileManager(int m, string txt, string bin) : M(m){
    ofstream outFile;
    outFile.open(bin, ios::binary); //Abre esse arquivo apenas na primeira execução da classe

    txtFile.open(txt, ios::in | ios::out); //Abre e mantém aberto durante a execução

    readAndWrite(outFile);
    outFile.close(); 

    binFile.open(bin, ios::binary | ios::in | ios::out); //Abre e mantém aberto durante a execução
}

void FileManager::readAndWrite(ostream& binFile){ //recebe tanto ofstrem e fstream
    int n;

    while(txtFile >> n){

        Node treeNode(M);
        treeNode.K[0] = MIN;
        treeNode.n = n;
        treeNode.K[n+1] = MAX;
        txtFile >> treeNode.A[0]; //Le a primeira subarvore

        for(int i = 1; i <= treeNode.n; i++){ //Le as respectivas chaves e as respectivas subarvores, de acordo com o valor N
            txtFile >> treeNode.K[i];
            txtFile >> treeNode.A[i];
        }  
        
        // Escrever os dados da estrutura sequencialmente
        binFile.write((const char *)(&treeNode), sizeof(Node));

    }

    txtFile.clear(); //limpar flags de erro
    txtFile.seekg(0, ios::beg); //voltar o ponteiro no começo do arquivo
}