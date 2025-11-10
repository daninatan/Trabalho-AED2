/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(string databaseFileName, string databaseBinaryName, char reconstruct){
    this->databaseFileName = databaseFileName;
    this->databaseBinaryName = databaseBinaryName;
    if(reconstruct == 's'){
        writeBinary();
    }   
}

void DatabaseManager::print(){
    system("clear || cls");
    databaseBinary.open(databaseBinaryName, ios::in | ios::binary);
    databaseBinary.clear();
    databaseBinary.seekg(0, ios::beg);
    cout << "Chave, Nome, Idade, UF\n\n";
    DatabaseReg reg;
    while(databaseBinary.read((char *)(&reg), sizeof(DatabaseReg))){
        if(reg.active){
            cout << reg.key << " " << reg.name << " " << reg.age << " " << reg.uf << "\n";
        }
    }
    databaseBinary.close();
}

//cria uma árvore com base nos dados da base de dados, a partir do m do usuario
void DatabaseManager::createTree(fstream& treeFile, TreeManager* tree){
    DatabaseManager::DatabaseReg reg;
    databaseBinary.open(databaseBinaryName, ios::in | ios::binary);
    int i = 1;
    while(databaseBinary.read((char *)(&reg), sizeof(DatabaseReg))){
        tree->insertB(treeFile, reg.key, i);
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
        reg.active = true;
        databaseBinary.write((const char *)(&reg), sizeof(DatabaseReg));
    }
    databaseBinary.close();
    databaseFile.close();
}

//adiciona um registro na database, tanto no txt quanto no biário
void DatabaseManager::addRegister(DatabaseManager::DatabaseReg reg, int b){
    int index = 1;
    databaseBinary.open(databaseBinaryName, ios::in | ios::out | ios::binary);
    databaseBinary.seekp((b - 1) * sizeof(DatabaseReg), ios::beg);
    databaseBinary.write((const char *)(&reg), sizeof(DatabaseReg));
    databaseBinary.close();
}

int DatabaseManager::findB(){
    DatabaseReg reg;
    int index = 1;
    databaseBinary.open(databaseBinaryName, ios::in | ios::binary);
    databaseBinary.clear();
    databaseBinary.seekg(0, ios::beg);
    while(databaseBinary.read((char *)(&reg), sizeof(DatabaseReg))){
        if(!reg.active){
            databaseBinary.close();
            return index;
        }
        index++;
    }
    databaseBinary.close();
    return index;
}

void DatabaseManager::removeRegister(int b){
    DatabaseReg reg;

    fstream file(databaseBinaryName, ios::in | ios::out | ios::binary);

    if(!file){
        cout << "Erro ao abrir o arquivo para remoção!\n";
        return;
    }

    // posiciona e lê o registro
    file.seekg((b - 1) * sizeof(DatabaseReg));
    file.read(reinterpret_cast<char*>(&reg), sizeof(DatabaseReg));

   /* if(!file.good()){
        file.clear(); 
    }*/

    // marca como removido
    reg.active = false;

    // regrava
    file.seekp((b - 1) * sizeof(DatabaseReg));
    file.write(reinterpret_cast<const char*>(&reg), sizeof(DatabaseReg));

    file.flush();
    file.close();
}


