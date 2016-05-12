%{#include "y.tab.h"
#include <stdio.h>

%}

%%
"("  return(OPENBRACKET);
")"  return(CLOSEBRACKET);
";"  return (SEMICOLON);
[-+*/] {yylval.an_op=yytext[0];
		return(operator);}
[0-9]+ {yylval.a_number = atoi(yytext); return number;};
[ \t\n]     ;
.       {ECHO;
		 yyerror ("unexpected character");
		}
%%