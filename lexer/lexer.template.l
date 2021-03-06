/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
yyFlexLexer lexer;
#include "parser.hxx"
%}


%option noyywrap c++
%option yylineno

DIGIT    [0-9]
ID       [a-z_][A-Za-z0-9_]*
TYPE_ID       [A-Z][A-Za-z0-9_]*
%x       C_COMMENT
%%
{DIGIT}+    {
                    yylval.unode_ptr_t = mkInteger(atoi(lexer.YYText()));
                    return NUM_int;
            }

[(-|+)]{DIGIT}+"."{DIGIT}*[(e|E)(-|+){DIGIT}+]        {
                    yylval.unode_ptr_t = mkDouble(atof(lexer.YYText()));
                    return NUM_double;
            }

@#lexer_rule_list#@

{ID}        { yylval.unode_ptr_t = mkIdentifier(lexer.YYText());return ID; }
{TYPE_ID}        { yylval.unode_ptr_t = mkTypeUser(lexer.YYText()); return TYPE_ID; }

"//".*\n    
"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>.    { }     

[ \t\n]+          /* eat up whitespace */

<<EOF>>     { return END_OF_FILE;}

.           { cout << "Unrecoginized token" << endl;}

%%

int yylex (void){

  int enum_num = lexer.yylex();
  // cout << enum_num << " <=> " << lexer.YYText() << endl;
  return enum_num;
}

void yyerror (char const * err_msg){
  cerr << err_msg << endl;
}



// int main(int argc, char* argv[] )
// {
//   while(1){
//     int enum_num = lexer.yylex();
//     cout << enum_num << " <=> " << lexer.YYText() << endl;
//     if(enum_num == END_OF_FILE){
//       break;
//     }
//   }
// }
