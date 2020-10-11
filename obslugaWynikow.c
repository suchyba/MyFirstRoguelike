#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obslugaWynikow.h"

listaWynikow* odczytajNajlepszeWyniki() /// tworzy listê
{
    FILE* plik = fopen("wyniki.wyn", "r");
    if(plik == NULL)
    {
        printf("Wystapil blad podczas otwierania pliku: wyniki.wyn\n");
        return NULL;
    }

    listaWynikow* lista = malloc(sizeof(listaWynikow));

    if(lista == NULL)
    {
        printf("Nastapil blad podczas alokowania pamieci na liste.\n");
        fclose(plik);
        return NULL;
    }
    lista -> nastepnyElement = NULL;
    lista -> dane.czas = -1;

    listaWynikow* pierwszyElement = lista;

    elementWyniku* wczytanyElement = malloc(sizeof(elementWyniku));
    if(wczytanyElement == NULL)
    {
        printf("Wystapil blad podczas alokowania pamieci wczytanegoElementu.\n");
        fclose(plik);
        return NULL;
    }

    char slowo1[20] = " ";
    char slowo2[20] = " ";
    int liczba = 0;

    // Wczytywanie pierwszego elementu
    if (fscanf(plik, "%s%s", slowo1, slowo2) == EOF)
    {
        printf("Lista jest pusta\n");
        free(wczytanyElement);
        fclose(plik);
        return lista;
    }

    strcpy(wczytanyElement -> nazwaGracza, slowo2);

    fscanf(plik, "%s%s", slowo1, slowo2);
    strcpy(wczytanyElement -> mapa, slowo2);

    fscanf(plik, "%s%d", slowo1, &liczba);
    wczytanyElement -> czas = liczba;

    lista = wstawNaKoncu(lista, *wczytanyElement);

    // Wczytywanie reszty elementow
    while(fscanf(plik, "%s%s", slowo1, slowo2) != EOF)
    {
        strcpy(wczytanyElement -> nazwaGracza, slowo2);

        fscanf(plik, "%s%s", slowo1, slowo2);
        strcpy(wczytanyElement -> mapa, slowo2);

        fscanf(plik, "%s%d", slowo1, &liczba);
        wczytanyElement -> czas = liczba;

        lista = wstawNaKoncu(lista, *wczytanyElement);
    }

    fclose(plik);
    return pierwszyElement;
}
listaWynikow* wstawNaKoncu(listaWynikow* lista, elementWyniku element)
{
    listaWynikow* karetka = lista;

    if(karetka == NULL)
    {
        printf("Lista nie moze byc pusta!!!\n");
        return NULL;
    }

    // Wstawianie na pierwsze miejsce
    if(karetka -> dane.czas == -1)
    {
        karetka -> dane.czas = element.czas;
        strcpy(karetka -> dane.mapa, element.mapa);
        strcpy(karetka -> dane.nazwaGracza, element.nazwaGracza);
        karetka -> nastepnyElement = NULL;
        return karetka;
    }

    // przewijanie na przedostatnia pozycje
    while(karetka -> nastepnyElement != NULL)
    {
        karetka = karetka -> nastepnyElement;
    }

    karetka -> nastepnyElement = malloc(sizeof(listaWynikow));
    if(karetka -> nastepnyElement == NULL)
    {
        printf("Wystapil blad podczas alokacji elementu.\n");
        return NULL;
    }

    (karetka -> nastepnyElement) -> dane.czas = element.czas;
    strcpy((karetka -> nastepnyElement) -> dane.mapa, element.mapa);
    strcpy((karetka -> nastepnyElement) -> dane.nazwaGracza, element.nazwaGracza);

    (karetka -> nastepnyElement) -> nastepnyElement = NULL;

    return (karetka -> nastepnyElement);
}
void zapiszNajlepszeWyniki(listaWynikow* lista)
{
    if(lista == NULL || lista -> dane.czas == -1)
    {
        printf("Lista jest pusta.\n");
        return;
    }

    FILE* plik = fopen("wyniki.wyn", "w");
    if(plik == NULL)
    {
        printf("Blad otwierania pliku wyniki.wyn.\n");
        return;
    }

    listaWynikow* nastepnyElementLok = lista -> nastepnyElement;


    while(lista != NULL)
    {
        elementWyniku* testowy = &(lista -> dane);

        fprintf(plik, "Gracz: %s\nMapa: %s\nCzas: %d\n", lista -> dane.nazwaGracza, lista -> dane.mapa, lista -> dane.czas);

        free(lista);
        lista = nastepnyElementLok;
        if(nastepnyElementLok != NULL)
            nastepnyElementLok = nastepnyElementLok -> nastepnyElement;
    }

    fclose(plik);
}
listaWynikow* wstawNaMiejscu(listaWynikow* lista, elementWyniku element)
{
    listaWynikow* doWstawienia = malloc(sizeof(listaWynikow));

    doWstawienia -> dane.czas = element.czas;

    strcpy(doWstawienia -> dane.mapa, element.mapa);

    strcpy(doWstawienia -> dane.nazwaGracza, element.nazwaGracza);

    if(lista == NULL)
    {
        lista = doWstawienia;
        return doWstawienia;
    }

    listaWynikow* karetka = lista;

    // Na liscie nie ma elementow
    if(karetka -> dane.czas == -1)
    {
        free(lista);
        lista = doWstawienia;
        doWstawienia -> nastepnyElement = NULL;

        return doWstawienia;
    }

    // Wyszukiwanie mapy na liscie
    listaWynikow* poprzedniKaretka = NULL;
    while(karetka != NULL)
    {
        if(strcoll(karetka -> dane.mapa, element.mapa) == 0)
        {
            break;
        }
        poprzedniKaretka = karetka;
        karetka = karetka -> nastepnyElement;
    }
    // Mapa jeszcze nie widnieje na liscie
    if(karetka == NULL)
    {
        poprzedniKaretka -> nastepnyElement = doWstawienia;
        doWstawienia -> nastepnyElement = NULL;
        ///return doWstawienia;
        return lista;
    }
    // Mapa jest na liscie
    else
    {
        while(karetka != NULL && karetka -> dane.czas < element.czas && strcoll(karetka -> dane.mapa, element.mapa) == 0)
        {
            poprzedniKaretka = karetka;
            karetka = karetka -> nastepnyElement;
        }
        if(karetka == NULL)
        {
            poprzedniKaretka -> nastepnyElement = doWstawienia;
            doWstawienia -> nastepnyElement = NULL;
            ///return doWstawienia;
            return lista;
        }
        else
        {
            poprzedniKaretka -> nastepnyElement = doWstawienia;
            doWstawienia -> nastepnyElement = karetka;
            ///return doWstawienia;
            return lista;
        }
    }
}
void wypiszListe(listaWynikow* lista)
{
    listaWynikow* karetka = lista;

    if(karetka == NULL || karetka -> dane.czas == -1)
    {
        system("cls");
        printf("\tTablica Wynikow\n");
        printf("################################\n");
        printf("\nNie posiadasz zadnych zapisanych gier...\n");
        int klawisz = getch();
        return;
    }

    int wybranaMapa = 0;

    int i = 0;
    int j = 0;

    string tabMap[25];

    for(i = 0; i < 25; ++i)
        tabMap[i] = NULL;

    int ostatnioOdczytanaMapa = 0;
    tabMap[0] = malloc(51 * sizeof(char));
    strcpy(tabMap[0], karetka -> dane.mapa);

    karetka = karetka -> nastepnyElement;

    // Wczytuje wszystkie rodzaje map ktore sa zawarte w pliku z wynikami (po jednym egzemplazu kazdego)
    while(karetka != NULL)
    {
        if(strcoll(tabMap[ostatnioOdczytanaMapa], karetka -> dane.mapa) != 0)
        {
            ++ostatnioOdczytanaMapa;
            tabMap[ostatnioOdczytanaMapa] = malloc(51 * sizeof(char));
            strcpy(tabMap[ostatnioOdczytanaMapa], karetka -> dane.mapa);
        }

        karetka = karetka -> nastepnyElement;
    }

    // reset karetki
    karetka = lista;

    enum KodyKlawiszy {STRZALKA_GORA = 72, STRZALKA_DOL = 80, STRZALKA_PRAWO = 77, STRZALKA_LEWO = 75, ESC = 27};

    int wyjscie = 0;
    int klawisz = -1;

    // tu wyswietlanie paska wyboru mapy
    while(wyjscie == 0)
    {
        karetka = lista;

        system("cls");
        printf("\tTablica Wynikow\n");
        printf("################################\n");
        for(i = 0; i < 5; ++i)
        {
            for(j = 0; j < 5; ++j)
            {
                if(tabMap[i * 5 + j] == NULL)
                    break;

                printf("[%c] %s\t", wybranaMapa == (i * 5 + j) ? 'X' : ' ', tabMap[i * 5 + j]);

            }
            printf("\n");
            if(j != 5)
                break;
        }

        // tu wyswietlanie wynikow

        printf("\n");
        printf("Lp.\tGracz:\t\tCzas:\n");
        int wypisano = 0;
        int iterator = 1;
        while(karetka != NULL)
        {
            if(strcoll(karetka -> dane.mapa, tabMap[wybranaMapa]) == 0)
            {
                printf("%d)\t%s\t\t%ds\n", iterator, karetka -> dane.nazwaGracza, karetka -> dane.czas);
                ++iterator;
                wypisano = 1;
            }
            else
            {
                if(wypisano == 1)
                    break;
            }

            karetka = karetka -> nastepnyElement;
        }


        // tu obsluga klawiszy
        klawisz = getch();
        switch(klawisz)
        {
            case ESC:
            {
                wyjscie = 1;
                break;
            }
            case STRZALKA_DOL:
            {
                if(wybranaMapa + 5 > ostatnioOdczytanaMapa)
                    break;
                else
                {
                    wybranaMapa += 5;
                }
                break;
            }
            case STRZALKA_GORA:
            {
                if(wybranaMapa - 5 < 0)
                    break;
                else
                {
                    wybranaMapa -= 5;
                }
                break;
            }
            case STRZALKA_PRAWO:
            {
                if(wybranaMapa + 1 > ostatnioOdczytanaMapa)
                    break;
                else
                {
                    ++wybranaMapa;
                }
                break;
            }
            case STRZALKA_LEWO:
            {
                if(wybranaMapa - 1 < 0)
                    break;
                else
                {
                    --wybranaMapa;
                }
                break;
            }
        }
    }
    for(i = 0; i <= ostatnioOdczytanaMapa; ++i)
    {
        free(tabMap[i]);
    }
}
