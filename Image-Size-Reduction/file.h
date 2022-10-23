#ifndef FILE_H
#define FILE_H

#include "ui_mainwindow.h"

#include <QPixmap>
#include <QString>
class File
{
    QPixmap pixmap;



public:
    File(Ui::MainWindow *ui, const QString file_name);

    void ShowFile();


};

#endif // FILE_H
