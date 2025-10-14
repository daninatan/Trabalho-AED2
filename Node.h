#include <string>
#include <vector>
using namespace std;

#ifndef NODE_H
#define NODE_H

struct Node {
    int M; //tamanho
    int n;
    int K[50]; //valor máximo
    int A[50]; 
    int B[50];

    Node(int m) : M(m) { //inicialização
        for(int i = 0; i < M; i++)
            A[i] = 0;
        for(int i = 0; i < M + 1; i++)
            K[i] = 0;
    }
}; 

#endif