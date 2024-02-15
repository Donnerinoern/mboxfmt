#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include "generator.h"
#include "parser.h"
#include "formatter.h"

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
    std::cout << "Usage: mboxto [arg [...]] <input> <output>\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  --help                   -h\n";
    std::cout << "  --force FILE_EXT         -f FILE_EXT\n\n";
    std::cout << "Valid file extensions:\n";
    std::cout << "  txt\n";
    std::cout << "  html\n";
    std::cout << "  md\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        help();
        return 1;
    }

    bool force {false}; // TODO: Fix
    std::vector<std::string> args {argv + 1, argv + argc};
    std::string input_filename {};
    std::string output_filename {};
    std::string_view file_ext {};
    for (auto it {args.begin()}; it != args.end(); it++) {
        if (it->compare("-h") == 0 || it->compare("--help") == 0) {
            help();
            return 0;
        } else if (it->compare("-f") == 0 || it->compare("--force") == 0) {
            if (std::next(it) == args.end()) {
                help();
                return 1;
            } else {
                force = true;
                file_ext = *std::next(it);
            }
        } else if (std::next(it) == args.end()) {
            input_filename = *std::prev(it);
            output_filename = *it;
        }
    }

    if (output_filename.contains(".") && !force) {
        file_ext = output_filename.substr(output_filename.find_last_of(".")+1, output_filename.size()-1);
    } else if (!force) {
        std::cout << "Output filename is not valid.\n";
        return 1;
    }

    if (!std::filesystem::is_regular_file(input_filename)) {
        std::cout << "File is not regular file.\n";
    }

    std::ifstream file_stream {input_filename};
    if (!file_valid(file_stream)) {
        return 1;
    }
    std::stringstream string_stream;
    string_stream << file_stream.rdbuf();
    file_stream.close();

    Generator::Mode mode {Generator::modes.at(file_ext)};

    Parser parser {string_stream};
    std::map<Parser::FieldType, std::string> map {parser.parse_file()};

    Formatter formatter {map, mode};
    formatter.format();

    Generator generator {map, output_filename, mode};
    generator.generate_file();
}
