#ifndef RIBBON_H
#define RIBBON_H

#include <QWidget>
#include <QtGlobal>
#include <QFont>
#include <array>
#include <mutex>

class ribbon : public QWidget
{
    Q_OBJECT
    static constexpr int BATCH = 40;
    static constexpr int UNDERLINE = 5;
    static constexpr int HEIGHT = BATCH + UNDERLINE;
    static constexpr int SIZE = 300;
    static QFont font;
    using element_t = uchar;
    using storage_t = std::array<element_t, SIZE>;

public:
    explicit ribbon(QWidget *parent = nullptr);


    uchar get() const noexcept;
public slots:
    void move_right() noexcept;
    void move_left() noexcept;
    void set(uchar) noexcept;

protected:
    void paintEvent(QPaintEvent* e) noexcept override;

signals:
private:
    uchar at(int index) noexcept;

    mutable std::mutex m;
    storage_t arr{};
    int cursor{0};
};

#endif // RIBBON_H
