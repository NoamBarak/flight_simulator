
#include <algorithm>
#include "ex1.h"

// Value
Value::Value(double val) {
    this->value = val;
}

double Value::calculate() {
    return this->value;
}

// Binary Operator
BinaryOperator::BinaryOperator(Expression *l, Expression *r) {
    this->left = l;
    this->right = r;
}

Expression *BinaryOperator::getLeft() {
    return this->left;
}

Expression *BinaryOperator::getRight() {
    return this->right;
}

double BinaryOperator::calculate() { return 0; }


// Plus
double Plus::calculate() {
    return (this->getLeft()->calculate() + this->getRight()->calculate());
}


// Minus
double Minus::calculate() {
    return (this->getLeft()->calculate() - this->getRight()->calculate());
}


// Mult
double Mul::calculate() {
    return (this->getLeft()->calculate() * this->getRight()->calculate());
}

// Div
double Div::calculate() {
    if (this->getRight()->calculate() == 0) {
        throw "division by zero";
    }
    return (this->getLeft()->calculate() / this->getRight()->calculate());
}

// Unary
UnaryOperator::UnaryOperator(Expression *e1) {
    this->exp = e1;
}

Expression *UnaryOperator::getExpression() {
    return this->exp;
}

double UPlus::calculate() {
    return this->getExpression()->calculate();
}

double UMinus::calculate() {
    return (-1) * (this->getExpression()->calculate());
}

// Variable
Variable::Variable(string str, double val1) {
    this->name = str;
    this->val = val1;
}

string Variable::getString() {
    return this->name;
}

double Variable::getVal() {
    return this->val;
}

double Variable::calculate() {
    return this->getVal();
}

Variable &Variable::operator++() {
    this->val = this->val + 1;
    return *this;
}

Variable &Variable::operator++(int) {
    this->val = this->val + 1;
    return *this;
}

Variable &Variable::operator--() {
    this->val = this->val - 1;
    return *this;
}

Variable &Variable::operator--(int) {
    this->val = this->val - 1;
    return *this;
}

Expression *Interpreter::interpret(string equation) {
    /*cout << "Interpreter map Variables: " << endl;
    for (auto &it: interpreter.getVariables()) {
        cout << "\tName: " << it.first << ", Val: " << it.second << endl;
    }*/
    cout<<"Interpreter here 1 equation: "<< equation <<endl;
    std::string::iterator end_pos = std::remove(equation.begin(), equation.end(), ' ');
    equation.erase(end_pos, equation.end());
    int n = equation.length();
    //char chars[n + 1];
    char *chars = new char[n + 1];
    strcpy(chars, (equation).c_str());
    stack<char> stack;
    queue<string> queue;
    int leftBar = 0;
    int rightBar = 0;
    int numOp = 0;
    int numsOrVars = 0;
    //cout<<"Interpreter here 2 "<<endl;
    for (int i = 0; i < n; i++) {
       // cout<<"Interpreter here 8 "<<endl;
        char a = chars[i];
        //valid
        // in A-Z / a-z
        bool inABC = ((65 <= chars[i] && chars[i] <= 90) || (97 <= chars[i] && chars[i] <= 122) || chars[i]==95);
        bool isOp = (isOperator(chars[i]));
        bool inNums = ((48 <= (int) (chars[i]) && (int) (chars[i]) <= 57) || chars[i] == 46)|| chars[i]=='0';
        //cout<<"Interpreter here 10 "<<endl;
        if (!isOp && !inNums && !(inABC)){
            cout<<"problematic char: "<< chars[i]<<endl;
            throw "invalid";
        }
        // If it's an operator
        if (isOperator(chars[i])) {
          //  cout<<"Interpreter here 4 "<<endl;
            // checking that the next char is a valid char
            bool flag = false;
            bool inNums1 = ((48 <= (int) (chars[i+1]) && (int) (chars[i+1]) <= 57) || chars[i] == 46)|| chars[i+1]=='0';
            bool inABC1 = ((65 <= chars[i+1] && chars[i+1] <= 90) || (97 <= chars[i+1] && chars[i+1] <= 122) || chars[i+1]==95);
            if (chars[i] == '-' && !(isOperator(chars[i + 1])) &&
                !(inNums1) && !inABC1)
                throw "illegal math expression";
            if (isOperator(chars[i + 1])) {
                if (chars[i] == '(' && (chars[i + 1] == '*' || chars[i + 1] == '/'))
                    flag = true;
                else if (chars[i] == ')' && chars[i + 1] == '(')
                    flag = true;
                else if (chars[i] == '(' || chars[i] == ')')
                    flag = false;
                else if (chars[i + 1] != '(' && chars[i + 1] != ')')
                    flag = true;
                if (flag)
                    throw "illegal math expression";
            }
            // Operator identifying
            char op = '^';
            // checking if it's an unary operator
            if (i == 0 || chars[(i - 1)] == '(') {
                // + will be $, - will be %
                if (chars[i] == '+') {
                    op = '$';}
                if (chars[i] == '-') {
                    op = '%';
                }
                if (chars[i] == '(') {
                    op = '(';
                }
                if (chars[i] == ')') {
                    op = ')';
                }
            } else {
                op = chars[i];
            }
            if (op == '(') {
                leftBar++;
                stack.push(op);
                continue;
            }
            // if it's - )
            if (op == ')') {
                rightBar++;
                while (stack.top() != '(' && leftBar > 0) {
                    std::string s(1, stack.top());
                    queue.push(s);
                    stack.pop();
                }
                stack.pop();
                continue;
            }
            numOp++;
            // While the stack has more "powerful" operator
            if (!stack.empty()) {
                while (!stack.empty() && (precedence(stack.top()) > precedence(chars[i]))) {
                    std::string s(1, stack.top());
                    queue.push(s);
                    stack.pop();
                }
            }
            stack.push(op);
            //cout<<"Interpreter here 5 "<<endl;
        } else {
            // If it's a number
            if (inNums) {
                int start = i;
                while ((!isOperator(chars[i + 1]) && (48 <= (int) (chars[i + 1]) && (int) (chars[i + 1]) <= 57)) ||
                       chars[i + 1] == 46) {
                    i++;
                }
                int x = (i - start) + 1;
                std::string s(x, ' ');
                for (int j = start; j <= i; j++) {
                    s[j - start] = chars[j];
                }
                queue.push(s);
                numsOrVars++;
            } else {
              //  cout<<"Interpreter here 3 "<<endl;
                // it's a variable
                int start = i;
                bool inNums1 = ((48 <= (int) (chars[i+1]) && (int) (chars[i+1]) <= 57) || chars[i+1] == 46);
                while (((i < n - 1) && !isOperator(chars[i + 1]) &&
                        (inNums1 ||(65 <= chars[i+1] && chars[i+1] <= 90) ||
                         (97 <= chars[i+1] && chars[i+1] <= 122))) || chars[i+1]=='_') {
                    i++;
                }
                int x = (i - start) + 1;
                std::string s(x, ' ');
                for (int j = start; j <= i; j++) {
                    if (j == n)
                        break;
                    s[j - start] = chars[j];
                }
                queue.push(s);
                numsOrVars++;
            }
        }
    }
    bool atLeastOneVarOrNum = (numsOrVars > 0);
    if (leftBar != rightBar)
        throw "barracks missing";
    if (numOp == 0 && (numsOrVars == 1)) {
        isOperator('-');
    } else if (!atLeastOneVarOrNum)
        throw "no vars or numbers";
    /* else if (numOp == 0) {
         throw "no operation";
     }*/
    // No more to read - push operators
    while (!stack.empty()) {
        std::string str(1, stack.top());
        queue.push(str);
        stack.pop();
    }
    delete[] (chars);
    //cout<<"Interpreter here 9 "<<endl;
    return reversePolish(queue);
}

map<string, double> Interpreter::getVariables() {
    return this->variables;
}

void Interpreter::setVariables(string string1) {
    int n = string1.length();
    char *char_array = new char[n + 1];
    strcpy(char_array, string1.c_str());
    // Returns first token
    char *savePtr;
    char *token = strtok_r(char_array, ";", &savePtr);
    map<string, double>::iterator itr;
    std::string s(token);
    while (token != NULL) {
        s = token;
        addVariable(s);
        token = strtok_r(NULL, ";", &savePtr);
    }
    delete[] (char_array);
}

void Interpreter::addVariable(string str) {
    int n = str.length();
    char *char_array = new char[n + 1];
    strcpy(char_array, str.c_str());
    char *token1 = strtok(char_array, "=");
    string key = token1;
    token1 = strtok(NULL, "=");
    string value = token1;
    // checking valid input
    int keyLen = key.length();
    char *key_arr = new char[keyLen + 1];
    strcpy(key_arr, key.c_str());
    for (int i = 0; i < keyLen; i++) {
        bool inABC = ((65 <= (int) (key_arr[i]) && (int) (key_arr[i]) <= 90) ||
                      (97 <= (int) (key_arr[i]) && (int) (key_arr[i]) <= 122) || key_arr[i]==95);
        if (i == 0) {
            if (!(inABC))
                throw "illegal variable assignment!";
        } else {
            //
            if (!(inABC) && ((int) (key_arr[i]) != 95) && (!(48 <= (int) (key_arr[i]) && (int) (key_arr[i]) <= 57)))
                throw "illegal variable assignment!";
        }
    }
    // Checking the value input
    int numDot = 0;
    for (int i = keyLen + 1; i < n; i++) {
        if (char_array[i] == 45)
            // if the value is: - , -x
            if (!(48 <= (int) (char_array[i + 1]) && (int) (char_array[i + 1]) <= 57)) {
                throw "illegal variable assignment!";
            }
        if (i == (keyLen + 1))
            if (!(48 <= (int) (char_array[i]) && (int) (char_array[i]) <= 57) && char_array[i] != 45)
                throw "illegal variable assignment!";
        if (char_array[i] == 46)
            numDot++;
        if ((!(48 <= (int) (char_array[i]) && (int) (char_array[i]) <= 57)) && ((int) (char_array[i]) != 46) &&
            ((int) (char_array[i]) != 45))
            throw "illegal variable assignment!";
        if (numDot > 1)
            throw "illegal variable assignment!";
    }
    double val = stod(value);
    // Checking if the key already exists
    map<string, double>::iterator itrEnd;
    map<string, double>::iterator itrFind;
    itrEnd = this->variables.end();
    itrFind = this->variables.find(key);
    if (itrEnd->first == key) {
        itrEnd->second = val;
        return;
    } else if (itrFind != itrEnd) {
        itrFind->second = val;
        return;
    }
    delete[] (char_array);
    delete[] (key_arr);
    this->variables.insert(pair<string, double>(key, val));
}

Expression *Interpreter::reversePolish(queue<string> queue) {
    stack<Expression *> stack;
    while (!queue.empty()) {
        string str = queue.front();
        bool flag = false;
        if (!isPolishOperator(str)) {
            map<string, double>::iterator itr;
            for (itr = this->variables.begin(); itr != this->variables.end(); ++itr) {
                if (itr->first == str) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                // if it's one of the variables
                stack.push(new Variable(itr->first, itr->second));
            } else {
                // making sure it's a number and not an invalid variable
                int n = str.length();
                char *char_array = new char[n + 1];
                strcpy(char_array, str.c_str());
                for (int j = 0; j < n; j++) {
                    if (!(48 <= (int) (char_array[j]) && (int) (char_array[j]) <= 57) && char_array[j] != 46)
                        throw "Bad Input";
                    if (j == 0 && char_array[j] == 46)
                        throw "Bad Input";
                }
                delete[] (char_array);
                stack.push(new Value(stod(str)));
            }
        } else {
            // Checking which operator
            if (str[0] == '$' || str[0] == '%') {
                Expression *expression = stack.top();
                stack.pop();
                if (str[0] == '$') {
                    stack.push(new UPlus(expression));
                }
                if (str[0] == '%') {
                    stack.push(new UMinus(expression));
                }
            } else {
                // Reverse order
                Expression *expression1 = stack.top();
                stack.pop();
                Expression *expression2 = stack.top();
                stack.pop();
                if (str[0] == '+') {
                    stack.push(new Plus(expression2, expression1));
                }
                if (str[0] == '-') {
                    stack.push(new Minus(expression2, expression1));
                }
                if (str[0] == '*') {
                    stack.push(new Mul(expression2, expression1));
                }
                if (str[0] == '/') {
                    stack.push(new Div(expression2, expression1));
                }
            }
        }
        queue.pop();
    }
    return stack.top();

}

bool Interpreter::isOperator(char c) {
    return c == '-' || c == '+' || c == '*' || c == '/' || c == '(' ||
           c == ')';
}

bool Interpreter::isPolishOperator(string c) {
    return c[0] == '-' || c[0] == '+' || c[0] == '*' || c[0] == '/' || c[0] == '$' || c[0] == '%';
}

int Interpreter::precedence(char c) {
    if (c == '$' || c == '%') {
        return 3;
    } else if (c == '*' || c == '/') {
        return 2;
    } else if (c == '+' || c == '-') {
        return 1;
    } else if (c == '(' || c == ')') {
        return 0;
    } else
        return (-1);
}