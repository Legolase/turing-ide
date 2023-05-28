#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
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

protected:
    void keyPressEvent(QKeyEvent *event) noexcept override;

private:
    program prg{this, &r, &status, accesable};

    std::atomic<bool> accesable{true};
    ribbon r{};
    QLabel status;


    QTextEdit editor;
    QTextEdit logs;
    QVBoxLayout main_layer{};
    QHBoxLayout viewer{};
    QHBoxLayout buttons_rib{};
    QHBoxLayout buttons_prog{};

    QPushButton load{"Load"};
    QPushButton run{"Run"};
    QPushButton stop{"Stop"};
    QPushButton pause{"Pause"};
    QPushButton step{"Step"};

    QPushButton lf{"<"};
    QPushButton rg{">"};
};
#endif // MAINWINDOW_H
