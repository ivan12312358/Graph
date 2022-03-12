#pragma once

#include <memory>
#include <vector>
#include <tuple>
#include <string>

#include <FlexLexer.h>
#include "parser.tab.hh"


namespace yy {

    class Driver final {
        std::unique_ptr<FlexLexer> plex;
        std::vector<std::tuple<int, int, int>> edges;
    public:
        Driver(): plex{new yyFlexLexer} {}

        parser::token_type yylex(parser::semantic_type *yylval)
        {
            parser::token_type tt = static_cast<parser::token_type>(plex->yylex());

            switch (tt)
            {
                case yy::parser::token_type::NUMBER:
                    yylval->build<int>() = std::stoi(plex->YYText());
                    break;
                case yy::parser::token_type::LEXERR:
                    throw std::runtime_error(std::string("unrecognized symbol: ") + plex->YYText());
            }

            return tt;
        }

        void parse()
        {
            parser parser(this);
            parser.parse();
        }

        void push_back(int vert1, int vert2, int edata) { edges.push_back({vert1, vert2, edata}); }

        using EIter = std::vector<std::tuple<int, int, int>>::iterator;

        EIter begin() { return edges.begin(); }
        EIter end()   { return edges.end(); }

        ~Driver() = default;
    };

} // namespace yy
