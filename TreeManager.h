/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


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

    public:

        void readAndWrite(string txtFile, string binFile);
        void mSearch(string binFile, int x);

        
};

#endif