using namespace std;
#include <string>
#include <iostream>
#include <fstream>
#include <random>

struct carta {
    char Palo ;
    int Categoria ;
    int Valor ;
    bool Jugada ;
};

struct arbolpinta {
    carta carta;
    arbolpinta *izq;
    arbolpinta *der;
};

struct nodomazo {
    carta mano;
    nodomazo *sig;
};






int main(){
    carta micarta;
    micarta.Palo;
    micarta.Categoria;
    micarta.Valor;
    micarta.Jugada;

    arbolpinta* arboles[4] = {nullptr, nullptr, nullptr, nullptr};
    nodomazo* mazo = nullptr;
    nodomazo* mano = nullptr;
    barajar(mazo, arboles);




    return 0;
}

/*funcion para barajar el mazo de cartas en la lista enlazada, debe retornar el mazo barajado*/
void barajar(nodomazo* &mazo, arbolpinta* arboles[]){
    carta cartas[52];
    int indice = 0;
    //revisar
    /*
    for(int palo = 0; palo < 4; palo++){
        for(int categoria = 1; categoria <= 13; categoria++){
            cartas[indice].Palo = (palo == 0) ? 'C' : (palo == 1) ? 'D' : (palo == 2) ? 'E' : 'T';
            cartas[indice].Categoria = categoria;
            cartas[indice].Valor = (categoria > 10) ? 10 : categoria;
            cartas[indice].Jugada = false;
            indice++;
        }

    }*/

}