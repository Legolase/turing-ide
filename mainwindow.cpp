#include "mainwindow.h"

#include <QKeyEvent>
#include <QPainter>
#include <QRectF>
#include <QTextOption>
#include <QTime>
#include <QToolBar>
#include <QSplitter>

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

    set_label_speed();
    speed.setMinimumWidth(50);
    speed.setFont(QFont{"Consolas", 15});
    speed_slider.setRange(0, 1000);
    speed_slider.setSliderPosition(prg.speed);
//===
    logs.setReadOnly(true);

    status.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    status.setMinimumWidth(50);
    viewer.addWidget(&r, 1);
    viewer.addWidget(&status, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    buttons_rib.addWidget(&lf, 0);
    buttons_rib.addWidget(&rg, 0);
    buttons_rib.addWidget(&speed_slider, 1);
    buttons_rib.addWidget(&speed, 0);

    QToolBar* bar = addToolBar("ToolBar");

    run = bar->addAction(QIcon(QPixmap(":/image/source/Run.png")), "Run");
    stop = bar->addAction(QIcon(QPixmap(":/image/source/Stop.png")), "Stop");
    bar->addSeparator();
    pause = bar->addAction(QIcon(QPixmap(":/image/source/Pause.png")), "Pause");
    step = bar->addAction(QIcon(QPixmap(":/image/source/Step.png")), "Step");
    bar->addSeparator();
    load = bar->addAction(QIcon(QPixmap(":/image/source/Load.png")), "Load");

    connect(run, &QAction::triggered, this, &MainWindow::run_slot);
    connect(pause, &QAction::triggered, this, &MainWindow::pause_slot);
    connect(stop, &QAction::triggered, this, &MainWindow::stop_slot);
    connect(step, &QAction::triggered, this, &MainWindow::step_slot);
    connect(load, &QAction::triggered, this, &MainWindow::upload_slot);

    connect(&lf, &QPushButton::clicked, this, &MainWindow::left_slot);
    connect(&rg, &QPushButton::clicked, this, &MainWindow::right_slot);

//    connect(&speed_slider, &QSlider::valueChanged, &speed,
//            static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
    connect(&speed_slider, &QSlider::valueChanged, this, &MainWindow::set_label_speed);
    connect(&speed_slider, &QSlider::valueChanged, this, &MainWindow::move_speed);

    QSplitter* splitter = new QSplitter{};
    splitter->setOrientation(Qt::Vertical);

    main_layer.addLayout(&viewer, 0);
    main_layer.addLayout(&buttons_rib, 0);
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

void MainWindow::move_speed(int val)
{
    prg.speed = val;
}

void MainWindow::set_label_speed()
{
    static const QString ping("Ping: ");
    speed.setText(ping + QString::number(speed_slider.value()));
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
