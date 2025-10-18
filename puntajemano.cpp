#include "Tarea2.cpp"
using namespace std;
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>

int puntajemano(nodoMazo* mano){
    int puntaje = 0;
    nodoMazo* aux = mano;
    if (IsHighCard(mano)){
        //carta mas alta
        int maxCategoria = 0;
        while (aux != NULL){
            if (aux->mano.Categoria > maxCategoria){
                maxCategoria = aux->mano.Categoria;
                valor = aux->mano.Valor;
            }
            aux = aux->sig;
        }
        puntaje += valor;
        puntaje += puntajetipomano(mano);
        return puntaje;
    }
    if (IsPair(mano)){
        nodoMazo* auxmano=mano;
        while (auxmano!=NULL){
            int categoriaActual=auxmano->mano.Categoria;
            nodoMazo* aux2=auxmano->sig;
            while (aux2!=NULL){
                if (aux2->mano.Categoria==categoriaActual){
                    puntaje += auxmano->mano.Valor * 2;
                    puntaje += puntajetipomano(mano);
                    return puntaje;
                }
                aux2=aux2->sig;
            }
        }
    }
    if (IsTwoPair(mano)){
        int paresEncontrados = 0;
        nodoMazo* auxmano = mano;
        while (auxmano != NULL){
            int categoriaActual = auxmano->mano.Categoria;
            nodoMazo* aux2 = auxmano->sig;
            while (aux2 != NULL){
                if (aux2->mano.Categoria == categoriaActual){
                    puntaje += auxmano->mano.Valor * 2;
                    paresEncontrados++;
                    break;
                }
                aux2 = aux2->sig;
            }
            auxmano = auxmano->sig;
        }
        if (paresEncontrados == 2){
            puntaje += puntajetipomano(mano);
            return puntaje;
        }
    }
    if (IsThreeOfAKind(mano)){
        nodoMazo* auxmano=mano;
        while (auxmano!=NULL){
            int categoriaActual=auxmano->mano.Categoria;
            nodoMazo* aux2=auxmano->sig;
            while (aux2!=NULL){
                if (aux2->mano.Categoria==categoriaActual){
                    nodoMazo* aux3=aux2->sig;
                    while (aux3!=NULL){
                        if (aux3->mano.Categoria==categoriaActual){
                            puntaje += auxmano->mano.Valor * 3;
                            puntaje += puntajetipomano(mano);
                            return puntaje;
                        }
                    }
                }
                aux2=aux2->sig;
            }
            auxmano=auxmano->sig;
        }
    }
    if (IsFourOfAKind(mano)){
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
                                    puntaje += auxmano->mano.Valor * 4;
                                    puntaje += puntajetipomano(mano);
                                    return puntaje;
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
    }
    if (IsFiveOfAKind(mano)){
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
                                            puntaje += auxmano->mano.Valor * 5;
                                            puntaje += puntajetipomano(mano);
                                            return puntaje;
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
    }
    if (IsFullHouse(mano)){
        nodoMazo* auxmano=mano;
        int valorTercia=0;
        int valorPar=0;
        while (auxmano!=NULL){
            int categoriaActual=auxmano->mano.Categoria;
            int contador=0;
            nodoMazo* aux2=auxmano;
            while (aux2!=NULL){
                if (aux2->mano.Categoria==categoriaActual){
                    contador++;
                }
                aux2=aux2->sig;
            }
            if (contador==3){
                valorTercia=auxmano->mano.Valor * 3;
            }
            else if (contador==2){
                valorPar=auxmano->mano.Valor * 2;
            }
            auxmano=auxmano->sig;
        }
        puntaje += valorTercia + valorPar;
        puntaje += puntajetipomano(mano);
        return puntaje;
    }
    if (IsStraight(mano) || IsFlush(mano) || IsStraightFlush(mano) || IsRoyalFlush(mano)){
        nodoMazo* aux=mano;
        int Categoria = 0;
        int Valor = 0;
        while (aux != NULL){
            if (aux->mano.Categoria > Categoria){
                Categoria = aux->mano.Categoria;
                Valor = aux->mano.Valor;
                puntaje += Valor;
            }
            aux = aux->sig;
        }
        puntaje += puntajetipomano(mano);
        return puntaje;
    }
}