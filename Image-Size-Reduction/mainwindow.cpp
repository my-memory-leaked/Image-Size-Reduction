#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    // Pop up after file selection
    QMessageBox::information(this,
                             "Information", "Selected file: " + file_path);

    // Cuts out the filename
    file_name = file_path.section("/",-1,-1);

    ui->OpenedFileLabel->setText(file_name); // Change the label into filename

    // Wszystko wyżej dobrze

    QPixmap pixmap;


   pixmap.load(file_path);
   //QLabel* lbl = new QLabel(this);
       /** set content to show center in label */

   //QSize size = qApp->screens()[0]->size();
    const QWidget widget;

    pixmap.scaled(ui->OriginalPicture->height(), ui->OriginalPicture->width());


   //label->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));

   ui->OriginalPicture->setPixmap(pixmap.scaled(ui->OriginalPicture->height(), ui->OriginalPicture->width(), Qt::KeepAspectRatio));
   ui->OriginalPicture->setMask(pixmap.mask());
}


void MainWindow::on_compressPushButton_clicked()
{
    ptr_bmp_file = new kp::BitMap(getFilePath()); // old bitmap

    QPixmap resizedPixmap;
    resizedPixmap.load(ptr_bmp_file->getFileDestination().data());

    QWidget resizedWidget;

    resizedPixmap.scaled(ui->ResizedPicture->height(), ui->ResizedPicture->width());

    ui->ResizedPicture->setPixmap(resizedPixmap.scaled(ui->ResizedPicture->height(), ui->ResizedPicture->width(), Qt::KeepAspectRatio));
    ui->ResizedPicture->setMask(resizedPixmap.mask());
}