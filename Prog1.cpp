/*
Name: Mohammad Khalil, Jayden Garcia, Gage Ashbaugh
CPSC 323 Project Assignment 2
Miss Susmitha Padda
Date: 4/30/2023
*/

#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>

using namespace std;

const string startSymbol = "$";
const vector<string> nonTerminals = {"E", "T", "F", "Q", "R"};
const vector<string> terminals = {"a", "+", "-", "*", "/", "(", ")"};

unordered_map<string, vector<string>> productions = {
    {"E", {"TQ"}},
    {"T", {"FR"}},
    {"Q", {"+TQ", "-TQ", "epsilon"}},
    {"R", {"*FR", "/FR", "epsilon"}},
    {"F", {"(E)", "a"}}
};

unordered_map<string, vector<string>> first = {
    {"E", {"(", "a"}},
    {"T", {"(", "a"}},
    {"Q", {"+", "-", "epsilon"}},
    {"R", {"*", "/", "epsilon"}},
    {"F", {"(", "a"}}
};

unordered_map<string, vector<string>> follow = {
    {"E", {"$", ")"}},
    {"T", {"$", ")", "+", "-"}},
    {"Q", {"$", ")"}},
    {"R", {"$", ")", "+", "-"}},
    {"F", {"$", ")", "+", "-", "*", "/"}}
};

unordered_map<string, unordered_map<string, string>> table = {
    {"E", {{"a", "TQ"}, {"(", "TQ"}}},
    {"T", {{"a", "FR"}, {"(", "FR"}}},
    {"Q", {{"+", "+TQ"}, {"-", "-TQ"}, {")", "epsilon"}, {"$", "epsilon"}}},
    {"R", {{"+", "epsilon"}, {"-", "epsilon"}, {"*", "*FR"}, {"/", "/FR"}, {")", "epsilon"}, {"$", "epsilon"}}},
    {"F", {{"a", "a"}, {"(", "(E)"}}}
};
void PrintStack(stack<string> s)
{
    // If stack is empty then return
    if (s.empty()) {
        return;
    }

    string x = s.top();
 
    // Pop the top element of the stack
    s.pop();
 
    // Recursively call the function PrintStack
    PrintStack(s);
 
    // Print the stack element starting
    // from the bottom
    cout << x << " ";
 
    // Push the same element onto the stack
    // to preserve the order
    s.push(x);
}

bool traceInputString(string inputString) {
    stack<string> parseStack;
    parseStack.push("$");
    parseStack.push("E");
    int inputStringIndex = 0;
    while (!parseStack.empty()) {
        string top = parseStack.top();
        
        parseStack.pop();
        PrintStack(parseStack);
       
        if (find(nonTerminals.begin(), nonTerminals.end(), top) != nonTerminals.end()) {
            string inputStringChar = string(1, inputString[inputStringIndex]);
            if (table[top].find(inputStringChar) == table[top].end()) {
                cout << "String is not accepted/invalid" << endl;
                return false;
            }
            string production = table[top][inputStringChar];
            cout << setw(15) << top << " -> " << production;
            if (production != "epsilon") {
                for (int i = production.length() - 1; i >= 0; i--) {
                    parseStack.push(string(1, production[i]));
                }
            }
        } else if (find(terminals.begin(), terminals.end(), top) != terminals.end()) {
            if (top == string(1, inputString[inputStringIndex])) {
                inputStringIndex++;
            } else {
                cout << "String is not accepted/invalid" << endl;
                return false;
            }
        } else if (top == "$") {
            if (inputStringIndex == inputString.length() - 1) {
                cout << "String is accepted/valid" << endl;
                return true;
            } else {
                cout << "String is not accepted/invalid" << endl;
                return false;
            }
        } else {
            cout << "String is not accepted/invalid" << endl;
            return false;
        }
        cout << endl;
    }
    cout << "String is not accepted/invalid" << endl;
    return false;
}

int main() {
    string inputString;
    cout << "Input: ";
    cin >> inputString;
    traceInputString(inputString);
    return 0;
}