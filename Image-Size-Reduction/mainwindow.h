#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <ui_mainwindow.h>

#include "globals.h"
#include "RLECompression.h"
#include "bmp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString filePath;
    QString fileName;
    QFile file;

    kp::BitMap* ptr_bmp_file; // bmp file to compress

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    [[nodiscard]] std::string getFilePath() const { return filePath.toStdString(); }

private slots:

    void on_OpenFileButton_clicked();

    void on_compressPushButton_clicked();


    void on_threadNumVerticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
