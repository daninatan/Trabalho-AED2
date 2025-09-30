/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


#include <iostream>
#include <fstream>
#include <string>
#include <climits> //biblioteca para os valores maximos e minimos do int

using namespace std;

#ifndef TREEMANAGER_H
#define TREEMANAGER_H

#define MAX INT_MAX //maior e menor int representável
#define MIN INT_MIN

class TreeManager{
    int M;

    

    public:

        struct Result {
            int p;
            int i;
            bool success;
        };
        
        TreeManager(int m);
        Result mSearch(fstream& binFile, int x);
        void insertB(fstream& binFile, int x);
        int getParentIndex(fstream& binFile, int child);
        void printTree(fstream& binFile);     
};

#endif