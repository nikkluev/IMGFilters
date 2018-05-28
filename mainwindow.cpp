#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "filters.h"
#include <ctime>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    label = new QLabel("Hello!", this);
    label->setGeometry(10, 10, 150, 20);
    label->show();
}

MainWindow::~MainWindow()
{
    if(src_img != nullptr)
        delete src_img;

    if(result != nullptr)
        delete result;

    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushOpenButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open image"), "", tr("Images (*.png *.xpm *.jpg)"));
    src_img = new QImage(path, nullptr);

    if(src_img->isNull())
    {
        (new QErrorMessage(this))->showMessage("Can't open file");
        return;
    }

    result = new QImage(*src_img);
}

void MainWindow::on_pushButtonShGr_clicked()
{
    if(src_img == nullptr)
    {
        (new QErrorMessage(this))->showMessage("File is not opened!");
        return;
    }

    time_t start, finish;

    if(isMthr)
    {
        start = clock();
        toShadesGrayMthr(src_img, result, red, green, blue);
        finish = clock();
    }
    else
    {
        start = clock();
        toShadesGraySthr(src_img, result, red, green, blue);
        finish = clock();
    }
    drawImg(result);

    label->setText(QString("%1 ms").arg(finish - start));
    label->show();
}

void MainWindow::on_doubleSpinBoxR_valueChanged(double arg1)
{
    red = arg1;
}

void MainWindow::on_doubleSpinBoxG_valueChanged(double arg1)
{
    green = arg1;
}

void MainWindow::on_doubleSpinBoxB_valueChanged(double arg1)
{
    blue = arg1;
}

void MainWindow::drawImg(QImage *img)
{
    if(view != nullptr)
    {
        delete view;
    }

    scene.clear();
    scene.addPixmap(QPixmap::fromImage(*img));
    view = new QGraphicsView(&scene);
    view->show();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    gaussRadius = arg1;
}

void MainWindow::on_gaussBlurButton_clicked()
{
    if(src_img == nullptr)
    {
        (new QErrorMessage(this))->showMessage("File is not opened!");
        return;
    }

    if(gaussRadius == 0)
    {
        (new QErrorMessage(this))->showMessage("Set radius");
        return;
    }

    time_t start, finish;

    if(isMthr)
    {
        start = clock();
        gaussBlurMthr(src_img, result, gaussRadius);
        finish = clock();
    }
    else
    {
        start = clock();
        gaussBlurSthr(src_img, result, gaussRadius);
        finish = clock();
    }
    drawImg(result);

    label->setText(QString("%1 ms").arg(finish - start));
    label->show();
}

void MainWindow::on_radioMThrButton_clicked(bool checked)
{
    isMthr = checked;
}

void MainWindow::on_radioSThrButton_clicked(bool checked)
{
    isMthr = !checked;
}

void MainWindow::on_pushSaveButton_clicked()
{
    if(result == nullptr)
    {
        (new QErrorMessage(this))->showMessage("No result");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Images (*.png *.xpm *.jpg)"));

    if(!result->save(fileName))
    {
        (new QErrorMessage(this))->showMessage("Can't save file");
        return;
    }

    label->setText(QString("File has been saved"));
    label->show();
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    horizontal = checked;
}

void MainWindow::on_radioButton_2_clicked(bool checked)
{
    horizontal = !checked;
}

void MainWindow::on_spinMBlurBox_valueChanged(int arg1)
{
    motionRadius = arg1;
}

void MainWindow::on_pushMotionButton_clicked()
{
    if(src_img == nullptr)
    {
        (new QErrorMessage(this))->showMessage("File is not opened!");
        return;
    }

    if(motionRadius == 0)
    {
        (new QErrorMessage(this))->showMessage("Set radius");
        return;
    }

    time_t start, finish;

    if(isMthr)
    {
        start = clock();
        motionBlurMthr(src_img, result, motionRadius, horizontal);
        finish = clock();
    }
    else
    {
        start = clock();
        motionBlurSthr(src_img, result, motionRadius, horizontal);
        finish = clock();
    }
    drawImg(result);

    label->setText(QString("%1 ms").arg(finish - start));
    label->show();
}
