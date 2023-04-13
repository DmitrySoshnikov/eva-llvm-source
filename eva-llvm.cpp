/**
 * Programming Language with LLVM
 *
 * Course info:
 * http://dmitrysoshnikov.com/courses/programming-language-with-llvm/
 *
 * (C) 2023-present Dmitry Soshnikov <dmitry.soshnikov@gmail.com>
 */

/**
 * Eva LLVM executable.
 */

#include <fstream>
#include <iostream>
#include <string>

#include "./src/EvaLLVM.h"

void printHelp() {
  std::cout << "\nUsage: eva-llvm [options]\n\n"
            << "Options:\n"
            << "    -e, --expression  Expression to parse\n"
            << "    -f, --file        File to parse\n\n";
}

int main(int argc, char const *argv[]) {
  if (argc != 3) {
    printHelp();
    return 0;
  }

  /**
   * Expression mode.
   */
  std::string mode = argv[1];

  /**
   * Program to execute.
   */
  std::string program;

  /**
   * Simple expression.
   */
  if (mode == "-e") {
    program = argv[2];
  }

  /**
   * Eva file.
   */
  else if (mode == "-f") {
    // Read the file:
    std::ifstream programFile(argv[2]);
    std::stringstream buffer;
    buffer << programFile.rdbuf() << "\n";

    // Program:
    program = buffer.str();
  }

  /**
   * Compiler instance.
   */
  EvaLLVM vm;

  /**
   * Generate LLVM IR.
   */
  vm.exec(program);

  return 0;
}