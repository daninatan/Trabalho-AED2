/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902

Resposta pergunta D) Não funciona. Sim, é esperado, pois ele sempre lê o primeiro registro como sendo a raiz, portanto, quando tiramos 
a raíz do primeiro registro o algoritmo msearch não funciona.
*/

#include "TreeManager.h"

using namespace std;

void mSearchMenu(TreeManager T){
    int key;
    char confirmation;

    do{
        cout << "Chave de busca: ";
        if(!(cin >> key)){
            cout << "Tipo inválido de chave" << endl;
            break;
        }
        
        T.mSearch("mvias.bin", key);
        //T.mSearch("mvias2.bin", key);
        //T.mSearch("mvias2_2.bin", key);

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
    treeManager.readAndWrite("mvias2_2.txt", "mvias2_2.bin");
    treeManager.printTree("mvias.bin");
    mSearchMenu(treeManager);
}