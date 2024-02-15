#ifndef FORMATTER_H
#define FORMATTER_H

#include "generator.h"
#include "parser.h"
#include <map>
#include <string>

class Formatter {
    public:
        enum Charset {
            ISO8859_1
        };
        Formatter(std::map<Parser::FieldType, std::string>&, Generator::Mode);
        void format();

    private:
        void format_subject(std::string&);
        void format_text(std::string&);
        void format_html(std::string&);
        void format_md(std::string&);
        void clean_subject(std::string&);
        void clean_end_line(std::string&);
        void replace_chars(std::string&);
        std::map<Parser::FieldType, std::string>& m_contents;
        Generator::Mode m_mode;
        Charset m_charset;
};

#endif
