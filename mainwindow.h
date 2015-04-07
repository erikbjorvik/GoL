#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QDebug>
#include <cstdio>

class MainWindow : public QWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0); // do not allow implicit conversions
    ~MainWindow() {}

    void render(QPainter *painter);
    QTimer *timer;
    int hoyde;
    int vidde;
    int generasjonNr;
    bool generasjon[40000];
    bool neste[40000];

    const char* filnavn;
    void lesFil(const char* const filnavn);


public slots:
    void renderLater();
    void renderNow();
    void nyGenerasjon();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* ev);

     void contextMenuEvent(QContextMenuEvent *event);

private:
    QBackingStore backingStore; // drawing area
    bool        update_pending;
};


#endif // MAINWINDOW_H
