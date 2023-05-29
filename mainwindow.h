#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QIcon>
#include <QPixmap>
#include <QSlider>
#include "program.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


private slots:
    void upload_slot();
    void pause_slot();
    void stop_slot();
    void step_slot();
    void run_slot();

    void left_slot();
    void right_slot();

    void move_speed(int);
    void set_label_speed();
protected:
    void keyPressEvent(QKeyEvent *event) noexcept override;

private:
    program prg{this, &r, &status, accesable};

    std::atomic<bool> accesable{true};
    ribbon r{};
    QLabel status;

    QSlider speed_slider{Qt::Horizontal};
    QLabel speed;


    QTextEdit editor;
    QTextEdit logs;
    QVBoxLayout main_layer{};
    QHBoxLayout viewer{};
    QHBoxLayout buttons_rib{};

    QAction* load;
    QAction* run;
    QAction* stop;
    QAction* pause;
    QAction* step;

    QPushButton lf{"<"};
    QPushButton rg{">"};
};
#endif // MAINWINDOW_H
