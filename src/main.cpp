#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include "parser.h"
#include "generator.h"

bool file_valid(std::ifstream& file_stream) {
    if (!file_stream.good()) {
        std::cout << "File does not exist.\n";
        return false;
    }
    file_stream.peek();
    if (file_stream.eof()) {
        std::cout << "File is empty.\n";
        return false;
    }
    return true;
}

void help() {
    std::cout << "Usage: mboxtomd [arg [...]] <filename>\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  --help             -h\n";
    std::cout << "  --mode MODE        -m MODE\n";
    std::cout << "  --output FILENAME  -o FILENAME\n\n";
    std::cout << "Modes:\n";
    std::cout << "  plain\n";
    std::cout << "  html\n";
    std::cout << "  md\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        help();
        return 1;
    }

    std::string_view valid_modes{"plain html md"};
    std::vector<std::string_view> args {argv + 1, argv + argc};
    std::string_view mode {};
    std::optional<std::string_view> output_name {};
    for (auto it {args.begin()}; it != args.end(); it++) {
        if (it->compare("-m") == 0 || it->compare("--mode") == 0) {
            if (!valid_modes.contains(*std::next(it)) || std::next(it) == args.end()) {
                std::cout << "Enter a mode.\n";
                return 1;
            } else {
                mode = *std::next(it);
            }
        } else if (it->compare("-o") == 0 || it->compare("--output") == 0) {
            if (std::next(it) != args.end()) {
                output_name = *std::next(it);
            } else {
                std::cout << "Missing output filename.\n";
            }
        } else if (it->compare("-h") == 0 || it->compare("--help") == 0) {
            help();
            return 0;
        }
    }

    std::string filename {args.back()};
    std::ifstream file_stream (filename);
    if (!file_valid(file_stream)) {
        return 1;
    }
    std::stringstream string_stream;
    string_stream << file_stream.rdbuf();
    file_stream.close();
    std::string_view input {string_stream.str()};

    Parser parser {input};
    std::map<Parser::FieldType, std::string_view> map {parser.parse_file()};
    Generator generator {map, output_name};
    Generator::Mode type {Generator::get_mode(mode)};
    generator.generate(type);
}
