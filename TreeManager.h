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
    int root;

    public:

        struct Result {
            int p;
            int i;
            bool success;
        };
        
        TreeManager(int m, int root);
        Result mSearch(fstream& binFile, int x, int *b = nullptr);
        int insertB(fstream& binFile, int x, int b);
        int deleteB(fstream& binFile, int x, int &b);
        void removeFromNode(fstream& binFile, int nodeIndex, int key);
        void balanceAfterRemove(fstream& binFile, int nodeIndex);
        void fuseNodes(fstream& binFile, int parentIndex, int leftPos, int rightPos);
        int getParentIndex(fstream& binFile, int child);
        void printTree(fstream& binFile);  
        int getRoot();
        void updateRoot(int newRoot);   
        int getNextIndex(fstream& binfile);
};

#endif