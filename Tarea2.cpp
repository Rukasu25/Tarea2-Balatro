using namespace std;
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>

struct carta {
    char Palo ;
    int Categoria ;
    int Valor ;
    bool Jugada ;
};

struct arbolpinta {
    carta cartas[13];
    arbolpinta *izq;
    arbolpinta *der;
};

struct nodomazo {
    carta mano;
    nodomazo *sig;
};



/*funcion para barajar el mazo de cartas en la lista enlazada, debe guardar el mazo barajado*/
void barajar(nodomazo* &mazo, arbolpinta* arboles[]){
    carta cartas[52];
    int indice = 0;
    for(int palo = 0; palo < 4; palo++){
        for(int categoria = 1; categoria <= 13; categoria++){
            //esto define el palo de la carta
            if (palo == 0){ //corazones
                cartas[indice].Palo = 'C';
            } else if (palo == 1){ //diamantes
                cartas[indice].Palo = 'D';
            } else if (palo == 2){ //espadas
                cartas[indice].Palo = 'E';
            } else { //treboles
                cartas[indice].Palo = 'T';

            }
            cartas[indice].Categoria = categoria;
            if (categoria == 1){
                cartas[indice].Valor = 11; //as
            } else if (categoria > 10){
                cartas[indice].Valor = 10; //figuras
            } else {
                cartas[indice].Valor = categoria; //cartas numericas
            }
            cartas[indice].Jugada = false;
            indice++;
        }

    }
    //barajar las cartas
    random_device rd;
    mt19937 g(rd());
    for (int i = 0; i < 52; i++){
        int j = g() % 52;
        carta temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }

    //crear la lista enlazada del mazo
    mazo = nullptr;
    nodomazo* ultimo = nullptr;
    for (int i = 0; i < 52; i++){
        nodomazo* nuevo = new nodomazo;
        nuevo->mano = cartas[i];
        nuevo->sig = nullptr;
        if (mazo == nullptr){
            mazo = nuevo;
        } else {
            ultimo->sig = nuevo;
        }
        ultimo = nuevo;

}
    //llenar los arboles de cada palo
    for (int i = 0; i < 4; i++){
        arboles[i] = nullptr;
    }
    nodomazo* actual = mazo;
    while (actual != nullptr){
        int palo;
        if (actual->mano.Palo == 'C'){
            palo = 0;
        } else if (actual->mano.Palo == 'D'){
            palo = 1;
        } else if (actual->mano.Palo == 'E'){
            palo = 2;
        } else {
            palo = 3;
        }
        arbolpinta* nuevo = new arbolpinta;
        nuevo->cartas[actual->mano.Categoria - 1] = actual->mano;
        nuevo->izq = nullptr;
        nuevo->der = nullptr;
        if (arboles[palo] == nullptr){
            arboles[palo] = nuevo;
        } else {
            arbolpinta* temp = arboles[palo];
            while (true){
                if (actual->mano.Categoria < temp->cartas[0].Categoria){
                    if (temp->izq == nullptr){
                        temp->izq = nuevo;
                        break;
                    } else {
                        temp = temp->izq;
                    }
                } else {
                    if (temp->der == nullptr){
                        temp->der = nuevo;
                        break;
                    } else {
                        temp = temp->der;
                    }
                }
            }
        }
        actual = actual->sig;
    }


}

/*funcion para repartir las cartas a los jugadores, debe modificar la lista enlazada del mazo y crear la lista enlazada de la mano*/
void repartir(nodomazo* &mazo, nodomazo* &mano){
    mano = nullptr;
    nodomazo* ultimo = nullptr;
    for (int i = 0; i < 8; i++){
        if (mazo == nullptr){
            cout << "No hay suficientes cartas en el mazo" << endl;
            return;
        }
        nodomazo* nuevo = new nodomazo;
        nuevo->mano = mazo->mano;
        nuevo->sig = nullptr;
        if (mano == nullptr){
            mano = nuevo;
        } else {
            ultimo->sig = nuevo;
        }
        ultimo = nuevo;
        nodomazo* temp = mazo;
        mazo = mazo->sig;
        delete temp;
    }
}

void imprimirMano(nodomazo* mano) {
    cout << "Cartas en la mano del jugador:" << endl;
    while (mano != nullptr) {
        cout << mano->mano.Palo << mano->mano.Categoria << " ";
        mano = mano->sig;
    }
}
void imprimirMazo(nodomazo* mazo) {
    cout << "Cartas en el mazo:" << endl;
    while (mazo != nullptr) {
        cout << mazo->mano.Palo << mazo->mano.Categoria << " ";
        mazo = mazo->sig;
    }
    cout << endl;
}



//esta funcion permite al jugador jugar o descartar su mano, seleccionando a lo más 5 cartas de su mano

void jugarmano(arbolpinta* arboles[], nodomazo* &mano, int &manos, int &descartes){
    int seleccion[5];
    char palo;
    int cartaSeleccion = 0; //cantidad de cartas a seleccionar
    cout << "Seleccione a lo mas 5 cartas de su mano para jugar:" << endl;
    int categoria;
    char decision; //J para jugar, D para descartar
    int indice; //indice de la(s) carta(s) a seleccionar
    /*puntaje de cada categoria*/
    int HighCard = 50; //carta mas alta
    int Pair = 40; //par
    int TwoPair = 80; //doble par
    int ThreeOfAKind = 120; //Tercia
    int Straight = 150; //escalera
    int Flush = 200; //color
    int FourOfAKind = 400; //poker
    int FullHouse = 300; //Full House
    int StraightFlush = 500; //Escalera corrida
    int RoyalFlush = 1000; //Escalera real
    int FiveOfAKind = 800; //quintilla

    cout << "ingrese J o para jugar o ingrese D para descartar cartas, un numero a lo mas 5 para la cantidad de cartas a seleccionar y despues, los indices de las cartas seleccionadas: ";



    imprimirMano(mano);
    while (cartaSeleccion <= 5){
        cin >> decision >> cartaSeleccion >> indice;
        if (decision != 'J' & decision != 'D'){
            cout << "Decision invalida, intente de nuevo" << endl;
            continue;
        }
        if (indice < 1 || indice > 8){
            cout << "Indice invalido, intente de nuevo" << endl;
            continue;
        }
        if (decision = 'J'){
            //se juega la(s) carta(s), estas anotan puntaje dependiendo de su categoria y la combinacion de cartas jugadas
            nodomazo* temp = mano;
            for (int i = 1; i < indice; i++){
                if (temp == nullptr){
                    cout << "Indice invalido, intente de nuevo" << endl;
                    break;
                }
                temp = temp->sig;
            }
            if (temp == nullptr){
                continue;
            }
            categoria = temp->mano.Categoria;
            palo = temp->mano.Palo;
            int paloIndice;
            if (palo == 'C'){
            //CHECKPOINT, aqui quedan los avances de hoy 10/10/2025
            }
        }
    }
}

    







int main(){
    //inicializar los arboles y el mazo
    arbolpinta* arboles[4] = {nullptr, nullptr, nullptr, nullptr};
    nodomazo* mazo = nullptr;
    nodomazo* mano = nullptr;
    int manos = 4;
    int descartes = 3;
    barajar(mazo, arboles);
    repartir(mazo, mano);
    imprimirMano(mano);
    jugarmano(arboles, mano, manos, descartes);




    return 0;
}