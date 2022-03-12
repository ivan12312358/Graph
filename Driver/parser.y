%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
namespace yy { class Driver; }
}

%code
{
#include "driver.hpp"

namespace yy {

parser::token_type yylex (parser::semantic_type* yylval, Driver* driver);
}
}

%token
    <int> NUMBER
    VERTEX "--"
    EDATA  ","
    LEXERR
;

%nterm
    STM
;

%start program

%%

program: STM program    {}
       | %empty         {}

STM: NUMBER VERTEX NUMBER EDATA NUMBER { driver->push_back($1, $3, $5); }

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval, Driver* driver)
{
    return driver->yylex(yylval);
}

void parser::error(const std::string&) {}

} /* namespace yy */
