#include "mainwindow.h"

#include <QKeyEvent>
#include <QPainter>
#include <QRectF>
#include <QTextOption>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    logs.setReadOnly(true);
    viewer.addWidget(&r, 1);
    viewer.addWidget(&status, 0);

    buttons_rib.addWidget(&lf);
    buttons_rib.addWidget(&rg);

    buttons_prog.addWidget(&run);
    buttons_prog.addWidget(&stop);
    buttons_prog.addWidget(&step);
    buttons_prog.addWidget(&upload);

    connect(&run, &QPushButton::clicked, this, &MainWindow::run_slot);
    connect(&stop, &QPushButton::clicked, this, &MainWindow::stop_slot);
    connect(&step, &QPushButton::clicked, this, &MainWindow::step_slot);
    connect(&upload, &QPushButton::clicked, this, &MainWindow::upload_slot);

    connect(&lf, &QPushButton::clicked, this, &MainWindow::left_slot);
    connect(&rg, &QPushButton::clicked, this, &MainWindow::right_slot);

    main_layer.addLayout(&viewer, 0);
    main_layer.addLayout(&buttons_rib, 0);
    main_layer.addLayout(&buttons_prog, 0);
    main_layer.addWidget(&editor);
    QWidget* w = new QWidget;
    w->setLayout(&main_layer);

    splitter.addWidget(w);
    splitter.addWidget(&logs);
    splitter.setOrientation(Qt::Vertical);

    setCentralWidget(&splitter);
    resize(700, 500);
}

void MainWindow::upload_slot()
{
    stop_slot();
    logs.clear();
    try {
        prg.set_program(editor.toPlainText().toStdString());
    } catch (bad_parse const& e) {
        logs.setText(e.what());
        return;
    }
    logs.setText("Built successed");
}

void MainWindow::stop_slot()
{
    prg.set_stage(program_stage::STOP);
    accesable = true;
}

void MainWindow::step_slot()
{
    accesable = false;
    prg.set_stage(program_stage::STEP);
}

void MainWindow::run_slot()
{
    accesable = false;
    prg.set_stage(program_stage::WORK);
}

void MainWindow::left_slot()
{
    if (!accesable) {
        return;
    }
    r.move_left();
}

void MainWindow::right_slot()
{
    if (!accesable) {
        return;
    }
    r.move_right();
}

void MainWindow::keyPressEvent(QKeyEvent *event) noexcept
{
    if (!accesable) {
        event->ignore();
        return;
    }
    if (event->text().size() > 0) {
        r.set(event->text()[0].cell());
    }
    event->accept();
}

