#ifndef DOTFUN_AST_BASE_H
#define DOTFUN_AST_BASE_H

namespace dotfun {
    struct Expr;
    struct Stmt;

    struct ExprVisitor {
        virtual void visitBinary(class Binary& expr) = 0;
        virtual void visitLiteral(class Literal& expr) = 0;
        virtual void visitVariable(class Variable& expr) = 0;
        virtual void visitUnary(class Unary& expr) = 0;
        virtual void visitCall(class Call& expr) = 0;

        virtual void visitAssign(class Assign& expr) = 0;
        virtual void visitLogical(class Logical& expr) = 0;
        virtual void visitGrouping(class Grouping& expr) = 0;

        virtual ~ExprVisitor() = default;
    };

    struct StmtVisitor {
        virtual void visitLet(class Let& stmt) = 0;
        virtual void visitVal(class Val& stmt) = 0;
        virtual void visitGlobal(class Global& stmt) = 0;
        virtual void visitExpression(class Expression& stmt) = 0;
        virtual void visitIf(class If& stmt) = 0;
        virtual void visitWhile(class While& stmt) = 0;
        virtual void visitFunction(class Function& stmt) = 0;
        virtual void visitReturn(class Return& stmt) = 0;

        virtual void visitClass(class Class& stmt) = 0;
        virtual void visitInterface(class Interface& stmt) = 0;
        virtual void visitEnum(class Enum& stmt) = 0;
        virtual void visitStruct(class Struct& stmt) = 0;

        virtual void visitFor(class For& stmt) = 0;
        virtual void visitBreak(class Break& stmt) = 0;
        virtual void visitContinue(class Continue& stmt) = 0;

        virtual void visitImport(class Import& stmt) = 0;
        virtual void visitExport(class Export& stmt) = 0;
        virtual void visitSwitch(class Switch& stmt) = 0;
        virtual void visitCase(class Case& stmt) = 0;
        virtual void visitDefault(class Default& stmt) = 0;
        virtual void visitTry(class Try& stmt) = 0;
        virtual void visitThrow(class Throw& stmt) = 0;

        virtual ~StmtVisitor() = default;
    };

    struct Expr {
        virtual ~Expr() = default;
        virtual void accept(ExprVisitor& visitor) = 0;
    };

    struct Stmt {
        virtual ~Stmt() = default;
        virtual void accept(StmtVisitor& visitor) = 0;
    };
}

#endif
