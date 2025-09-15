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
        outFile.write((const char *)(&treeNode), sizeof(Node)); 
    }
    inFile.close();
    outFile.close();
}

TreeManager::Result TreeManager::mSearch(string binFile, int x){

    Result result;
    ifstream file;
    file.open(binFile, ios::in);
    Node p, q;
    file.read((char*)(&p), sizeof(Node));
    cout << p.A[0] << endl;
    cout << p.K[0] << endl;

    while(p.A[0] != 0 || p.A[1] != 0 || p.A[2] != 0){
        int index;

        if(x < p.K[0]){
            index = 0;
        }else if(x >= p.K[0] || x < p.K[1]){
            index = 1;
        }else{
            index = 2;
        }

        if(x == p.K[index]){
            break;
        }else{
            if(p.A[index] == 0){
                cout << index << endl;
                cout << p.A[index] << endl;
                cout << p.K[0] << endl;
                break;
            }else{
                file.seekg(p.A[index] * sizeof(Node), ios::beg);
                file.read((char*)(&p), sizeof(Node));
            }
           
        }
    }
    cout << "Nao achou";
    return result;
}

