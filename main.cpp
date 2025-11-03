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
#include "DatabaseManager.h"
#include <cstdlib>

using namespace std;

void menu(){
    int key, m, choice, searchValue, bResult;
    char confirmation;
    TreeManager::Result searchResult;
    DatabaseManager database("database.txt", "database.bin");
    DatabaseManager::DatabaseReg reg;

    cout << "Digite o valor de m: ";
    cin >> m;
    cout << "\n\n";

   // FileManager F(m, "mvias.txt", "mvias.bin");
    fstream binFile("mvias.bin", ios::in | ios::out | ios::binary | ios::trunc);
    TreeManager T(m);
    database.createTree(binFile, &T);

    do{
        system("clear || cls");

        cout << "1 - Imprimir o indice\n2 - Imprimir Arquivo Principal\n3 - Buscar elemento\n4 - Inserir elemento\n5 - Remover Elemento\n6 - Sair\n\n";
        cin >> choice;

        switch (choice) {
            case 1:
                T.printTree(binFile);
                cin.ignore();
                cin.get();
                break;
            case 2:
                database.print();
                cin.ignore();
                cin.get();
                break;
            case 3:
                system("clear || cls");       
                cout << "Escolha o valor para procurar: ";
                cin >> searchValue;
                searchResult = T.mSearch(binFile, searchValue, &bResult);
                cout << "\n\nResultado: (" << searchResult.i << ", " << searchResult.p << ", " << searchResult.success << ")";
                if(searchResult.success){
                    reg = database.search(bResult);
                    cout << "\n\nRegistro:\n\n";
                    cout << reg.key << " " << reg.name << " " << reg.age << " " << reg.uf << "\n";
                }else{
                    cout << "\n\nRegistro nao encontrado!\n\n";
                }
                
                cin.ignore();
                cin.get();
                break;
            case 4:
                system("clear || cls");
                cout << "Digite a chave: ";
                cin >> reg.key;
                cout << "\nDigite o nome: ";
                cin >> reg.name;
                cout << "\nDigite a idade: ";
                cin>> reg.age;
                cout << "\nDigite a UF: ";
                cin >> reg.uf;
                if(T.insertB(binFile, reg.key, database.lastB + 1)){
                    database.addRegister(reg);
                    database.lastB++;
                    cout << "\n\nAdicionado com sucesso!";
                }else{
                    cout << "\n\nJá existe esta chave";
                }
                cin.ignore();
                cin.get();
                break;
            case 5:
                system("clear || cls");
                cout << "Digite a chave: ";
                cin >> reg.key;
                int variavelBTemporariaParaTestar;

                if(T.deleteB(binFile, reg.key, variavelBTemporariaParaTestar)){
                    cout << "\n\nDeu certo aff";
                }else{
                    cout << "\n\nNão existe esta chave";
                }
                cin.ignore();
                cin.get();
                
                break;
            case 6:
                T.updateRoot(0);
                binFile.close();
                break;
            default:
                cout << "\nEscolha inválida, tente novamente";
                cin.ignore();
                cin.get();
        }
        
    }while(choice != 6);
}

int main(){
    system("clear || cls");
    menu();

}