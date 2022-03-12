%option c++
%option yylineno

%{ 
#include "parser.tab.hh"
int yyFlexLexer::yywrap() { return 1; }
%}

WS      		[ \t\n\v]+
NUMBER   		[0-9]+


%%

"//".*
{WS}
{NUMBER}                { return yy::parser::token_type::NUMBER; }
"--"                    { return yy::parser::token_type::VERTEX; }
","                     { return yy::parser::token_type::EDATA;  }
.			            { return yy::parser::token_type::LEXERR; }

%%
