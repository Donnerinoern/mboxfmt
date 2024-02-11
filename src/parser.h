#ifndef NEW_PARSER_H
#define NEW_PARSER_H

#include <cstdint>
#include <map>
#include <optional>
#include <string_view>

class Parser {
    public:
        enum FieldType {
            FROM, TO, SUBJECT, DATE, CONTENT_PLAIN, CONTENT_HTML
        };
        Parser(std::stringstream&);
        std::map<Parser::FieldType, std::string> parse_file();

    private:
        std::stringstream& m_source;
        std::map<std::string_view, size_t> m_keywords {
            {"From:", FROM},
            {"To:", TO},
            {"Subject:", SUBJECT},
            {"Date:", DATE},
            {"Content-Type: text/plain", CONTENT_PLAIN},
            {"Content-Type: text/html", CONTENT_HTML}
        };
};

#endif
