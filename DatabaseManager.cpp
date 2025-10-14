#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(string databaseFileName, string databaseBinaryName){
    this->databaseFileName = databaseFileName;
    this->databaseBinaryName = databaseBinaryName;
    writeBinary();
}

void DatabaseManager::write(){
    system("clear || cls");
    databaseFile.open(databaseFileName, ios::in);
    cout << "Chave, Nome, Idade, UF\n\n";
    DatabaseReg reg;
    while(databaseFile >> reg.key >> reg.name >> reg.age >> reg.uf){
        cout << reg.key << " " << reg.name << " " << reg.age << " " << reg.uf << "\n";
    }
    databaseFile.close();
}

void DatabaseManager::createTree(fstream& treeFile, TreeManager* tree){
    DatabaseManager::DatabaseReg reg;
    databaseBinary.open(databaseBinaryName, ios::in | ios::binary);
    int i = 1;
    while(databaseBinary.read((char *)(&reg), sizeof(DatabaseReg))){
        tree->insertB(treeFile, reg.key, i);
        lastB = i;
        i++;
    }
}

DatabaseManager::DatabaseReg DatabaseManager::search(int b){
    DatabaseReg reg;
    databaseBinary.open(databaseBinaryName, ios::in | ios::binary);
    databaseBinary.seekg((b-1) * sizeof(DatabaseReg), ios::beg);
    databaseBinary.read((char*)(&reg), sizeof(DatabaseReg));
    databaseBinary.close();
    return reg;
}

void DatabaseManager::writeBinary(){
    DatabaseReg reg;
    databaseFile.open(databaseFileName, ios::in);
    databaseBinary.open(databaseBinaryName, ios::out | ios::binary);
    while(databaseFile >> reg.key >> reg.name >> reg.age >> reg.uf){
        databaseBinary.write((const char *)(&reg), sizeof(DatabaseReg));
    }
    databaseBinary.close();
    databaseFile.close();
}

void DatabaseManager::addRegister(DatabaseManager::DatabaseReg reg){
    databaseFile.open(databaseFileName, ios::app);
    databaseBinary.open(databaseBinaryName, ios::app | ios::binary);
    databaseFile << reg.key << " " << reg.name << " " << reg.age << " " << reg.uf << "\n";
    databaseBinary.write((const char *)(&reg), sizeof(DatabaseReg));
    databaseBinary.close();
    databaseFile.close();
}