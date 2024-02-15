#include "generator.h"
#include "parser.h"
#include <cstddef>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <iostream>

Generator::Generator(std::map<Parser::FieldType, std::string> map, std::string output_name, Generator::Mode mode)
    : m_map {map}
    , m_output_name {output_name}
    , m_mode {mode}
{}

void Generator::generate_file() {
    switch (m_mode) {
        case TXT:
            generate_txt();
            break;
        case HTML:
            generate_html();
            break;
        case MD:
            generate_md();
            break;
    }
}

void Generator::generate_txt() {
    std::fstream file_stream {m_output_name, std::ios::out};
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
    std::fstream file_stream {m_output_name, std::ios::out};
    file_stream << tag << m_map.at(Parser::FROM) << close_tag;
    file_stream << tag << m_map.at(Parser::TO) << close_tag;
    file_stream << tag << m_map.at(Parser::DATE) << close_tag;
    file_stream << tag << m_map.at(Parser::SUBJECT) << close_tag;
    file_stream << m_map.at(Parser::CONTENT_HTML) << '\n';
    file_stream.close();
}

void Generator::generate_md() {
    std::fstream file_stream {m_output_name, std::ios::out};
    file_stream << "### Header\n\n";
    file_stream << m_map.at(Parser::FROM) << "\n\n";
    file_stream << m_map.at(Parser::TO) << "\n\n";
    file_stream << m_map.at(Parser::DATE) << "\n\n";
    file_stream << m_map.at(Parser::SUBJECT) << "\n\n";
    file_stream << "---\n\n";
    file_stream << "####" << m_map.at(Parser::SUBJECT).substr(8) << "\n\n";
    std::string content {m_map.at(Parser::CONTENT_PLAIN)};
    for (size_t i{0}; i < content.size(); i++) {
        if (content.at(i) == '\n') {
            content.replace(i++, 1, "\n\n");
        }
    }
    file_stream << content << '\n';
}
