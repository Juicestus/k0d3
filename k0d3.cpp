/**
 * @file main.cpp
 * @author Justus Languell (jus@gtsbr.org)
 * @brief 
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// ***************************** This is 80 characters ***************************

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#define stackConstains(x) (stack.find(x) != stack.end())

std::map<std::string, double> stack;

/**
 * @brief Prints an error in red to console and exits the program.
 * 
 * @param n Line number error occurred on, -1 for no line.
 * @param s Error message.
 */
void error(int n, std::string s) 
{
    if (n >= 0) {
        printf("\u001b[31mError: \"%s\" on line %d\u001b[0m\n", s.c_str(), n);
    } else {
        printf("\u001b[31mError: \"%s\"\u001b[0m\n", s.c_str());
    }
    exit(0);
}

/**
 * @brief Reads a file and returns the contents as string.
 * 
 * @param path Path of the file to be read.
 * @return String of the file contents.
 */
std::string readFile(const std::string path)
{
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        error(-1, "Could not open file");
    }
    // this is fucked
    auto theFirstParameter = std::istreambuf_iterator<char>(inputFile);
    auto theSecondParameter = std::istreambuf_iterator<char>();
    return std::string(theFirstParameter, theSecondParameter);
}

/**
 * @brief Removes comments from the file content.
 * 
 * @param s Code with comments.
 * @return Code with comments removed.
 */
std::string removeComments(std::string s) 
{
    bool comment = false;
    std::string commentless = "";
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '(') {
            comment = true;
        } else if (c == ')') {
            comment = false;
        } else {
            if (!comment) {
                commentless += c;
            }
        }
    }
    return commentless;
}

/**
 * @brief Resolve a string symbol to a double
 * 
 * @param s String of the symbol to resolve.
 * @return Resolved symbol as a double.
 */
double resolveSymbol(std::string s)
{
    double val = 0.;
    if (s[0] == '#' || s[0] == '?') {
        if (stackConstains(s)) {
            val = stack[s];
        } else {
            stack.insert(std::pair<std::string, double>(s, 0.));
            val = 0.0;
        }
    } else {
        try {
            val = std::stod(s);
        } catch (std::invalid_argument& e) {
            val = 0.0;
        } catch (std::out_of_range) {
            val = 0.0;
        }
    }
    return val;
}

/**
 * @brief Replace all instances of search with replace in subject.
 * 
 * @param subject The string to replace isntances with.
 * @param search The string to search for.
 * @param replace The string to replace with.
 * @return std::string Subject with instances of search replaced with replace.
 */
std::string replaceString(std::string subject, const std::string& search,
                          const std::string& replace) 
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

/**
 * @brief 
 * 
 * @param subject The string to be parsed.
 * @param delimiter The delimiter character.
 * @return std::vector<std::string> Vector of seperated strings.
 */
std::vector<std::string> splitString(std::string subject, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string currentToken = "";
    for (int i = 0; i < subject.length(); i++) {
        if (subject[i] == delimiter) {
            if (currentToken != "") {
                tokens.push_back(currentToken);
            }
            currentToken = "";
        } else {
            currentToken += subject[i];
        }
    }
    return tokens;
}

/**
 * @brief Executes the code from the string content.
 * 
 * @param content Code to be executed.
 */

void execute(std::string content) 
{
    content = removeComments(content);
    content = replaceString(content, "\n", "");
    std::vector<std::string> lines = splitString(content, ';');

    std::vector<std::string> indexs;
    for (int i = 0; i < lines.size(); i++) {
        std::string gotoPointer = splitString(lines[i], ' ')[0];
        indexs.push_back(gotoPointer);
    } 

    int n = 0;
    while (n < lines.size()) {
        std::vector elements = splitString(lines[n], ' ');

        // FLOW CONTROL

        // Goto If
        if (elements[1] == "$") {
            double condition = resolveSymbol(elements[2]);
            if (condition > 0) {
                std::string symbol = elements[3];
                if (symbol[0] == '$') {
                    symbol = symbol.substr(1);
                    for (int i = 0; i < indexs.size(); i++) {
                        if (indexs[i] == symbol) {
                            n = i - 1;
                        }
                    }
                } 
            }
        }

        // Numeric operations

        // Add
        if (elements[1] == "+") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], left + right));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Subtract
        if (elements[1] == "-") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], left - right));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Multiply
        if (elements[1] == "*") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], left * right));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Divide
        if (elements[1] == "/") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], left / right));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Modulo
        if (elements[1] == "%") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], fmod(left, right)));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Power
        if (elements[1] == "^") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], pow(left, right)));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Round
        if (elements[1] == "@") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '#') {
                double scale = pow(10, right);
                stack.insert(std::pair<std::string, double>
                    (elements[4], round(left * scale) / scale));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // BOOLEAN OPERATIONS

        // Equal
        if (elements[1] == "=") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '?') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], (left == right) ? 1.0 : 0.0));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Less Than
        if (elements[1] == "<") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '?') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], (left < right) ? 1.0 : 0.0));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Greater Than
        if (elements[1] == ">") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '?') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], (left > right) ? 1.0 : 0.0));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Not
        if (elements[1] == "!") {
            double param = resolveSymbol(elements[2]);
            if (elements[3][0] == '?') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], (!param) ? 1.0 : 0.0));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // And
        if (elements[1] == "&") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '?') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], (left && right) ? 1.0 : 0.0));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // Or
        if (elements[1] == "|") {
            double left = resolveSymbol(elements[2]);
            double right = resolveSymbol(elements[3]);
            if (elements[4][0] == '?') {
                stack.insert(std::pair<std::string, double>
                    (elements[4], (left || right) ? 1.0 : 0.0));
            } else {
                error(n, "Invalid return variable!");
            }
        }

        // INPUT / OUTPUT

        // Write Ascii to console
        if (elements[1] == "\"") {
            for (int i = 2; i < elements.size(); i++) {
                if (elements[i][0] == '.') {
                    std::cout << ((resolveSymbol(elements[i]) > 0) ? '1' : '0');
                } else if (elements[i][0] == '#') {
                    //std::cout << resolveSymbol(elements[i]);
                    std::cout << (char) (int) (resolveSymbol(elements[i]));
                } else if (elements[i][0] == '"') {
                    std::cout << elements[i].substr(1);
                } else {
                }
            }
        }

        // Write number values to console
        if (elements[1] == "#") {
            for (int i = 2; i < elements.size(); i++) {
                if (elements[i][0] == '.') {
                    std::cout << ((resolveSymbol(elements[i]) > 0) ? '1' : '0');
                } else if (elements[i][0] == '#') {
                    std::cout << resolveSymbol(elements[i]);
                    //std::cout << (char) (int) (resolveSymbol(elements[i]));
                } else if (elements[i][0] == '"') {
                    std::cout << elements[i].substr(1);
                } else {
                }
            }
        }

        // Reads double from console
        if (elements[1] == "?") {
            if (elements[2][0] == '#') {
                try {
                    std::string input;
                    std::getline(std::cin, input);
                    double in = stod(input);
                    stack.insert(std::pair<std::string, double>
                        (elements[2], in));
                } catch (std::invalid_argument) {
                    error(n, "Invalid input!");
                } catch (std::out_of_range) {
                    error(n, "Input out of range!");
                }
            } else {
                error(n, "Invalid return variable!");
            }
        }

        n++;     

    }
}

/**
 * @brief Main function.
 */
int main(int argc, char **argv)
{
    if (argc < 2) {
        error(-1, "No input file specified.");
    } else {
        std::string input = readFile(argv[1]);
        execute(input);
    }
}