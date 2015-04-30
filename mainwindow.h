#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QDebug>
#include <cstdio>

class generasjon;

class MainWindow : public QWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0); // do not allow implicit conversions
    ~MainWindow() {}

    void render(QPainter *painter);
    QTimer *timer;
    generasjon *g;

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
