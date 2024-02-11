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

std::string_view Parser::peek_two() {
    std::string_view sv {m_source.substr(peekPos, 2)};
    peekPos += 1;
    return sv;
}

bool Parser::is_at_end() {
    return pos >= m_source.size();
}

std::optional<std::pair<Parser::FieldType, std::string_view>> Parser::extract_field(uint8_t c) {
    peekPos = pos;
    if (!is_at_end() && peek() == c) { // TODO:  WHYYYY???
        while (!is_at_end() && peek() != ' ');
        std::string_view field_name {m_source.substr(pos-1, peekPos-pos)};
        if (m_keywords.contains(field_name)) {
            Parser::FieldType field_type {static_cast<Parser::FieldType>(m_keywords.at(field_name))};
            while (!is_at_end() && peek() != '\n');
            std::pair<FieldType, std::string_view> pair {field_type, m_source.substr(pos-1, peekPos-pos)};
            return pair;
        }
    }
    return {};
}

std::optional<std::pair<Parser::FieldType, std::string_view>> Parser::extract_content_field(uint8_t c) {
    peekPos = pos;
    if (!is_at_end() && peek() == c) {
        while (!is_at_end() && peek() != ';');
        std::string_view content_type {m_source.substr(pos-1, peekPos-pos)};
        if (m_keywords.contains(content_type)) {
            Parser::FieldType field_type {};
            if (content_type.contains("plain")) {
                field_type = CONTENT_PLAIN;
            } else {
                field_type = CONTENT_HTML;
            }
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

std::map<Parser::FieldType, std::string_view> Parser::parse_file() {
    std::map<Parser::FieldType, std::string_view> map {};
    while (!is_at_end()) {
        uint8_t character = advance();
        std::optional<std::pair<Parser::FieldType, std::string_view>> result {};
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
            map.emplace(result.value());
        }
    }
    return map;
}
