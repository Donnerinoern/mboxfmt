#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include "parser.h"

class Generator {
    public:
        enum Mode {
            PLAIN,
            HTML,
            MD
        };

        Generator(std::map<Parser::FieldType, std::string>, std::optional<std::string_view>, Mode);
        void generate_file();
        inline static const std::map<std::string_view, Mode> modes {
            {"plain", PLAIN},
            {"html", HTML},
            {"md", MD}
        };

    private:
        void generate_plain();
        void generate_html();
        void generate_md();
        std::string format_filename(const std::string&);

        std::map<Parser::FieldType, std::string> m_map;
        std::optional<std::string_view> m_output_name;
        Mode m_mode;
};

#endif
