#include "FileManager.h"
#include <fstream>

void FileManager::readAndWrite(string txtFile, string binFile){
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

void FileManager::readBin(string binFile){

    Node node;
    ifstream inFile;
    inFile.open(binFile, ios::binary);

    while(inFile.read((char*) (&node), sizeof(Node))){ 
        cout << node.n << " " << node.A[0] << " ";
    }
}

