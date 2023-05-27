#include "ribbon.h"

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QRectF>

QFont ribbon::font{"Consolas", 16};

ribbon::ribbon(QWidget *parent)
    : QWidget{parent}
{
    setFixedHeight(HEIGHT);
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = '_';
    }
}

uchar ribbon::get() const noexcept
{
    return arr[cursor];
}

void ribbon::move_right() noexcept
{
    if (++cursor >= SIZE) {
        cursor = 0;
    }
    update();
}

void ribbon::move_left() noexcept
{
    if (--cursor < 0) {
        cursor = SIZE - 1;
    }
    update();
}

void ribbon::set(uchar value) noexcept
{
    arr[cursor] = value;
    update();
}

void ribbon::paintEvent(QPaintEvent *e) noexcept
{
    const int width = this->width();
    const int height = HEIGHT;
    const int number_cells = width / BATCH;
    const int begin = (width % BATCH) / 2;
    const int half_width = number_cells / 2;

    QPainter p(this);
    p.setPen(QPen(Qt::black));
    p.setBrush(QBrush(Qt::black));
    p.setFont(font);
    p.drawRect(0, 0, width, height);
    p.setPen(QPen(Qt::white));
    p.setBrush(QBrush(Qt::white));


    for (int i = 0; i < number_cells; ++i) {
        p.setPen(QPen(Qt::white));
        p.drawRect(begin + (BATCH * i) + 1, 1, BATCH - 2, BATCH - 2);
        p.setPen(QPen(Qt::black));
        p.drawText(
            QRectF{static_cast<qreal>(begin + (BATCH * i) + 2), 2, BATCH - 4, BATCH - 4},
            QString(QChar{at(-half_width + i)}),
            QTextOption{Qt::AlignHCenter | Qt::AlignVCenter}
        );
        if (-half_width + i == 0) {
            p.setPen(QPen(Qt::green));
            p.setBrush(QBrush(Qt::green));
            p.drawRect(begin + (BATCH * i)+1, BATCH, BATCH-2, UNDERLINE);
            p.setPen(QPen(Qt::white));
            p.setBrush(QBrush(Qt::white));
        }
    }
}

uchar ribbon::at(int index) noexcept
{
    index = (cursor + index) % SIZE;
    if (index < 0) {
        index += SIZE;
    }
    return arr[index];
}
