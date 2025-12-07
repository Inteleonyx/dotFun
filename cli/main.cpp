#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>

#include "../include/dotFun/lexer/lexer.h"
#include "../include/dotFun/parser/parser.h"
#include "../include/dotFun/interpreter/interpreter.h"

using namespace dotFun;

class DotFun {
  public:
    static bool hadError;
    static bool hadRuntimeError;

    DotFun() : interpreter(std::make_unique<Interpreter>()) {}

    void runFile(const std::string& path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cout << "dotFun: failed to open file: " << path << "\n";
            return;
        }

        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        run(source);

        if (hadError) exit(65);
        if (hadRuntimeError) exit(70);
    }

    void runPrompt() {
        std::cout << "DotFun REPL v0.1\n";
        std::string line;

        while (std::cout << ">> " && std::getline(std::cin, line)) {
            if (line.empty()) continue;
            run(line);
            hadError = false;
        }
    }

  private:
    std::unique_ptr<Interpreter> interpreter;

    void run(const std::string& source) {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        if (hadError) return;

        Parser parser(tokens);
        std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

        if (hadError) return;
        try {
            interpreter->interpret(statements);
        } catch (const std::runtime_error& e) {
            std::cerr << "Erro em tempo de execução: " << e.what() << std::endl;
            hadRuntimeError = true;
        }
    }
};

bool DotFun::hadError = false;
bool DotFun::hadRuntimeError = false;

void error(int line, const std::string& message) {
    std::cerr << "[Linha " << line << "] Erro: " << message << std::endl;
    DotFun::hadError = true;
}

void error(const Token& token, const std::string& message) {
    if (token.type == dotFun::TokenType::EOF_TOKEN) {
        error(token.line, " no final do arquivo: " + message);
    } else {
        std::cerr << "[Linha " << token.line << "] Erro em '" << token.lexeme << "': " << message << std::endl;
    }
    DotFun::hadError = true;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Uso: dotfun [caminho_do_arquivo]\n";
        return 64;
    }

    DotFun dotfun;

    if (argc == 2) {
        std::string path = argv[1];

        if (!std::filesystem::exists(path)) {
            std::cout << "dotFun: file not found: " << path << "\n";
            return 1;
        }

        dotfun.runFile(path);
    } else {
        dotfun.runPrompt();
    }

    return 0;
}