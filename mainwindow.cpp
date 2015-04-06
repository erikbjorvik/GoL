#include "mainwindow.h"

MainWindow::MainWindow(QWindow *parent) : QWindow(parent), backingStore(this)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nyGenerasjon()));
    timer->start(50); //Hver generasjon varer i 50ms.

    // set up viewing window
    create();
    setGeometry(100, 100, 600, 600); // set resolution of window

    generasjonNr = 0;
    //fyll array:
    for (int i = 0; i<40000; i++) {
        generasjon[i] = false;
    }

    generasjon[20080] = true;
    generasjon[20081] = true;
    generasjon[20082] = true;
    generasjon[20083] = true;
    generasjon[20084] = true;
    generasjon[20085] = true;
    generasjon[20086] = true;
    generasjon[20087] = true;

    generasjon[20089] = true;
    generasjon[20090] = true;
    generasjon[20091] = true;
    generasjon[20092] = true;
    generasjon[20093] = true;

    generasjon[20097] = true;
    generasjon[20098] = true;
    generasjon[20099] = true;

    generasjon[20105] = true;
    generasjon[20106] = true;
    generasjon[20107] = true;
    generasjon[20108] = true;
    generasjon[20109] = true;
    generasjon[20110] = true;
    generasjon[20111] = true;

    generasjon[20113] = true;
    generasjon[20114] = true;
    generasjon[20115] = true;
    generasjon[20116] = true;
    generasjon[20117] = true;

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
       antallNaboer=0;
       nesteCelle = false;
        if (generasjon[i]!=false)
        farge = QColor((rand() % 255),(rand() % 255),(rand() % 255));
        else
        farge = QColor(Qt::white);


        //qDebug()<<"cellevidde: " << celleVidde << "cellehoyde" << celleHoyde;

        painter->fillRect(xKor,yKor,width()/200,height()/200,farge); //cellene vil tilpasse seg vindustr.

        if (((i)%199)!=0 && i!=0 && generasjon[i+1]==true) //høyre
            antallNaboer++;
        if (((i)%199)!=1 && i!=0 && generasjon[i-1]==true) //venstre
            antallNaboer++;
        if ((i>199) && generasjon[i-200]==true) //over
            antallNaboer++;
        if ((i<39601) && generasjon[i+200]==true) //under
            antallNaboer++;
        if ((i>199) && ((i)%199)!=1 && i!=0 && generasjon[i-199]==true) //over-venstre
            antallNaboer++;
        if ((i>199) && ((i)%199)!=0 && i!=0 && generasjon[i-201]==true) //over-høyre
            antallNaboer++;
        if ((i<39799) && ((i)%199)!=1 && i!=0 && generasjon[i+199]==true) //under-venstre
            antallNaboer++;
        if ((i<39799) && ((i)%199)!=0 && i!=0 && generasjon[i+201]==true) //under-venstre
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

        xKor += (width()/200);
        if ((i%200)==0 && i!=0) {
            yKor += height()/200;
            xKor = 0;
        }

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
