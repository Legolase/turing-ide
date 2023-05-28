#include "program.h"
#include <QString>

program::program(QObject *parent, ribbon* r, QLabel* lb, std::atomic<bool>& fr)
    : QObject{parent}, rib(r), label(lb), free_ribbon{fr}
{
    worker = std::thread([this]{
        thread_main();
    });
}

program::~program() noexcept
{
    set_stage(program_stage::EXIT);
    worker.join();
}

void program::set_program(const std::string & code)
{
    std::lock_guard lk(m);
    prog = tprog(code);
    set_stage(program_stage::PAUSE);
}

void program::set_stage(program_stage stg) noexcept
{
    if (stage_token == program_stage::EXIT) {
        return;
    } else if (stg == program_stage::STOP) {
        std::lock_guard lk(m);
        restart = true;
    }
    if (stg == program_stage::STOP ||
        stg == program_stage::PAUSE ||
        stg == program_stage::EXIT) {
        free_ribbon = true;
    }
    stage_token = stg;
    program_updated.notify_one();
}

void program::thread_main()
{
    tprog local_prog;
    std::pair<int, action> result;
    while(true) {
        {
            std::unique_lock ulk(m);
            program_updated.wait(ulk, [this](){ return stage_token != program_stage::PAUSE && stage_token != program_stage::STOP; });
            if (stage_token == program_stage::EXIT) {
                return;
            }
            if (prog.has_value()) {
                local_prog = std::move(*prog);
                prog = std::nullopt;
            }
            if (restart) {
                restart = false;
                local_prog.restart();
            }
        }

        result = local_prog.make_step(rib->get());
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        if (result.first == -1) {
            set_stage(program_stage::STOP);
            continue;
        }
        rib->set(static_cast<uchar>(result.first));
        label->setText(QString(local_prog.get_current_stage().c_str()));
        if (result.second == action::LEFT) {
            rib->move_left();
        } else if (result.second == action::RIGHT) {
            rib->move_right();
        }

        if (stage_token == program_stage::STEP) {
            set_stage(program_stage::PAUSE);
        }
    }
}
