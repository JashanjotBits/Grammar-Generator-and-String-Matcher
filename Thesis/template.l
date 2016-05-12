%{#include "y.tab.h"
#include <stdio.h>

%}

%%
//if token not an identifier

"(token)"   return(tokenName);

//if the token is an identifier 

[identifierPattern]  {
						//code to extract the value of identifier
						yyval.identifier_name= tokenValue;
						return(tokenName);
					}

[ \t\n]     ;
.       {ECHO;
		 yyerror ("unexpected character");
		}


%%

