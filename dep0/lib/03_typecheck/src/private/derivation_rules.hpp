#pragma once

#include "dep0/typecheck/ast.hpp"
#include "dep0/typecheck/derivation.hpp"

namespace dep0::typecheck {

struct derivation_rules
{
    // At the moment there is no requirements to make any derivation; but could add some, if useful.
    template <typename T>
    static derivation_t<T> make_derivation()
    {
        return derivation_t<T>{};
    }
};

template <typename... Args>
module_t make_legal_module(Args&&... args)
{
    return module_t{derivation_rules::make_derivation<module_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
type_def_t make_legal_type_def(Args&&... args)
{
    return type_def_t{derivation_rules::make_derivation<type_def_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
func_def_t make_legal_func_def(Args&&... args)
{
    return func_def_t{derivation_rules::make_derivation<func_def_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
func_arg_t make_legal_func_arg(Args&&... args)
{
    return func_arg_t{derivation_rules::make_derivation<func_arg_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
type_t make_legal_type(Args&&... args)
{
    return type_t{derivation_rules::make_derivation<type_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
body_t make_legal_body(Args&&... args)
{
    return body_t{derivation_rules::make_derivation<body_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
stmt_t make_legal_stmt(Args&&... args)
{
    return stmt_t{derivation_rules::make_derivation<stmt_t>(), std::forward<Args>(args)...};
}

template <typename... Args>
expr_t make_legal_expr(sort_t sort, Args&&... args)
{
    return expr_t{derivation_rules::make_derivation<expr_t>(), std::move(sort), std::forward<Args>(args)...};
}

} // namespace dep0::typecheck
