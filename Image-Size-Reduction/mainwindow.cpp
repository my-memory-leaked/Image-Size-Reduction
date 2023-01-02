#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->threadNumVerticalSlider->setSliderPosition(std::thread::hardware_concurrency());
}


MainWindow::~MainWindow()
{
    delete ui;
    if(!ptr_bmp_file)
        delete ptr_bmp_file;
}



// TODO clean this code below
void MainWindow::on_OpenFileButton_clicked()
{
    // Gets pull path of the chosen file
    filePath = QFileDialog::getOpenFileName(this,
                                             tr("Open File"),
                                             "../Image-Size-Reduction/Images",
                                             tr("Image Files (*.bmp)"));
    // Pop up after file selection
    if(!filePath.isEmpty())
        QMessageBox::information(this,
                                 "Information", "Selected file: " + filePath);

    // Cuts out the filename
    fileName = filePath.section("/",-1,-1);

    ui->OpenedFileLabel->setText(fileName); // Change the label into filename

    QPixmap orgPicPixmap;
    orgPicPixmap.load(filePath);
    orgPicPixmap.scaled(ui->OriginalPicture->height(), ui->OriginalPicture->width());

    ui->OriginalPicture->setPixmap(orgPicPixmap.scaled(ui->OriginalPicture->height(), ui->OriginalPicture->width(), Qt::KeepAspectRatio));
    ui->OriginalPicture->setMask(orgPicPixmap.mask());

}


void MainWindow::on_compressPushButton_clicked()
{
    double compressionTimeStart = GetTickCount();
    ptr_bmp_file = new kp::BitMap(getFilePath()); // old bitmap
    double timeElapsed = GetTickCount() - compressionTimeStart;
    ui->compressionTimeElapsedLabel->setText(QString::number(timeElapsed) + " ms");

    QPixmap resizedPixmap;
    resizedPixmap.load(ptr_bmp_file->getFileDestination().data());
    resizedPixmap.scaled(ui->ResizedPicture->height(), ui->ResizedPicture->width());

    ui->ResizedPicture->setPixmap(resizedPixmap.scaled(ui->ResizedPicture->height(), ui->ResizedPicture->width(), Qt::KeepAspectRatio));
    ui->ResizedPicture->setMask(resizedPixmap.mask());
}


void MainWindow::on_threadNumVerticalSlider_valueChanged(int value)
{
    ui->threadNumberLabel->setText(QString::number(value));
}

