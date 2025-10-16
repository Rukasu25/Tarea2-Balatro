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

struct arbolPinta {
    carta cartas;
    arbolPinta *izq;
    arbolPinta *der;
};

struct nodoMazo {
    carta mano;
    nodoMazo *sig;
};

//funcion para crear los arbolers de cada pinta
arbolPinta* ABB_pinta(carta cartas[], int inicio, int final){
    if(final<inicio) return NULL;
    int medio=(inicio+final)/2;
    arbolPinta* nuevo_nodo=new arbolPinta();
    nuevo_nodo->cartas = cartas[medio];
    nuevo_nodo->izq=ABB_pinta(cartas,inicio, medio-1);
    nuevo_nodo->der=ABB_pinta(cartas, medio+1, final);
    return nuevo_nodo;

}

//esta funcion crea los arboles de cada pinta
void crearArboles(arbolPinta* arboles[]){
    char palos[4]={'C', 'D', 'E', 'T'};
    for(int i=0; i<4; i++){
        carta cartitas[13];
        //llenamos la cada carta con sus valores correspondientes
        for(int j=1; j<=13; j++){
            cartitas[j-1].Palo=palos[i];
            cartitas[j-1].Categoria=j;
            cartitas[j-1].Valor=(j == 1) ? 11 : (j > 10 ? 10 : j);
            cartitas[j-1].Jugada=false;
        }
        arboles[i]=ABB_pinta(cartitas,0,12); //luego mandamos cada pinta (set de cartas) a ser ordenada como un abb
    }
}

//esto es para asegurar qaue todas las cartas estén, su campo jugada, en false
void iniciarJugada(arbolPinta* raiz){
    if(raiz==NULL) return;
    raiz->cartas.Jugada=false;
    iniciarJugada(raiz->izq);
    iniciarJugada(raiz->der);
}

//esto es para obtener el indice del palo a saber
int indicePalo(char palo) {
    if (palo == 'C') return 0;
    if (palo == 'D') return 1;
    if (palo == 'E') return 2;
    if (palo == 'T') return 3;
    return -1;
}

//funcion para buscar una carta, por su categoría, en el arbol que corresponda
arbolPinta* buscarCartaCategoria(arbolPinta* raiz, int categoria){
    if(raiz==NULL || raiz->cartas.Categoria==categoria) return raiz;
    if(categoria<raiz->cartas.Categoria){
        return buscarCartaCategoria(raiz->izq, categoria);
    }
    return buscarCartaCategoria(raiz->der, categoria);
}

//funcion para marcar una carta como jugada en el arbol respectivo
void marcarJugada(arbolPinta *arboles[], carta c){
    int indice_palo=indicePalo(c.Palo);
    arbolPinta *nodo = buscarCartaCategoria(arboles[indice_palo], c.Categoria); //creamos un nodo para marcar la carta deseada
    if(nodo) nodo->cartas.Jugada=true;
}

//a continuacion se implementarán 2 funciones para reocrrer el arbol en inorden
//una es para las cartas disponibles y la otra es para las no disponibles
void InordenDisp(arbolPinta *raiz){
    if(raiz==NULL) return;

    InordenDisp(raiz->izq);
    if(raiz->cartas.Jugada==false){
        cout<< raiz->cartas.Categoria<<raiz->cartas.Palo<<" ";
    }
    InordenDisp(raiz->der);
}
void InordenNoDisp(arbolPinta *raiz){
    if(raiz==NULL) return;

    InordenNoDisp(raiz->izq);
    if(raiz->cartas.Jugada==true){
        cout<< raiz->cartas.Categoria<<raiz->cartas.Palo<<" ";
    }
    InordenNoDisp(raiz->der);
}

//funcion para mostrar las disponibles/nodisponibles de cada pinta(arbol)
void mostrarCartas(arbolPinta * arboles[]){
    cout<<"Disponibles: "<<endl;

    //hay que iterar sobre cada árbol para rescatar las disponibles
    int i;
    for(i=0; i<4; i++){
        InordenDisp(arboles[i]); //se llama a la funcion que recorre los arboles en inorden
    }

    cout<<"\nNo Disponibles: "<<endl;
    for(i=0; i<4; i++){
        InordenNoDisp(arboles[i]);
    }
    cout<<"\n";
}

//funcion para agregar cartas al final del mazo(lista)
void AppendMazo(nodoMazo *&mazo, carta carta){
    nodoMazo *nodo= new nodoMazo();
    nodo->mano=carta;
    nodo->sig=NULL;
    if(mazo==NULL){
        mazo=nodo;
    }
    else{
        nodoMazo *aux=mazo;
        while(aux->sig!=NULL){
            aux=aux->sig;
        }
        aux->sig=nodo;
    }

}

//funcion para eliminar una carta por su indice
void eliminarPorIndice(nodoMazo* &mazo, int indice) {
    if (mazo == NULL) return;
    if (indice==0) {
        nodoMazo* aux=mazo;
        mazo= mazo->sig;
        delete aux;
        return;
    }
    nodoMazo* aux=mazo;
    for (int i=0; i<indice-1; i++) {
        if (aux->sig==NULL) return;
            aux= aux->sig;
    }
    if (aux->sig==NULL) return;
    nodoMazo* aEliminar=aux->sig;
    aux->sig=aEliminar->sig;
    delete aEliminar;
}

//funcion para determinar el largo del mazo/lista
int listSize(nodoMazo *mazo){
    int count=0;
    nodoMazo *aux= mazo;
    while(aux->sig!=NULL){
        aux=aux->sig;
        count++;
    }
    return count;   
}
//función para liberar la memoria de la lista mazo
void eliminarLista(nodoMazo *&mazo){
    //liberamos nodo por nodo
    while(mazo!=NULL){
        nodoMazo *aux=mazo; //creamos un nodo auxiliar que apuntará a la cabeza de la lista. esto se hace pa que no se pierda la referencia de la lista,"mazo" en este caso
        mazo=mazo->sig;
        delete aux;
    }
}

//esta funcion ordena las cartas descendentemente por categoria, teniendo en cuenta que As=14
void ordenarManoDesc(nodoMazo* &mazo) {
    if (mazo == nullptr) return;
    bool swap;
    do {
        swap=false;
        nodoMazo* actual= mazo;
        while(actual->sig!=NULL){
            int cat1=(actual->mano.Categoria==1)?14 : actual->mano.Categoria;//en esta línea y la que está justo abajo se ve si la carta es As o no
            int cat2 =(actual->sig->mano.Categoria==1)?14 : actual->sig->mano.Categoria;
            if(cat1<cat2){
                carta temp=actual->mano;
                actual->mano=actual->sig->mano;
                actual->sig->mano=temp;
                swap =true;
            }
            actual=actual->sig;
        }
    } while(swap);
}

//funcion para imprimir la mano
void mostrarMano(nodoMazo *mano){
    nodoMazo *aux=mano;//creamos un nodo auxiliar para acceder a la lista y poder operar sobre ella

    while(aux->sig!=NULL){// nota para futuro xd: para iterar sobre listas es conveniente usar usar un while
        cout<<aux->mano.Categoria<< aux->mano.Palo<<" ";
        aux=aux->sig;
    }
    cout<<"\n";
}

//funcion para barajar las cartas y crear la lista enlazada
void barajar(nodoMazo* &mazo) {
    carta cartas[52];
    int indice = 0;
    char palos[4] = {'C', 'D', 'E', 'T'};
    for(int p=0; p<4; p++) {
        for (int cat=1; cat<=13; cat++) {
            cartas[indice].Palo=palos[p];
            cartas[indice].Categoria=cat;
            cartas[indice].Valor= (cat==1) ? 11 : (cat > 10 ? 10 : cat);
            cartas[indice].Jugada=false;
            indice++;
        }
    }
    //aqui se baraja
    srand(time(NULL));
    for (int i=51; i>0; i--) {
        int j = rand() % (i + 1);
        carta temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }

    //ahora se crea la lista
    eliminarLista(mazo);
    for (int i=0; i<52; i++) {
        AppendMazo(mazo, cartas[i]);
    }
}

//esto reparte las cartas que están en el mazo a la mano
void repartir(nodoMazo *&mazo, nodoMazo *&mano, int max, arbolPinta *arboles[]){
    int cartasEnMano=listSize(mano); //vemos cuantas cartas hay en la mano
    int cartasARepartir=max-cartasEnMano;//vemos cuantas se repartirán
    for(int i=0; i<cartasARepartir; i++){
        if(mazo==NULL) break;
        carta carta=mazo->mano;//rescatamos la carta que está en la cima del mazo/cabeza de la lista
        AppendMazo(mano, carta);//se agrega la carta a la mano
        marcarJugada(arboles,carta);
        nodoMazo *aux= mazo;//se crea un nodo auxiliar para no perder la referencia del nodo lista(en este caso, mazo)
        mazo=mazo->sig;
        delete aux;//se borra el nodo que se repartió
    }
    ordenarManoDesc(mano);//finalmente se ordena la mano
}

void eliminarSeleccionadas

/*FALTA VER LOS TIPOS DE MANO


*/



//*HASTA AQUI ESCRIBÍ*//

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
    int indice[cartaSeleccion]; //indice de la(s) carta(s) a seleccionar
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


    //ARREGLAR ESTOOOOOOOOOOOOOOOOO
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

//ahora voy a intentar crear la lista enlazada de la mano, es decir de no se de la jugada
class listaMano{
    private:
    nodomazo* mazo;
    nodomazo* mano;
    public:
    listaMano() {mano=NULL;};
    ~listaMano(){}





};







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