#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QDebug>

class MainWindow : public QWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0); // do not allow implicit conversions
    ~MainWindow() {}

    void render(QPainter *painter);
    QTimer *timer;
    std::vector< std::vector< int > > celleListe; //4d-vektor.
    std::vector< std::vector< int > > neste; //for neste generasjon.
    int hoyde;
    int vidde;
    int generasjon;

public slots:
    void renderLater();
    void renderNow();
    void nyGenerasjon();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* ev);

private:
    QBackingStore backingStore; // drawing area
    bool        update_pending;
};


#endif // MAINWINDOW_H
