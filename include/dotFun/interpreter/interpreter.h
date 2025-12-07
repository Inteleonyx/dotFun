#ifndef DOTFUN_INTERPRETER_H
#define DOTFUN_INTERPRETER_H

#include <vector>
#include <memory>
#include <iostream>

// Inclui os cabeçalhos da AST, Lexer e auxiliares
#include "../ast/ast_base.h"
#include "../ast/stmt.h"
#include "../ast/expr.h"
#include "../lexer/token.h"
#include "./environment.h"
#include "./value.h"

namespace dotFun {

    class Interpreter : public ExprVisitor, public StmtVisitor {
      public:
        Interpreter();

        void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

      private:
        std::shared_ptr<Environment> m_globals;
        std::shared_ptr<Environment> m_environment;

        Value evaluate(Expr& expr);

        void execute(Stmt& stmt);

        void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements,
                          std::shared_ptr<Environment> environment);

        void runtimeError(const Token& token, const std::string& message);

        Value visitLiteral(Literal& expr) override;
        Value visitGrouping(Grouping& expr) override;
        Value visitUnary(Unary& expr) override;
        Value visitBinary(Binary& expr) override;
        Value visitVariable(Variable& expr) override;
        Value visitAssign(Assign& expr) override;
        Value visitLogical(Logical& expr) override;
        Value visitCall(Call& expr) override;

        void visitExpression(Expression& stmt) override;
        void visitLet(Let& stmt) override;
        void visitVal(Val& stmt) override;
        void visitGlobal(Global& stmt) override;
        void visitIf(If& stmt) override;
        void visitWhile(While& stmt) override;
        void visitFor(For& stmt) override;
        void visitReturn(Return& stmt) override;
        void visitBreak(Break& stmt) override;
        void visitContinue(Continue& stmt) override;
        void visitBlock(Block& stmt) override;
        void visitFunction(Function& stmt) override;

        // --- OUTROS VISITORS (PLACEHOLDERS/INCOMPLETOS) ---
        void visitClass(Class& stmt) override;
        void visitInterface(Interface& stmt) override;
        void visitEnum(Enum& stmt) override;
        void visitStruct(Struct& stmt) override;
        void visitImport(Import& stmt) override;
        void visitExport(Export& stmt) override;
        void visitTry(Try& stmt) override;
        void visitThrow(Throw& stmt) override;
        void visitSwitch(Switch& stmt) override;
        void visitCase(Case& stmt) override;
        void visitDefault(Default& stmt) override;

        void checkNumberOperand(const Token& op, const Value& operand);
        void checkNumberOperands(const Token& op, const Value& left, const Value& right);
        bool isTruthy(const Value& value);
    };

}

#endif
