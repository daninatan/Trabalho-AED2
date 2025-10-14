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
        int lastB;

        struct DatabaseReg{
            int key;
            char name[100];
            int age;
            char uf[3];
        };

        DatabaseManager(string databaseFileName, string databaseBinaryName);
        void write();
        DatabaseReg search(int b);
        void addRegister(DatabaseReg reg);
        void createTree(fstream& treeFile, TreeManager* tree);
};

#endif 