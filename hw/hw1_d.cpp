#include <iostream>
#include <exception>
#include <string>
#include <regex>

// Exception Handling
#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
#define OPERATION_PATTERN "([0-9]+)\\s*([\\+\\-\\*\\/])\\s*([0-9]+)\\s*([\\+\\-\\*\\/])\\s*([0-9]+)"

class UnknownOperatorException: public std::exception {
public:
    const char* what() const throw() {
        return "Unknown operator";
    }
};

class ZeroDivisionException: public std::exception {
public:
    const char* what() const throw() {
        return "Division by zero error";
    }
};

class OperationException: public std::exception {
public:
    const char* what() const throw() {
        return "Invalid operation";
    }
};

bool isValidOperator(char op) {
    return (op == '+' || op == '-' || op == '*' || op == '/');
}

double resolve(char op, int a, int b) {
    try {
        if (!isValidOperator(op))
            throw UnknownOperatorException();
        if (op == '/' && b == 0)
            throw ZeroDivisionException();

        switch (op) {
            case '+':
                return a + b;
            case '-':
                return a - b;
            case '*':
                return a * b;
            case '/':
                return static_cast<double>(a) / b;
            default:
                throw UnknownOperatorException();
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        throw;
    }
}

int calculate(char op1, char op2, int a, int b, int c) {
    double result = resolve(op2, b, c);
    result = resolve(op1, a, ROUND_2_INT(result));
    return ROUND_2_INT(result);
}

bool isValidOperation(const std::string& operation) {
    std::regex pattern(OPERATION_PATTERN);
    return std::regex_match(operation, pattern);
}

int main() {
    std::string operation;

    std::cout << "Enter operation (e.g., 5 + 3 * 2): ";
    std::getline(std::cin, operation);

    if (!isValidOperation(operation)) {
        throw OperationException();
    }

    std::smatch match;
    std::regex pattern(OPERATION_PATTERN);
    if (std::regex_search(operation, match, pattern)) {
        int a = std::stoi(match[1].str());
        char op1 = match[2].str()[0];
        int b = std::stoi(match[3].str());
        char op2 = match[4].str()[0];
        int c = std::stoi(match[5].str());

        std::cout << "Result: " << operation << " = " << calculate(op1, op2, a, b, c) << std::endl;
    }

    return 0;
}

