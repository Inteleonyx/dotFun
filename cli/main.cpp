#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "dotFun: no input file provided\n";
        return 1;
    }

    std::string path = argv[1];

    if (!std::filesystem::exists(path))
    {
        std::cout << "dotFun: file not found: " << path << "\n";
        return 1;
    }

    std::ifstream input(path);
    if (!input.is_open())
    {
        std::cout << "dotFun: failed to open file: " << path << "\n";
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    std::cout << "dotFun running file: " << path << "\n";

    std::cout << source << "\n";

    return 0;
}
