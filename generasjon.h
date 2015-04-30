#ifndef GENERASJON_H
#define GENERASJON_H
#import "mainwindow.h"

class generasjon
{
public:
    generasjon();
    ~generasjon();

    int hoyde;
    int vidde;
    int generasjonNr;
    bool generasjonTbl[40000];
    bool nesteTbl[40000];

    const char* filnavn;
    void lesFil(const char* const filnavn);
    void fyllOpp();
    void initGen();
    bool erLevende(int indeks);
    void setNeste(int indeks, bool verdi);
    int finnAntallNaboer(int i);
    bool bestemSkjebne(int i);
    void gNrPlussEn();
    void nyGenerasjon();
};

#endif // GENERASJON_H
