#include "mainwindow.h"

#include <QKeyEvent>
#include <QPainter>
#include <QRectF>
#include <QTextOption>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    editor.setFont(QFont("Consolas"));
    logs.setFont(QFont("Consolas"));
    QFont f = status.font();
    f.setPointSize(14);
    status.setFont(f);

    QPalette p = editor.palette();
    p.setColor(QPalette::Base, QColor(70, 70, 70));
    p.setColor(QPalette::Text, Qt::white);
    editor.setPalette(p);

    p = logs.palette();
    p.setColor(QPalette::Base, QColor(30, 30, 30));
    p.setColor(QPalette::Text, Qt::white);
    logs.setPalette(p);
//===
    logs.setReadOnly(true);

    status.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    status.setMinimumWidth(50);
    viewer.addWidget(&r, 1);
    viewer.addWidget(&status, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    buttons_rib.addWidget(&lf);
    buttons_rib.addWidget(&rg);

    buttons_prog.addWidget(&run);
    buttons_prog.addWidget(&stop);
    buttons_prog.addWidget(&pause);
    buttons_prog.addWidget(&step);
    buttons_prog.addWidget(&load);

    connect(&run, &QPushButton::clicked, this, &MainWindow::run_slot);
    connect(&pause, &QPushButton::clicked, this, &MainWindow::pause_slot);
    connect(&stop, &QPushButton::clicked, this, &MainWindow::stop_slot);
    connect(&step, &QPushButton::clicked, this, &MainWindow::step_slot);
    connect(&load, &QPushButton::clicked, this, &MainWindow::upload_slot);

    connect(&lf, &QPushButton::clicked, this, &MainWindow::left_slot);
    connect(&rg, &QPushButton::clicked, this, &MainWindow::right_slot);

    QSplitter* splitter = new QSplitter{};
    splitter->setOrientation(Qt::Vertical);

    main_layer.addLayout(&viewer, 0);
    main_layer.addLayout(&buttons_rib, 0);
    main_layer.addLayout(&buttons_prog, 0);
    main_layer.addWidget(splitter, 1);

    splitter->addWidget(&editor);
    splitter->addWidget(&logs);
    main_layer.setContentsMargins(0,0,0,0);

    QWidget* main_widget = new QWidget();
    main_widget->setLayout(&main_layer);
    setCentralWidget(main_widget);
}

void MainWindow::upload_slot()
{
    pause_slot();
    try {
        prg.set_program(editor.toPlainText().toStdString());
    } catch (bad_parse const& e) {
        logs.append(QTime::currentTime().toString("hh:mm:ss $ ") + "Error: " + QString(e.what()));
        return;
    }
    logs.append(QTime::currentTime().toString("hh:mm:ss $ ") + "Built successed");
}

void MainWindow::pause_slot()
{
    prg.set_stage(program_stage::PAUSE);
    accesable = true;
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
