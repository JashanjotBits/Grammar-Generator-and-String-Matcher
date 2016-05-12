%{#include "y.tab.h"
#include <stdio.h>

%}

%%
"("  {//printf("( ");
	  return(OPENBRACKET);
	}
")"  {//printf(") ");
	  return(CLOSEBRACKET);
	}
"{"  {//printf("{ ");
	  return(OPENBRACE);
	}
"}"  {//printf("} ");
	 return(CLOSEBRACE);
	}
","  {//printf(", ");
	  return(COMMA);
	}
[A-Za-z0-9] {
		yylval.a_char = yytext[0];
		//printf("%s ",yytext);
		return (CHAR);
		}
[ \t\n]     ;
.       {ECHO;
		 yyerror ("unexpected character");
		}

%%

 