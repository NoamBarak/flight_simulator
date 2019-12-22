//
// Created by noam on 11/11/2019.
//
#include <cstring>
#include <stack>
#include <regex>
#include "Interpreter.h"
#include "map"
#include "math.h"
#include "queue"

void Interpreter::setVariables(string str) {
    int len = str.length() + 1;
    //char varArr[len];
    char *varArr = new char[len];
    strcpy(varArr, str.c_str());
    vector<string> vec;
    char *splitArr = strtok(varArr, ";");
    while (splitArr) {
        vec.push_back(splitArr);
        splitArr = strtok(NULL, ";");
    }
    int vecSize = vec.size();
    for (int i = 0; i < vecSize; i++) {
        int place = vec[i].find("=");
        if (!isNumber(vec[i].substr(place + 1, vec[i].length() - 1))) {
            throw ("illegal variable assignment!");
        }
        double number;
        number = stod(vec[i].substr(place + 1, vec[i].length() - 1));
        string varName = vec[i].substr(0, place);
        if (!std::regex_match(varName, std::regex("[A-Za-z_][A-Za-z0-9_]*"))) {
            throw ("illegal variable name!");
        }
        //check if already a key
        if (varNval.count(vec[i].substr(0, place))) {
            auto it = varNval.find(vec[i].substr(0, place));
            varNval.erase(it);
        }
        this->varNval.insert(pair<string, double>(vec[i].substr(0, place), number));
    }
}

Expression *Interpreter::interpret(string st) {
    queue<string> q;
    int place;
    string sub, last = "noOp", oper;
    // check if there are two operators in a row
    bool lastIndex = (isOp(st.at(0)));
    bool curIndex;
    int stLength1 = st.length();
    for (int i = 1; i < stLength1; i++) {
        curIndex = isOp(st.at(i));
        if (lastIndex && curIndex) {
            throw "illegal math expression";
        }
        lastIndex = isOp(st.at(i));
    }
    if(!checkParen(st))
        throw ("illegal math expression");
    //split st according to operators
    int stLength2 = st.length();
    for (int i = 0; i < stLength2;) {
        place = findPlace(st, i);//place of the operator
        if (place != -1) {//push the chars between two operators
            sub = st.substr(i, place - i);
        } else
            sub = st.substr(i);//if the last place not operator
        if ((place - i) > 0) {
            //check if 'sub' is a key. if TRUE- push the value to q(queue)
            if (varNval.count(sub)) {
                auto it = varNval.find(sub);
                q.push(to_string((it->second)));
            }
                //if FALSE- push sub to q (queue)
            else {
                if (!isNumber(sub)) {
                    throw ("illegal math expression");
                }
                q.push(sub);
            }
        }
        //push the operator
        if (place != -1) {
            oper = st.substr(place, 1);
            if (place > 1) {
                last = st.substr(place - 1, 1);
            }
            //check if unary operator
            if (last == "(" || last == "noOp") {
                if (oper == "-")
                    oper = "$";//new sign for unary minus
                else {
                    if (oper == "+")
                        oper = "#";//new sign for unary plus
                }
            }
            q.push(oper);
            i = place + 1;//the index after the operator
        } else {
            q.push(st.substr(i));
            break;
        }

    }

    double num = evaluate(q);
    return (new Value(num));
}


double Interpreter::evaluate(queue<string> tokens) {
    queue<string> outputQueue;
    stack<string> opStack;//operators stack
    stack<double> numbersStack;
    bool opOrnot;
    while (!tokens.empty()) {
        if (regex_match(tokens.front(), std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")))) {
            outputQueue.push(tokens.front());
        }
        opOrnot = isOp(tokens.front().at(0));
        int stackPre = -1;
        if (opOrnot) {
            if (!opStack.empty())
                stackPre = precedence(opStack.top().at(0));
            int tokenPre = precedence(tokens.front().at(0));
            while (((stackPre >= tokenPre)) && (!opStack.empty())) {
                outputQueue.push(opStack.top());
                opStack.pop();
            }
            opStack.push(tokens.front());
        }

        char whichParen = tokens.front().at(0);
        if (whichParen == '(') {
            opStack.push(tokens.front());
        }
        if (whichParen == ')') {
            while (opStack.top() != "(") {
                outputQueue.push(opStack.top());
                opStack.pop();
            }
            if (opStack.top() == "(") {
                opStack.pop();
            }
        }
        tokens.pop();

    }
    while (!opStack.empty()) {
        outputQueue.push(opStack.top());
        opStack.pop();
    }

    opOrnot = isOp(outputQueue.front().at(0));
    while (!outputQueue.empty()) {
        opOrnot = isOp(outputQueue.front().at(0));
        while (!opOrnot) {
            numbersStack.push(stod(outputQueue.front()));
            outputQueue.pop();
            opOrnot = isOp(outputQueue.front().at(0));
        }
        if (!outputQueue.empty()) {
            string op = outputQueue.front();
            outputQueue.pop();
            if (op != "$" && op != "#") {//binary calculation
                double num2 = numbersStack.top();
                numbersStack.pop();
                double num1 = numbersStack.top();
                numbersStack.pop();
                double ans = binaryCalc(num1, num2, op);
                numbersStack.push(ans);
                //cout << "ans-    " << ans << endl;
            } else { //unary calculation
                double num = numbersStack.top();
                numbersStack.pop();
                double ans = unaryCalc(num, op);
                numbersStack.push(ans);
                //   cout << "ans-    " << ans << endl;

            }
        }
    }
    return (numbersStack.top());

}

double Interpreter::unaryCalc(double num, string op) {
    double ans;
    if (op == "#") {
        ans = UPlus(new Value(num)).calculate();
    } else if (op == "$") {
        ans = UMinus(new Value(num)).calculate();
    }
    return ans;
}

double Interpreter::binaryCalc(double num1, double num2, string op) {
    double ans;
    if (op == "+") {
        ans = Plus(new Value(num1), new Value(num2)).calculate();
    }
    if (op == "-") {
        ans = Minus(new Value(num1), new Value(num2)).calculate();
    }
    if (op == "*") {
        ans = Mul(new Value(num1), new Value(num2)).calculate();
    }
    if (op == "/") {
        ans = Div(new Value(num1), new Value(num2)).calculate();
    }
    return ans;
}


//return the index of the closest operator
int Interpreter::findPlace(string st, int index) {
    int place;
    char options[] = {'(', ')', '+', '-', '*', '/'};
    place = st.find_first_of(options, index);
    return place;
}


//if operator- return TRUE. else- return FALSE (using ASCII table)
bool Interpreter::isOp(char ch) {
    if (ch >= 42 && ch <= 47) {//ASCII table
        if (ch != 44 && ch != 46)//44, 46- not operators
            return true;
        return false;
    }
    if (ch == '#' || ch == '$')
        return true;
    return false;

}

int Interpreter::precedence(char op) {
    if (op == '$' || op == '#') {
        return 0;
    }

    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    return -1;
}


bool Interpreter::checkParen(string st) {
    stack<char> parenStack;
    int stLength = st.length();
    for (int i = 0; i < stLength; i++) {
        if (st.at(i) == '(') {
            parenStack.push('(');
        }
        if (st.at(i) == ')') {
            if (parenStack.empty())
                return false;
            parenStack.pop();
        }
    }
    if (!parenStack.empty())
        return false;
    return true;
}

bool Interpreter::isNumber(const std::string &st) {
    int count = 0; // count the dots
    if (st.empty())
        return false;
    int i = 1;
    if (((st.at(0) != '-')) && (!(isdigit(st.at(0))))) {
        return false;
    }
    int sLength = st.length();
    while (i < sLength) {
        if (st.at(i) == '.') {
            count++;
            if (count > 1) {
                return false;
            }
        } else if (!isdigit(st.at(i))) {
            return false;
        }
        i++;
    }
    return true;
}