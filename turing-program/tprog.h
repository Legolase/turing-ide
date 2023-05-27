#ifndef TPROG
#define TPROG

#include <string>
#include <map>
#include <vector>
#include <QtGlobal>
#include <stdexcept>
#include <istream>


struct command {
    uchar stage{0};
    uchar symbol{0};

    command() noexcept = default;
    command(uchar, uchar) noexcept;
    command(command const&) noexcept = default;

    command& operator=(command const&) noexcept = default;

    bool operator<(command const& other) const noexcept;
};

struct bad_parse : std::runtime_error {
    bad_parse(const char* issue) noexcept;
    virtual ~bad_parse() noexcept = default;
};

enum class action : uchar {
    STAY = 0,
    LEFT = 1,
    RIGHT = 2
};

std::istream& operator>>(std::istream& in, action& act) noexcept;

struct tprog {
    tprog() noexcept = default;
    tprog(std::string const& text_code);
    tprog(tprog const&);
    tprog(tprog&&) noexcept;

    tprog& operator=(tprog const& other);
    tprog& operator=(tprog&& other) noexcept;

    void swap(tprog&) noexcept;

    std::pair<int, action> make_step(uchar in) noexcept;

    std::string get_current_stage() const;

private:
    std::map<command, std::pair<command, action>> program_;
    std::vector<std::string> int2stage;
    std::map<std::string, uchar> stage2int;

    uchar stage{0};
};

#endif
