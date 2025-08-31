#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
//Esta función tiene como entrada un vector y un dato de tipo string, el cual retorna true o false dependiendo si el dato ya existe en el vector a partir del uso de find().
bool existeVector(vector<string> lista, string word){
    return find(lista.begin(),lista.end(),word) != lista.end();
    }
int main(){
    string line, path="./archivos_deportes";
    DIR *direccion;
    vector<string> deporte_categoria;
    direccion=opendir(path.c_str());
    mkdir("./Carpeta actual", 0777);
    while(dirent*i=readdir(direccion)){   
        string name = i->d_name;
        if(name!="." && name!=".."){
            string dep_aux,cat_aux, medalla,path_archivo="./archivos_deportes/"+name;
            ifstream archivo(path_archivo);
            archivo.is_open();                  
            int j=0;
            while(getline(archivo, line)){
                if(j==0){dep_aux=line;}
                else if(j==1){cat_aux=line;}
                else if(j==2){medalla=line;}
                j++;
            }
            string aux=dep_aux+"/"+cat_aux;
            if(existeVector(deporte_categoria,aux)==false){
                deporte_categoria.push_back(aux);
                if(existeVector(deporte_categoria,dep_aux)==false){
                    deporte_categoria.push_back(dep_aux);
                    mkdir(("./Carpeta actual/"+dep_aux).c_str(), 0777);
                }
                mkdir(("./Carpeta actual/"+aux).c_str(), 0777);
                mkdir(("./Carpeta actual/"+aux+"/ConMedalla").c_str(), 0777);
                mkdir(("./Carpeta actual/"+aux+"/SinMedalla").c_str(), 0777);
            }
            if(medalla=="Sin Medalla"){
                string p="Carpeta actual/"+dep_aux+"/"+cat_aux+"/SinMedalla/"+name;
                archivo.close();
                if(rename(path_archivo.c_str(),p.c_str())!=0){
                    cout<<"Todo mal"<<endl;
                };
            }
            else{
                string p="Carpeta actual/"+dep_aux+"/"+cat_aux+"/ConMedalla/"+name;
                archivo.close();
                if(rename(path_archivo.c_str(),p.c_str())!=0){
                    cout<<"Todo mal"<<endl;
                }
            }
        }
    }
    closedir(direccion);
    return 0;
}
