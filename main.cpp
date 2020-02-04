#include <iostream>
#include <fstream>
#include <string>
#include "ArgumentManager.h"
struct NodeS{
    std::string data;
    NodeS*next{};
};
class stringStack{
public:
    NodeS* top;
    bool isEmpty(){
        return top == nullptr;
    }
    stringStack(){top= nullptr;}
    std::string peek();
    void push(std::string c);
    std::string pop();
};
std::string stringStack::peek() {
    NodeS*p = top;
    return p->data;
}
void stringStack::push(std::string c){
    auto*p = new NodeS;
    p->data = c;
    p->next = top;
    top = p;
}
std::string stringStack::pop() {
    if(isEmpty())
        return "";
    NodeS*p = top;
    top = top->next;
    std::string x = p->data;
    free(p);
    return x;
}
///////////////////////////////////////////////////
struct Node{
    char data;
    Node*next;
};
class stack{
private:
    Node* top;
public:
    stack(){top= nullptr;}
    bool isEmpty(){
        return top == nullptr;
    }
    char peek();
    void push(char c);
    char pop();
};
char stack::peek(){
        Node *t = top;
        char x = t->data;
        return x;
    }
void stack::push(char c) {
    Node* p  = new Node;
    p->data = c;
    p->next = top;
    top = p;
}
char stack::pop(){
    Node* p = top;
    char x = p->data;
    top = top->next;
    free(p);
    return x;
}
bool isParanthesisValid(std::string line,ofstream& write){
    stack stack2;
    while(!stack2.isEmpty()){
        stack2.pop();
    }
    for(int i = 0;i<line.length();i++)
    {
        if(line[i] == '('){
            stack2.push(line[i]);
        }
        else if(line[i] ==')')
        {
            if(i != line.length()-1)
            {
                if(line[i+1] == '(')
                {
                    write << "error" << std::endl;
                    return false;
                }
            }
            if(stack2.isEmpty())
            {
                write << "error" << std::endl;
                return false;
            }
            stack2.pop();
        }
    }
    if(!stack2.isEmpty())
    {
        write << "error" << std::endl;
        return false;
    }
    return true;
}

bool isOperator(char a){
    if(a == '+' || a=='-' || a=='/' || a=='*');
        return true;
}
bool hasNumbers(std::string line,ofstream& write){
    for(int i=0;i<line.length();i++){
        if(isdigit(line[i]))
            return true;
    }
    write << "error" << std::endl;
    return false;
}

bool isErrorforOperators(std::string line,ofstream& write){
    if(!isOperator(line[line.length()-1])) {
        write << "error" << std::endl;
        return true;
    }
    for(int i = 0;i<line.length();i++)
    {
        if(i+1<line.length())
        {
                if (!isdigit(line[i]) && (!isdigit(line[i + 1])))
                {
                    if(line[i+1] == '(' || line[i+1] == ')' || line[i] == '(' || line[i] == ')')
                        {return false;}
                    write << "error" << std::endl;
                    return true;
                }
        }
    }
    return false;
}
int precedence(char c){
    if(c == '+' || c == '-')
        return 1;
    else if(c== '*' || c =='/')
        return 2;
    return 0;
}

bool hasError(std::string line, ofstream& write){
    if(line.find_first_not_of("0123456789=*+-/()") != std::string::npos){
        write << "error " << std::endl;
        return true;
    }
    return false;
}
bool hasmoreOper(std::string line, ofstream& write){
    int n = 1;
    int o = 0;
    for(int i = 0;i<line.length();i++){
        if(isdigit(line[i]))
            n++;
        else
            o++;
    }
    if(n==1 && o>n){
        write << "error" << std::endl;
        return true;
    }
    return false;
}
int main(int argc,char*argv[]) {
    ArgumentManager am(argc,argv);
    const std::string in = am.get("input");
    const std::string out = am.get("output");
    std::string line,temp;
    std::string postFix;
    long long int num1= 0;
    long long int num2 = 0;
    std::ifstream read;
    long long int result = 0;
    std::ofstream write;
    std::string token;
    std::string zero = "0";
    read.open(in);
    write.open(out);
    stack stack1;
    while (getline(read, line)) {
        int x;
        int i = 0;
        if (line[line.length() - 1] == '\r') {
            line.erase(line.size() - 1); //To delete the trailing \r
        }
        temp = line;
        if (!line.empty()) {
            result = 0;
            postFix = "";
            if ((isParanthesisValid(line,write)) &&
                (!isErrorforOperators(line,write)) && !hasError(line,write) && !hasmoreOper(line,write))
            { //Check if the equation is valid for Parenthesis
                line.insert(0, zero);
                if (line[0] == '-') {
                    x = 3;
                    line.insert(0, zero);
                } else {
                    x = 1;
                }
                while (x < line.length()) {
                    if (line[x] == '-') {
                        if (x > 0 && (line[x - 1] == ')' || isdigit(line[x-1]))) {
                            x++;
                        } else {
                            line.insert(x, zero);
                            x += 2;
                        }
                    } else {
                        x++;
                    }
                }
                while (i < line.length()) { //GO THROUGHT THE LINE INDEX i
                    if (isdigit(line[i])) {
                        token = "";
                        while (isdigit(line[i])) {
                            token += line[i];
                            i++;
                        }
                        postFix += token + " ";
                    } else if (!isdigit(line[i])) {
                        if (stack1.isEmpty()) {
                            stack1.push(line[i]);
                            i++;
                        } else {
                            if (!stack1.isEmpty()) {
                                if (stack1.peek() == '(') {
                                    stack1.push(line[i]);
                                } else if (line[i] == '(') { stack1.push(line[i]); }
                                else if (line[i] == ')') {
                                    while (stack1.peek() != '(') {
                                        std::string temp1 = std::string(1, stack1.pop());
                                        temp1 += " ";
                                        postFix += temp1;
                                    }
                                    if (!stack1.isEmpty())
                                        stack1.pop();
                                } else if (!stack1.isEmpty() &&
                                           ((precedence(line[i]) > precedence(stack1.peek())))) {
                                    stack1.push(line[i]);
                                } else if ((!stack1.isEmpty() && (precedence(line[i]) < precedence(stack1.peek()))) ||
                                           precedence(line[i]) == precedence(stack1.peek())) {
                                    if (!stack1.isEmpty()) {
                                        std::string tempString = std::string(1, stack1.pop());
                                        //if(precedence(stack1.peek()) > precedence(line[i]) || precedence(stack1.peek()) == precedence(line[i]))
                                        postFix += tempString + " ";
                                        stack1.push(line[i]);
                                    }
                                }
                            }
                            i++;
                        }
                    }
                }
                while (!stack1.isEmpty()) {
                    std::string tempString = std::string(1, stack1.pop());
                    //if(precedence(stack1.peek()) > precedence(line[i]) || precedence(stack1.peek()) == precedence(line[i]))
                    postFix += tempString + " ";
                }
                //Empty out stack
                std::string evaluationTokens;
                stringStack stringStack1{};
                std::stringstream ss(postFix);
                while (ss >> evaluationTokens) {
                    if (isdigit(evaluationTokens[0])) {
                        stringStack1.push(evaluationTokens); //Pushes number token in stack
                    } else {
                        if (evaluationTokens == "+") {
                            if (!stringStack1.isEmpty()) {
                                num1 = stoll(stringStack1.pop());
                            }
                            if (!stringStack1.isEmpty()) {
                                num2 = stoll(stringStack1.pop());
                            }
                            result = num2 + num1;
                            std::stringstream sr;
                            sr << result;
                            std::string pushResult;
                            sr >> pushResult;
                            stringStack1.push(pushResult);
                        } else if (evaluationTokens == "-") {
                            if (!stringStack1.isEmpty()) {
                                num1 = stoll(stringStack1.pop());
                            }
                            if (!stringStack1.isEmpty()) {
                                num2 = stoll(stringStack1.pop());
                            }
                            result = num2 - num1;
                            std::stringstream sr;
                            sr << result;
                            std::string pushResult;
                            sr >> pushResult;
                            stringStack1.push(pushResult);
                        } else if (evaluationTokens == "*") {
                            if (!stringStack1.isEmpty()) {
                                num1 = stoll(stringStack1.pop());
                            }
                            if (!stringStack1.isEmpty()) {
                                num2 = stoll(stringStack1.pop());
                            }
                            result = num2 * num1;
                            std::stringstream sr;
                            sr << result;
                            std::string pushResult;
                            sr >> pushResult;
                            stringStack1.push(pushResult);
                        } else if (evaluationTokens == "/") {
                            if (!stringStack1.isEmpty()) {
                                num1 = stoll(stringStack1.pop());
                            }
                            if (!stringStack1.isEmpty()) {
                                num2 = stoll(stringStack1.pop());
                            }
                            result = num2 / num1;
                            std::stringstream sr;
                            sr << result;
                            std::string pushResult;
                            sr >> pushResult;
                            stringStack1.push(pushResult);
                        }
                    }
                }
                write << temp << "=" << result << std::endl;

            }
        } else {
            write << "error" << std::endl;
    }
    }
    if(line.empty()){
        write << "error" << std::endl;
    }

    read.close();
    write.close();
    return 0;
}
