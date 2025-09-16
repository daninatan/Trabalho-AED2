#include <iostream>
#include <fstream>
#include <string>
#include <climits> //para pegar o máximo e o mínimo int

using namespace std;

#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#define M 3 //quantidade de vias
#define MAX INT_MAX //maior e menor int representável
#define MIN INT_MIN;

class TreeManager{

    struct Node {
        int n;
        int K[M - 1 + 2];
        int A[M];
    };

    struct Result { //struct para retornar o resultado 
        Node* node;
        int i;
        bool b;
    };

    public:
        
        void readAndWrite(string txtFile, string binFile);
        Result mSearch(string binFile, int x);
};

#endif