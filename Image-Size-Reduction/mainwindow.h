#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "globals.h"
#include "RLECompression.h"

#include "bmp.h"

#include "file.h"   // should be removed



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString file_path;
    QString file_name;
    QFile file;

    BitMap* ptr_bmp_file; // bmp file to compress

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::string getFilePath() { return file_path.toStdString(); }

private slots:

    void on_OpenFileButton_clicked();

    void on_compressPushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
