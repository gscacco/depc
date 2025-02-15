#include "dep0/parser/parse.hpp"

#include "dep0/antlr4/DepCLexer.h"
#include "dep0/antlr4/DepCParser.h"
#include "dep0/antlr4/DepCParserVisitor.h"

#include "dep0/fmap.hpp"
#include "dep0/mmap.hpp"

#include <antlr4-runtime/antlr4-runtime.h>

#include <algorithm>
#include <optional>

namespace dep0::parser {

std::optional<source_text> get_text(source_text const src, antlr4::Token const& token)
{
    std::optional<source_text> res;
    if (token.getStartIndex() != INVALID_INDEX and token.getStopIndex() != INVALID_INDEX)
        res.emplace(src.substr(token.getStartIndex(), token.getStopIndex() + 1 - token.getStartIndex()));
    return res;
}

std::optional<source_text> get_text(source_text const src, antlr4::ParserRuleContext const& ctx)
{
    std::optional<source_text> res;
    if (ctx.getStart()->getTokenIndex() <= ctx.getStop()->getTokenIndex())
        res.emplace(src.substr(
            ctx.getStart()->getStartIndex(),
            ctx.getStop()->getStopIndex() + 1 - ctx.getStart()->getStartIndex()));
    return res;
}

std::optional<source_loc_t> get_loc(source_text const src, antlr4::Token const& token)
{
    std::optional<source_loc_t> res;
    if (auto txt = get_text(src, token))
        res.emplace(token.getLine(), 1+token.getCharPositionInLine(), std::move(*txt));
    return res;
}

std::optional<source_loc_t> get_loc(source_text const src, antlr4::ParserRuleContext const& ctx)
{
    std::optional<source_loc_t> res;
    if (auto txt = get_text(src, ctx))
        res.emplace(ctx.getStart()->getLine(), 1+ctx.getStart()->getCharPositionInLine(), std::move(*txt));
    return res;
}

struct parse_visitor_t : dep0::DepCParserVisitor
{
    source_text const src;

    explicit parse_visitor_t(source_text const src) :
        src(src)
    { }

    virtual std::any visitModule(DepCParser::ModuleContext* ctx) override
    {
        assert(ctx);
        return module_t{
            get_loc(src, *ctx).value(),
            fmap(ctx->typeDef(), [this] (auto* x) { return std::any_cast<type_def_t>(visitTypeDef(x)); }),
            fmap(ctx->funcDef(), [this] (auto* x) { return std::any_cast<func_def_t>(visitFuncDef(x)); })
        };
    }

    virtual std::any visitTypeDef(DepCParser::TypeDefContext* ctx) override
    {
        assert(ctx);
        auto const loc = get_loc(src, *ctx).value();
        auto const name = get_text(src, *ctx->name).value();
        auto const sign = get_text(src, *ctx->sign).value();
        auto const width = get_text(src, *ctx->width).value();
        std::optional<source_text> min, max;
        // if we don't have 2 ellipsis, there's at least one of min (which could be 0) and max
        if (ctx->ELLIPSIS().size() < 2ul)
        {
            if (ctx->min)
                min = get_text(src, *ctx->min).value();
            else if (ctx->zero)
                min = get_text(src, *ctx->zero).value();
            if (ctx->max)
                max = get_text(src, *ctx->max).value();
        }
        auto const w =
            width == "8" ? ast::width_t::_8 :
            width == "16" ? ast::width_t::_16 :
            width == "32" ? ast::width_t::_32 :
            ast::width_t::_64;
        if (sign == "unsigned")
        {
            if (not min or min != "0")
                throw error_t{"lower bound of unsigned integer must be 0", loc};
            return type_def_t{loc, type_def_t::integer_t{name, ast::sign_t::unsigned_v, w, max}};
        }
        else if (min.has_value() xor max.has_value())
            throw error_t{"lower and upper bound of signed integer must be either both present or both missing", loc};
        else if (min)
        {
            if (min != max)
                throw error_t{"lower and upper bound of signed integer must have same absolute value", loc};
            return type_def_t{loc, type_def_t::integer_t{name, ast::sign_t::signed_v, w, max}};
        }
        else
            return type_def_t{loc, type_def_t::integer_t{name, ast::sign_t::signed_v, w, std::nullopt}};
    }

    virtual std::any visitFuncDef(DepCParser::FuncDefContext* ctx) override
    {
        assert(ctx);
        assert(ctx->type());
        assert(ctx->name);
        assert(ctx->body());
        return func_def_t{
            get_loc(src, *ctx).value(),
            get_text(src, *ctx->name).value(),
            expr_t::abs_t{
                fmap(ctx->funcArg(), [this](auto *x) { return std::any_cast<func_arg_t>(visitFuncArg(x)); }),
                std::any_cast<type_t>(visitType(ctx->type())),
                std::any_cast<body_t>(visitBody(ctx->body()))
            }};
    }

    virtual std::any visitType(DepCParser::TypeContext* ctx) override
    {
        assert(ctx);
        if (ctx->primitiveType()) return visitPrimitiveType(ctx->primitiveType());
        if (ctx->funcType()) return visitFuncType(ctx->funcType());
        if (ctx->typeVar()) return visitTypeVar(ctx->typeVar());
        throw error_t{"unexpected alternative when parsing TypeContext", get_loc(src, *ctx).value()};
    }

    virtual std::any visitPrimitiveType(DepCParser::PrimitiveTypeContext* ctx) override
    {
        assert(ctx);
        auto const loc = get_loc(src, *ctx).value();
        if (ctx->KW_BOOL()) return type_t{loc, type_t::bool_t{}};
        if (ctx->KW_UNIT_T()) return type_t{loc, type_t::unit_t{}};
        if (ctx->KW_I8_T()) return type_t{loc, type_t::i8_t{}};
        if (ctx->KW_I16_T()) return type_t{loc, type_t::i16_t{}};
        if (ctx->KW_I32_T()) return type_t{loc, type_t::i32_t{}};
        if (ctx->KW_I64_T()) return type_t{loc, type_t::i64_t{}};
        if (ctx->KW_U8_T()) return type_t{loc, type_t::u8_t{}};
        if (ctx->KW_U16_T()) return type_t{loc, type_t::u16_t{}};
        if (ctx->KW_U32_T()) return type_t{loc, type_t::u32_t{}};
        if (ctx->KW_U64_T()) return type_t{loc, type_t::u64_t{}};
        throw error_t{"unexpected alternative when parsing PrimitiveTypeContext", loc};
    }

    virtual std::any visitFuncType(DepCParser::FuncTypeContext* ctx) override
    {
        assert(ctx);
        assert(ctx->retType);
        return type_t{
            get_loc(src, *ctx).value(),
            type_t::arr_t{
                fmap(
                    ctx->funcArg(),
                    [this] (DepCParser::FuncArgContext* const x)
                    {
                        return std::any_cast<func_arg_t>(visitFuncArg(x));
                    }),
                std::any_cast<type_t>(visitType(ctx->retType))}};
    }

    virtual std::any visitTypeVar(DepCParser::TypeVarContext* ctx) override
    {
        assert(ctx);
        assert(ctx->name);
        return type_t{get_loc(src, *ctx).value(), type_t::var_t{get_text(src, *ctx->name).value()}};
    }

    virtual std::any visitFuncArg(DepCParser::FuncArgContext* ctx) override
    {
        assert(ctx);
        if (ctx->KW_TYPENAME())
            return func_arg_t{
                get_loc(src, *ctx).value(),
                func_arg_t::type_arg_t{
                    ctx->name
                        ? std::optional{type_t::var_t{get_text(src, *ctx->name).value()}}
                        : std::nullopt
                }};
        if (ctx->type())
            return func_arg_t{
                get_loc(src, *ctx).value(),
                func_arg_t::term_arg_t{
                    std::any_cast<type_t>(visitType(ctx->type())),
                    ctx->name
                        ? std::optional{expr_t::var_t{get_text(src, *ctx->name).value()}}
                        : std::nullopt
                }};
        throw error_t{"unexpected alternative when parsing FuncArgContext", get_loc(src, *ctx).value()};
    }

    virtual std::any visitBody(DepCParser::BodyContext* ctx) override
    {
        assert(ctx);
        return body_t{
            get_loc(src, *ctx).value(),
            fmap(ctx->stmt(), [this] (auto* x) { return std::any_cast<stmt_t>(visitStmt(x)); })
        };
    }

    virtual std::any visitStmt(DepCParser::StmtContext* ctx) override
    {
        assert(ctx);
        if (ctx->funcCallStmt()) return std::any_cast<stmt_t>(visitFuncCallStmt(ctx->funcCallStmt()));
        if (ctx->ifElse()) return std::any_cast<stmt_t>(visitIfElse(ctx->ifElse()));
        if (ctx->returnStmt()) return std::any_cast<stmt_t>(visitReturnStmt(ctx->returnStmt()));
        throw error_t{"unexpected alternative when parsing StmtContext", get_loc(src, *ctx).value()};
    }

    virtual std::any visitFuncCallStmt(DepCParser::FuncCallStmtContext* ctx) override
    {
        assert(ctx);
        assert(ctx->funcCall());
        return stmt_t{
            get_loc(src, *ctx).value(),
            std::any_cast<expr_t::app_t>(visitFuncCall(ctx->funcCall()))};
    }

    virtual std::any visitIfElse(DepCParser::IfElseContext* ctx) override
    {
        assert(ctx);
        assert(ctx->cond);
        assert(ctx->true_branch);
        return stmt_t{
            get_loc(src, *ctx).value(),
            stmt_t::if_else_t{
                visitExpr(ctx->cond),
                std::any_cast<body_t>(visitBodyOrStmt(ctx->true_branch)),
                ctx->false_branch
                    ? std::optional{std::any_cast<body_t>(visitBodyOrStmt(ctx->false_branch))}
                    : std::nullopt}};
    }

    virtual std::any visitReturnStmt(DepCParser::ReturnStmtContext* ctx) override
    {
        assert(ctx);
        return stmt_t{
            get_loc(src, *ctx).value(),
            stmt_t::return_t{
                ctx->expr()
                    ? std::optional{visitExpr(ctx->expr())}
                    : std::nullopt}};
    }

    virtual std::any visitBodyOrStmt(DepCParser::BodyOrStmtContext* ctx) override
    {
        assert(ctx);
        if (ctx->body()) return visitBody(ctx->body());
        if (ctx->stmt()) return body_t{get_loc(src, *ctx).value(), {std::any_cast<stmt_t>(visitStmt(ctx->stmt()))}};
        throw error_t{"unexpected alternative when parsing BodyOrStmtContext", get_loc(src, *ctx).value()};
    }

    virtual std::any visitPlusExpr(DepCParser::PlusExprContext* ctx) override
    {
        assert(ctx);
        assert(ctx->lhs);
        assert(ctx->rhs);
        return expr_t{
            get_loc(src, *ctx).value(),
            expr_t::arith_expr_t{
                expr_t::arith_expr_t::plus_t{
                    visitExpr(ctx->lhs),
                    visitExpr(ctx->rhs)}}};
    }

    virtual std::any visitVarExpr(DepCParser::VarExprContext* ctx) override
    {
        assert(ctx);
        assert(ctx->var);
        return expr_t{get_loc(src, *ctx->var).value(), expr_t::var_t{get_text(src, *ctx->var).value()}};
    }

    virtual std::any visitTypeExpr(DepCParser::TypeExprContext* ctx) override
    {
        assert(ctx);
        assert(ctx->type());
        return expr_t{get_loc(src, *ctx->type()).value(), std::any_cast<type_t>(visitType(ctx->type()))};
    }

    virtual std::any visitBooleanExpr(DepCParser::BooleanExprContext* ctx)
    {
        assert(ctx);
        assert(ctx->value);
        return expr_t{
            get_loc(src, *ctx).value(),
            expr_t::boolean_constant_t{get_text(src, *ctx->value).value() == "true"}};
    }

    virtual std::any visitNumericExpr(DepCParser::NumericExprContext* ctx)
    {
        assert(ctx);
        assert(ctx->value);
        return expr_t{
            get_loc(src, *ctx).value(),
            expr_t::numeric_constant_t{
                ctx->sign ? std::optional{get_text(src, *ctx->sign)->view()[0]} : std::nullopt,
                get_text(src, *ctx->value).value()}};
    }

    virtual std::any visitFuncCallExpr(DepCParser::FuncCallExprContext* ctx) override
    {
        assert(ctx);
        assert(ctx->funcCall());
        return expr_t{get_loc(src, *ctx).value(), std::any_cast<expr_t::app_t>(visitFuncCall(ctx->funcCall()))};
    }

    virtual std::any visitFuncCall(DepCParser::FuncCallContext* ctx) override
    {
        assert(ctx);
        assert(ctx->name);
        return expr_t::app_t{
            expr_t{
                get_loc(src, *ctx).value(),
                expr_t::var_t{get_text(src, *ctx->name).value()}},
            fmap(ctx->expr(), [this] (auto* ctx) { return visitExpr(ctx); })};
    }

    expr_t visitExpr(DepCParser::ExprContext* ctx)
    {
        assert(ctx);
        if (auto const p = dynamic_cast<DepCParser::PlusExprContext*>(ctx))
            return std::any_cast<expr_t>(visitPlusExpr(p));
        if (auto const p = dynamic_cast<DepCParser::NumericExprContext*>(ctx))
            return std::any_cast<expr_t>(visitNumericExpr(p));
        if (auto const p = dynamic_cast<DepCParser::BooleanExprContext*>(ctx))
            return std::any_cast<expr_t>(visitBooleanExpr(p));
        if (auto const p = dynamic_cast<DepCParser::FuncCallExprContext*>(ctx))
            return std::any_cast<expr_t>(visitFuncCallExpr(p));
        if (auto const p = dynamic_cast<DepCParser::VarExprContext*>(ctx))
            return std::any_cast<expr_t>(visitVarExpr(p));
        if (auto const p = dynamic_cast<DepCParser::TypeExprContext*>(ctx))
            return std::any_cast<expr_t>(visitTypeExpr(p));
        throw error_t{"unexpected alternative when parsing ExprContext", get_loc(src, *ctx).value()};
    }
};

struct FirstErrorListener : antlr4::ANTLRErrorListener
{
    source_text const src;
    std::optional<error_t> error;

    explicit FirstErrorListener(source_text const src) :
        src(src)
    { }

    void reportAmbiguity(antlr4::Parser*,
        antlr4::dfa::DFA const&,
        std::size_t,
        std::size_t,
        bool,
        antlrcpp::BitSet const&,
        antlr4::atn::ATNConfigSet*) override
    {
        // this is not really an error
    }

    void reportAttemptingFullContext(
        antlr4::Parser*,
        antlr4::dfa::DFA const&,
        std::size_t,
        std::size_t,
        antlrcpp::BitSet const&,
        antlr4::atn::ATNConfigSet*) override
    {
        // this is not really an error
    }

    void reportContextSensitivity(
        antlr4::Parser*,
        antlr4::dfa::DFA const&,
        std::size_t startIndex,
        std::size_t stopIndex,
        std::size_t,
        antlr4::atn::ATNConfigSet*) override
    {
        // this is not really an error
    }

    void syntaxError(
        antlr4::Recognizer*,
        antlr4::Token* token,
        std::size_t line,
        std::size_t col,
        std::string const& msg,
        std::exception_ptr) override
    {
        if (error) return;
        auto text = token ? get_text(src, *token) : std::nullopt;
        error = error_t{msg, source_loc_t{line, 1+col, text ? std::move(*text) : src.substr(0, 0)}};
    }
};

expected<module_t> parse(std::filesystem::path const& path) noexcept
{
    auto source = mmap(path);
    if (not source)
        return source.error();
    auto input = antlr4::ANTLRInputStream(*source);
    dep0::DepCLexer lexer(&input);
    FirstErrorListener error_listener{*source};
    lexer.removeErrorListeners();
    lexer.addErrorListener(&error_listener);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    if (error_listener.error)
        return std::move(*error_listener.error);
    dep0::DepCParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&error_listener);
    dep0::DepCParser::ModuleContext* module = parser.module();
    if (error_listener.error)
        return std::move(*error_listener.error);
    try
    {
        parse_visitor_t visitor(*source);
        return std::any_cast<module_t>(module->accept(&visitor));
    }
    catch (error_t const& e) // we don't like to throw... this is an exceptional case (pun not intended)
    {
        return e;
    }
}

}
