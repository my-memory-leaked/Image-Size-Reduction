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
    if(!ptrBmpFile)
        delete ptrBmpFile;
}

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

    ui->OriginalPicture->setPixmap(orgPicPixmap.scaled(ui->OriginalPicture->height(), 
                                   ui->OriginalPicture->width(), Qt::KeepAspectRatio));
    ui->OriginalPicture->setMask(orgPicPixmap.mask());

}


void MainWindow::on_compressPushButton_clicked()
{
    // Compress
    if(ui->assemblerButton->isChecked() || ui->CButton->isChecked())
    {
        try
        {
            double compressionTimeStart = GetTickCount();

            ptrBmpFile = new kp::BitMap(getFilePath());

            double timeElapsed = GetTickCount() - compressionTimeStart;

            ui->compressionTimeElapsedLabel->setText(QString::number(timeElapsed) + " ms");

            QPixmap resizedPixmap;
            resizedPixmap.load(ptrBmpFile->getFileDestination().data());
            resizedPixmap.scaled(ui->ResizedPicture->height(), ui->ResizedPicture->width());

            ui->ResizedPicture->setPixmap(resizedPixmap.scaled(ui->ResizedPicture->height(),
                                          ui->ResizedPicture->width(), Qt::KeepAspectRatio));
            ui->ResizedPicture->setMask(resizedPixmap.mask());
        }
        catch (const char* errorMessage)
        {
            QMessageBox::information(this,
                "Error", errorMessage);
        }

    }
    else
    {
        QMessageBox::information(this,
                                 "Error", "Select compressing method!");
    }
}


void MainWindow::on_threadNumVerticalSlider_valueChanged(int value)
{
    ui->threadNumberLabel->setText(QString::number(value));
}

