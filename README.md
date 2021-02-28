# Shell-Commands
#Description                
#For this assignment, you will write a primitive shell that will read & execute commands using functional notation, sort of like sin(x ) in math.                 

#Input                
#Read & execute commands from each filename argument. If no arguments are given, read & execute commands from standard input.                 

#Input format                
#The input will be a series of lines. Each line will consist of a number of whitespace-separated words. The first word in a line will be the command, the following words will be arguments. Ignore lines that contain no words.                 

#A backslash escapes the next character, making it not special. The backslash itself does not become part of the word. For example, foo\ bar is treated as the seven-character word foo bar, S\pock\(o\) is treated as the eight-character word Spock(o), \ Kirk\  is treated as the six-character word  Kirk , and Bo\nes is treated as the five-letter word Bones. The effect of a backslash as the last character of a line is undefined. Unlike C++, \n and \t are not translated into newline and tab. In the examples below, the echo program is doing that translation, not hw2.                 

#( and ) are always words by themselves, even if they occur next to other text, unless escaped.                 

#A non-empty input line must be of this form:

#command(optional arguments)
#for example:

#echo(hi there)
#If an input line is not of this form, produce an error message and stop the program.                 

#No special shell syntax is recognized. For example, the characters #*[];'"><|&~ have no particular significance.                 

#Language                
#I call “(” and “)” parentheses, or parens. Some people call them brackets.                 

#Executing commands                
#The file ~cs253/Example/execute.cc contains this function:                 

    bool execute(vector<string> command);
It executes a command with arguments, returning true iff it succeeds. It also contains a main() that you should exclude from your program. You may copy code from this file and add it to your main.cc.                 

Of course, execute() doesn’t want those opening and closing parens. They shouldn’t be in the vector that you pass to execute(). If the input command is echo(alpha beta\ gamma delta), then the vector passed to execute() should contain:

echo
alpha
beta gamma
delta

EXAMPLE FILE AND RUN

% cat data
date()
echo(Good so far.)
        echo   (   \ 1   <    2 * 3 + 11162  &\&    44    >   5   )    
echo(-e 1\)  dogs\\n2\) cats)
sync()
echo(-e supercali\\n\\t\(fragilistic\)\\n\\t\\texpialidocious)
date(+%F\ %T)
goofy(this will not work)
echo(0001001001110010)
% ./hw2 <data
Wed Feb 17 16:34:28 MST 2021
Good so far.
 1 < 2 * 3 + 11162 && 44 > 5
1) dogs
2) cats
supercali
        (fragilistic)
                expialidocious
2021-02-17 16:34:28
./hw2: can’t run: goofy(this will not work)
% cat d1
  date (  )
% cat d2
echo(Hello from d2!)
% ./hw2 d1 d2
Wed Feb 17 16:34:28 MST 2021
Hello from d2!
% echo -e ' \n\r\techo\f(\talpha\fbeta\v332402) ' | ./hw2
alpha beta 332402
