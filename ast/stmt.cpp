#include "../include/dotFun/ast/stmt.h"

#include <optional>

namespace dotfun {

    // Let
    Let::Let(std::string name, std::optional<std::string> type, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), type(std::move(type)), initializer(std::move(initializer)) {}

    void Let::accept(StmtVisitor& visitor) {
        visitor.visitLet(*this);
    }

    // Val
    Val::Val(std::string name, std::optional<std::string> type, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), type(std::move(type)), initializer(std::move(initializer)) {}

    void Val::accept(StmtVisitor& visitor) {
        visitor.visitVal(*this);
    }

    // Global
    Global::Global(std::string name, std::optional<std::string> type, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), type(std::move(type)), initializer(std::move(initializer)) {}

    void Global::accept(StmtVisitor& visitor) {
        visitor.visitGlobal(*this);
    }

    // Expression
    Expression::Expression(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    void Expression::accept(StmtVisitor& visitor) {
        visitor.visitExpression(*this);
    }

    // If
    If::If(std::unique_ptr<Expr> condition,
           std::unique_ptr<Stmt> thenBranch,
           std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void If::accept(StmtVisitor& visitor) {
        visitor.visitIf(*this);
    }

    // While
    While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    void While::accept(StmtVisitor& visitor) {
        visitor.visitWhile(*this);
    }

    // For
    For::For(std::unique_ptr<Stmt> initializer,
             std::unique_ptr<Expr> condition,
             std::unique_ptr<Expr> increment,
             std::unique_ptr<Stmt> body)
        : initializer(std::move(initializer)),
          condition(std::move(condition)),
          increment(std::move(increment)),
          body(std::move(body)) {}

    void For::accept(StmtVisitor& visitor) {
        visitor.visitFor(*this);
    }

    // Break
    void Break::accept(StmtVisitor& visitor) {
        visitor.visitBreak(*this);
    }

    // Continue
    void Continue::accept(StmtVisitor& visitor) {
        visitor.visitContinue(*this);
    }

    // Function
    Function::Function(std::string name,
                       bool isAsync,
                       std::vector<std::pair<std::string, std::optional<std::string>>> params,
                       std::optional<std::string> returnType,
                       std::vector<std::unique_ptr<Stmt>> body)
        : name(std::move(name)),
          isAsync(isAsync),
          params(std::move(params)),
          returnType(std::move(returnType)),
          body(std::move(body)) {}

    void Function::accept(StmtVisitor& visitor) {
        visitor.visitFunction(*this);
    }

    // Return
    Return::Return(std::unique_ptr<Expr> value)
        : value(std::move(value)) {}

    void Return::accept(StmtVisitor& visitor) {
        visitor.visitReturn(*this);
    }

    // Class
    Class::Class(std::string name,
                 std::optional<std::string> superClass,
                 AccessModifier accessModifier,
                 std::vector<std::unique_ptr<Stmt>> members)
        : name(std::move(name)),
          superClass(std::move(superClass)),
          accessModifier(accessModifier),
          members(std::move(members)) {}

    void Class::accept(StmtVisitor& visitor) {
        visitor.visitClass(*this);
    }

    // Interface
    Interface::Interface(std::string name,
                         std::vector<std::string> superInterfaces,
                         AccessModifier accessModifier,
                         std::vector<std::unique_ptr<Stmt>> members)
        : name(std::move(name)),
          superInterfaces(std::move(superInterfaces)),
          accessModifier(accessModifier),
          members(std::move(members)) {}

    void Interface::accept(StmtVisitor& visitor) {
        visitor.visitInterface(*this);
    }

    Enum::Enum(std::string name,
               AccessModifier accessModifier,
               std::vector<std::string> values)
        : name(std::move(name)),
          accessModifier(accessModifier),
          values(std::move(values)) {}

    void Enum::accept(StmtVisitor& visitor) {
        visitor.visitEnum(*this);
    }

    Struct::Struct(std::string name,
                   AccessModifier accessModifier,
                   std::vector<std::unique_ptr<Stmt>> members)
        : name(std::move(name)),
          accessModifier(accessModifier),
          members(std::move(members)) {}

    void Struct::accept(StmtVisitor& visitor) {
        visitor.visitStruct(*this);
    }

    Switch::Switch(std::unique_ptr<Expr> expression, std::vector<std::unique_ptr<Stmt>> cases)
        : expression(std::move(expression)), cases(std::move(cases)) {}

    void Switch::accept(StmtVisitor& visitor) {
        visitor.visitSwitch(*this);
    }

    Case::Case(std::unique_ptr<Expr> value, std::vector<std::unique_ptr<Stmt>> body)
        : value(std::move(value)), body(std::move(body)) {}

    void Case::accept(StmtVisitor& visitor) {
        visitor.visitCase(*this);
    }

    Default::Default(std::vector<std::unique_ptr<Stmt>> body)
        : body(std::move(body)) {}

    void Default::accept(StmtVisitor& visitor) {
        visitor.visitDefault(*this);
    }

    Import::Import(std::string moduleName)
        : moduleName(std::move(moduleName)) {}

    void Import::accept(StmtVisitor& visitor) {
        visitor.visitImport(*this);
    }

    Export::Export(std::string symbolName)
        : symbolName(std::move(symbolName)) {}

    void Export::accept(StmtVisitor& visitor) {
        visitor.visitExport(*this);
    }

    Try::Try(std::unique_ptr<Stmt> tryBlk, std::unique_ptr<Stmt> catchBlk, std::unique_ptr<Stmt> finallyBlk)
        : tryBlock(std::move(tryBlk)), catchBlock(std::move(catchBlk)), finallyBlock(std::move(finallyBlk)) {}

    void Try::accept(StmtVisitor& visitor) {
        visitor.visitTry(*this);
    }

    Throw::Throw(std::unique_ptr<Expr> exception)
        : exception(std::move(exception)) {}

    void Throw::accept(StmtVisitor& visitor) {
        visitor.visitThrow(*this);
    }

}
