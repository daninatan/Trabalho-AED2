/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(string databaseFileName, string databaseBinaryName){
    this->databaseFileName = databaseFileName;
    this->databaseBinaryName = databaseBinaryName;
    writeBinary();
}

//printa a database
void DatabaseManager::print(){
    system("clear || cls");
    databaseFile.open(databaseFileName, ios::in);
    cout << "Chave, Nome, Idade, UF\n\n";
    DatabaseReg reg;
    while(databaseFile >> reg.key >> reg.name >> reg.age >> reg.uf){
        cout << reg.key << " " << reg.name << " " << reg.age << " " << reg.uf << "\n";
    }
    databaseFile.close();
}

//cria uma árvore com base nos dados da base de dados, a partir do m do usuario
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

//busca um registro a partir do b
DatabaseManager::DatabaseReg DatabaseManager::search(int b){
    DatabaseReg reg;
    databaseBinary.open(databaseBinaryName, ios::in | ios::binary);
    databaseBinary.clear(); 
    databaseBinary.seekg((b-1) * sizeof(DatabaseReg), ios::beg);
    databaseBinary.read((char*)(&reg), sizeof(DatabaseReg));
    databaseBinary.close();
    return reg;
}

//escreve um arquivo binario com a base de dados para procurar os registros a partir do b
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

//adiciona um registro na database, tanto no txt quanto no biário
void DatabaseManager::addRegister(DatabaseManager::DatabaseReg reg){
    databaseFile.open(databaseFileName, ios::app);
    databaseBinary.open(databaseBinaryName, ios::app | ios::binary);
    databaseFile << reg.key << " " << reg.name << " " << reg.age << " " << reg.uf << "\n";
    databaseBinary.seekp(0, ios::end);
    databaseBinary.write((const char *)(&reg), sizeof(DatabaseReg));
    databaseBinary.close();
    databaseFile.close();
}