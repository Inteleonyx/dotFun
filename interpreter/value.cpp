#include "../include/dotFun/interpreter/value.h"

#include <sstream>

namespace dotFun {
    std::string Value::toString() const {
        return std::visit([](auto&& arg) -> std::string {
                              using T = std::decay_t<decltype(arg)>;

                              if constexpr (std::is_same_v<T, std::monostate>) {
                                  return "nil";
                              } else if constexpr (std::is_same_v<T, bool>) {
                                  return arg ? "true" : "false";
                              } else if constexpr (std::is_same_v<T, double>) {
                                  std::string s = std::to_string(arg);
                                  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                                  if (s.back() == '.') s.pop_back();
                                  return s;
                              } else if constexpr (std::is_same_v<T, std::string>) {
                                  return arg;
                              } else if constexpr (std::is_convertible_v<T, std::shared_ptr<Callable>>) {
                                  if (arg) return arg->toString();
                                  return "<callable>";
                              } else if constexpr (std::is_convertible_v<T, std::shared_ptr<Instance>>) {
                                  if (arg) return "<instance>";
                                  return "nil";
                              } else {
                                  return "<?>";
                              }
                          }, m_value);
    }

    Closure::Closure(std::unique_ptr<Stmt> declaration, std::shared_ptr<Environment> closureEnvironment)
         : m_declaration(std::move(declaration)), m_closureEnvironment(closureEnvironment) {
    }

    size_t Closure::arity() const {
        return 0;
    }

    Value Closure::call(Interpreter& interpreter, const std::vector<Value>& arguments)
    {
        return Value();
    }

    std::string Closure::toString() const {
         return "<fun " + dynamic_cast<Function*>(m_declaration.get())->name + ">";
         return "<fun>";
    }
}