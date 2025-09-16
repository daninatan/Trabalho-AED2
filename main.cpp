/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902*/

#include "TreeManager.h"

using namespace std;

void mSearch(){
    TreeManager T;
    int key;
    char confirmation;

    do{
        cout << "Chave de busca: ";
        if(!(cin >> key)){
            cout << "Tipo inválido de chave" << endl;
            break;
        }
        
        T.mSearch("mvias2.bin", key);
        //T.mSearch("mvias.bin", key);

        cout << "Continuar busca (s/n)? ";
        cin >> confirmation;

        if (confirmation == 'n')
            break;
        else if(confirmation != 's'){
            cout << "Entrada inválida" << endl;
            break;
        }

    }while(true);
}

int main(){
    TreeManager treeManager;
    treeManager.readAndWrite("mvias.txt", "mvias.bin");
    treeManager.readAndWrite("mvias2.txt", "mvias2.bin");
    mSearch();
}