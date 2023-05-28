#include "tprog.h"
#include <sstream>

std::istream& operator>>(std::istream& in, action& act) noexcept {
    uchar i;
    in >> i;
    if (i == '>') {
        act = action::RIGHT;
    } else if (i == '<') {
        act = action::LEFT;
    } else if (i == '^') {
        act = action::STAY;
    }
    return in;
}

command::command(uchar st, uchar s) noexcept : stage(st), symbol(s)
{

}

bool command::operator<(const command &other) const noexcept {
    if (stage != other.stage) {
        return stage < other.stage;
    }
    return symbol < other.symbol;
}

tprog::tprog(std::string const& text_code)
{
    std::stringstream in_code(text_code);
    std::string word;
    action act;
    command from(0, 0), to(0,0);

    // Set start stage
    in_code >> word;
    if (word != "start:") {
        throw bad_parse("Program must set start stage");
    }
    in_code >> word;
    stage2int[word] = 0;
    int2stage.push_back(std::move(word));

    while (in_code >> word) {
        if (stage2int.find(word) == stage2int.end()) {
            stage2int[word] = int2stage.size();
            int2stage.push_back(word);
        }
        from.stage = stage2int[word];
        in_code >> from.symbol;

        in_code >> word;
        if (word != "->") {
            throw bad_parse("command must have translate token \"->\" ");
        }

        in_code >> word;
        if (stage2int.find(word) == stage2int.end()) {
            stage2int[word] = int2stage.size();
            int2stage.push_back(word);
        }
        to.stage = stage2int[word];
        in_code >> to.symbol >> act;

        program_[from] = {to, act};
    }
}

tprog::tprog(const tprog & other) :
    program_{other.program_},
    int2stage{other.int2stage},
    stage2int{other.stage2int},
    stage{other.stage}
{

}

tprog::tprog(tprog && other) noexcept :
    program_{std::move(other.program_)},
    int2stage{std::move(other.int2stage)},
    stage2int{std::move(other.stage2int)},
    stage{other.stage}
{

}

tprog &tprog::operator=(const tprog &other)
{
    if (this != &other) {
        tprog(other).swap(*this);
    }
    return *this;
}

tprog &tprog::operator=(tprog&&other) noexcept
{
    if (this != &other) {
        tprog(std::move(other)).swap(*this);
    }
    return *this;
}

void tprog::swap(tprog & other) noexcept
{
    using std::swap;
    swap(program_, other.program_);
    swap(int2stage, other.int2stage);
    swap(stage2int, other.stage2int);
    swap(stage, other.stage);
}

std::pair<int, action> tprog::make_step(uchar in) noexcept
{
    command comm{stage, in};
    auto it = program_.find(comm);
    if (it == program_.end()) {
        stage = 0;
        return {-1, action::STAY};
    }
    auto result = it->second;
    stage = result.first.stage;
    return {result.first.symbol, result.second};
}

std::string tprog::get_current_stage() const
{
    return int2stage[stage];
}

void tprog::restart() noexcept
{
    stage = 0;
}

bad_parse::bad_parse(const char *issue) noexcept : std::runtime_error(issue)
{

}
