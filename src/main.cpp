#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string_view>
#include <vector>
#include "parser.h"
#include "src/generator.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: mboxtomd [arg [...]] filename\n\n"; // TODO: Replace with usage:
        std::cout << "Arguments:\n";
        std::cout << "  --mode MODE        -m MODE\n\n";
        std::cout << "Modes:\n";
        std::cout << "  plain\n";
        std::cout << "  html\n";
        std::cout << "  md\n";
        return 1;
    }

    std::string_view valid_args{"plain html md"};

    std::vector<std::string_view> args {argv + 1, argv + argc};
    std::string_view mode {};
    for (auto it {args.begin()}; it != args.end(); it++) {
        if (it->compare("-m") == 0 || it->compare("--mode") == 0) {
            if (!valid_args.contains(*std::next(it)) || std::next(it) == args.end()) {
                std::cout << "Enter a mode.\n";
                return 1;
            } else {
                mode = *std::next(it);
            }
        }
    }

    std::ifstream file_stream {argv[argc-1]};
    if (!file_stream.good()) {
        std::cout << "File does not exist.\n";
        return 1;
    }
    file_stream.peek();
    if (file_stream.eof()) {
        std::cout << "File is empty.\n";
    }
    std::stringstream string_stream;
    string_stream << file_stream.rdbuf();
    file_stream.close();
    std::string_view input {string_stream.str()};

    Parser parser {input};
    // parser.parse_file();
    std::map<Parser::FieldType, std::string_view> map {parser.parse_file_map()};
    Generator generator {map};
    Generator::Mode type {Generator::get_mode(mode)};
    generator.generate(type);
}
