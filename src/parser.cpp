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
    std::string buffer {};
    bool extract {false};
    Parser::FieldType active_extract {};
    for (std::string line; std::getline(m_source, line);) {
        if (line.size() <= 0) {
            continue;
        }

        if (extract) {
            if (!(line.at(0) == ' ') && active_extract == SUBJECT) {
                map.emplace(active_extract, "Subject:" + buffer);
                buffer.clear();
                extract = false;
            } else if (line.starts_with("-->")) {
                continue;
            } else if (line.starts_with("--")) {
                map.emplace(active_extract, buffer);
                buffer.clear();
                extract = false;
            } else if (line.starts_with("Content-Transfer-Encoding:")) {
                continue;
            } else {
                buffer.append("\n" + line);
            }
        } else {
            if (line.starts_with("To: ")) {
                map.emplace(TO, line);
            } else if (line.starts_with("From: ")) {
                map.emplace(FROM, line);
            } else if (line.starts_with("Date: ")) {
                map.emplace(DATE, line);
            } else if (line.starts_with("Subject:")) {
                if (line.size() <= 8) {
                    active_extract = SUBJECT;
                    extract = true;
                } else {
                    map.emplace(SUBJECT, line);
                }
            } else if (line.starts_with("Content-Type: ")) {
                if (line.contains("text/plain")) {
                    active_extract = CONTENT_PLAIN;
                    extract = true;
                } else if (line.contains("text/html")) {
                    active_extract = CONTENT_HTML;
                    extract = true;
                }
            }
        }
    }
    return map;
}
