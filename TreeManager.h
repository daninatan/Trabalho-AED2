#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#define M 3 //quantidade de vias
#define MAX 2147483648; //maior e menor int representável
#define MIN -2147483648;

class TreeManager{

    struct Node {
        int n;
        int K[M - 1];
        int A[M];
    };

    struct Result { //struct para retornar o resultado 
        Node* node;
        int i;
        bool b;
    };

    public:
        
        void readAndWrite(string txtFile, string binFile);
        void readBin(string binFile);
        Result mSearch(Node* T, int x);
};

#endif