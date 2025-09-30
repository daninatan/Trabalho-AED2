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
#include <cstdlib>

using namespace std;

void menu(){
    int key, m, choice, searchValue, insertValue;
    char confirmation;
    TreeManager::Result searchResult;

    cout << "Digite o valor de m: ";
    cin >> m;
    cout << "\n\n";

    TreeManager T(m);
    FileManager F(m, "mvias.txt", "mvias.bin");

    do{
        system("clear || cls");

        cout << "1 - Imprimir o indice\n2 - Buscar elemento\n3 - Inserir elemento\n4 - Sair\n\n";
        cin >> choice;

        switch (choice) {
            case 1:
                T.printTree(F.binFile);
                cin.ignore();
                cin.get();
                break;
            case 2:
                system("clear || cls");       
                cout << "Escolha o valor para procurar: ";
                cin >> searchValue;
                searchResult = T.mSearch(F.binFile, searchValue);
                cout << "\n\nResultado: (" << searchResult.i << ", " << searchResult.p << ", " << searchResult.success << ")";
                cin.ignore();
                cin.get();
                break;
            case 3:
                system("clear || cls");
                cout << "Digite um valor: ";
                cin >> insertValue;
                T.insertB(F.binFile, insertValue);
                break;
            case 4:
                break;
            default:
                cout << "\nEscolha inválida, tente novamente";
                cin.ignore();
                cin.get();
        }
        
    }while(choice != 4);
}

int main(){
    system("clear || cls");
    menu();

}