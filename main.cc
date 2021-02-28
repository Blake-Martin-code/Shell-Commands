#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <climits>
#include <sys/types.h>  // fork() & wait()
#include <unistd.h>     // fork(), execvp()
#include <cstdlib>      // exit()
#include <wait.h>       // wait()
#include <fstream>

using namespace std;

bool execute(vector<string> command);

void emit(vector<string> stringV){
// print out each string in the vector with format "Word is "word"\n
        for (long unsigned int i = 0; i < stringV.size(); i++){
                cout << "Word is \"" << stringV[i] << "\"\n";
}
}

int wordMax(vector<string> v){
// return the maximum size length for the strings in the given vector
        long unsigned int max = 0;
        for (long unsigned int i = 0; i < v.size(); i++){
                if (v[i].size() > max) max = v[i].size();
}
        return max;
}

int wordMin(vector<string> v){
// return the minimum size length for the strings in the given vector
        long unsigned int min = INT_MAX;
        for (long unsigned int  i = 0; i < v.size(); i++){
                if (v[i].size() < min) min = v[i].size();
}
        return min;
}

vector<string> removeParens(vector<string> v){
        vector<string> newV;
        for(long unsigned int i = 0; i < v.size(); i++){
                if((v[i] == "(") || (v[i] == ")")){
                        continue;
                }
                else{
                        newV.push_back(v[i]);
                }
        }
        return newV;
}
void vectorPrint(vector<string> v){
        for (long unsigned int i = 0; i < v.size(); i++){
                cout<< v[i] << " ";
}
        cout<< "\n";
}

bool unescapedParens(vector<string> v){
        //if there is a unescaped parens that is not left paren as second word
        // or a right paren as last word return true, else return false
        for(long unsigned int i = 2; i < v.size()-2; i++){
                if(v[i] == "(" || v[i] == ")"){
                        return true;
                }
        }
        return false;
}

bool missingParens(vector<string> v){
        // if secon word is not an open paren or last
        //word is not a closing paren return true
        // else return false
        if(v[1] != "(" || v[v.size()-1] != ")") return true;
        return false;
}



void lineParse(string line, vector<string> &V, string fileName){
        string temp;
        while(line.size() > 0){
                
                int firstChar = line.find_first_of(" \f\n\r\t\v \\ ()");

                if(firstChar == 0){
                        // "echo(this is \a test)" 
                        if(line[firstChar] == '\\'){
                                temp.push_back(line[firstChar+1]);
                                line.erase(0,2);
                        }
                        else if (line[firstChar] == '('){
                                V.push_back("(");
                                line.erase(0,1);

                        }
                        else if (line[firstChar] == ')'){
                                if (line[firstChar+1] == ')'){
                                        V.push_back(")");
                                        line.erase(1,1);
                                }
                                V.push_back(")");
                                line.erase(0,1);
                                if(missingParens(V)){
                                        cerr<< fileName<< ": missing opening or closing parens: ";
                                        vectorPrint(V);
                                        exit(1);
                                }
                                if(unescapedParens(V)){
                                        cerr<< fileName<< ": unescaped parens: ";
                                        vectorPrint(V);
                                        exit(1);
                                }
                                if(!execute(removeParens(V))){
                                        cerr<< fileName<< ": Can't run command: ";
                                        vectorPrint(V);
                                        exit(1);
                                }
                                V.clear();
                                temp.clear();
                                
                        }
                        else{
                                if(temp.empty()){
                                        line.erase(0,1);
                                }
                                else{
                                        V.push_back(temp);
                                        temp.clear();
                                        line.erase(0,1);
                                }
                        }
                }
                else{
                        // "what\ifthis\isit "
                        // echo(what\if this\isit))
                        if(line[firstChar] == '\\'){
                                temp.append(line.substr(0,firstChar));
                                temp.push_back(line[firstChar+1]);
                                line.erase(0,firstChar+2);
                        }
                        else if(line[firstChar] == '('){
                                temp.append(line.substr(0,firstChar));
                                V.push_back(temp);
                                V.push_back("(");
                                line.erase(0,firstChar+1);
                                temp.clear();
                        }
                        else if(line[firstChar] == ')'){
                                if(line[firstChar+1] == ')'){
                                        temp.append(line.substr(0,firstChar));
                                        V.push_back(temp);
                                        V.push_back(")");
                                        V.push_back(")");
                                        line.erase(0,firstChar+2);
                                        temp.clear();
                                }
                                else{
                                        temp.append(line.substr(0,firstChar));
                                        V.push_back(temp);
                                        V.push_back(")");
                                        line.erase(0,firstChar+1);
                                        temp.clear();
                                }
                                if(missingParens(V)){
                                        cerr<< fileName<< ": missing opening or closing parens: ";
                                        vectorPrint(V);
                                        exit(1);
                                }
                                if(unescapedParens(V)){
                                        cerr<< fileName << ": unescaped parens: ";
                                        vectorPrint(V);
                                        exit(1);
                                }
                                if(!execute(removeParens(V))){
                                        cerr<< fileName<< ": Can't run command: ";
                                        vectorPrint(V);
                                        exit(1);
                                }
                                V.clear();
                                temp.clear();

                        }
                        else{
                                temp.append(line.substr(0,firstChar));
                                V.push_back(temp);
                                line.erase(0,firstChar+1);
                                temp.clear();
                        }
                }
        }

}

bool execute(vector<string> command) {
    // Convert vector of C++ strings to an array of C strings for exectp():
    vector<char *> argv;                // vector of C string pointers
    for (auto &v : command)             // for each C++ string:
        argv.push_back(v.data());       //    add address of C-string inside it
    argv.push_back(nullptr);            // argv ends with a null pointer

    // Duplicate ourself: -1:error, 0:child, >0:parent
    const auto pid = fork();            // step #1: copy ourself
    if (pid < 0)
        return false;                   // fork failure?
    if (pid == 0) {                     // child?
        execvp(argv[0], argv.data());   // step #2: replace child with new prog
        exit(0xff);                     // only got here if execvp() failed.
    }
    int status;
    wait(&status);                      // step #3: wait for child to finish
    return status != 0xff00;            // true iff we succeeded
}

int main(int argc, char *argv[]){
        string line;
        vector<string> v;
        string hwName = string(argv[0]);
        if(argc > 1){
                for(int i = 1; i < argc; i++){
                        string fileName = string(argv[i]);
                        ifstream in(fileName);
                        if(!in.is_open()){
                                cerr<< hwName<< ": cannot be opened for reading: "<< fileName<< "\n";
                                exit(1);
                        }
                        while(getline(in, line)){
                                line.append("\n");
                                lineParse(line, v, hwName);
}
}
}
        else{
                // iterate through and get every line in the text document
                while(getline(cin, line)){
                        line.append("\n");
                        lineParse(line , v, hwName);
}
}
        // determine if there are any words in the given file
        // if there is not send a message to error output
//         if (v.size() == 0){
//                 cerr << argv[0] << ": no words were encountered\n";
//                 return 0;
// }
//      emit(v);
//      executeVectorArgs(v, hwName);

        //cout << "Words: " << v.size() << "\n";
        //cout << "Word range: " << wordMin(v) << "-" << wordMax(v) << "\n";
        return 0;


}



