#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushOpenButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open image"), "", tr("Images (*.png *.xpm *.jpg)"));
}
