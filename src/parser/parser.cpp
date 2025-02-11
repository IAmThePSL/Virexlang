#include "../../include/parser/parser.h"
#include "../../include/lexer/lexer.h"
#include "../../include/statements/allstatements.h"
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>

// Global variable store (converted to a class member)
std::vector<Parser::Variable> variables;

Parser::Parser(Lexer &lexer) : lexer(lexer) {}

void Parser::executeStatement() {
    // Get the current token
    Token current_token = *lexer.lexNextToken();

    // Simple handling print statements
    if (current_token.type == TokenType::TOKEN_PRINT) {
        handlePrintStatement();
    }
    else if (current_token.type == TokenType::TOKEN_IF) {
        handleIfStatement();
    }
    else if (current_token.type == TokenType::TOKEN_WHILE) {
        handleWhileStatement();
    }
    else if (current_token.type == TokenType::TOKEN_LET || current_token.type == TokenType::TOKEN_BOOL || current_token.type == TokenType::TOKEN_INT || current_token.type == TokenType::TOKEN_STR) {
        handleVariableDeclaration(false);  // 0 -> let
    }
    else if (current_token.type == TokenType::TOKEN_CONST) {
        handleVariableDeclaration(true);  // 1 -> const
    }
    else {
        std::cerr << "Error: Unknown statement type." << std::endl;
    }

    destroyToken(&current_token); // Ensure token is destroyed
}

Parser::Variable* Parser::findVariable(const std::string &name) {
    for (auto &var : variables) {
        if (var.name == name) {
            return &var;
        }
    }
    return nullptr;
}

void Parser::addVariable(const std::string &name, const std::string &type, void* value, bool isConst) {
    if (findVariable(name)) {
        std::cerr << "Error: Variable '" << name << "' is already defined" << std::endl;
        return;
    }

    Variable var;
    var.name = name;
    var.type = type;
    var.isConst = isConst;

    if (type == "int") {
        var.value.intValue = *reinterpret_cast<int*>(value);
    }
    else if (type == "str") {
        var.value.strValue = *reinterpret_cast<std::string*>(value);
    }
    else if (type == "bool") {
        var.value.boolValue = *reinterpret_cast<bool*>(value);
    }

    variables.push_back(var);
}

void Parser::cleanupVariables() {
    variables.clear();
}

bool Parser::isNumber(const std::string &str) {
    if (str.empty()) return false;
    int i = 0;
    if (str[i] == '-') i++; // Skip leading minus sign if present
    bool hasDot = false;
    for (; i < str.length(); i++) {
        if (str[i] == '.') {
            if (hasDot) return false; // Multiple dots not allowed
            hasDot = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int Parser::evaluateExpression(Token* condition) {
    std::string expr = condition->lexeme;
    trimWhitespace(expr);

    // Handle Parentheses (recursively evaluate inside them)
    if (expr.front() == '(') {
        size_t closingParen = expr.find(')');
        if (closingParen == std::string::npos) {
            std::cerr << "Error: Mismatched parentheses" << std::endl;
            return 0;
        }
        expr = expr.substr(1, closingParen - 1);  // Strip the parentheses
        int result = evaluateExpression(createToken(TOKEN_BOOL, expr.c_str(), 0, 0));
        destroyToken(condition);  // Ensure token is destroyed
        return result;
    }

    // Handle basic comparisons (e.g., x == 10)
    if (expr.find("==") != std::string::npos) {
        auto [left, right] = splitExpression(expr, "==");
        if (isNumber(left) && isNumber(right)) {
            return std::stod(left) == std::stod(right);
        } else {
            return left == right;  // String comparison
        }
    }

    if (expr.find("!=") != std::string::npos) {
        auto [left, right] = splitExpression(expr, "!=");
        if (isNumber(left) && isNumber(right)) {
            return std::stod(left) != std::stod(right);
        } else {
            return left != right;
        }
    }

    if (expr.find(">") != std::string::npos) {
        auto [left, right] = splitExpression(expr, ">");
        return std::stod(left) > std::stod(right);
    }

    if (expr.find("<") != std::string::npos) {
        auto [left, right] = splitExpression(expr, "<");
        return std::stod(left) < std::stod(right);
    }

    if (expr.find(">=") != std::string::npos) {
        auto [left, right] = splitExpression(expr, ">=");
        return std::stod(left) >= std::stod(right);
    }

    if (expr.find("<=") != std::string::npos) {
        auto [left, right] = splitExpression(expr, "<=");
        return std::stod(left) <= std::stod(right);
    }

    // Handle arithmetic operations
    if (expr.find('+') != std::string::npos) {
        auto [left, right] = splitExpression(expr, "+");
        return std::stod(left) + std::stod(right);
    }

    if (expr.find('-') != std::string::npos) {
        auto [left, right] = splitExpression(expr, "-");
        return std::stod(left) - std::stod(right);
    }

    if (expr.find('*') != std::string::npos) {
        auto [left, right] = splitExpression(expr, "*");
        return std::stod(left) * std::stod(right);
    }

    if (expr.find('/') != std::string::npos) {
        auto [left, right] = splitExpression(expr, "/");
        double rightValue = std::stod(right);
        if (rightValue == 0) {
            std::cerr << "Error: Division by zero" << std::endl;
            return 0;
        }
        return std::stod(left) / rightValue;
    }

    // Logical AND (&&) and OR (||)
    if (expr.find("&&") != std::string::npos) {
        auto [left, right] = splitExpression(expr, "&&");
        return evaluateExpression(createToken(TOKEN_BOOL, left.c_str(), 0, 0)) &&
               evaluateExpression(createToken(TOKEN_BOOL, right.c_str(), 0, 0));
    }

    if (expr.find("||") != std::string::npos) {
        auto [left, right] = splitExpression(expr, "||");
        return evaluateExpression(createToken(TOKEN_BOOL, left.c_str(), 0, 0)) ||
               evaluateExpression(createToken(TOKEN_BOOL, right.c_str(), 0, 0));
    }

    // Handle boolean literals
    if (expr == "true") return 1;
    if (expr == "false") return 0;

    return isNumber(expr) ? std::stod(expr) : 0;
}

std::pair<std::string, std::string> Parser::splitExpression(const std::string &expr, const std::string &delimiter) {
    size_t pos = expr.find(delimiter);
    std::string left = expr.substr(0, pos);
    std::string right = expr.substr(pos + delimiter.size());
    trimWhitespace(left);
    trimWhitespace(right);
    return {left, right};
}
