#include <iostream>
using namespace std;
#define size 100

// ---------------- Stack Implementation -----------------
class Stack {
public:
    char arr[size];     // store operators
    int top;

    Stack() { top = -1; }

    bool isFull() {
        return (top == size - 1);
    }

    bool is_empty() {
        return (top == -1);
    }

    void push(char val) {
        if (isFull()) {
            cout << "Stack Overflow\n";
            return;
        }
        arr[++top] = val;  // store value
    }

    void pop() {
        if (is_empty()) {
            cout << "Stack Underflow\n";
            return;
        }
        top--;
    }

    char peek() {       // renamed from top() to avoid conflict
        if (is_empty()) return '\0';
        return arr[top];
    }
};

// ---------------- Conversion Class -----------------
class OperationCompiler {
public:
    string prefix;
    Stack s;

    // check if it’s operand (A-Z, a-z, or 0-9)
    bool isOperand(char token) {
        return ((token >= '0' && token <= '9') || 
                (token >= 'a' && token <= 'z') ||
                (token >= 'A' && token <= 'Z'));
    }

    // precedence of operator
    int precedence(char val) {
        if (val == '^') return 3;
        if (val == '*' || val == '/') return 2;
        if (val == '+' || val == '-') return 1;
        return 0;
    }

    // function to reverse string
    string reverseString(string str) {
        string rev = "";
        for (int i = str.length() - 1; i >= 0; i--)
            rev += str[i];
        return rev;
    }

    // Convert Infix to Prefix (manual stack logic)
    string infixToPrefix(string token) {
        prefix = "";

        // Step 1: reverse input
        token = reverseString(token);

        // Step 2: swap brackets
        for (int i = 0; i < token.length(); i++) {
            if (token[i] == '(') token[i] = ')';
            else if (token[i] == ')') token[i] = '(';
        }

        // Step 3: scan each character
        for (int i = 0; i < token.length(); i++) {
            char ch = token[i];

            if (isOperand(ch)) {
                prefix += ch;   // directly add operand
            }
            else if (ch == '(') {
                s.push(ch);
            }
            else if (ch == ')') {
                while (!s.is_empty() && s.peek() != '(') {
                    prefix += s.peek();
                    s.pop();
                }
                if (!s.is_empty()) s.pop();  // remove '('
            }
            else { // operator
                while (!s.is_empty() && precedence(s.peek()) > precedence(ch)) {
                    prefix += s.peek();
                    s.pop();
                }
                s.push(ch);
            }
        }

        // Step 4: Pop all remaining operators
        while (!s.is_empty()) {
            prefix += s.peek();
            s.pop();
        }

        // Step 5: Reverse the prefix string to get final answer
        prefix = reverseString(prefix);
        return prefix;
    }
};

// ---------------- Main Function -----------------
int main() {
    OperationCompiler op;
    string infix;

    cout << "Enter infix expression: ";
    getline(cin, infix);

    string result = op.infixToPrefix(infix);

    cout << "Prefix Expression: " << result << endl;
    return 0;
}
