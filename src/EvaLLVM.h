/**
 * Programming Language with LLVM
 *
 * Course info:
 * http://dmitrysoshnikov.com/courses/programming-language-with-llvm/
 *
 * (C) 2023-present Dmitry Soshnikov <dmitry.soshnikov@gmail.com>
 */

/**
 * Eva to LLVR IR compiler.
 */

#ifndef EvaLLVM_h
#define EvaLLVM_h

#include <iostream>
#include <map>
#include <regex>
#include <string>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include "./Environment.h"
#include "./Logger.h"
#include "./parser/EvaParser.h"

using syntax::EvaParser;

/**
 * Environment type.
 */
using Env = std::shared_ptr<Environment>;

/**
 * Class info. Contains struct type and field names.
 */
struct ClassInfo {
  llvm::StructType* cls;
  llvm::StructType* parent;
  std::map<std::string, llvm::Type*> fieldsMap;
  std::map<std::string, llvm::Function*> methodsMap;
};

/**
 * Index of the vTable in the class fields.
 */
static const size_t VTABLE_INDEX = 0;

/**
 * Each class has set of reserved fields at the
 * beginning of its layout. Currently it's only
 * the vTable used to resolve methods.
 */
static const size_t RESERVED_FIELDS_COUNT = 1;

// Generic binary operator:
#define GEN_BINARY_OP(Op, varName)         \
  do {                                     \
    auto op1 = gen(exp.list[1], env);      \
    auto op2 = gen(exp.list[2], env);      \
    return builder->Op(op1, op2, varName); \
  } while (false)

class EvaLLVM {
 public:
  EvaLLVM() : parser(std::make_unique<EvaParser>()) {
    moduleInit();
    setupExternFunctions();
    setupGlobalEnvironment();
    setupTargetTriple();
  }

  /**
   * Executes a program.
   */
  void exec(const std::string& program) {
    // 1. Parse the program
    auto ast = parser->parse("(begin " + program + ")");

    // 2. Compile to LLVM IR:
    compile(ast);

    // Print generated code.
    module->print(llvm::outs(), nullptr);

    std::cout << "\n";

    // 3. Save module IR to file:
    saveModuleToFile("./out.ll");
  }

 private:
  /**
   * Compiles an expression.
   */
  void compile(const Exp& ast) {
    // 1. Create main function:
    fn = createFunction(
        "main",
        llvm::FunctionType::get(/* return type */ builder->getInt32Ty(),
                                /* vararg */ false),
        GlobalEnv);

    createGlobalVar("VERSION", builder->getInt32(42));

    // 2. Compile main body:
    gen(ast, GlobalEnv);

    builder->CreateRet(builder->getInt32(0));
  }

  /**
   * Main compile loop.
   */
  llvm::Value* gen(const Exp& exp, Env env) {
    switch (exp.type) {
      /**
       * ----------------------------------------------
       * Numbers.
       */
      case ExpType::NUMBER:
        // Implement here...

      /**
       * ----------------------------------------------
       * Strings.
       */
      case ExpType::STRING: {
        // Implement here...
      }

      /**
       * ----------------------------------------------
       * Symbols (variables, operators).
       */
      case ExpType::SYMBOL:
        /**
         * Boolean.
         */
        if (exp.string == "true" || exp.string == "false") {
          // Implement here...
        } else {
          // Variables and functions:

          // Implement here...
        }

      /**
       * ----------------------------------------------
       * Lists.
       */
      case ExpType::LIST:
        auto tag = exp.list[0];

        /**
         * ----------------------------------------------
         * Special cases.
         */
        if (tag.type == ExpType::SYMBOL) {
          auto op = tag.string;

          // --------------------------------------------
          // Binary math operations:

          if (op == "+") {
            // Implement here...
          }

          else if (op == "-") {
            // Implement here...
          }

          else if (op == "*") {
            // Implement here...
          }

          else if (op == "/") {
            // Implement here...
          }

          // --------------------------------------------
          // Compare operations: (> 5 10)

          // UGT - unsigned, greater than
          else if (op == ">") {
            // Implement here...
          }

          // ULT - unsigned, less than
          else if (op == "<") {
            // Implement here...
          }

          // EQ - equal
          else if (op == "==") {
            // Implement here...
          }

          // NE - not equal
          else if (op == "!=") {
            // Implement here...
          }

          // UGE - greater or equal
          else if (op == ">=") {
            // Implement here...
          }

          // ULE - less or equal
          else if (op == "<=") {
            // Implement here...
          }

          // --------------------------------------------
          // Branch instruction:

          /**
           * (if <cond> <then> <else>)
           */
          else if (op == "if") {
            // Compile <cond>:
            auto cond = gen(exp.list[1], env);

            // Implement here...
          }

          // --------------------------------------------
          // While loop:

          /**
           * (while <cond> <body>)
           *
           */
          else if (op == "while") {
            // Implement here...
          }

          // --------------------------------------------
          // Function declaration: (def <name> <params> <body>)
          //

          else if (op == "def") {
            return compileFunction(exp, /* name */ exp.list[1].string, env);
          }

          // --------------------------------------------
          // Variable declaration: (var x (+ y 10))
          //
          // Typed: (var (x number) 42)
          //
          // Note: locals are allocated on the stack.

          if (op == "var") {
            // Implement here...
          }

          // --------------------------------------------
          // Variable update: (set x 100)
          // Property update (set (prop self x) 100)

          else if (op == "set") {
            // Implement here...
          }

          // --------------------------------------------
          // Blocks: (begin <expressions>)

          else if (op == "begin") {
            // Block scope:
            auto blockEnv = std::make_shared<Environment>(
                std::map<std::string, llvm::Value*>{}, env);

            // Compile each expression within the block.
            // Result is the last evaluated expression.
            llvm::Value* blockRes;

            for (auto i = 1; i < exp.list.size(); i++) {
              // Generate expression code.
              blockRes = gen(exp.list[i], blockEnv);
            }

            return blockRes;
          }

          // --------------------------------------------
          // printf extern function:
          //
          // (printf "Value: %d" 42)
          //

          else if (op == "printf") {
            // Implement here...
          }

          // --------------------------------------------
          // Class declaration:
          //
          // (class A <super> <body>)
          //

          else if (op == "class") {
            // Implement here...
          }

          // --------------------------------------------
          // New operator:
          //
          // (new <class> <args>)
          //

          else if (op == "new") {
            return createInstance(exp, env, "");
          }

          // --------------------------------------------
          // Prop access:
          //
          // (prop <instance> <name>)
          //

          else if (op == "prop") {
            // Implement here...
          }

          // --------------------------------------------
          // Method access:
          //
          // (method <instance> <name>)
          //
          // (method (super <class>) <name>)
          //

          else if (op == "method") {
            // Implement here...
          }

          // --------------------------------------------
          // Function calls:
          //
          // (square 2)

          else {
            auto callable = gen(exp.list[0], env);

            // Implement here...
          }
        }

        // --------------------------------------------
        // Method calls:
        //
        // ((method p getX) p 2)

        else {
          auto loadedMethod = (llvm::LoadInst*)gen(exp.list[0], env);

          auto fnTy = (llvm::FunctionType*)(loadedMethod->getPointerOperand()
                                                ->getType()
                                                ->getContainedType(0)
                                                ->getContainedType(0));

          // Implement here...
        }

        break;
    }

    // Unreachable:
    return builder->getInt32(0);
  }

  /**
   * Returns field index.
   */
  size_t getFieldIndex(llvm::StructType* cls, const std::string& fieldName) {
    auto fields = &classMap_[cls->getName().data()].fieldsMap;
    auto it = fields->find(fieldName);
    return std::distance(fields->begin(), it) + RESERVED_FIELDS_COUNT;
  }

  /**
   * Returns method index.
   */
  size_t getMethodIndex(llvm::StructType* cls, const std::string& methodName) {
    // Implement here...
  }

  /**
   * Creates an instance of a class.
   */
  llvm::Value* createInstance(const Exp& exp, Env env,
                              const std::string& name) {
    // Implement here...
  }

  /**
   * Allocates an object of a given class on the heap.
   */
  llvm::Value* mallocInstance(llvm::StructType* cls, const std::string& name) {
    // Implement here...
  }

  /**
   * Returns size of a type in bytes.
   */
  size_t getTypeSize(llvm::Type* type_) {
    return module->getDataLayout().getTypeAllocSize(type_);
  }

  /**
   * Inherits parent class fields.
   */
  void inheritClass(llvm::StructType* cls, llvm::StructType* parent) {
    // Implement here...
  }

  /**
   * Extracts fields and methods from a class expression.
   */
  void buildClassInfo(llvm::StructType* cls, const Exp& clsExp, Env env) {
    // Implement here...
  }

  /**
   * Builds class body from class info.
   */
  void buildClassBody(llvm::StructType* cls) {
    // Implement here...
  }

  /**
   * Creates a vtable per class.
   *
   * vTable stores method references to support
   * inheritance and methods overloading.
   */
  void buildVTable(llvm::StructType* cls) {
    // Implement here...
  }

  /**
   * Tagged lists.
   */
  bool isTaggedList(const Exp& exp, const std::string& tag) {
    return exp.type == ExpType::LIST && exp.list[0].type == ExpType::SYMBOL &&
           exp.list[0].string == tag;
  }

  /**
   * (var ...)
   */
  bool isVar(const Exp& exp) { return isTaggedList(exp, "var"); }

  /**
   * (def ...)
   */
  bool isDef(const Exp& exp) { return isTaggedList(exp, "def"); }

  /**
   * (new ...)
   */
  bool isNew(const Exp& exp) { return isTaggedList(exp, "new"); }

  /**
   * (prop ...)
   */
  bool isProp(const Exp& exp) { return isTaggedList(exp, "prop"); }

  /**
   * (super ...)
   */
  bool isSuper(const Exp& exp) { return isTaggedList(exp, "super"); }

  /**
   * Returns a type by name.
   */
  llvm::StructType* getClassByName(const std::string& name) {
    return llvm::StructType::getTypeByName(*ctx, name);
  }

  /**
   * Extracts var or parameter name considering type.
   *
   * x -> x
   * (x number) -> x
   */
  std::string extractVarName(const Exp& exp) {
    return exp.type == ExpType::LIST ? exp.list[0].string : exp.string;
  }

  /**
   * Extracts var or parameter type with i32 as default.
   *
   * x -> i32
   * (x number) -> number
   */
  llvm::Type* extractVarType(const Exp& exp) {
    return exp.type == ExpType::LIST ? getTypeFromString(exp.list[1].string)
                                     : builder->getInt32Ty();
  }

  /**
   * Returns LLVM type from string representation.
   */
  llvm::Type* getTypeFromString(const std::string& type_) {
    // number -> i32
    if (type_ == "number") {
      return builder->getInt32Ty();
    }

    // string -> i8* (aka char*)
    if (type_ == "string") {
      return builder->getInt8Ty()->getPointerTo();
    }

    // Classes:
    return classMap_[type_].cls->getPointerTo();
  }

  /**
   * Whether function has return type defined.
   */
  bool hasReturnType(const Exp& fnExp) {
    return fnExp.list[3].type == ExpType::SYMBOL &&
           fnExp.list[3].string == "->";
  }

  /**
   * Exp function to LLVM function params.
   *
   * (def square ((number x)) -> number ...)
   *
   * llvm::FunctionType::get(returnType, paramTypes, false);
   */
  llvm::FunctionType* extractFunctionType(const Exp& fnExp) {
    auto params = fnExp.list[2];

    // Return type:
    auto returnType = hasReturnType(fnExp)
                          ? getTypeFromString(fnExp.list[4].string)
                          : builder->getInt32Ty();

    // Parameter types:
    std::vector<llvm::Type*> paramTypes{};

    for (auto& param : params.list) {
      auto paramName = extractVarName(param);
      auto paramTy = extractVarType(param);

      // The `self` name is special, meaning instance of a class:
      paramTypes.push_back(
          paramName == "self" ? (llvm::Type*)cls->getPointerTo() : paramTy);
    }

    return llvm::FunctionType::get(returnType, paramTypes, /* varargs */ false);
  }

  /**
   * Compiles a function.
   *
   * Untyped: (def square (x) (* x x)) - i32 by default
   *
   * Typed: (def square ((x number)) -> number (* x x))
   */
  llvm::Value* compileFunction(const Exp& fnExp, std::string fnName, Env env) {
    // Implement here...
  }

  /**
   * Allocates a local variable on the stack. Result is the alloca instruction.
   */
  llvm::Value* allocVar(const std::string& name, llvm::Type* type_, Env env) {
    varsBuilder->SetInsertPoint(&fn->getEntryBlock());

    auto varAlloc = varsBuilder->CreateAlloca(type_, 0, name.c_str());

    // Add to the environment:
    env->define(name, varAlloc);

    return varAlloc;
  }

  /**
   * Creates a global variable.
   */
  llvm::GlobalVariable* createGlobalVar(const std::string& name,
                                        llvm::Constant* init) {
    // Implement here...
  }

  /**
   * Define external functions (from libc++)
   */
  void setupExternFunctions() {
    // Implement here...
  }

  /**
   * Creates a function.
   */
  llvm::Function* createFunction(const std::string& fnName,
                                 llvm::FunctionType* fnType, Env env) {
    // Implement here...
  }

  /**
   * Creates function prototype (defines the function, but not the body)
   */
  llvm::Function* createFunctionProto(const std::string& fnName,
                                      llvm::FunctionType* fnType, Env env) {
    auto fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage,
                                     fnName, *module);
    verifyFunction(*fn);

    // Install in the environment:
    env->define(fnName, fn);

    return fn;
  }

  /**
   * Creates function block.
   */
  void createFunctionBlock(llvm::Function* fn) {
    // Implement here...
  }

  /**
   * Creates a basic block. If the `fn` is passed, the block is
   * automatically appended to the parent function. Otherwise,
   * the block should later be appended manually via
   * fn->getBasicBlockList().push_back(block);
   */
  llvm::BasicBlock* createBB(std::string name, llvm::Function* fn = nullptr) {
    return llvm::BasicBlock::Create(*ctx, name, fn);
  }

  /**
   * Saves IR to file.
   */
  void saveModuleToFile(const std::string& fileName) {
    std::error_code errorCode;
    llvm::raw_fd_ostream outLL(fileName, errorCode);
    module->print(outLL, nullptr);
  }

  /**
   * Initialize the module.
   */
  void moduleInit() {
    // Open a new context and module.
    ctx = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("EvaLLVM", *ctx);

    // Create a new builder for the module.
    builder = std::make_unique<llvm::IRBuilder<>>(*ctx);

    // Vars builder:
    varsBuilder = std::make_unique<llvm::IRBuilder<>>(*ctx);
  }

  /**
   * Sets up The Global Environment.
   */
  void setupGlobalEnvironment() {
    // Implement here...
  }

  /**
   * Sets up target triple.
   */
  void setupTargetTriple() {
    // llvm::sys::getDefaultTargetTriple()
    // Implement here...
  }

  /**
   * Parser.
   */
  std::unique_ptr<EvaParser> parser;

  /**
   * Global Environment (symbol table).
   */
  std::shared_ptr<Environment> GlobalEnv;

  /**
   * Currently compiling class.
   */
  llvm::StructType* cls = nullptr;

  /**
   * Class info.
   */
  std::map<std::string, ClassInfo> classMap_;

  /**
   * Currently compiling function.
   */
  llvm::Function* fn;

  /**
   * Global LLVM context.
   * It owns and manages the core "global" data of LLVM's core
   * infrastructure, including the type and constant unique tables.
   */
  std::unique_ptr<llvm::LLVMContext> ctx;

  /**
   * A Module instance is used to store all the information related to an
   * LLVM module. Modules are the top level container of all other LLVM
   * Intermediate Representation (IR) objects. Each module directly contains a
   * list of globals variables, a list of functions, a list of libraries (or
   * other modules) this module depends on, a symbol table, and various data
   * about the target's characteristics.
   *
   * A module maintains a GlobalList object that is used to hold all
   * constant references to global variables in the module.  When a global
   * variable is destroyed, it should have no entries in the GlobalList.
   * The main container class for the LLVM Intermediate Representation.
   */
  std::unique_ptr<llvm::Module> module;

  /**
   * Extra builder for variables declaration.
   * This builder always prepends to the beginning of the
   * function entry block.
   */
  std::unique_ptr<llvm::IRBuilder<>> varsBuilder;

  /**
   * IR Builder.
   *
   * This provides a uniform API for creating instructions and inserting
   * them into a basic block: either at the end of a BasicBlock, or at a
   * specific iterator location in a block.
   */
  std::unique_ptr<llvm::IRBuilder<>> builder;
};

#endif