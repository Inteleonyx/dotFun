#include "../include/dotFun/interpreter/environment.h"
#include <stdexcept>
#include <iostream>

namespace dotFun {
    static void runtimeError(const Token& token, const std::string& message) {
        std::cerr << "[Line " << token.line << ", Column " << token.column
                  << "] Runtime error: " << message << std::endl;

        throw std::runtime_error("Runtime Error occurred.");
    }

    Environment::Environment() : m_enclosing(nullptr) {}

    Environment::Environment(std::shared_ptr<Environment> enclosing)
        : m_enclosing(enclosing) {}

    void Environment::define(const std::string& name, Value value) {
        m_values[name] = value;
    }

    std::shared_ptr<Environment> Environment::ancestor(int distance) {
        std::shared_ptr<Environment> environment = shared_from_this();
        for (int i = 0; i < distance; ++i) {
            if (!environment->m_enclosing) {
                throw std::runtime_error("Internal error: Access to non-existent ancestral scope.");
            }
            environment = environment->m_enclosing;
        }
        return environment;
    }

    Value Environment::getAt(int distance, const std::string& name) {
        std::shared_ptr<Environment> targetEnv = ancestor(distance);

        auto it = targetEnv->m_values.find(name);
        if (it != targetEnv->m_values.end()) {
            return it->second;
        }

        runtimeError({dotFun::TokenType::IDENTIFIER, name, name, 0, 0},
                     "Internal error: Variable '" + name + "' not found within the target scope.");
        return Value();
    }

    void Environment::assignAt(int distance, const Token& name, Value value) {
        std::shared_ptr<Environment> targetEnv = ancestor(distance);

        auto it = targetEnv->m_values.find(name.lexeme);
        if (it != targetEnv->m_values.end()) {
            it->second = value;
            return;
        }

        runtimeError(name, "Internal error: Attempt to assign to a variable not found in the target scope.");
    }

    Value Environment::get(const Token& name) {
        auto it = m_values.find(name.lexeme);
        if (it != m_values.end()) {
            return it->second;
        }

        if (m_enclosing != nullptr) {
            return m_enclosing->get(name);
        }

        runtimeError(name, "Variable '" + name.lexeme + "' has not been defined.");
        return Value();
    }


    void Environment::assign(const Token& name, Value value) {
        auto it = m_values.find(name.lexeme);
        if (it != m_values.end()) {
            it->second = value;
            return;
        }

        if (m_enclosing != nullptr) {
            m_enclosing->assign(name, value);
            return;
        }

        runtimeError(name, "Cannot assign value: Variable '" + name.lexeme + "' has not been defined.");
    }

}