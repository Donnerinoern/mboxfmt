#include "parser.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

Parser::Parser(std::stringstream& input)
    : m_source {input}
{}

std::map<Parser::FieldType, std::string> Parser::parse_file() {
    std::map<Parser::FieldType, std::string> map {};
    bool count_lines {false};
    bool extract {false};
    Parser::FieldType content_type_enum {};
    std::string content {};
    for (std::string line; std::getline(m_source, line);) {
        if (line.size() == 0) {
            continue;
        } else if (!count_lines) {
            char c {line.at(0)};
            switch (c) {
                case 'F':
                case 'T':
                case 'S':
                case 'D': {
                    std::stringstream stream {line};
                    std::string field_name {};
                    std::getline(stream, field_name, ' ');
                    if (m_keywords.contains(field_name)) {
                        map.emplace(static_cast<Parser::FieldType>(m_keywords.at(field_name)), line);
                    }
                    break;
                }
                case 'C': {
                    std::stringstream stream {line};
                    std::string content_type {};
                    std::getline(stream, content_type, ';');
                    if (m_keywords.contains(content_type)) {
                        content_type_enum = static_cast<Parser::FieldType>(m_keywords.at(content_type));
                        count_lines = true;
                    }
                    break;
                }
            }
        } else {
            if (line.starts_with("Content-") && !extract) {
                extract = true;
                continue;
            } else if (!line.starts_with("--") && !line.ends_with("--")) {
                content.append(line + '\n');
            } else {
                count_lines = false;
                extract = false;
                map.emplace(content_type_enum, content);
                content = "";
            }
        }
    }
    return map;
}
