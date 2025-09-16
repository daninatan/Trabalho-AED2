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
        treeNode.K[0] = MIN;
        treeNode.n = n;
        treeNode.K[n+1] = MAX;
        inFile >> treeNode.A[0]; //Le a primeira subarvore

        for(int i = 1; i <= treeNode.n; i++){ //Le as respectivas chaves e as respectivas subarvores, de acordo com o valor N
            inFile >> treeNode.K[i];
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

    while(true){
        int index;

        for(int i = 0; i <= p.n; i++){
            if(p.K[i] <= x && x < p.K[i+1]){
                index = i;
            }
        }
        cout << index << endl;
        cout << p.K[index] << endl;

        if(x == p.K[index]){
            cout << "Achou";
            return result;
            break;
        }else{     
            if(p.A[index] == 0){
                break;
            }else{
                file.seekg((p.A[index] - 1) * sizeof(Node), ios::beg);
                file.read((char*)(&p), sizeof(Node));
            }
           
        }
    }
    cout << "Nao achou";
    return result;
}

