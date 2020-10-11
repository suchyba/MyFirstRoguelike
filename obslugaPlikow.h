#ifndef OBSLUGAPLIKOW_H_INCLUDED
#define OBSLUGAPLIKOW_H_INCLUDED

#include "obslugaMapy.h"

#define cstring const char*
#define string char*

struct ObiektMapy
{
    int numerZnaku;
    typPolowy typ;
};
typedef struct ObiektMapy obiektMapy;

vector2D wczytajRozmiarMapy(unsigned int poziom, cstring scierzka);

// Wczytauje <mapę> o <rozmiarach> oraz poziomie <poziom> z pliku o <ścierzce>
// Zwraca 0 - powodzenie w innym przypadku zwraca kod błędu
elementMapy** wczytajMapeZPliku(vector2D rozmiar, unsigned int poziom, obiektMapy* listaObiektow, unsigned int ileObiektow, cstring scierzka, int czyNaKoncu);

// Zwraca rozmiar mapy zapisanej w pliku o <scierzce> oraz o <poziomie>
vector2D wczytajParametryMapy(unsigned int poziom, cstring scierzka, int* iloscObiektow);

int wczytajLiczbePlikow();

char** listujPliki(int ile, int wielkosc);

obiektMapy* listujObiektyMapy(unsigned int poziom, cstring scierzka, unsigned int ile);

#endif // OBSLUGAPLIKOW_H_INCLUDED
