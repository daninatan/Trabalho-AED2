//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//OBS: CLASSE UTILIZADA PARA LER E ESCREVER MVIAS.TXT, NO ENTANTO NAO ESTÁ SENDO USADO NESSA PARTE DO TRABALHO
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <iostream>
#include <fstream>
#include <string>
#include <climits>

using namespace std;

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#define MAX INT_MAX 
#define MIN INT_MIN

class FileManager{
private:
    int M;
    
public:
    fstream txtFile;
    fstream binFile;
    fstream databaseFile;
    FileManager(int m, string txtFile, string binFile);
    void readAndWrite(ostream& binFile);
};

#endif