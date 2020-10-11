#ifndef OBSLUGAWYNIKOW_H_INCLUDED
#define OBSLUGAWYNIKOW_H_INCLUDED

#define string char*

struct ElementWyniku
{
    char nazwaGracza[6];
    char mapa[51];
    int czas;
};
typedef struct ElementWyniku elementWyniku;

struct ListaWynikow
{
    elementWyniku dane;
    struct ListaWynikow* nastepnyElement;
};
typedef struct ListaWynikow listaWynikow;


listaWynikow* odczytajNajlepszeWyniki(); /// tworzy listê

void zapiszNajlepszeWyniki(listaWynikow* lista);    /// usuwa liste

listaWynikow* wstawNaKoncu(listaWynikow* lista, elementWyniku element);

listaWynikow* wstawNaMiejscu(listaWynikow* lista, elementWyniku element);

listaWynikow* czyscListe(listaWynikow* lista);

void wypiszListe(listaWynikow* lista);

#endif // OBSLUGAWYNIKOW_H_INCLUDED
