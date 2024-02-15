#include "formatter.h"
#include "generator.h"
#include "parser.h"
#include <iostream>
#include <iterator>
#include <map>
#include <string>

Formatter::Formatter(std::map<Parser::FieldType, std::string>& contents, Generator::Mode mode)
    : m_contents {contents}
    , m_mode {mode}
{}

void Formatter::format() {
    format_subject(m_contents.at(Parser::SUBJECT));
    switch (m_mode) {
        case Generator::TXT:
            format_text(m_contents.at(Parser::CONTENT_PLAIN));
            break;
        case Generator::MD:
            format_text(m_contents.at(Parser::CONTENT_PLAIN));
            format_md(m_contents.at(Parser::CONTENT_PLAIN));
            break;
        case Generator::HTML:
            format_html(m_contents.at(Parser::CONTENT_HTML));
            break;
    }
}

void Formatter::format_subject(std::string& subject_string) {
    if (subject_string.contains("=?iso-8859-1?Q?")) {
        m_charset = ISO8859_1;
        clean_subject(subject_string);
        if (subject_string.contains(":\n")) {
            subject_string.erase(subject_string.find(":\n")+1, 1);
        }
        do {
            subject_string.erase(subject_string.find("=?iso-8859-1?Q?"), 15);
        } while (subject_string.contains("=?iso-8859-1?Q?"));
        if (subject_string.ends_with("?=")) {
            subject_string.erase(subject_string.size()-2, 3);
        }
        do {
            subject_string.replace(subject_string.find("=E5"), 3, "å");
        } while (subject_string.contains("=E5"));
        do {
            subject_string.replace(subject_string.find("_"), 1, " ");
        } while (subject_string.contains("_"));
    }
}

void Formatter::format_text(std::string& text_string) {
    if (m_charset == ISO8859_1) {
        clean_end_line(text_string);
        replace_chars(text_string);
    }
}

void Formatter::format_html(std::string& html_string) {
    if (m_charset == ISO8859_1) {
        clean_end_line(html_string);
        replace_chars(html_string);
    }
    html_string.insert(html_string.find("</head>"), "</style>\n");
}

void Formatter::format_md(std::string& md_string) {
    bool reach_space {false};
    for (auto it{md_string.end()}; it != md_string.begin(); it--) {
        if (*it == '<' && *std::next(it) == 'h' && *std::prev(it) != ']') {
            reach_space = true;
            md_string.insert(it, ']');
        } else if (reach_space && *std::prev(it) == ' ') {
            reach_space = false;
            md_string.insert(it, '[');
        }
    }
    do {
        md_string.replace(md_string.find("<h"), 1, "(");
        md_string.replace(md_string.find(">"), 1, ")");
    } while (md_string.contains("<h"));
}

void Formatter::clean_subject(std::string& string) {
    if (string.contains("=\n")) {
        do {
            string.erase(string.find("=\n")-1, 4);
        } while (string.contains("=\n"));
    }
}

void Formatter::clean_end_line(std::string& string) {
    if (string.contains("=\n")) {
        do {
            string.erase(string.find("=\n"), 2);
        } while (string.contains("=\n"));
    }
}

void Formatter::replace_chars(std::string& string) {
    do {
        string.replace(string.find("=E5"), 3, "å");
    } while (string.contains("=E5"));
    do {
        string.replace(string.find("=F8"), 3, "ø");
    } while (string.contains("=F8"));
}
