%{
 #include <stdio.h>
 #include <stdlib.h>
enum treetype {//names of all the possible node types};
struct Tree {
	enum treetype nodetype;
   	union {                             //this union structure is needed incase the node can have varying arity
   		union{	
   			union{

   			}left;

   			union{

   			}right;						

   		}binaryNode;

   		union{

   		}trinaryNode;
   		.
   		.
   		.
   		.
   		union{

   		}nAryNode;
   		   
   	} node;
   
};

static tree *make_treeNode1(){
 			//function definition
}

static tree *make_treeNode2(){
 			//function definition
}

.
.
.
.

static tree *make_treeNodeN(){
 			//function definition
}

static void printtree (tree *t, int level) {
 int i;
 #define step 4
   if (t)
     switch (t->nodetype)
     {
       for(i=0;i<level;i++)
       		printf(" ");
       case node1:
       	//print statement and recursive call to it's child nodes
        break;
       case node2:
       	//print statement and recursive call to it's child nodes
        break;
       .
       .
       .
       .
       case nodeN:
       	//print statement and recursive call to it's child nodes
        break;
        
     }
 }

%}

%union {
   //declare all the datatypes used in the grammar
 }

%start startingNonTerminal
%token //list of tokens
%type type1 //list of non terminals having type1
%type type2 //list of non terminals having type2
.
.
.
.
%type typeN //list of non terminals having typeN

%%

//$$ represents the value returned by the rule. In other words it's the value of the left hand side variable
//Similarly, the values of right hand sides variables (non teminals) are $1,$2,.....,SN,depending on the order in which they appear in //the rule
startingNonTerminal : rule {
					  $$=make_tree(//parameters);
		  			  printtree($$,0);
		  			  exit(0);	
		  			};

rule2           	:  terminals and non terminals     {
						//action for the rule. The function called in this function generally has $1, $2 etc. as parameters
	
						};

.
.
.
.
ruleN			  :        {
										//action for this rule
					};

%%






#include "lex.yy.c"

main()
{
 return yyparse();
}

int yyerror( char *s ) { fprintf( stderr, "Can't parse the string\n"); }
    

yywrap()
{
  return(1);
}



