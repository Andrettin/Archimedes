#pragma once

namespace archimedes::exception {

extern void report(const std::exception_ptr &exception);
extern std::string to_string(const std::exception &exception);

}
