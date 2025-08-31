#include<iostream>
#include<algorithm>
#include<random>
#include<fstream>
#include<vector>
#include<string>
#include<sys/types.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<semaphore.h>
using namespace std;

struct Game{
    vector<vector<string>> mazo;
    vector<vector<string>> pila_descarte;
    vector<vector<string>> mano_jugador;
    vector<vector<string>> mano_bot1;
    vector<vector<string>> mano_bot2;
    vector<vector<string>> mano_bot3;
    sem_t semaforo1;
    sem_t semaforo2;
    sem_t semaforo3;
    sem_t semaforo4;
    bool cambio_sentido;
    int pos;
    int roba;
    bool terminar_game;
    bool roba_uno;
    int pids[4];
};

void create_mazo(vector<vector<string>>&vec){
    vector<string> aux;
    for(int i=0;i<10;i++){//Cartas de color entre 0 y 9
        if(i!=0){
            for(int j=0;j<2;j++){
                aux.push_back(to_string(i));
                aux.push_back("rojo");
                vec.push_back(aux);
                aux.clear();
                aux.push_back(to_string(i));
                aux.push_back("azul");
                vec.push_back(aux);
                aux.clear();
                aux.push_back(to_string(i));
                aux.push_back("amarillo");
                vec.push_back(aux);
                aux.clear();
                aux.push_back(to_string(i));
                aux.push_back("verde");
                vec.push_back(aux);
                aux.clear();
            }
        }
        else{//i==0
            aux.push_back(to_string(i));
            aux.push_back("rojo");
            vec.push_back(aux);
            aux.clear();
            aux.push_back(to_string(i));
            aux.push_back("azul");
            vec.push_back(aux);
            aux.clear();
            aux.push_back(to_string(i));
            aux.push_back("amarillo");
            vec.push_back(aux);
            aux.clear();
            aux.push_back(to_string(i));
            aux.push_back("verde");
            vec.push_back(aux);
            aux.clear();
        }
    }
    for(int i=0;i<2;i++){//Cambio de sentido
        aux.push_back("Cambio de sentido");
        aux.push_back("rojo");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Cambio de sentido");
        aux.push_back("azul");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Cambio de sentido");
        aux.push_back("amarillo");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Cambio de sentido");
        aux.push_back("verde");
        vec.push_back(aux);
        aux.clear();
    }
    for(int i=0;i<2;i++){//Salta
        aux.push_back("Salta");
        aux.push_back("rojo");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Salta");
        aux.push_back("azul");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Salta");
        aux.push_back("amarillo");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Salta");
        aux.push_back("verde");
        vec.push_back(aux);
        aux.clear();
    }
    for(int i=0;i<2;i++){//Roba Dos
     aux.push_back("Roba Dos");
        aux.push_back("rojo");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Roba Dos");
        aux.push_back("azul");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Roba Dos");
        aux.push_back("amarillo");
        vec.push_back(aux);
        aux.clear();
        aux.push_back("Roba Dos");
        aux.push_back("verde");
        vec.push_back(aux);
        aux.clear();
    }
    for(int i=0;i<4;i++){//Comodín
        aux.push_back("Comodin");
        aux.push_back("negro");
        vec.push_back(aux);
        aux.clear();
    }
    for(int i=0;i<4;i++){//Comodín Roba Cuatro
        aux.push_back("Comodin Roba Cuatro");
        aux.push_back("negro");
        vec.push_back(aux);
        aux.clear();
    }
    //Desordena el mazo
    random_device dv;
    mt19937 g(dv());
    shuffle(vec.begin(),vec.end(),g);
}

void create_hand(vector<vector<string>>&vec, Game* &shared_game){
    for(int i = 0; i < 7; i++){
        vec.push_back(shared_game->mazo[0]);
        shared_game->mazo.erase(shared_game->mazo.begin());
    }
}

void robar_carta(Game* &shared_game, int &pos){
    if(pos==0){
        shared_game->mano_jugador.push_back(shared_game->mazo[0]);
        shared_game->mazo.erase(shared_game->mazo.begin());
    }
    else if(pos==1){
        shared_game->mano_bot1.push_back(shared_game->mazo[0]);
        shared_game->mazo.erase(shared_game->mazo.begin());
    }
    else if(pos==2){
        shared_game->mano_bot2.push_back(shared_game->mazo[0]);
        shared_game->mazo.erase(shared_game->mazo.begin());
    }
    else if(pos==3){
        shared_game->mano_bot3.push_back(shared_game->mazo[0]);
        shared_game->mazo.erase(shared_game->mazo.begin());
    }
}

void borra_carta(vector<string>&carta,Game* &shared_game, int &pos){
    if(pos==0){
        int tam=shared_game->mano_jugador.size();
        for(int i = 0; i < tam; i++){
            if(shared_game->mano_jugador[i] == carta){
                swap(shared_game->mano_jugador[0], shared_game->mano_jugador[i]);
                shared_game->mano_jugador.erase(shared_game->mano_jugador.begin());
                break;
            }
        }
    }
    else if(pos==1){
        int tam=shared_game->mano_bot1.size();
        for(int i = 0; tam; i++){
            if(shared_game->mano_bot1[i] == carta){
                swap(shared_game->mano_bot1[0], shared_game->mano_bot1[i]);
                shared_game->mano_bot1.erase(shared_game->mazo.begin());
                break;
            }
        }
    }
    else if(pos==2){
        int tam=shared_game->mano_bot2.size();
        for(int i = 0; i < tam; i++){
            if(shared_game->mano_bot2[i] == carta){
                swap(shared_game->mano_bot1[0], shared_game->mano_bot2[i]);
                shared_game->mano_bot2.erase(shared_game->mano_bot2.begin());
                break;
            }
        }
    }
    else if(pos==3){
        int tam=shared_game->mano_bot3.size();
        for(int i = 0; i < tam; i++){
            if(shared_game->mano_bot3[i] == carta){
                swap(shared_game->mano_bot1[0], shared_game->mano_bot3[i]);
                shared_game->mano_bot3.erase(shared_game->mano_bot3.begin());
                break;
            }
        }
    }
}

bool probar_carta(vector<string>&carta,Game* &shared_game, int &pos){
    vector<string>carta_ds=shared_game->pila_descarte[shared_game->pila_descarte.size()-1];
    if(carta[1]=="negro"){//Comodines
        if(carta[0]=="Comodin"){
            int color;
            if(pos == 0){
                bool valido = false;
                while(valido == false){
                    cout<<"¿A qué color deseas cambiar?"<<endl;
                    cout<<"(0) Rojo"<< endl; 
                    cout<<"(1) Azul"<< endl; 
                    cout<<"(2) Amarillo"<<endl; 
                    cout<<"(3) Verde"<<endl; 
                    cin >> color;
                    if(color!=0&&color!=1&&color!=2&&color!=3){
                        cout<<"¡Warning! Has escogido un índice no existente. Vuelve a escoger."<<endl;
                    }
                    else{
                        valido = true;
                    }
                }
            }
            else{
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> distrib(0, 4);
                color = distrib(gen);
            }
            if(color==0){
                shared_game->pila_descarte[0][1] = "rojo";
                borra_carta(carta, shared_game, pos);
            }
            else if(color==1){
                shared_game->pila_descarte[0][1] = "azul";
                borra_carta(carta, shared_game, pos);
            }
            else if(color==2){
                shared_game->pila_descarte[0][1] = "amarillo";
                borra_carta(carta, shared_game, pos);
            }
            else if(color==3){
                shared_game->pila_descarte[0][1] = "verde";
                borra_carta(carta, shared_game, pos);
            }
        }
        else if(carta[0]=="Comodin Roba Cuatro"){
            //Hacer lo que deba hacer
            int color;
            if(pos == 0){
                bool valido = false;
                while(valido == false){
                    cout<<"¿A qué color deseas cambiar?"<<endl;
                    cout<<"(0) Rojo"<< endl; 
                    cout<<"(1) Azul"<< endl; 
                    cout<<"(2) Amarillo"<<endl; 
                    cout<<"(3) Verde"<<endl; 
                    cin >> color;
                    if(color!=0&&color!=1&&color!=2&&color!=3){
                        cout<<"¡Warning! Has escogido un índice no existente. Vuelve a escoger."<<endl;
                    }
                    else{
                        valido = true;
                    }
                }
            }
            else{
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> distrib(0, 4);
                color = distrib(gen);
            }
            if(color==0){
                shared_game->pila_descarte[0][1] = "rojo";
                shared_game->roba = 4;
                borra_carta(carta, shared_game, pos);
            }
            else if(color==1){
                shared_game->pila_descarte[0][1] = "azul";
                shared_game->roba = 4;
                borra_carta(carta, shared_game, pos);
            }
            else if(color==2){
                shared_game->pila_descarte[0][1] = "amarillo";
                shared_game->roba = 4;
                borra_carta(carta, shared_game, pos);
            }
            else if(color==3){
                shared_game->pila_descarte[0][1] = "verde";
                shared_game->roba = 4;
                borra_carta(carta, shared_game, pos);
            }
        }
    }
    else if(carta[0]=="Roba Dos"&&carta[1]==carta_ds[1]){//Roba 2
        //Hacer lo que deba hacer
        shared_game->pila_descarte.pop_back();
        shared_game->pila_descarte.push_back(carta);
        borra_carta(carta, shared_game, pos);
        shared_game->roba = 2;
    }
    else if(carta[0]=="Salta"&&carta[1]==carta_ds[1]){//Salta
        //Hacer lo que deba hacer
        shared_game->pila_descarte.pop_back();
        shared_game->pila_descarte.push_back(carta);
        borra_carta(carta, shared_game, pos);   
        if(shared_game->cambio_sentido == true){
            shared_game->pos -= 1;
        }
        else{
            shared_game->pos += 1;
        }
    }
    else if(carta[0]=="Cambio de sentido"&&carta[1]==carta_ds[1]){//Cambio de sentido
        //Hacer lo que deba hacer
        borra_carta(carta, shared_game, pos);
        if(shared_game->cambio_sentido == true){
            shared_game->cambio_sentido = false;
        }
        else{
            shared_game->cambio_sentido = true;
        }
    }
    else if(carta[0]==carta_ds[0]||carta[1]==carta_ds[1]){//Tira una carta numerica
        //Hacer lo que deba hacer
        shared_game->pila_descarte.erase(shared_game->pila_descarte.begin());
        shared_game->pila_descarte.push_back(carta);
        cout<<shared_game->pila_descarte[0][0]<<" "<<shared_game->pila_descarte[0][1]<<endl;
        borra_carta(carta, shared_game, pos);
    }
    else{
        return true;
    }
    return false;
}

void elegir_carta(vector<string>&carta,Game* &shared_game,int &pos){
    if(pos==0){//jugador
        cout<<"Es tu turno!!"<<endl;
        int i, elec;
        bool intento=true;
        while(intento==true){
            int tam=shared_game->mano_jugador.size();
            for(i=0;i<tam;i++){
                cout<<"("<<i<<") "<<shared_game->mano_jugador[i][0]<<" "<<shared_game->mano_jugador[i][1]<<endl;
            }
            if(shared_game->roba_uno==true){
                cout<<"("<<i<<") Sacar una carta del mazo"<<endl;
            }
            if(shared_game->roba_uno==false){
                cout<<"("<<i<<") No tirar carta"<<endl;
            }
            cout<<"Carta a escoger: ";cin>>elec;
            if(elec>=0 && elec<tam){
                intento=false;
                carta.push_back(shared_game->mano_jugador[elec][0]);
                carta.push_back(shared_game->mano_jugador[elec][1]);
            }
            else if(elec==tam&&shared_game->roba_uno==false){
                intento=false;
                shared_game->roba_uno=true;
            }
            else if(elec==tam&&shared_game->roba_uno==true){
                shared_game->roba_uno=false;
                shared_game->mano_jugador.push_back(shared_game->mazo[0]);
                shared_game->mazo.erase(shared_game->mazo.begin());
            }
            else{
                cout<<"¡Warning! Has escogido un índice no existente. Vuelve a escoger."<<endl;
            }
        }
    }
    else if(pos==1){//bot1
        int i=0, tam=shared_game->mano_bot1.size();
        bool intento=true;
        while(i<tam&&intento==true){
            carta.push_back(shared_game->mano_bot1[i][0]);
            carta.push_back(shared_game->mano_bot1[i][1]);
            intento=probar_carta(carta,shared_game,pos);
            if(intento==false){
                i=tam;
            }
            i++;
            carta.clear();
        }
        if(intento==true){
            carta.push_back(shared_game->mazo[i][0]);
            carta.push_back(shared_game->mazo[i][1]);
            shared_game->mazo.erase(shared_game->mazo.begin());
            shared_game->mano_bot1.push_back(carta);
            intento=probar_carta(carta,shared_game,pos);
            carta.clear();
        }
    }
    else if(pos==2){//bot2
        int i=0, tam=shared_game->mano_bot2.size();
        bool intento=true;
        while(i<tam&&intento==true){
            carta.push_back(shared_game->mano_bot2[i][0]);
            carta.push_back(shared_game->mano_bot2[i][1]);
            intento=probar_carta(carta,shared_game,pos);
            if(intento==false){
                i=shared_game->mano_bot2.size();
            }
            i++;
            carta.clear();
        }
        if(intento==true){
            carta.push_back(shared_game->mazo[i][0]);
            carta.push_back(shared_game->mazo[i][1]);
            shared_game->mazo.erase(shared_game->mazo.begin());
            shared_game->mano_bot2.push_back(carta);
            intento=probar_carta(carta,shared_game,pos);
            carta.clear();
        }
    }
    else if(pos==3){//bot3
        int i=0;
        bool intento=true;
        int tam=shared_game->mano_bot3.size();
        while(i<tam&&intento==true){
            carta.push_back(shared_game->mano_bot3[i][0]);
            carta.push_back(shared_game->mano_bot3[i][1]);
            intento=probar_carta(carta,shared_game,pos);
            if(intento==false){
                i=shared_game->mano_bot3.size();
            }
            i++;
            carta.clear();
        }
        if(intento==true){
            carta.push_back(shared_game->mazo[i][0]);
            carta.push_back(shared_game->mazo[i][1]);
            shared_game->mazo.erase(shared_game->mazo.begin());
            shared_game->mano_bot3.push_back(carta);
            intento=probar_carta(carta,shared_game,pos);
            carta.clear();
        }
    }
}

int main(){
    cout<<"--- Bienvenido a Uno ---"<<endl<<endl;
    
    //Crear memoria compartida
    int mem = shmget(IPC_PRIVATE, sizeof(Game), IPC_CREAT|0666);
    
    //Mapear memoria compartida
    Game* shared_game = (Game*)shmat(mem, nullptr, 0);

    //Inicio del mazo/juego
    create_mazo(shared_game->mazo);
    cout<<"✓ Mazo creado"<<endl<<"✓ Mazo revuelto"<<endl<<endl;
    //Crear pila descarte evitando comodines
    int tam=shared_game->mazo.size();
    for(int i = 0; i < tam; i++){
        if(shared_game->mazo[i][1] != "negro"){
            shared_game->pila_descarte.push_back(shared_game->mazo[0]);
            shared_game->mazo.erase(shared_game->mazo.begin());
            break;
        }
        else{
            shared_game->mazo.push_back(shared_game->mazo[i]);
            shared_game->mazo.erase(shared_game->mazo.begin());
        }
    }
    shared_game->cambio_sentido = false;
    shared_game->terminar_game = false;
    shared_game->roba = 0;
    shared_game->roba_uno = true;
    shared_game->pos = 0;
    
    //Crear semaforos
    sem_init(&shared_game->semaforo1, 1, 0); 
    sem_init(&shared_game->semaforo2, 1, 0);
    sem_init(&shared_game->semaforo3, 1, 0);
    sem_init(&shared_game->semaforo4, 1, 0);

    //Crear forks
    pid_t pid_padre = getpid();
    int ids[4];

    for(int i=0;i<4;i++){
        ids[i]=fork();
        if(ids[i]==0){
            if(ids[i]==0&&i==0){//Mano jugador
                cout<<"✓ Mano del jugador creada"<<endl;
                create_hand(shared_game->mano_jugador,shared_game);
                shared_game->pids[i] = getpid();
                sem_wait(&shared_game->semaforo1);
                break;
            }
            else if(ids[i]==0&&i==1){//Mano bot1
                cout<<"✓ Mano del bot1 creada"<<endl;
                create_hand(shared_game->mano_bot1,shared_game);
                shared_game->pids[i] = getpid();
                sem_wait(&shared_game->semaforo2);
                break;
            }
            else if(ids[i]==0&&i==2){//Mano bot2
                cout<<"✓ Mano del bot2 creada"<<endl;
                create_hand(shared_game->mano_bot2,shared_game);
                shared_game->pids[i] = getpid();
                sem_wait(&shared_game->semaforo3);
                break;
            }
            else if(ids[i]==0&&i==3){//Mano bot3
                cout<<"✓ Mano del bot3 creada"<<endl<<endl;
                create_hand(shared_game->mano_bot3,shared_game);
                shared_game->pids[i] = getpid();
                sem_wait(&shared_game->semaforo4);
                break;
            }
        }
    }

    //Crear turnos
    if(getpid()!=pid_padre){
        while(shared_game->terminar_game==false){
            //Esperar el turno
            if(shared_game->pids[0] == getpid()){
                sem_wait(&shared_game->semaforo1);
            }
            else if(shared_game->pids[1] == getpid()){
                sem_wait(&shared_game->semaforo2);
            }
            else if(shared_game->pids[2] == getpid()){
                sem_wait(&shared_game->semaforo3);
            }
            else if(shared_game->pids[3] == getpid()){
                sem_wait(&shared_game->semaforo4);
            }
            //Comprobar final
            if(shared_game->pos == 4){
                exit(0);
            }

            //Comprobar roba carta
            if(shared_game->roba != 0){
                for(int i = 0; i < shared_game->roba; i++){
                    robar_carta(shared_game, shared_game->pos);
                }
                shared_game->roba = 0;
            }

            //Tirar alguna carta
            cout<<"Carta actual pila descarte: "<<shared_game->pila_descarte[0][0]<<" "<<shared_game->pila_descarte[0][1]<<endl;
            if(shared_game->pos==0){
                bool intento=true;
                while(intento==true){
                    vector<string>carta;
                    elegir_carta(carta,shared_game,shared_game->pos);
                    if(carta.size()>0){
                        intento=probar_carta(carta,shared_game,shared_game->pos);
                    }
                    else{
                        break;
                    }
                    if(intento==true){
                        cout<<"Su carta no puede ser usada en este turno. Vuelva a intentar"<<endl;
                    }
                    carta.clear();
                }
            }
            else{
                vector<string>carta;
                elegir_carta(carta,shared_game,shared_game->pos);
                carta.clear();
            }
            //Siguiente turno
            if(shared_game->cambio_sentido==false){
                shared_game->pos=(shared_game->pos)+1;
                if(shared_game->pos>=4){
                    shared_game->pos=0;
                }
            }
            else{
                shared_game->pos=shared_game->pos-1;
                if(shared_game->pos<=-1){
                    shared_game->pos=3;
                }
            }

            //Comprobar si alguien gana
            if(shared_game->mano_bot1.size()==0||shared_game->mano_bot2.size()==0||shared_game->mano_bot3.size()==0||shared_game->mano_jugador.size()==0||shared_game->mazo.size()==0){
                shared_game->terminar_game=true;
                shared_game->pos = 4;
                exit(0);
            }

            //Liberar a los otros jugadores
            if(shared_game->pos == 0){
                sem_post(&shared_game->semaforo1);
            }
            else if(shared_game->pos == 1){
                sem_post(&shared_game->semaforo2);
            }
            else if(shared_game->pos == 2){
                sem_post(&shared_game->semaforo3);
            }
            else if(shared_game->pos == 3){
                sem_post(&shared_game->semaforo4);
            }
        }
    }
    else{
        sem_post(&shared_game->semaforo1);
        sem_post(&shared_game->semaforo2);
        sem_post(&shared_game->semaforo3);
        sem_post(&shared_game->semaforo4);
        sleep(2);
    }
    sem_post(&shared_game->semaforo1);
    //Proceso padre espera a los hijos
    for(int i=0;i<4;i++){
        wait(NULL);
    }
    //Revisar quien tiene 0 cartas y es el ganador 
    if(shared_game->mano_jugador.size()==0){
        cout<<"¡Ganáste, felicidades! :]"<<endl;
        cout<<"Cantidad de cartas restantes de los demás juagdores:"<<endl;
        cout<<"Bot1: "<<shared_game->mano_bot1.size()<<endl;
        cout<<"Bot2: "<<shared_game->mano_bot2.size()<<endl;
        cout<<"Bot3: "<<shared_game->mano_bot3.size()<<endl;
    }
    else if(shared_game->mano_bot1.size()==0){
        cout<<"Ganó bot1, mejor suerte la próxima vez"<<endl;
        cout<<"Cantidad de cartas restantes de los demás juagdores:"<<endl;
        cout<<"Jugador: "<<shared_game->mano_jugador.size()<<endl;
        cout<<"Bot2: "<<shared_game->mano_bot2.size()<<endl;
        cout<<"Bot3: "<<shared_game->mano_bot3.size()<<endl;
    }
    else if(shared_game->mano_bot2.size()==0){
        cout<<"Ganó bot2, mejor suerte la próxima vez"<<endl;        
        cout<<"Jugador: "<<shared_game->mano_jugador.size()<<endl;
        cout<<"Bot1: "<<shared_game->mano_bot1.size()<<endl;
        cout<<"Bot3: "<<shared_game->mano_bot3.size()<<endl;
    }
    else if(shared_game->mano_bot3.size()==0){
        cout<<"Ganó bot3, mejor suerte la próxima vez"<<endl;
        cout<<"Jugador: "<<shared_game->mano_jugador.size()<<endl;
        cout<<"Bot1: "<<shared_game->mano_bot1.size()<<endl;
        cout<<"Bot2: "<<shared_game->mano_bot2.size()<<endl;
    }
    else if(shared_game->mazo.size()==0){
        cout<<endl<<"Mazo sin cartas"<<endl;
        vector<vector<string>>vec;
        vector<string>vecaux;
        int cartj,cart1,cart2,cart3;
        cartj=shared_game->mano_jugador.size();
        cart1=shared_game->mano_bot1.size();
        cart2=shared_game->mano_bot2.size();
        cart3=shared_game->mano_bot3.size();
        vecaux.push_back(to_string(cartj));
        vecaux.push_back("Jugador");
        vec.push_back(vecaux);
        vecaux.clear();
        vecaux.push_back(to_string(cart1));
        vecaux.push_back("Bot1");
        vec.push_back(vecaux);
        vecaux.clear();
        vecaux.push_back(to_string(cart2));
        vecaux.push_back("Bot2");
        vec.push_back(vecaux);
        vecaux.clear();
        vecaux.push_back(to_string(cart3));
        vecaux.push_back("Bot3");
        vec.push_back(vecaux);
        vecaux.clear();
        sort(vec.begin(),vec.end());
        for(int i=0;i<4;i++){
            if(i==0){
                cout<<"Ganador: "<< vec[i][1]<<endl;
                cout<<"Cantidad de cartas restantes de los demás juagdores:"<<endl;
            }
            else{
                cout<<vec[i][1]<<": "<<vec[i][0]<<endl;
            }
        }
    }
    sleep(1);
    //Eliminar memoria compartida
    shmdt(shared_game);
    shmctl(mem, IPC_RMID, nullptr); 
    return 0;
}