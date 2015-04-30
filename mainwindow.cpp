#include "mainwindow.h"
#include <QMenuBar>
#include <fstream>
#include <QMessageBox>
#include "generasjon.h"


MainWindow::MainWindow(QWindow *parent) : QWindow(parent), backingStore(this)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nyGenerasjon())); //når timer signal: kall nyGenerasjon.
    timer->start(50); //Hver generasjon varer i 50ms.

    // set up viewing window
    create();
    setGeometry(100, 100, 600, 600); // setter vindustørrelse

    g->initGen();
    g->lesFil("/Users/ErikBjorvik/untitled1/gosper.txt"); //Import i Life 1.05

}

void MainWindow::nyGenerasjon() {
    g->initGen();
    MainWindow::renderLater();
    g->gNrPlussEn();
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

// Her tegnes det som skal tegnes.
void MainWindow::render(QPainter *painter)
{
    int xKor = 0;
    int yKor = 0;
    QColor farge;
    int antallNaboer;
    bool nesteCelle;
    for (int i = 0; i<40000; i++) {

    if (g->erLevende(i))
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
        antallNaboer = g->finnAntallNaboer(i);
        g->setNeste(i, g->bestemSkjebne(i));

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
