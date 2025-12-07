#ifndef DOTFUN_ENVIRONMENT_H
#define DOTFUN_ENVIRONMENT_H

#include <string>
#include <memory>
#include <unordered_map>
#include "../lexer/token.h"
#include "./value.h"

namespace dotFun {
    class Environment : public std::enable_shared_from_this<Environment> {
      public:
        Environment();

        Environment(std::shared_ptr<Environment> enclosing);

        void define(const std::string& name, Value value);

        Value get(const Token& name);

        void assign(const Token& name, Value value);

        Value getAt(int distance, const std::string& name);

        void assignAt(int distance, const Token& name, Value value);

      private:
        std::shared_ptr<Environment> m_enclosing;

        std::unordered_map<std::string, Value> m_values;

        std::shared_ptr<Environment> ancestor(int distance);
    };

}

#endif