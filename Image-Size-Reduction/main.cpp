#include "mainwindow.h"


#include <QApplication>
#include <QSize>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static QSize WINDOW_SIZE(WINDOW_WIDTH, WINDOW_HEIGHT);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(WINDOW_SIZE);
    w.show();
    return QApplication::exec();
}
