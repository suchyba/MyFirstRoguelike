#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "obslugaMapy.h"
#include "obslugaPlikow.h"
#include "obslugaWynikow.h"

// Kody klawiszy by polepszyć czytelność kodu
enum KodyKlawiszy {STRZALKA_GORA = 72, STRZALKA_DOL = 80, STRZALKA_PRAWO = 77, STRZALKA_LEWO = 75, ENTER = 13, SPACJA = 32, ESC = 27};

// Kody wartości zwracanych przez funkcję obslugaKlawisza w celu polepszenia czytelności kodu
enum KodyZwracane {OBSLUZONE, PAUZA, AKCJA};
typedef enum KodyZwracane kodyZwracane;

// Pomocnicze wektory statyczne <- co z tym zrobić???
const vector2D LEWO = {0, -1};
const vector2D PRAWO = {0, 1};
const vector2D GORA = {-1, 0};
const vector2D DOL = {1, 0};

//Pomocnicze definiowanie typów by ³awiej obs³ugiwaæ ci¹gi tekstowe
#define string char*
#define cstring const char*


// Definicje funkcji
void tura(vector2D rozmiar, elementMapy **mapa);
kodyZwracane obslugaKlawiszaGra(vector2D rozmiar, elementMapy **mapa);
string menuListyMap(unsigned int ile, unsigned int wielkosc, char **lista);
listaWynikow* gra(listaWynikow* lista);
elementMapy** wczytywanieMapy(vector2D* rozmiar, unsigned int poziom, char* wybranyPlik, int czyPrzejscieZWyzszegoPoziomu);
int czyszczenieMapy(vector2D rozmiar, elementMapy** mapa);
int menuGlowne();
void menuAutor();
void menuInstrukcja();
void obslugaMenuGlowne();
int menuTakNie();
elementWyniku* podsumowanieRozgrywki(int czasRozpoczecia, int czasZakonczenia, int poziomy, string nazwaPliku, listaWynikow* lista);
string pobierzNazweGracza();

int main()
{
    obslugaMenuGlowne();
    return 0;
}
void obslugaMenuGlowne()
{
    listaWynikow* lista = odczytajNajlepszeWyniki();

    int glownaOpcja = -1;
    for(;;)
    {
        glownaOpcja = menuGlowne();
        switch(glownaOpcja)
        {
            case 1:
            {
                lista = gra(lista);
                break;
            }
            case 2:
            {
                menuInstrukcja();
                break;
            }
            case 3:
            {
                wypiszListe(lista);
                break;
            }
            case 4:
            {
                menuAutor();
                break;
            }
            case 5:
            {
                if(menuTakNie() == 1)
                {
                    zapiszNajlepszeWyniki(lista);
                    return;
                }
                else
                    continue;
            }
        }
    }
}
listaWynikow* gra(listaWynikow* lista)
{
    //                                      WCZYTANIE PLIKU
    // #########################################################################################3

    int ilePlikow = wczytajLiczbePlikow();
    int wielkoscCiaguPliku = 20;

    char** pliki = listujPliki(ilePlikow, wielkoscCiaguPliku);
    int i = 0;

    string wybranyPlik = menuListyMap(ilePlikow, wielkoscCiaguPliku, pliki);

    if(wybranyPlik == NULL)
        return lista;

    //                                  WCZYTANIE MAPY
    // #########################################################################################3
    int koniecGry = 0;

    int poziom = 1;
    vector2D rozmiarMapy = {-1, -1};
    elementMapy** mapa = wczytywanieMapy(&rozmiarMapy, poziom, wybranyPlik, 0);

    //                                  GRA SAMA W SOBIE
    // #########################################################################################3

    int przejscie = 0;

    int czasRozpoczecia = time(NULL);

    while(koniecGry == 0)
    {
        for(;;)
        {
            system("cls");
            if(mapa == NULL)
                printf("Cos poszlo nie tak...\n");


            ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiarMapy, getPozycjaGracza(rozmiarMapy, mapa), 2, 1);
            printf("\tPoziom: %d", poziom);
            wyswietlMape(rozmiarMapy, mapa);
            kodyZwracane zwrocony = obslugaKlawiszaGra(rozmiarMapy, mapa);
            if(zwrocony == PAUZA)
            {
                if(menuTakNie() == 1)
                    koniecGry = 1;
                else
                    continue;

                break;
            }
            else if(zwrocony == AKCJA)
            {
                vector2D pozycjaGracza = getPozycjaGracza(rozmiarMapy, mapa);
                if(mapa[pozycjaGracza.wiersz][pozycjaGracza.kolumna].czyToKoncowePole)
                {
                    przejscie = 1;
                    break;
                }
                else if(mapa[pozycjaGracza.wiersz][pozycjaGracza.kolumna].czyToPoczatkowePole)
                {
                    przejscie = -1;
                    break;
                }
            }
        }
        if(przejscie == 1)
        {
            ++poziom;

            czyszczenieMapy(rozmiarMapy, mapa);
            mapa = NULL;

            mapa = wczytywanieMapy(&rozmiarMapy, poziom, wybranyPlik, 0);

            przejscie = 0;

            if(mapa == NULL)
            {
                // Oznacza ze nic nie ma dalej - koniec gry
                system("cls");
                printf("Brawo udalo ci sie wygrac...\n");

                lista = podsumowanieRozgrywki(czasRozpoczecia, time(NULL), poziom - 1, wybranyPlik, lista);

                koniecGry = 1;
            }
            // Tu przejsice na poczatek petli gry
            continue;
        }
        else if(przejscie == -1)
        {
            --poziom;
            if(poziom <= 0)
            {
                printf("To jest pierwszy poziom, nie mozesz przejsc wczesniej!!!");
                Sleep(1000);
                przejscie = 0;
                poziom = 1;

                // Tutaj powrót na początek pętli gry
                continue;
            }

            czyszczenieMapy(rozmiarMapy, mapa);
            mapa = NULL;

            mapa = wczytywanieMapy(&rozmiarMapy, poziom, wybranyPlik, 1);

            przejscie = 0;

            // Przejście na początek pętli gry
            continue;
        }
    }


    //                                  CZYSZCZENIE MAPY I PLIKU
    // #########################################################################################3

    for(i = 0; i < ilePlikow; ++i)
        free(pliki[i]);

    free(pliki);
    return lista;
}
elementMapy** wczytywanieMapy(vector2D* rozmiar, unsigned int poziom, char* wybranyPlik, int czyPrzejscieZWyzszegoPoziomu)
{
    int iloscObiektow = -1;
    vector2D rozmiarMapy = wczytajParametryMapy(poziom, wybranyPlik, &iloscObiektow);

    if(iloscObiektow <= 0 || rozmiarMapy.wiersz <= 0 || rozmiarMapy.kolumna <= 0)      // zwracanie błędu odczytu listy obiektów
        return NULL;


    obiektMapy* listaObiektow = listujObiektyMapy(poziom, wybranyPlik, iloscObiektow);
    if(listaObiektow == NULL)
        return NULL;

    elementMapy** mapaWsk = wczytajMapeZPliku(rozmiarMapy, poziom, listaObiektow, iloscObiektow, wybranyPlik, czyPrzejscieZWyzszegoPoziomu);

    if(mapaWsk == NULL)
        return NULL;

    free(listaObiektow);

    rozmiar->wiersz = rozmiarMapy.wiersz;
    rozmiar->kolumna = rozmiarMapy.kolumna;

    return mapaWsk;
}

int czyszczenieMapy(vector2D rozmiar, elementMapy** mapa)
{
    int i = 0;
    for(i = 0; i < rozmiar.wiersz; ++i)
        free(mapa[i]);

    free(mapa);
}

kodyZwracane obslugaKlawiszaGra(vector2D rozmiar, elementMapy **mapa)
{
    for(;;)
    {
        int klawisz = getch();
        switch(klawisz)
        {
            case STRZALKA_GORA:
            {
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 0);
                przesunGracza(rozmiar, mapa, GORA);
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 1);
                return OBSLUZONE;
            }
            case STRZALKA_DOL:
            {
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 0);
                przesunGracza(rozmiar, mapa, DOL);
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 1);
                return OBSLUZONE;
            }
            case STRZALKA_PRAWO:
            {
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 0);
                przesunGracza(rozmiar, mapa, PRAWO);
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 1);
                return OBSLUZONE;
            }
            case STRZALKA_LEWO:
            {
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 0);
                przesunGracza(rozmiar, mapa, LEWO);
                ustawPoziomWidocznosciObszaruWokoloGracza(mapa, rozmiar, getPozycjaGracza(rozmiar, mapa), 2, 1);
                return OBSLUZONE;
            }
            case ESC:
            {
                return PAUZA;
            }
            case ENTER:
            {
                return AKCJA;
            }
        }
    }

}
int menuGlowne()
{
    int wybranaOpcja = 1;
    int klawisz = -1;
    for(;;)
    {
        system("cls");
        printf("\tRetro Labirynt\n");
        printf("#############################\n");
        printf("[%c] Graj!\n", wybranaOpcja == 1 ? 'X' : ' ');
        printf("[%c] Instrukcja\n", wybranaOpcja == 2 ? 'X' : ' ');
        printf("[%c] Najlepsze wyniki\n", wybranaOpcja == 3 ? 'X' : ' ');
        printf("[%c] Autor\n", wybranaOpcja == 4 ? 'X' : ' ');
        printf("[%c] Wyjscie\n", wybranaOpcja == 5 ? 'X' : ' ');
        klawisz = getch();

        switch(klawisz)
        {
            case STRZALKA_DOL:
            {
                if(wybranaOpcja == 5)
                    wybranaOpcja = 1;
                else
                    ++wybranaOpcja;

                break;
            }
            case STRZALKA_GORA:
            {
                if(wybranaOpcja == 1)
                    wybranaOpcja = 5;
                else
                    --wybranaOpcja;

                break;
            }
            case ENTER:
            {
                return wybranaOpcja;
            }
        }
    }

    return -1;
}
void menuAutor()
{
    system("cls");
    printf("\tAutor\n");
    printf("#####################\n");
    printf("Autorem jest niejaki Bartlomiej Suchecki\n");
    printf("Jest widywany na zajeciach I semestru informatyki na PB\n");
    printf("Nalezy do grupy PS8\n");
    printf("Jego numer indeksu: PB...\n\n");
    printf("Nacisnij dowolny klawisz by kontynuowac...\n");
    int czekaj = getch();
}
void menuInstrukcja()
{
    system("cls");
    printf("\tInstrukcja\n");
    printf("##########################\n");
    printf("Twoja postac jest reprezentowana przez znak: @\n");
    printf("Poruszasz sie za pomoca strzalek.\n");
    printf("Elementow specjalnych urzywasz przy pomocy klawisza ENTER.\n");
    printf("Konczysz gre za pomoca klawisza ESC.\n\n");
    printf("Nacisnij dowolny klawisz by kontynuowac...\n");
    int czekaj = getch();
}
string menuListyMap(unsigned int ile, unsigned int wielkosc, char **lista)
{
    int wybranaOpcja = 0;
    int koniec = 0;
    int klawisz = 0;
    int i = 0;

    while(koniec == 0)
    {
        system("cls");
        printf("Wybierz plik ktory chcesz zaladowac:\n");
        for(i = 0; i < ile; ++i)
        {
            printf("[%c] %s\n", (i == wybranaOpcja ? 'X' : ' '), lista[i]);
        }
        klawisz = getch();
        switch(klawisz)
        {
            case STRZALKA_GORA:
            {
                if(wybranaOpcja == 0)
                    wybranaOpcja = ile - 1;
                else
                    --wybranaOpcja;

                break;
            }
            case STRZALKA_DOL:
            {
                if(wybranaOpcja == ile - 1)
                    wybranaOpcja = 0;
                else
                    ++wybranaOpcja;

                break;
            }
            case ENTER:
            {
                koniec = 1;
                break;
            }
            case ESC:
            {
                koniec = 1;
                return NULL;
            }
        }
    }

    system("cls");
    return lista[wybranaOpcja];
}
int menuTakNie()
{
    for(;;)
    {
        printf("Czy jestes pewien? [T/N]: ");
        char znak;
        fflush(stdin);
        scanf("%c", &znak);
        if(znak == 'T' || znak == 't')
            return 1;
        else if(znak == 'N' || znak == 'n')
            return 0;
        else
            continue;
    }
}
elementWyniku* podsumowanieRozgrywki(int czasRozpoczecia, int czasZakonczenia, int poziomy, string nazwaPliku, listaWynikow* lista)
{
    string nazwaGracza = pobierzNazweGracza();

    int czasGry = czasZakonczenia - czasRozpoczecia;
    system("cls");
    printf("\tPodsumowanie\n");
    printf("#############################\n");
    printf("Gracz : %s\n", nazwaGracza);
    printf("Mapa: %s\n", nazwaPliku);
    printf("Ilosc poziomow: %d\n", poziomy);
    printf("Czas rozgrywki: %ds\n\n", czasGry);

    elementWyniku wynik;

    wynik.czas = czasGry;
    strcpy(wynik.mapa, nazwaPliku);
    strcpy(wynik.nazwaGracza, nazwaGracza);

    lista = wstawNaMiejscu(lista, wynik);

    printf("Aby kontynuowac nacisnij dowolny klawisz...\n");
    int czekaj = getch();
    free(nazwaGracza);
    return lista;
}
string pobierzNazweGracza()
{
    string nazwaGracza = malloc(6 * sizeof(char));
    int koniec = 0;

    while(koniec == 0)
    {
        system("cls");
        printf("Podaj nazwe uzytkownika (nie moze byc dluzsza niz 5 liter): ");
        scanf("%s", nazwaGracza);
        if(strlen(nazwaGracza) <= 5)
        {
            printf("Wprowadzona nazwa to: %s\n", nazwaGracza);
            if(menuTakNie() == 1)
                koniec = 1;
        }
        else
        {
            printf("Wprowadzona nazwa jest za dluga!!!\n");
            Sleep(500);
        }
    }
    return nazwaGracza;
}
