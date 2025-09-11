/*Realizar pesquisas em árvore de busca m-vias, com m = 3

a) Um procedimento que lê a árvore, a partir do arquivo-texto mvias.txt, no formato: n, A0,
(K1,A1), ..., (Kn,An) e grava cada registro no arquivo binário em disco mvias.bin. Isso significa
que o usuário deverá fornecer os valores de n, A0, (K1,A1), ..., (Kn,An) corretamente no arquivo
mvias.txt e que a árvore (arquivo binário) já deverá residir em disco para as partes seguintes do
trabalho. Omita, no arquivo mvias.txt, os separadores tais como vírgulas ou parênteses no
formato do nó se isso for mais conveniente para a leitura dos dados;

b) Um método que implementa o algoritmo mSearch;

c) O programa principal que deverá ler valores de chave fornecidos pelo usuário (pelo teclado) e,
usando mSearch, indicar se encontrou ou não a chave no índice. Se encontrar, a localização da chave
no índice também deve ser impressa; caso contrário, mostrar a localização de onde na árvore a chave
pode ser inserida, conforme o seguinte exemplo de execução:
*/

#include "FileManager.h"

using namespace std;

int main(){
    FileManager fileManager;
    fileManager.readAndWrite("mvias.txt", "mvias.bin");
    cout << "\n";
    fileManager.readBin("mvias.bin");
}