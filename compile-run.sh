#
# Programming Language with LLVM
#
# Course info: http://dmitrysoshnikov.com/courses/programming-language-with-llvm/
#
# (C) 2023-present Dmitry Soshnikov <dmitry.soshnikov@gmail.com>
#

# Compile main:
clang++ -o eva-llvm `llvm-config --cxxflags --ldflags --system-libs --libs core` eva-llvm.cpp -fexceptions

# Run main:
./eva-llvm

# Execute generated IR:
# lli ./out.ll

# Optimize the output:
opt ./out.ll -O3 -S -o ./out-opt.ll

# Compile ./out.ll with GC:
#
# Note: to install GC_malloc:
#
#   brew install libgc
#
clang++ -O3 -I/usr/local/include/gc/ ./out-opt.ll /usr/local/lib/libgc.a -o ./out

# Run the compiled program:
./out

# Print result:
echo $?

printf "\n"
