#include "parser.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

Parser::Parser(std::string_view input)
    : m_source {input}
{}

uint8_t Parser::advance() {
    return m_source.at(pos++);
}

uint8_t Parser::peek() {
    return m_source.at(peekPos++);
}

uint8_t Parser::inactive_peek() {
    return m_source.at(peekPos+1);
}

std::string_view Parser::peek_two() {
    std::string_view sv {m_source.substr(peekPos, 2)};
    peekPos += 1;
    return sv;
}

uint8_t Parser::peek_next() {
    return m_source.at(peekPos+2);
}

bool Parser::is_at_end() {
    return pos >= m_source.size();
}

std::optional<std::string_view> Parser::extract_field(uint8_t c) {
    peekPos = pos;
    if (!is_at_end() && peek() == c) { // TODO:  WHYYYY???
        while (!is_at_end() && peek() != ' ');
        std::string_view field_name {m_source.substr(pos-1, peekPos-pos)};
        if (m_keywords.contains(field_name)) {
            while (!is_at_end() && peek() != '\n');
            std::string_view full_field = m_source.substr(pos-1, peekPos-pos);
            return full_field;
        }
    }
    return {};
}

// std::optional<std::string_view> Parser::extract_content(uint8_t c) {
//     // std::cout << "Extracting content.\n";
//     peekPos = pos;
//     if (!is_at_end() && peek() == c) {
//         while (!is_at_end() && peek() != ';'); // && peekPos >= m_source.size()
//         std::string_view content_name {m_source.substr(pos-1, peekPos-pos)};
//         size_t i, j;
//         if (m_keywords.contains(content_name)) {
//             std::cout << content_name << '\n' << '\n';
//             while (!is_at_end() && peek() != '\n' && peek_next() != '\n');
//             i = peekPos+4;
//             while (!is_at_end() && peek() != '-' && peek_next() != '-');
//             j = peekPos;
//             std::string_view content_view {m_source.substr(i, j-i)};
//             // pos = j;
//             return content_view;
//         }
//     }
//     return {};
// }

std::optional<std::string_view> Parser::extract_content_field(uint8_t c) {
    peekPos = pos;
    if (!is_at_end() && peek() == c) {
        while (!is_at_end() && peek() != ';');
        std::string_view content_type {m_source.substr(pos-1, peekPos-pos)};
        if (m_keywords.contains(content_type)) {
            // std::cout << content_type << '\n';
            pos = peekPos;
            while(peek_two() != "\n\n");
            pos = peekPos;
            while(peek_two() != "\n\n");
            std::string_view content {m_source.substr(pos+1, peekPos-pos-2)};
            return content;
        }
    }
    return {};
}

std::optional<std::pair<Parser::FieldType, std::string_view>> Parser::extract_content_field_map(uint8_t c) {
    peekPos = pos;
    if (!is_at_end() && peek() == c) {
        while (!is_at_end() && peek() != ';');
        std::string_view content_type {m_source.substr(pos-1, peekPos-pos)};
        if (m_keywords.contains(content_type)) {
            // FieldType content_type {content_type == "Content-Type: text/plain" ? CONTENT_PLAIN : CONTENT_HTML};
            Parser::FieldType field_type {};
            if (content_type.contains("plain")) {
                field_type = CONTENT_PLAIN;
            } else {
                field_type = CONTENT_HTML;
            }
            // std::cout << content_type << '\n';
            pos = peekPos;
            while(peek_two() != "\n\n");
            pos = peekPos;
            while(peek_two() != "\n\n");
            std::string_view content {m_source.substr(pos+1, peekPos-pos-2)};
            std::pair<FieldType, std::string_view> pair {field_type, content};
            return pair;
        }
    }
    return {};
}

std::vector<std::string_view> Parser::parse_file() {
    std::vector<std::string_view> field_vec{};
    while (!is_at_end()) {
        uint8_t character = advance();
        std::optional<std::string_view> result {};
        switch (character) {
            case 'F':
                result = extract_field('r');
                break;
            case 'T':
                result = extract_field('o');
                break;
            case 'S':
                result = extract_field('u');
                break;
            case 'D':
                result = extract_field('a');
                break;
            case 'C':
                result = extract_content_field('o');
                break;
        }
        if (result.has_value()) {
            field_vec.push_back(result.value());
        }
    }
    for (std::string_view field : field_vec) {
        std::cout << field << '\n';
    }
    return field_vec;
}

std::map<Parser::FieldType, std::string_view> Parser::parse_file_map() {
    std::map<Parser::FieldType, std::string_view> map {};
    while (!is_at_end()) {
        uint8_t character = advance();
        std::optional<std::string_view> result {};
        std::optional<std::pair<Parser::FieldType, std::string_view>> resultA {};
        Parser::FieldType field_type {};
        switch (character) {
            case 'F':
                result = extract_field('r');
                field_type = FROM;
                break;
            case 'T':
                result = extract_field('o');
                field_type = TO;
                break;
            case 'S':
                result = extract_field('u');
                field_type = SUBJECT;
                break;
            case 'D':
                result = extract_field('a');
                field_type = DATE;
                break;
            case 'C':
                resultA = extract_content_field_map('o');
                // field_type = CONTENT_HTML;
                break;
        }
        if (result.has_value()) {
            // map.insert()
            map.emplace(field_type, result.value());
        }
        if (resultA.has_value()) {
            map.emplace(resultA.value());
        }
    }
    // for (size_t i {0}; i < map.size(); i++) {
    //     std::cout << map[static_cast<Parser::FieldType>(i)] << '\n';
    // }
    return map;
}
