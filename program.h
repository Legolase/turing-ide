#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <QLabel>
#include <optional>
#include "ribbon.h"
#include "turing-program/tprog.h"

enum class program_stage : uchar {
    WORK,
    STEP,
    PAUSE,
    STOP,
    EXIT
};

class program : public QObject
{
    Q_OBJECT
public:
    explicit program(QObject *parent, ribbon* r, QLabel* lb, std::atomic<bool>&);

    ~program() noexcept;

    void set_program(std::string const&);
    void set_stage(program_stage) noexcept;

    void thread_main();

private:
    std::atomic<program_stage> stage_token{program_stage::STOP};
    std::condition_variable program_updated;

    std::mutex m;
    std::optional<tprog> prog{std::nullopt};
    bool restart{false};

    ribbon* rib;
    QLabel* label;
    std::atomic<bool>& free_ribbon;

    std::thread worker;
};

#endif // PROGRAM_H
