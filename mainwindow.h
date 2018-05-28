#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDockWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QErrorMessage>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //exit button
    void on_exitButton_clicked();
    //open image button
    void on_pushOpenButton_clicked();
    //turning image in shades of gray
    void on_pushButtonShGr_clicked();
    //changing red color coefficient
    void on_doubleSpinBoxR_valueChanged(double arg1);
    //changing green color coefficient
    void on_doubleSpinBoxG_valueChanged(double arg1);
    //changing blue color coefficient
    void on_doubleSpinBoxB_valueChanged(double arg1);
    //changing radius of gaussian blur
    void on_spinBox_valueChanged(int arg1);
    //gaussian blurring
    void on_gaussBlurButton_clicked();
    //turning on multithreading
    void on_radioMThrButton_clicked(bool checked);
    //turning off multithreading
    void on_radioSThrButton_clicked(bool checked);
    //saving image
    void on_pushSaveButton_clicked();
    //horizontal motion blurring
    void on_radioButton_clicked(bool checked);
    //vertical motion blurring
    void on_radioButton_2_clicked(bool checked);
    //changing radius of motion blurring
    void on_spinMBlurBox_valueChanged(int arg1);
    //motion blurring
    void on_pushMotionButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel* label;

    QImage* src_img{nullptr};
    QImage* result{nullptr};

    double red{0};
    double green{0};
    double blue{0};
    bool isMthr{true};
    int gaussRadius{0};
    int motionRadius{0};
    bool horizontal{true};

    //drawing image
    void drawImg(QImage* img);

    QGraphicsView* view{nullptr};
    QGraphicsScene scene;

};

#endif // MAINWINDOW_H
