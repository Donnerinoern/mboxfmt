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
            TXT,
            HTML,
            MD
        };

        Generator(std::map<Parser::FieldType, std::string>, std::string, Mode);
        void generate_file();
        inline static const std::map<std::string_view, Mode> modes {
            {"txt", TXT},
            {"html", HTML},
            {"md", MD}
        };

    private:
        void generate_txt();
        void generate_html();
        void generate_md();

        std::map<Parser::FieldType, std::string> m_map;
        std::string m_output_name;
        Mode m_mode;
};

#endif
