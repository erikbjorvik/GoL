#include "mainwindow.h"

MainWindow::MainWindow(QWindow *parent) : QWindow(parent), backingStore(this)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nyGenerasjon()));
    timer->start(5000); //Hver generasjon varer i 50ms.

    // set up viewing window
    create();
    setGeometry(100, 100, 600, 600); // set resolution of window

    generasjon = 0;
    //fyll array:
    int linje = 0;
    std::vector<int> nyLinje;
    for (int i = 0; i<40000; i++) {

        nyLinje.push_back((rand()%2));
        nyLinje.push_back(i%200);

        if ((i%200)==0 && i!=0) {
            linje++;
        }

        nyLinje.push_back(linje);
        celleListe.push_back(nyLinje);
    }

}

/*id  -  tilstand  -  plass_x  - plass_y
  0          1          +1        +0
  1          0          +2        +0
  3          0          +3        +3
*/

void MainWindow::nyGenerasjon() {
    qDebug() << "Ny generasjon";
    /*QPaintDevice *device = backingStore.paintDevice();
    QPainter painter(device);
    render(&painter);*/

    renderLater();

    if (generasjon!=0) {
        celleListe.clear();
        celleListe = neste;
    }

    generasjon++;
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
    int celleVidde;
    int celleHoyde;
    int xKor = 0;
    int yKor = 0;
    QColor farge;
    int antallNaboer = 0;
    std::vector<int> nesteGLinje;
   for (int i = 0; i<40000; i++) {

       if (i==0)
           nesteGLinje.clear();


        if (celleListe.at(i).at(0)!=0)
        farge = QColor((rand() % 255),(rand() % 255),(rand() % 255));
        else
        farge = QColor(Qt::white);

        celleVidde = width()/200;
        celleHoyde = height()/200;

        //qDebug()<<"cellevidde: " << celleVidde << "cellehoyde" << celleHoyde;

        painter->fillRect(xKor,yKor,celleVidde,celleHoyde,farge); //cellene vil tilpasse seg vindustr.

        //for neste generasjon:



        if (((i)%199)!=0 && celleListe.at(i+1).at(0)==1) //høyre
            antallNaboer++;
        if (((i)%199)!=1 && i!=0 && celleListe.at(i-1).at(0)==1) //venstre
            antallNaboer++;
        if ((i>199) && celleListe.at(i-199).at(0)==1) //over
            antallNaboer++;
        if ((i<39601) && celleListe.at(i+199).at(0)==1) //under
            antallNaboer++;

        if (celleListe.at(i).at(0)==1) {
            if (antallNaboer<2 || antallNaboer>3)
               nesteGLinje.push_back(0);
            else if (antallNaboer==2 || antallNaboer==3)
                nesteGLinje.push_back(1);

        }
        else if (celleListe.at(i).at(0)==0 && antallNaboer==3)
            nesteGLinje.push_back(1);

        neste.push_back(nesteGLinje);

        xKor += celleVidde;
        if ((i%200)==0 && i!=0) {
            yKor += celleHoyde;
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
