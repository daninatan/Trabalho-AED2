/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


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

void TreeManager::mSearch(string binFile, int x){
    ifstream file;
    file.open(binFile, ios::in);
    Node p; //utilizada para fazer as leitutas
    int q = 1; //utilizada para guardar em qual indice do arquivo o registro esta
    file.read((char*)(&p), sizeof(Node));

    while(true){
        int index;

        for(int i = 0; i <= p.n; i++){
            if(p.K[i] <= x && x < p.K[i+1]){
                index = i;
            }
        }

        if(x == p.K[index]){
            cout <<  "(" << q << ", " << index << ", true)" << endl; //retornado caso a chave tenha sido encontrada
            return;
            break;
        }else{     
            if(p.A[index] == 0){
                cout << "(" << q << ", " << index << ", false)" << endl; //retornado caso a chave nao tenha sido encontrada
                break;
            }else{
                q = p.A[index];
                file.seekg((p.A[index] - 1) * sizeof(Node), ios::beg);
                file.read((char*)(&p), sizeof(Node));
            }
           
        }
    }
    return;
}

//funcao para printar o arquivo completo na tela
void TreeManager::printTree(string binFile){
    ifstream file;
    file.open(binFile, ios::in);
    Node p;
    int index = 1;

    cout << "===========================================" << endl;
    cout << "No n, A[0], (K[1],A[1]), ... , (K[n],A[n])" << endl;
    while(file.read((char*)(&p), sizeof(Node))){
        
        cout << index << "  ";
        cout << p.n << "  ";
        cout << p.A[0] << ",    ";
        for(int i = 1; i <= p.n; i++){
            cout << "(" << p.K[i] << ",  " << p.A[i] << ")  ";
        }
        cout << endl;
        index++;
    }
    cout << "===========================================\n\n";
}

