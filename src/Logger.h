/**
 * Programming Language with LLVM
 *
 * Course info:
 * http://dmitrysoshnikov.com/courses/programming-language-with-llvm/
 *
 * (C) 2023-present Dmitry Soshnikov <dmitry.soshnikov@gmail.com>
 */

/**
 * Logger and error reporter.
 */

#ifndef Logger_h
#define Logger_h

#include <iostream>
#include <sstream>

class ErrorLogMessage : public std::basic_ostringstream<char> {
 public:
  ~ErrorLogMessage() {
    std::cerr << "Fatal error: " << str().c_str();
    exit(EXIT_FAILURE);
  }
};

#define DIE ErrorLogMessage()

#endif
