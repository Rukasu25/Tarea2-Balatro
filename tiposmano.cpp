#include "Tarea2.cpp"
using namespace std;
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>


bool IsHighCard(nodoMazo* mano){
    //una mano es High Card si no cumple con ninguna de las otras categorias, pero solo toma la carta mas alta en cuenta
    nodoMazo* aux=mano;
    int maxCategoria=0;
    while (aux!=NULL){
        if (aux->mano.Categoria>maxCategoria){
            maxCategoria=aux->mano.Categoria;
        }
        aux=aux->sig;
    }
    return true;
}
bool IsPair(nodoMazo* mano){
    nodoMazo* auxmano=mano;
    while (auxmano!=NULL){
        int categoriaActual=auxmano->mano.Categoria;
        nodoMazo* aux2=auxmano->sig;
        while (aux2!=NULL){
            if (aux2->mano.Categoria==categoriaActual){
                return true;
            }
            aux2=aux2->sig;
        }
    }
    return false;
}

bool IsTwoPair(nodoMazo* mano){
    int cuentapares=0;
    nodoMazo* auxmano = mano;
    while (auxmano!=NULL){
        int categoriaActual = auxmano->mano.Categoria;
        nodoMazo* aux2 = auxmano-> sig;
        while (aux2!=NULL){
            if (aux2->mano.Categoria==categoriaActual){
                cuentapares++;
                break;
            }
            aux2=aux2->sig;
        }
    }
    if (cuentapares==2){
        return true;
    }
    else{
        return false;
    }
}

bool IsThreeOfAKind(nodoMazo* mano){
    nodoMazo* auxmano=mano;
    while (auxmano!=NULL){
        int categoriaActual=auxmano->mano.Categoria;
        nodoMazo* aux2=auxmano->sig;
        while (aux2!=NULL){
            if (aux2->mano.Categoria==categoriaActual){
                nodoMazo* aux3=aux2->sig;
                while (aux3!=NULL){
                    if (aux3->mano.Categoria==categoriaActual){
                        return true;
                    }
                }
            }
            aux2=aux2->sig;
        }
        auxmano=auxmano->sig;
    }
    return false;
}

bool IsFourOfAKind(nodoMazo* mano){
    nodoMazo* auxmano=mano;
    while (auxmano!=NULL){
        int categoriaActual=auxmano->mano.Categoria;
        nodoMazo* aux2=auxmano->sig;
        while (aux2!=NULL){
            if (aux2->mano.Categoria==categoriaActual){
                nodoMazo* aux3=aux2->sig;
                while (aux3!=NULL){
                    if (aux3->mano.Categoria==categoriaActual){
                        nodoMazo* aux4=aux3->sig;
                        while(aux4!=NULL){
                            if (aux4->mano.Categoria==categoriaActual){
                                return true;
                            }
                        }
                    }
                    aux3=aux3->sig;
                }
                aux2=aux2->sig;
            }
            auxmano=auxmano->sig;
        }
    }
    return false;
}

bool IsFiveOfAKind(nodoMazo* mano){
    nodoMazo* auxmano=mano;
    while (auxmano!=NULL){
        int categoriaActual=auxmano->mano.Categoria;
        nodoMazo* aux2=auxmano->sig;
        while (aux2!=NULL){
            if (aux2->mano.Categoria==categoriaActual){
                nodoMazo* aux3=aux2->sig;
                while (aux3!=NULL){
                    if (aux3->mano.Categoria==categoriaActual){
                        nodoMazo* aux4=aux3->sig;
                        while(aux4!=NULL){
                            if (aux4->mano.Categoria==categoriaActual){
                                nodoMazo* aux5=aux4->sig;
                                while (aux5!=NULL){
                                    if (aux5->mano.Categoria==categoriaActual){
                                        return true;
                                    }
                                }
                            }
                        }
                        aux4=aux4->sig;
                    }
                    aux3=aux3->sig;
                }
                aux2=aux2->sig;
            }
            auxmano=auxmano->sig;
        }
    }
    return false;
}

bool IsFullHouse(nodoMazo* mano){
    return IsThreeOfAKind(mano) && IsPair(mano);
}


bool IsStraight(nodoMazo* mano){
    nodoMazo* aux=mano;
    int categorias[5];
    int index=0;
    while (aux!=NULL && index<5){
        categorias[index]=aux->mano.Categoria;
        aux=aux->sig;
        index++;
    }

    //ordenar las categorias, las cartas deberian ser consecutivas entre si (ejemplo: 5,6,7,8,9 o 10,11,12,13,1)
    for (int i=0; i<index-1; i++){
        for (int j=i+1; j<index; j++){
            if (categorias[i]>categorias[j]){
                int temp=categorias[i];
                categorias[i]=categorias[j];
                categorias[j]=temp;
            }

    }
}
    //verificar si son consecutivas
    for (int i=0; i<index-1; i++){
        if (categorias[i]+1 != categorias[i+1]){
            //caso especial para el as
            if (!(categorias[i]==13 && categorias[i+1]==1)){
                return false;
            }
        }
    }
    return true;
}


bool IsFlush(nodoMazo* mano){
    nodoMazo* aux=mano;
    char paloInicial=aux->mano.Palo;
    while (aux!=NULL){
        if (aux->mano.Palo!=paloInicial){
            return false;
        }
        aux=aux->sig;
    }
    return true;
}

bool IsStraightFlush(nodoMazo* mano){
    return IsStraight(mano) && IsFlush(mano);
}

bool IsRoyalFlush(nodoMazo* mano){
    nodoMazo* aux=mano;
    int categorias[5];
    int index=0;
    while (aux!=NULL && index<5){
        categorias[index]=aux->mano.Categoria;
        aux=aux->sig;
        index++;
    }
    //verificar si las categorias son 10,11,12,13,1
    bool tieneDiez=false, tieneOnce=false, tieneDoce=false, tieneTrece=false, tieneAs=false;
    for (int i=0; i<index; i++){
        if (categorias[i]==10) tieneDiez=true;
        else if (categorias[i]==11) tieneOnce=true;
        else if (categorias[i]==12) tieneDoce=true;
        else if (categorias[i]==13) tieneTrece=true;
        else if (categorias[i]==1) tieneAs=true;
    }
    return tieneDiez && tieneOnce && tieneDoce && tieneTrece && tieneAs && IsFlush(mano);
}