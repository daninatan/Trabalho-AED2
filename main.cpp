/*Participantes:
Guilherme Borges de Pádua Barbosa - 15653045
Daniel Natan dos Santos Brito - 15446902

Resposta pergunta D) Não funciona. Sim, é esperado, pois ele sempre lê o primeiro registro como sendo a raiz, portanto, quando tiramos 
a raíz do primeiro registro o algoritmo msearch não funciona.
*/

/*Abrir o arquivo fora da classe da árvore, fazer isso apenas uma vez e passar por parâmetro 
(daria pra criar uma classe só pra manipular o arquivo talvez e passar um objeto com o arquivo 
por parâmetro) Retornar uma estrutura com aquela tripla para poder fazer a 
inserção Guardar de alguma forma a raiz no arquivo e sempre ler ela primeiro */

#include "TreeManager.h"
#include "FileManager.h"

using namespace std;

void mSearchMenu(TreeManager T, fstream& F){
    int key;
    char confirmation;

    do{
        cout << "Chave de busca: ";
        if(!(cin >> key)){
            cout << "Tipo inválido de chave" << endl;
            break;
        }
        
        T.mSearch(F, key);
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
    TreeManager treeManager(3); //Tamanho 3 pra arvore, mas deixar como input
    FileManager fileManager(3, "mvias.txt", "mvias.bin");
    
    treeManager.printTree(fileManager.binFile);
    treeManager.insertB(fileManager.binFile, 16);
    //mSearchMenu(treeManager, fileManager.binFile);
    treeManager.printTree(fileManager.binFile);
}