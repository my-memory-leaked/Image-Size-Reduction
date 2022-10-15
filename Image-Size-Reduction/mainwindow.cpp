#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_OpenFileButton_clicked()
{
    // Gets pull path of the chosen file
    //file_path = QFileDialog::getOpenFileName(this,
     //                                        tr("Open File"),
       //                                      "C://",
         //                                    tr("Image Files (*.png *.jpg *.bmp)"));

    // To reduce time

    file_path = QFileDialog::getOpenFileName(this,
                                             tr("Open File"),
                                             "C:/Users/pisek/Documents/Test dla apki",
                                             tr("Image Files (*.png *.jpg *.bmp)"));

    // EOF To reduce time

    // Cuts out the filename
    QString file = file_path.section("/",-1,-1);

    // Pop up after file selection
    QMessageBox::information(this,
                             "Information", "Selected file: " + file);

    ui->OpenedFileLabel->setText(file); // Change the label into filename

}

