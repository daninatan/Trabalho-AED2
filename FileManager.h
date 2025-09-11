#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#define M 3 //quantidade de vias

class FileManager{

    struct Node {

    int n;
    int K[M - 1];
    int A[M];

    };

    public:
        
        void readAndWrite(string txtFile, string binFile);
        void readBin(string binFile);
};

#endif