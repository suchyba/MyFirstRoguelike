#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "obslugaPlikow.h"
#include "obslugaMapy.h"

elementMapy** wczytajMapeZPliku(vector2D rozmiar, unsigned int poziom, obiektMapy* listaObiektow, unsigned int ileObiektow, cstring scierzka, int czyNaKoncu)
{
    FILE* plik = fopen(scierzka, "r");
    if(plik == NULL)
    {
        printf("Blad otwierania pliku o scierzce: %s", scierzka);
        return NULL;
    }
    char slowo[20];

    while(fscanf(plik, "%s", slowo) != EOF)
    {
        if(strcoll(slowo, "Poziom:") == 0)
        {
            int odczytanaLiczba = -1;
            fscanf(plik, "%d", &odczytanaLiczba);

            if(odczytanaLiczba == poziom)
            {
                printf("Znaleziono mape o poziomie: %d\n", odczytanaLiczba);                  /// DEBUG

                // Przeskakiwanie do momentu gdzie znajduje się część związana z warstwami
                fscanf(plik, "%s", slowo);
                while(strcoll(slowo, "WarstwaI:") != 0)
                {
                    fscanf(plik, "%s", slowo);
                }

                printf("Dotarlem do %s\n", slowo);                                            /// DEBUG

                // Inicjalizowanie tablic, które będą przechowywały numery obiektów w warstwach
                int **warstwaI = (int**)malloc(rozmiar.wiersz * sizeof(int*));
                int **warstwaII = (int**)malloc(rozmiar.wiersz * sizeof(int*));
                int i = 0;
                for(i = 0; i < rozmiar.wiersz; ++i)
                {
                    warstwaI[i] = (int*)malloc(rozmiar.kolumna * sizeof(int));
                    warstwaII[i] = (int*)malloc(rozmiar.kolumna * sizeof(int));
                }

                if(warstwaI == NULL || warstwaII == NULL)
                {
                    printf("Wystapil blad w alokacji warstw!!!\n");
                    return NULL;
                }

                printf("Tablice warstw zostaly zaalokowane poprawnie.\n");                      /// DEBUG

                // Wczytywanie I warstwy
                int j = 0;
                for(i = 0; i < rozmiar.wiersz; ++i)
                {
                    for(j = 0; j < rozmiar.kolumna; ++j)
                    {
                        fscanf(plik, "%d", &odczytanaLiczba);
                        warstwaI[i][j] = odczytanaLiczba;
                    }
                }

                printf("Wczytano warstwe I poprawnie.\n");                                  /// DEBUG

                // Wczytywanie II warstwy
                fscanf(plik, "%s", slowo);
                if(strcoll(slowo, "WarstwaII:") == 0)
                {
                    for(i = 0; i < rozmiar.wiersz; ++i)
                    {
                        for(j = 0; j < rozmiar.kolumna; ++j)
                        {
                            fscanf(plik, "%d", &odczytanaLiczba);
                            warstwaII[i][j] = odczytanaLiczba;
                        }
                    }
                }
                else
                {
                    printf("Błąd odczytu informacji!");
                    return NULL;
                }

                printf("Wczytano warstwe II poprawnie.\n");                                 /// DEBUG

                int numerObiektuI = 0, numerObiektuII = 0;

                // Alokowanie pamięci
                elementMapy** mapa = malloc(rozmiar.wiersz * sizeof(elementMapy*));
                for(i = 0; i < rozmiar.wiersz; ++i)
                    mapa[i] = malloc(rozmiar.kolumna * sizeof(elementMapy));

                printf("Alokowanie pamieci na mape zakonczoe sukcesem.\n");                 /// DEBUG

                // Wypełnianie mapy
                for(i = 0; i < rozmiar.wiersz; ++i)
                {
                    for(j = 0; j < rozmiar.kolumna; ++j)
                    {
                        numerObiektuI = warstwaI[i][j];
                        numerObiektuII = warstwaII[i][j];

                        mapa[i][j].numerCharDol = listaObiektow[numerObiektuII].numerZnaku;
                        mapa[i][j].numerCharGora = listaObiektow[numerObiektuI].numerZnaku;
                        mapa[i][j].poziomWidocznosci = -1;
                        mapa[i][j].rodzajPola = listaObiektow[numerObiektuI].typ;

                        if(numerObiektuI == 0)
                        {
                            if(czyNaKoncu == 0)
                                mapa[i][j].czyJestTuGracz = 1;
                            else
                                mapa[i][j].czyJestTuGracz = 0;

                            mapa[i][j].czyToPoczatkowePole = 1;
                        }
                        else
                        {
                            mapa[i][j].czyJestTuGracz = 0;
                            mapa[i][j].czyToPoczatkowePole = 0;
                        }

                        if(numerObiektuI == 1)
                        {
                            if(czyNaKoncu == 1)
                                mapa[i][j].czyJestTuGracz = 1;

                            mapa[i][j].czyToKoncowePole = 1;
                        }
                        else
                            mapa[i][j].czyToKoncowePole = 0;
                    }
                }

                printf("Wypelniono mape.\n");                                                /// DEBUG

                // Czyszczenie tablic
                for(i = 0; i < rozmiar.wiersz; ++i)
                {
                    free(warstwaI[i]);
                    free(warstwaII[i]);
                    printf("[%d]\n", i);
                }
                free(warstwaI);
                free(warstwaII);
                fclose(plik);

                printf("Pamiec wyczyszczona.\n");                                           /// DEBUG

                return mapa;
            }
        }
    }
}
vector2D wczytajParametryMapy(unsigned int poziom, cstring scierzka, int* iloscObiektow)
{
    FILE* plik = fopen(scierzka, "r");
    if(plik == NULL)
    {
        printf("Blad otwierania pliku o scierzce: %s", scierzka);
        vector2D ret = {-1, -1};
        return ret;
    }
    char slowo[20] = " ";
    vector2D rozmiar = {-1, -1};
    while(fscanf(plik, "%s", slowo) != EOF)
    {
        if(strcoll(slowo, "Poziom:") == 0)
        {
            int odczytanyPoziom = -1;
            fscanf(plik, "%d", &odczytanyPoziom);

            if(odczytanyPoziom == poziom)
            {
                fscanf(plik, "%s", slowo);
                if(strcoll(slowo, "Rozmiar:") == 0)
                {
                    fscanf(plik, "%d %d", &rozmiar.wiersz, &rozmiar.kolumna);
                }

                fscanf(plik, "%s", slowo);
                if(strcoll(slowo, "Obiekty:") == 0)
                {
                    fscanf(plik, "%d", iloscObiektow);
                }
                fclose(plik);
                return rozmiar;
            }
        }
    }
    printf("Nie odnaleziono odpowiadajacej frazy w pliku: %s\n", scierzka);
    fclose(plik);
    return rozmiar;
}
// Dzia³a na takiej samej zasadzie jak poni¿sza
int wczytajLiczbePlikow()
{
    struct dirent* plik;
    DIR* katalog = opendir(".");
    if(katalog == NULL)
    {
        printf("Blad podczas otwierania katalogu o scierzce : /!\n");
        return -1;
    }
    int ile = 0;
    string ostaniePojawienieSieKropki;
    while(plik = readdir(katalog))
    {
        ostaniePojawienieSieKropki = strrchr(plik -> d_name, '.');
        if(ostaniePojawienieSieKropki != NULL && plik -> d_name != "." && plik -> d_name != "..")
        {
            if(strcoll(ostaniePojawienieSieKropki, ".maps") == 0)
                ++ile;
        }
    }

    closedir(katalog);
    free(plik);
    return ile;
}

char** listujPliki(int ile, int wielkosc)
{
    struct dirent* plik;
    DIR* katalog = opendir(".");
    if(katalog == NULL)
    {
        printf("Blad podczas otwierania katalogu o scierzce : /!\n");
        return NULL;
    }
    int i = 0;

    char **tab = malloc(ile * sizeof(char*));

    // Jest to wskaznik do ostatniej kropki znajdujacej sie w nazwie pliku
    string ostaniePojawienieSieKropki;
    while(plik = readdir(katalog))
    {
        // Funkcja zwraca wskaŸnik do ostatniego pojawienia siê znaku w tym przypadku '.'
        ostaniePojawienieSieKropki = strrchr(plik -> d_name, '.');
        // Sprawdzenie czy plik nie jest katalogiem ".", ".." oraz czy jest to wgl plik a nie katalog
        if(ostaniePojawienieSieKropki != NULL && plik -> d_name != "." && plik -> d_name != "..")
        {
            // Sprawdzenie czy plik ma odpowiednie rozszerzenie
            if(strcoll(ostaniePojawienieSieKropki, ".maps") == 0)
            {
                // Skopiowanie nazwy pliku do talbicy

                tab[i] = malloc(wielkosc * sizeof(char));

                strcpy(tab[i], plik -> d_name);

                ++i;
            }
        }
    }
    closedir(katalog);
    free(plik);
    return tab;
}

obiektMapy* listujObiektyMapy(unsigned int poziom, cstring scierzka, unsigned int ile)
{
    FILE* plik = fopen(scierzka, "r");
    if(plik == NULL)
    {
        printf("Blad otwierania pliku o scierzce: %s", scierzka);
        return NULL;
    }
    char slowo[20] = " ";
    while(fscanf(plik, "%s", slowo) != EOF)
    {
        if(strcoll(slowo, "Poziom:") == 0)
        {
            int odczytanaLiczba = -1;
            fscanf(plik, "%d", &odczytanaLiczba);

            if(odczytanaLiczba == poziom)
            {
                obiektMapy* lista = malloc(ile * sizeof(obiektMapy));

                fscanf(plik, "%s", slowo);
                while(strcoll(slowo, "Numer:") != 0)
                {   fscanf(plik, "%s", slowo);  }

                int numerZnaku = 0, numerObiektu = -1;
                typPolowy typPola = -1;

                fscanf(plik, "%d", &numerObiektu);
                fscanf(plik, "%s%d", slowo, &numerZnaku);
                fscanf(plik, "%s%d", slowo, &typPola);

                lista[numerObiektu].numerZnaku = numerZnaku;
                lista[numerObiektu].typ = typPola;

                int i = 0;
                for(i = 1; i < ile; ++i)
                {
                    fscanf(plik, "%s%d", slowo, &numerObiektu);
                    fscanf(plik, "%s%d", slowo, &numerZnaku);
                    fscanf(plik, "%s%d", slowo, &typPola);

                    lista[numerObiektu].numerZnaku = numerZnaku;
                    lista[numerObiektu].typ = typPola;
                }

                fclose(plik);
                return lista;
            }
        }
    }
    printf("Nie odnaleziono odpowiadajacej frazy lub poziomu w pliku: %s\n", scierzka);
    fclose(plik);
    return NULL;
}

