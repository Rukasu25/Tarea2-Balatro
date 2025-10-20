/*Balatro Balatrez esta jugando Balatro*/

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
    carta mano; //info de la carta
    nodoMazo *sig;
};

/*funcion para crear los arboles de cada pinta*/
arbolPinta* ABB_pinta(carta cartas[], int inicio, int final){
    if(final<inicio) return NULL;
    int medio=(inicio+final)/2;
    arbolPinta* nuevo_nodo=new arbolPinta();
    nuevo_nodo->cartas = cartas[medio];
    nuevo_nodo->izq=ABB_pinta(cartas,inicio, medio-1);
    nuevo_nodo->der=ABB_pinta(cartas, medio+1, final);
    return nuevo_nodo;

}

/*esta funcion crea los arboles de cada pinta de cartas*/
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

/*esto es para asegurar qaue todas las cartas estén, su campo jugada, en false*/
void iniciarJugada(arbolPinta* raiz){
    if(raiz==NULL) return;
    raiz->cartas.Jugada=false;
    iniciarJugada(raiz->izq);
    iniciarJugada(raiz->der);
}

/*esto es para obtener el indice del palo a conocer*/
int indicePalo(char palo) {
    if (palo == 'C') return 0;
    if (palo == 'D') return 1;
    if (palo == 'E') return 2;
    if (palo == 'T') return 3;
    return -1;
}

/*funcion para buscar una carta, por su categoría, en el arbol que corresponda*/
arbolPinta* buscarCartaCategoria(arbolPinta* raiz, int categoria){
    if(raiz==NULL || raiz->cartas.Categoria==categoria) return raiz;
    if(categoria<raiz->cartas.Categoria){
        return buscarCartaCategoria(raiz->izq, categoria);
    }
    return buscarCartaCategoria(raiz->der, categoria);
}

/*funcion para marcar una carta como jugada en el arbol respectivo*/
void marcarJugada(arbolPinta *arboles[], carta c){
    int indice_palo=indicePalo(c.Palo);
    arbolPinta *nodo = buscarCartaCategoria(arboles[indice_palo], c.Categoria); //creamos un nodo para marcar la carta deseada
    if(nodo) nodo->cartas.Jugada=true;
}

/*a continuacion se implementarán 2 funciones para reocrrer el arbol en inorden
una es para las cartas disponibles y la otra es para las no disponibles*/
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

/*funcion para mostrar las disponibles/nodisponibles de cada pinta(arbol)*/
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

/*funcion para agregar cartas al final del mazo(lista)*/
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

/*funcion para eliminar una carta por su indice*/
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


/*funcion para determinar el largo del mazo/lista*/
int listSize(nodoMazo *mazo){
    int count=0;
    nodoMazo *aux= mazo;
    while(aux!=NULL){
        aux=aux->sig;
        count++;
    }
    return count;   
}
/*función para liberar la memoria de la lista mazo*/
void eliminarLista(nodoMazo *&mazo){
    //liberamos nodo por nodo
    while(mazo!=NULL){
        nodoMazo *aux=mazo; //creamos un nodo auxiliar que apuntará a la cabeza de la lista. esto se hace pa que no se pierda la referencia de la lista,"mazo" en este caso
        mazo=mazo->sig;
        delete aux;
    }
}

/*esta funcion ordena las cartas descendentemente por categoria, teniendo en cuenta que As=14*/
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

/*funcion para imprimir la mano*/
void mostrarMano(nodoMazo *mano){
    nodoMazo *aux=mano;//creamos un nodo auxiliar para acceder a la lista y poder operar sobre ella

    while(aux!=NULL){
        cout<<aux->mano.Categoria<< aux->mano.Palo<<" ";
        aux=aux->sig;
    }
    cout<<"\n";
}

/*funcion para barajar las cartas y crear la lista enlazada*/
void Barajar(nodoMazo* &mazo) {
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

/*esto reparte las cartas que están en el mazo a la mano*/
void Repartir(nodoMazo *&mazo, nodoMazo *&mano, int max, arbolPinta *arboles[]){
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


/*esta funcion elimina las cartas por su índice, de forma descendiente*/
void eliminarSeleccionadas(nodoMazo *&mano, int indices[], int cantidad){
    // ordenar descendientemente 
    for(int i=0; i<cantidad; i++){
        for(int j=i+1; j<cantidad; j++){
            if(indices[j]>indices[i]){
                int aux=indices[j];
                indices[j]=indices[i];
                indices[i]=aux;
            }
        }
    }
    for(int i=0; i<cantidad;i++){
        eliminarPorIndice(mano, indices[i]);
    }
}


/*TIPOS MANO */
/*estas funciones determinan si la mano es o no es el tipo que deberia ser*/
bool IsPair(nodoMazo* mano){
    if (mano == nullptr) return false;
    int counts[14] = {0}; // índices 1 al 13
    nodoMazo* aux = mano;
    while (aux != nullptr){
        int c = aux->mano.Categoria;
        if (c >= 1 && c <= 13) counts[c]++;
        aux = aux->sig;
    }
    for (int i = 1; i <= 13; ++i) if (counts[i] >= 2) return true;
    return false;
}

bool IsTwoPair(nodoMazo* mano){
    if (mano == nullptr) return false;
    int counts[14] = {0};
    nodoMazo* aux = mano;
    while (aux != nullptr){
        int c = aux->mano.Categoria;
        if (c >= 1 && c <= 13) counts[c]++;
        aux = aux->sig;
    }
    int pairs = 0;
    for (int i = 1; i <= 13; ++i) if (counts[i] >= 2) pairs++;
    return pairs >= 2;
}

bool IsThreeOfAKind(nodoMazo* mano){
    if (mano == nullptr) return false;
    int counts[14] = {0};
    nodoMazo* aux = mano;
    while (aux != nullptr){
        int c = aux->mano.Categoria;
        if (c >= 1 && c <= 13) counts[c]++;
        aux = aux->sig;
    }
    for (int i = 1; i <= 13; ++i) if (counts[i] >= 3) return true;
    return false;
}
bool IsFourOfAKind(nodoMazo* mano){
    if (mano == nullptr) return false;
    // Para cada categoría en la mano, contamos cuántas veces aparece
    for (nodoMazo* a = mano; a != nullptr; a = a->sig) {
        int cat = a->mano.Categoria;
        int count = 0;
        for (nodoMazo* b = mano; b != nullptr; b = b->sig) {
            if (b->mano.Categoria == cat) count++;
        }
        if (count >= 4) return true;
    }
    return false;
}

bool IsStraight(nodoMazo* mano){
    if (mano == nullptr) return false;
    int val[5];
    int index = 0;
    nodoMazo* t = mano;
    // extraer hasta 5 cartas (la lista de por si ya es descendente)
    while (t != nullptr && index < 5){
        val[index++] = (t->mano.Categoria == 1) ? 14 : t->mano.Categoria; // tratar As como 14
        t = t->sig;
    }
    if (index != 5) return false; //solo consideramos escalera con exactamente 5 cartas

    //comprobar duplicados (no puede haber para ser escalera)
    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 5; ++j)
            if (val[i] == val[j]) return false;

    //comprobamos diferencia -1 entre vecinos
    for (int i = 0; i < 4; ++i){
        if (val[i] - val[i + 1] != 1) return false;
    }
    return true;
}

bool IsFlush(nodoMazo* mano){
    if (mano == nullptr) return false;
    int contador = 0;
    nodoMazo* t = mano;
    char palo = 0;
    while (t != nullptr){
        if (contador == 0) palo = t->mano.Palo;
        else if (t->mano.Palo != palo) return false;
        ++contador;
        t = t->sig;
    }
    return (contador == 5); // solo consideramos Color con exactamente 5 cartas
}
bool IsHighCard(nodoMazo* mano){
    if (mano == nullptr) return false;
    // High Card si no cumple ninguna otra categoría relevante
    return !(IsPair(mano) || IsTwoPair(mano) || IsThreeOfAKind(mano) || IsFourOfAKind(mano) || IsStraight(mano) || IsFlush(mano));
}


/*ahora toca la funcion para evaluar el tipo mano (carta mas alta, tercia, etc.) del jugador y asignar el puntaje correspondiente*/

int puntajetipomano(nodoMazo* mano){
    if(IsFourOfAKind(mano)){
        return 400;
    }
    else if(IsFlush(mano)){
        return 200;
    }
    else if(IsStraight(mano)){
        return 150;
    }
    else if(IsThreeOfAKind(mano)){
        return 120;
    }
    else if(IsTwoPair(mano)){
        return 80;
    }
    else if(IsPair(mano)){
        return 40;
    }
    else{
        return 50; //High Card
    }

}


/*el puntaje total de la mano jugada*/
int puntajemano(nodoMazo* mano){
    if (mano == nullptr) return 0;
    int counts[14] = {0};   // índices que van del 1 al 13
    int val[14]    = {0};   // valor asociado a cada categoría
    nodoMazo* t = mano;
    while (t != nullptr){
        int c = t->mano.Categoria;
        if (c >= 1 && c <= 13){
            counts[c]++;
            if (val[c] == 0) val[c] = t->mano.Valor;
        }
        t = t->sig;
    }
    // Póker
    for (int c = 13; c >= 1; --c){
        if (counts[c] >= 4){
            int puntaje = val[c] * 4 + puntajetipomano(mano);
            return puntaje;
        }
    }
    // Tercia
    for (int c = 13; c >= 1; --c){
        if (counts[c] >= 3){
            int puntaje = val[c] * 3 + puntajetipomano(mano);
            return puntaje;
        }
    }
    // Doble par
    int pares = 0;
    int sumaParesVal = 0;
    for (int c = 13; c >= 1; --c){
        if (counts[c] >= 2){
            pares++;
            sumaParesVal += val[c] * 2;
        }
    }
    if (pares >= 2){
        int puntaje = sumaParesVal + puntajetipomano(mano);
        return puntaje;
    }
    // Par
    for (int c = 13; c >= 1; --c){
        if (counts[c] >= 2){
            int puntaje = val[c] * 2 + puntajetipomano(mano);
            return puntaje;
        }
    }
    // Escalera / Color (ambos son 5 cartas)
    if (IsStraight(mano) || IsFlush(mano)){
    int sumaValor = 0;
    nodoMazo* u = mano;
    while (u != nullptr){
    sumaValor += u->mano.Valor;
    u = u->sig;
    }
    int puntaje = sumaValor + puntajetipomano(mano);
    return puntaje;
    }
    // High Card: tomar la carta más alta
    {
        int maxValor = 0;
        nodoMazo* u = mano;
        while (u != nullptr){
            if (u->mano.Valor > maxValor) maxValor = u->mano.Valor;
            u = u->sig;
        }
        int puntaje = maxValor + puntajetipomano(mano);
        return puntaje;
    }
    // nomas por seguridad
    return 0;
}




/*esta funcion existe para no hacer puros ifs dentro de otras funciones para saber qué tipo de mano se jugó*/
string arrojarTipoMano(nodoMazo *mano){
    if (mano == nullptr) return "Carta Más Alta";
    if (IsFourOfAKind(mano)) return "Póker";
    if (IsThreeOfAKind(mano)) return "Tercia";
    if (IsTwoPair(mano)) return "Doble Par";
    if (IsPair(mano)) return "Par";
    if (IsStraight(mano)) return "Escalera";
    if (IsFlush(mano)) return "Color";
    return "Carta Más Alta";
}

/*esta funcion maneja la decision del jugador, ya sea jugar, descartar o ver las cartas disponibles y no disponibles*/
void decisionJugar(arbolPinta *arboles[], nodoMazo *&mazo, nodoMazo *&mano, int &manosRestantes, int &descartesRestantes, int &puntajeTotal, int tamMano){
    char decision;
    int cantidad;
    cout<<"Juega: "<<endl;
    if(!(cin>>decision)) return;
    if(decision=='M' || decision=='m'){
        mostrarCartas(arboles);
        return;
    }
    if(!(cin>>cantidad)) return;
    if(cantidad<1 || cantidad>5){
        cout<<"Cantidad Invalida. "<<endl;
        return;
    }

    int n = listSize(mano);
    int indices[5];
    for(int i=0; i<cantidad; i++){
        if(!(cin >> indices[i])){
            cout<<"Entrada inválida."<<endl;
            return;
        }
        if(indices[i] < 0 || indices[i] >= n){
            cout<<"Indice inválido."<<endl;
            return;
        }
        // evitar índices repetidos
        for(int k=0; k<i; k++){
            if(indices[k] == indices[i]){
                cout<<"Indice repetido."<<endl;
                return;
            }
        }
    }

    carta seleccionadasArr[5];
    nodoMazo* seleccionLista = nullptr;
    for(int k=0; k<cantidad; k++){
        nodoMazo* aux = mano;
        int pasos = indices[k]; // desplazamientos desde cabeza
        for(int s=0; s<pasos; s++){
            if(aux==NULL) break;
            aux = aux->sig;
        }
        if(aux==NULL){
            cout<<"Indice inválido al rescatar."<<endl;
            eliminarLista(seleccionLista);
            return;
        }
        seleccionadasArr[k] = aux->mano;
        // construir lista temporal con AppendMazo (usa la misma estructura)
        AppendMazo(seleccionLista, seleccionadasArr[k]);
    }

    if(decision=='D' || decision=='d'){
        if(descartesRestantes<=0){
            cout<<"No quedan descartes. "<<endl;
            eliminarLista(seleccionLista);
            return;
        }
        eliminarSeleccionadas(mano, indices, cantidad); // elimina las cartas descartadas
        descartesRestantes--;
    }
    else if(decision=='J' || decision=='j'){
        if(manosRestantes<=0){
            cout<<"No quedan manos."<< endl;
            eliminarLista(seleccionLista);
            return;
        }
        // evaluar y puntuar EN BASE A LAS CARTAS SELECCIONADAS
        string tipoMano = arrojarTipoMano(seleccionLista);
        int puntosMano = puntajemano(seleccionLista);
        cout<<"Mano Jugada: "<<tipoMano<<endl;
        cout<<"Fichas anotadas "<< puntosMano <<endl;
        puntajeTotal += puntosMano;
        eliminarSeleccionadas(mano, indices, cantidad); // elimina las cartas jugadas
        manosRestantes--;
    }
    else{
        cout<<"Decision inválida."<<endl;
        eliminarLista(seleccionLista);
        return;
    }

    eliminarLista(seleccionLista); // liberar lista temporal
    Repartir(mazo, mano, tamMano, arboles);
}
/*esta funcion es la que se encarga del flujo de juego*/
bool partida(arbolPinta* arboles[], int pozo, string nombre_ciega, bool es_jefe){
    cout<<"Comienza la "<<nombre_ciega<<", pozo a vencer: "<<pozo<<" fichas"<<endl;
    for(int i=0; i<4; i++){
        iniciarJugada(arboles[i]); //iniciamos el campo Jugada como false, en todas las cartas
    }
    nodoMazo *mazo=NULL; //se crea el nodo mazo
    Barajar(mazo);//se baraja
    nodoMazo *mano=NULL;//creamos el nodo mano
    int tamMano=8;//se declara e inicializa el tamaño de la mano
    if(es_jefe) tamMano=7;//***se implemento el modificador grillete***
    Repartir(mazo, mano, tamMano, arboles);

    int puntajeTotal=0;
    int manosRestantes=4;
    int descartes=3;
    while(true){
        cout<<puntajeTotal<<" / "<<pozo<<" fichas"<<endl;
        cout<<listSize(mano)<<" / 52 "<< "cartas"<<endl;
        cout<<manosRestantes<< "J / "<<descartes<<"D"<<endl;
        mostrarMano(mano);
        decisionJugar(arboles, mazo, mano, manosRestantes, descartes, puntajeTotal, tamMano);
        if(puntajeTotal>=pozo) break;
        if(manosRestantes<=0) break;
    }
    eliminarLista(mazo);
    eliminarLista(mano);
    if(puntajeTotal>=pozo){
        cout<<nombre_ciega<<" vencida: "<<puntajeTotal<<" / "<<pozo;
        return true;
    }
    cout<<"Has perdido... "<<puntajeTotal<<" / "<<pozo;
    return false;
}

/*aqui se balatrea*/
int main(){
    
    arbolPinta *arboles[4];
    crearArboles(arboles);
    cout<<"Bienvenido al maravilloso juevo de DataBalatro!"<<endl;
    if(!partida(arboles, 250, "Ciega Pequeña", false)) return 0;
    if(!partida(arboles, 350, "Ciega Grande", false)) return 0;
    if(!partida(arboles, 500, "Ciega Jefe", true)) return 0;

    return 0;
}