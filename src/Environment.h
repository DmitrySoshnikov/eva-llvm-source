/**
 * Programming Language with LLVM
 *
 * Course info:
 * http://dmitrysoshnikov.com/courses/programming-language-with-llvm/
 *
 * (C) 2023-present Dmitry Soshnikov <dmitry.soshnikov@gmail.com>
 */

/**
 * Environment class (aka Symbol Table).
 */

#ifndef Environment_h
#define Environment_h

#include <map>
#include <memory>
#include <string>

#include "./Logger.h"
#include "llvm/IR/Value.h"

/**
 * Environment: names storage.
 */
class Environment : public std::enable_shared_from_this<Environment> {
 public:
  /**
   * Creates an environment with the given record.
   */
  Environment(std::map<std::string, llvm::Value*> record,
              std::shared_ptr<Environment> parent)
      : record_(record), parent_(parent) {}

  /**
   * Creates a variable with the given name and value.
   */
  llvm::Value* define(const std::string& name, llvm::Value* value) {
    // Implement here...
  }

  /**
   * Returns the value of a defined variable, or throws
   * if the variable is not defined.
   */
  llvm::Value* lookup(const std::string& name) {
    return resolve(name)->record_[name];
  }

 private:
  /**
   * Returns specific environment in which a variable is defined, or
   * throws if a variable is not defined.
   */
  std::shared_ptr<Environment> resolve(const std::string& name) {
    // Implement here...
  }

  /**
   * Bindings storage
   */
  std::map<std::string, llvm::Value*> record_;

  /**
   * Parent link
   */
  std::shared_ptr<Environment> parent_;
};

#endif
