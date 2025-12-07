cmake_minimum_required(VERSION 4.0)
project(dotFun)

set(CMAKE_CXX_STANDARD 20)

add_executable(dotFun cli/main.cpp
        include/dotFun/lexer/tokens.h
        include/dotFun/lexer/lexer.h
        include/dotFun/lexer/token.h
        lexer/lexer.cpp
        include/dotFun/ast/ast_base.h
        include/dotFun/ast/stmt.h
        include/dotFun/ast/expr.h
        ast/stmt.cpp
        ast/expr.cpp
        include/dotFun/parser/parser.h
        parser/parser.cpp
        include/dotFun/interpreter/interpreter.h
        include/dotFun/interpreter/environment.h
        include/dotFun/interpreter/value.h
        interpreter/value.cpp
        interpreter/environment.cpp
        interpreter/interpreter.cpp.old)
