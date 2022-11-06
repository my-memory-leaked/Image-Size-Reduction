#include "file.h"

#include <QDialog>
#include <QSize>
#include <QWidget>
#include <QLabel>

//#include "CppRLEEncode.h"


File::File(Ui::MainWindow *ui, const QString file_name)
{
    pixmap.load(file_name);
    //QLabel* lbl = new QLabel(this);
        /** set content to show center in label */



    //QSize size = qApp->screens()[0]->size();
    QWidget widget;

    QSize size = widget.frameSize();


    ui->OriginalPicture->setPixmap(pixmap);
    ui->OriginalPicture->setMask(pixmap.mask());


}

void File::ShowFile()

{

}
