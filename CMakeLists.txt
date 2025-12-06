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
        include/dotFun/ast/expr.h)
