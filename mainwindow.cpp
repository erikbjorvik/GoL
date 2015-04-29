#include "mainwindow.h"
#include <QMenuBar>
#include <fstream>
#include <QMessageBox>


MainWindow::MainWindow(QWindow *parent) : QWindow(parent), backingStore(this)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nyGenerasjon())); //når timer signal: kall nyGenerasjon.
    timer->start(50); //Hver generasjon varer i 50ms.

    // set up viewing window
    create();
    setGeometry(100, 100, 600, 600); // set resolution of window

    generasjonNr = 0;

    //fyll array med døde celler.
    for (int i = 0; i<40000; i++) {
        generasjon[i] = false;
    }

    lesFil("/Users/ErikBjorvik/untitled1/gosper.txt"); //Fil som skal leses, i Life 1.05 format


}

void MainWindow::lesFil(const char* const filnavn) {
    QFile file(filnavn);
    QString linje;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    if (QString::compare(in.readLine(), "#Life 1.05")) {
        qDebug()<<"KUNNE IKKE LESE: Må være Life 1.05 format";
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
                qDebug()<<"BESKRIVELSE:"<<linje<<endl;
            if (linje.startsWith("#P")) {
                mnstrStart = true;
                sx = linje.split(" ")[1].toInt();
                sy = linje.split(" ")[2].toInt();
                qDebug()<<"START MØNSTER FRA (x,y): "<<sx<<","<<sy<<endl;

                startIndeks = ((20099 + sx) + (sy*199));
                skrvC = startIndeks;
                qDebug()<<"startindeks blir: " << startIndeks;
            }
        }

        else {
            qDebug()<<"linjelengde: " << linje.length();
            for (int i=0; i<linje.length();i++) {
                if (QString::compare(linje.at(i),"*")==0) {
                    generasjon[skrvC]=true;
                    qDebug()<<"*"<<" indeks: " <<skrvC;

                 }
                else {
                     qDebug()<<"."<<" indeks: " <<skrvC;
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

void MainWindow::nyGenerasjon() {
    if (generasjonNr!=0)
        std::copy(neste,neste+40000,generasjon);

    renderLater();
    generasjonNr++;
}

// the main rendering method
void MainWindow::renderNow()
{
    // return if window is "hidden"
    if (!isExposed())
        return;

    // define the drawing canvas
    QRect rect(0, 0, width(), height());
    backingStore.beginPaint(rect);

    // create a painter device
    QPaintDevice *device = backingStore.paintDevice();
    QPainter painter(device);

    // draw a white background and call render
    painter.fillRect(0, 0, width(), height(), Qt::white);
    render(&painter); // main rendering method

    // flush drawing canvas
    backingStore.endPaint();
    backingStore.flush(rect);

    // a call to renderLater() at this point will enable animation...
}

// draw stuff inside window here
void MainWindow::render(QPainter *painter)
{
    int xKor = 0;
    int yKor = 0;
    QColor farge;
    int antallNaboer;
    bool nesteCelle;
   for (int i = 0; i<40000; i++) {


        if (generasjon[i]!=false)
        farge = QColor((rand() % 255),(rand() % 255),(rand() % 255));
        else
        farge = QColor(Qt::white);

        painter->fillRect(xKor,yKor,width()/200,height()/200,farge); //cellene vil tilpasse seg vindustr.

        xKor += (width()/200);
        if (((i+1)%200)==0 && i!=0) {
            yKor += height()/200;
            xKor = 0;
        }


        nesteCelle = false;
        antallNaboer=0;

        if ((((i+1)%200)!=0 || i==0) && generasjon[i+1]==true) //høyre
            antallNaboer++;
        if (((i+1)%200)!=1 && generasjon[i-1]==true) //venstre
            antallNaboer++;
        if ((i>199) && generasjon[i-200]==true) //over
            antallNaboer++;
        if ((i<39799) && generasjon[i+200]==true) //under
            antallNaboer++;
        if (((i>199) && ((i+1)%200)!=1) && generasjon[i-199]==true) //over-venstre
            antallNaboer++;
        if ((i>199) && (((i+1)%200)!=0 || i==0) && generasjon[i-201]==true) //over-høyre
            antallNaboer++;
        if ((i<39799) && (((i+1)%200)!=1) && generasjon[i+199]==true) //under-venstre
            antallNaboer++;
        if ((i<39799) && (((i+1)%200)!=0 || i==0) && generasjon[i+201]==true) //under-høyre
            antallNaboer++;


        if (generasjon[i]==true) {
            if (antallNaboer<2 || antallNaboer>3){
                nesteCelle = false;
            }
            else if (antallNaboer==2 || antallNaboer==3){
                nesteCelle = true;
            }

        }
        else if (generasjon[i]==false && antallNaboer==3){
                nesteCelle = true;
        }

        neste[i] = nesteCelle;

    }
}

// interface to keyboad
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Q)
        QGuiApplication::quit();
}

/**************
 * The following methods are "standard" Qt methods to enable drawing
 *************/

bool MainWindow::event(QEvent *event)
{
    // draw on canvas on update request
    if (event->type() == QEvent::UpdateRequest) {
        update_pending = false;
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

// implement the "standard" Qt renderLater method
// call this method if you wish to trigger rendering of a new image
void MainWindow::renderLater()
{
    // postEvent needs a QEvent allocated on the heap.
    // it will treat this object 'properly', so this is safe
    if (!update_pending) {
        update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

// implement the "standard" Qt resize event
void MainWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    backingStore.resize(resizeEvent->size());
    if (isExposed())
        renderNow();
}

// draw canvas if window is exposed
void MainWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()) {
        renderNow();
    }
}
