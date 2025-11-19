#include <sstream>
#include <string>
#include "ast.h"
#include "error.h"
using std::stringstream;

extern Lexer *scanner;

// convierte expr a bool
Expression* ToBool(Expression* e)
{
    // si ya es bool retorna
    if (e->type == ExprType::BOOL)
        return e;

    // int -> bool
    if (e->type == ExprType::INT)
    {
        int valor = stoi(e->token->lexeme);
        string resultado = (valor == 0 ? "false" : "true");

        Token* t;
        if (resultado == "true")
            t = new Token(Tag::TRUE, "true");
        else
            t = new Token(Tag::FALSE, "false");

        return new Constant(ExprType::BOOL, t);
    }

    return e;
}

// convierte expr a int
Expression* ToInt(Expression* e)
{
    // si ya es int retorna
    if (e->type == ExprType::INT)
        return e;

    // bool -> int
    if (e->type == ExprType::BOOL)
    {
        int valor = (e->token->lexeme == "true" ? 1 : 0);
        string s = (valor == 1 ? "1" : "0");

        Token* t = new Token(Tag::INTEGER, s);
        return new Constant(ExprType::INT, t);
    }

    return e;
}

// ----
// Node
// ----

Node::Node() : node_type(NodeType::UNKNOWN) {}
Node::Node(int t) : node_type(t) {}

// ---------
// Statement
// ---------

Statement::Statement() : Node(NodeType::STMT) {}
Statement::Statement(int type) : Node(type) {}

// ----------
// Expression
// ----------

Expression::Expression(Token *t) : Node(NodeType::EXPR), type(ExprType::VOID), token(t) {}
Expression::Expression(int ntype, int etype, Token *t) : Node(ntype), type(etype), token(t) {}

string Expression::Name()
{
    return token->lexeme;
}

string Expression::Type()
{
    switch (type)
    {
    case ExprType::INT:
        return "int";
        break;
    case ExprType::FLOAT:
        return "float";
        break;
    case ExprType::BOOL:
        return "bool";
        break;
    default:
        return "void";
    }
}

// --------
// Constant
// --------

Constant::Constant(int etype, Token *t) : Expression(NodeType::CONSTANT, etype, t) {}

// ----------
// Identifier
// ----------

Identifier::Identifier(int etype, Token *t) : Expression(NodeType::IDENTIFIER, etype, t) {}

// ------
// Access
// ------

Access::Access(int etype, Token * t, Expression * i, Expression * e): Expression(NodeType::ACCESS, etype, t), id(i), expr(e) {}

// -------
// Logical
// -------

Logical::Logical(Token *t, Expression *e1, Expression *e2)
    : Expression(NodeType::LOG, ExprType::BOOL, t), expr1(e1), expr2(e2)
{
    // convertir ambos operandos a bool
    expr1 = ToBool(expr1);
    expr2 = ToBool(expr2);

    // si despues de convertir no son bool, dar error
    if (expr1->type != ExprType::BOOL || expr2->type != ExprType::BOOL)
    {
        stringstream ss;
        ss << "'" << token->lexeme << "' usado con operandos no booleanos ("
           << expr1->Name() << ":" << expr1->Type() << ") ("
           << expr2->Name() << ":" << expr2->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----------
// Relational
// ----------

Relational::Relational(Token *t, Expression *e1, Expression *e2)
    : Expression(NodeType::REL, ExprType::BOOL, t), expr1(e1), expr2(e2)
{
    // convertir ambos a int
    expr1 = ToInt(expr1);
    expr2 = ToInt(expr2);

    // si despues de convertir no son int, error
    if (expr1->type != ExprType::INT || expr2->type != ExprType::INT)
    {
        stringstream ss;
        ss << "'" << token->lexeme << "' usado con operandos invalidos ("
           << expr1->Name() << ":" << expr1->Type() << ") ("
           << expr2->Name() << ":" << expr2->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----------
// Arithmetic
// ----------

Arithmetic::Arithmetic(int etype, Token *t, Expression *e1, Expression *e2)
    : Expression(NodeType::ARI, etype, t), expr1(e1), expr2(e2)
{
    expr1 = ToInt(expr1);
    expr2 = ToInt(expr2);

    if (expr1->type != ExprType::INT || expr2->type != ExprType::INT)
    {
        stringstream ss;
        ss << "'" << token->lexeme << "' usado con operandos invalidos ("
           << expr1->Name() << ":" << expr1->Type() << ") ("
           << expr2->Name() << ":" << expr2->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ---------
// UnaryExpr
// ---------

UnaryExpr::UnaryExpr(int etype, Token *t, Expression *e)
    : Expression(NodeType::UNARY, etype, t), expr(e)
{
    // operador !
    if (t->lexeme == "!")
        expr = ToBool(expr);

    // operador -
    if (t->lexeme == "-")
        expr = ToInt(expr);

    // si el resultado no coincide con el tipo esperado
    if (expr->type != etype)
    {
        stringstream ss;
        ss << "'" << token->lexeme << "' usado con tipo incorrecto ("
           << expr->Name() << ":" << expr->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----
// Seq
// ----

Seq::Seq(Statement *s, Statement *ss) : Statement(NodeType::SEQ), stmt(s), stmts(ss) {}

// ------
// Assign
// ------

Assign::Assign(Expression *i, Expression *e)
    : Statement(NodeType::ASSIGN), id(i), expr(e)
{
    // conversion automatica
    if (id->type == ExprType::BOOL)
        expr = ToBool(expr);
    else if (id->type == ExprType::INT)
        expr = ToInt(expr);

    // si aun no coinciden, error real
    if (id->type != expr->type)
    {
        stringstream ss;
        ss << "'=' usado con operandos de tipos diferentes ("
           << id->Name() << ":" << id->Type() << ") ("
           << expr->Name() << ":" << expr->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----
// If
// ----

If::If(Expression *e, Statement *s) : Statement(NodeType::IF_STMT), expr(e), stmt(s) {}

// -----
// While
// -----

While::While(Expression *e, Statement *s) : Statement(NodeType::WHILE_STMT), expr(e), stmt(s) {}

// --------
// Do-While
// --------

DoWhile::DoWhile(Statement *s, Expression *e) : Statement(NodeType::DOWHILE_STMT), stmt(s), expr(e) {}