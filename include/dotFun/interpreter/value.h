#ifndef DOTFUN_VALUE_H
#define DOTFUN_VALUE_H

#include "./interpreter.h"
#include "./environment.h"
#include "../ast/stmt.h"

#include <string>
#include <variant>
#include <memory>
#include <vector>
#include <unordered_map>

namespace dotFun {

    class Callable;
    class Instance;
    class Closure;

    class Value {
      public:
        using ValueType = std::variant<
            std::monostate,
            bool,
            double,
            std::string,
            std::shared_ptr<Callable>,
            std::shared_ptr<Instance>
            >;

        Value() : m_value(std::monostate{}) {}
        Value(bool v) : m_value(v) {}
        Value(double v) : m_value(v) {}
        Value(const std::string& v) : m_value(v) {}
        Value(std::shared_ptr<Callable> v) : m_value(v) {}
        Value(std::shared_ptr<Instance> v) : m_value(v) {}

        bool isNil() const { return m_value.index() == 0; }
        bool isBool() const { return m_value.index() == 1; }
        bool isNumber() const { return m_value.index() == 2; }
        bool isString() const { return m_value.index() == 3; }
        bool isCallable() const { return m_value.index() == 4; }

        template<typename T>
        const T& as() const {
            return std::get<T>(m_value);
        }

        template<typename T>
        T& as() {
            return std::get<T>(m_value);
        }

        std::shared_ptr<Callable> asCallable() const {
            return std::get<std::shared_ptr<Callable>>(m_value);
        }

        const std::string& asString() const {
            return std::get<std::string>(m_value);
        }

        std::string toString() const;

      private:
        ValueType m_value;
    };

    class Callable {
      public:
        virtual ~Callable() = default;

        virtual size_t arity() const = 0;

        virtual Value call(Interpreter& interpreter, const std::vector<Value>& arguments) = 0;
        virtual std::string toString() const = 0;
    };

    class Closure : public Callable {
      public:
        Closure(std::unique_ptr<Stmt> declaration, std::shared_ptr<Environment> closureEnvironment);

        size_t arity() const override;
        Value call(Interpreter& interpreter, const std::vector<Value>& arguments) override;
        std::string toString() const override;

      private:
        std::unique_ptr<Stmt> m_declaration; // Stmt::Function
        std::shared_ptr<Environment> m_closureEnvironment;

    };

}

#endif