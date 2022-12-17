#ifndef FILE_H
#define FILE_H

#include "ui_mainwindow.h"

#include <QPixmap>


class File
{
    QPixmap pixmap;

public:
    File(const Ui::MainWindow *ui, QString file_name);

    void ShowFile();
};

#endif // FILE_H
