/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/


#include <iostream>
#include <fstream>
#include <string>
#include "TreeManager.h"

using namespace std;

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

class DatabaseManager{

    private:
        fstream databaseFile;
        fstream databaseBinary;
        string databaseFileName;
        string databaseBinaryName;

        void writeBinary();

    public:
        int lastB; //guarda o valor do ultimo b para a funcao de adicionar

        struct DatabaseReg{ //estrutura para os registros
            int key;
            char name[100];
            int age;
            char uf[3];
        };

        DatabaseManager(string databaseFileName, string databaseBinaryName);
        void print();
        DatabaseReg search(int b);
        void addRegister(DatabaseReg reg);
        void createTree(fstream& treeFile, TreeManager* tree);
};

#endif 