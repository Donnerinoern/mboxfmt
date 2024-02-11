#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <map>
#include <optional>
#include <string_view>

class Parser {
    public:
        enum FieldType {
            FROM, TO, SUBJECT, DATE, CONTENT_PLAIN, CONTENT_HTML
        };
        Parser(std::string_view input);
        std::map<Parser::FieldType, std::string_view> parse_file();

    private:
        uint8_t advance();
        uint8_t peek();
        std::string_view peek_two();
        bool is_at_end();
        std::optional<std::pair<Parser::FieldType, std::string_view>> extract_field(uint8_t c);
        std::optional<std::pair<Parser::FieldType, std::string_view>> extract_content_field(uint8_t c);

        std::string_view m_source;
        size_t pos{0};
        size_t peekPos{0};
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
