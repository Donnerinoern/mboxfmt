#include "generator.h"
#include <cstddef>
#include <fstream>
#include <map>
#include <string>
#include <string_view>
#include <iostream>

Generator::Generator(std::map<Parser::FieldType, std::string_view> map)
    : m_map {map}
{}

void Generator::generate(Mode mode) {
    switch (mode) {
        case PLAIN:
            generate_plain();
            break;
        case HTML:
            generate_html();
            break;
        case MD:
            std::cout << "Not yet implemented...";
            break;
    }
}

void Generator::generate_plain() {
    std::string filename {format_filename(".txt")};
    std::fstream file_stream {filename, std::ios::out};
    file_stream << m_map.at(Parser::FROM) << '\n';
    file_stream << m_map.at(Parser::TO) << '\n';
    file_stream << m_map.at(Parser::DATE) << '\n';
    file_stream << m_map.at(Parser::SUBJECT) << "\n\n";
    file_stream << m_map.at(Parser::CONTENT_PLAIN) << '\n';
    file_stream.close();
}

void Generator::generate_html()  {
    constexpr std::string_view tag {"<h4>"};
    constexpr std::string_view close_tag {"</h4>\n"};
    std::string filename {format_filename(".html")};
    std::fstream file_stream {filename, std::ios::out};
    file_stream << tag << m_map.at(Parser::FROM) << close_tag;
    file_stream << tag << m_map.at(Parser::TO) << close_tag;
    file_stream << tag << m_map.at(Parser::DATE) << close_tag;
    file_stream << tag << m_map.at(Parser::SUBJECT) << close_tag;
    file_stream << m_map.at(Parser::CONTENT_HTML) << '\n';
    file_stream.close();
}

std::string Generator::format_filename(const std::string& file_ext) {
    std::string filename {m_map.at(Parser::SUBJECT)};
    size_t space_pos {};
    for (size_t i {0}; i < filename.size(); i++) {
        if (filename.at(i) == ':') {
            space_pos = i+2;
        }
    }
    filename = filename.substr(space_pos, filename.size()-space_pos)+file_ext;
    return filename;
}

Generator::Mode Generator::get_mode(std::string_view mode) {
    return s_modes.at(mode);
}
