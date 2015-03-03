/*
 * file:  dpp.l
 * ------------
 * Lex inupt file to generate the scanner for the preprocessor.
 * If you decide to use lex for the preprocesor, put your rules
 * here, otherwise the file can remain empty.
 */

%{
    #include <map>
    #include <string.h>
    #include "scanner.h"
    #include "utility.h"
    #include "errors.h"
    #include <iostream>
    std::map<string, string> defines;
%}

%%


(\")[^\n\"]*(\") {ECHO;}
"//".* {}
"/*"[^("*/")]* {ReportError::UntermComment();}
"/*"[^("*/")]*"*/" {for(int i = 0; yytext[i] != '\0'; ++i)
                        if(yytext[i] == '\n')
                               printf("\n");}
#(define)[[:blank:]]+[A-Z]*[[:blank:]]+.* {string def = yytext;
                int divide = def.find(' ', 8);
                defines[def.substr(8, divide-8)] = def.substr(divide+1);}


#[A-Z]* {string temp = yytext;
        string output =  defines[temp.substr(1)];
        if(defines.find(temp.substr(1)) == defines.end())
            ECHO;
        else printf("%s", output.c_str());}

%%
