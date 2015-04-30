#include <QFile>
#include <QTextStream>
#include "generasjon.h"
void generasjon::initGen()
{
    generasjonNr = 0;
    fyllOpp();
}

/**
 * Fyller opp tbl med døde celler.
 * @brief generasjon::fyllOpp
 */
void generasjon::fyllOpp() {
    for (int i = 0; i<40000; i++) {
        generasjonTbl[i] = false;
    }
}
/**
 * Importerer mønster i Life 1.05 format.
 * @brief generasjon::lesFil
 * @param filnavn Full filbane til fil.
 */
void generasjon::lesFil(const char* const filnavn) {
    QFile file(filnavn);
    QString linje;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    if (QString::compare(in.readLine(), "#Life 1.05")) {
        //qDebug()<<"KUNNE IKKE LESE: Må være Life 1.05 format";
        return;
    }

    bool mnstrStart = false;
    int sx;
    int sy;

    int startIndeks;
    int skrvC;
    int linjeNr = 0;
    while(!in.atEnd()) {
        linje = in.readLine();
        if (!mnstrStart) {
            if (linje.startsWith("#D"))
                //qDebug()<<"BESKRIVELSE:"<<linje<<std::endl;
            if (linje.startsWith("#P")) {
                mnstrStart = true;
                sx = linje.split(" ")[1].toInt();
                sy = linje.split(" ")[2].toInt();
                //std::cout<<"START MØNSTER FRA (x,y): "<<sx<<","<<sy<<std::endl;

                startIndeks = ((20099 + sx) + (sy*199));
                skrvC = startIndeks;
                //qDebug()<<"startindeks blir: " << startIndeks;
            }
        }

        else {
            //qDebug()<<"linjelengde: " << linje.length();
            for (int i=0; i<linje.length();i++) {
                if (QString::compare(linje.at(i),"*")==0) {
                    generasjonTbl[skrvC]=true;
                    //qDebug()<<"*"<<" indeks: " <<skrvC;

                 }
                else {
                     //qDebug()<<"."<<" indeks: " <<skrvC;
                }
                skrvC++;
            }
            linjeNr++;
            skrvC=startIndeks;
            skrvC+=200*linjeNr;
        }

    }

    file.close();
}
/**
 * @brief generasjon::erLevende
 * @param indeks
 * @return True om levende, false om død.
 */
bool generasjon::erLevende(int indeks) {
    if (generasjonTbl[indeks])
        return true;
    else
        return false;
}
/**
 * Setter verdi for neste generasjon
 * @brief generasjon::setNeste
 * @param indeks
 * @param verdi
 */
void generasjon::setNeste(int indeks, bool verdi) {
    nesteTbl[indeks] = verdi;
}

/**
 * Finner antall naboer for celle i. Endrer variabel referanse.
 * @brief generasjon::finnAntallNaboer
 * @param i
 * @param antallNaboer
 * @return
 */
int generasjon::finnAntallNaboer(int i) {
    int antallNaboer = 0;
    if ((((i+1)%200)!=0 || i==0) && erLevende(i+1)) //høyre
        antallNaboer++;
    if (((i+1)%200)!=1 && erLevende(i-1)) //venstre
        antallNaboer++;
    if ((i>199) && erLevende(i-200)) //over
        antallNaboer++;
    if ((i<39799) && erLevende(i+200)) //under
        antallNaboer++;
    if (((i>199) && ((i+1)%200)!=1) && erLevende(i-199)) //over-venstre
        antallNaboer++;
    if ((i>199) && (((i+1)%200)!=0 || i==0) && erLevende(i-201)) //over-høyre
        antallNaboer++;
    if ((i<39799) && (((i+1)%200)!=1) && erLevende(i+199)) //under-venstre
        antallNaboer++;
    if ((i<39799) && (((i+1)%200)!=0 || i==0) && erLevende(i+201)) //under-høyre
        antallNaboer++;

    return antallNaboer;
}

bool generasjon::bestemSkjebne(int i) {
    bool nesteCelle = false;
    int antallNaboer = finnAntallNaboer(i);
    if (erLevende(i)) {
        if (antallNaboer<2 || antallNaboer>3){
            nesteCelle = false;
        }
        else if (antallNaboer==2 || antallNaboer==3){
            nesteCelle = true;
        }

    }
    else if (!erLevende(i) && antallNaboer==3){
            nesteCelle = true;
    }

    return nesteCelle;
}

/**

 * @brief generasjon::nyGenerasjon
 */
void generasjon::nyGenerasjon() {
    if (generasjonNr!=0)
        std::copy(nesteTbl,nesteTbl+40000,generasjonTbl); //flytter neste til generasjon
}

void generasjon::gNrPlussEn() {
    generasjonNr++;
}



