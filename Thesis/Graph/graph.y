%{
 #include <stdio.h>
 #include <stdlib.h>
 enum treetype {vertex_node, edge_node,graph};
 typedef struct edge{
 		char v1,v2;
 }edge;

typedef struct Tree tree;
struct Tree {
	enum treetype nodetype;
   union {
   		tree *t;
   		edge e;
   		char v;     
   } left;
   tree *right;
};

static tree *make_edge(tree *r,edge e){
 			tree *t= (tree*) malloc (sizeof(tree));
 			t->nodetype=edge_node;
   			t->left.e=e;
   			t->right=r;
   			return t;
 }

 static tree *make_vertex(tree *r,char v){
 			tree *t= (tree*) malloc (sizeof(tree));
   			t->nodetype= vertex_node;
   			t->left.v=v;
   			t->right=r;
   			return t;
 }

 static tree *make_tree(tree *v,tree *e){
 			tree *t= (tree*) malloc (sizeof(tree));
   			t->nodetype= graph;
   			t->left.t=v;
   			t->right= e;
   			return t;
 }

 static void printtree (tree *t, int level) {
 int i;
 #define step 4
   if (t)
     switch (t->nodetype)
     {
       case graph:
       	printf ("Graph\n");
        printtree (t->left.t, level+step);        
        printtree (t->right, level+step);
        break;
        case edge_node:
        for(i=0;i<level;i++)
       		printf(" ");
        printf ("Edge: %c-%c\n",t->left.e.v1,t->left.e.v2);
        printtree (t->right, level+step); 
        break;
        case vertex_node:
        for(i=0;i<level;i++)
       		printf(" ");
        printf ("Vertex: %c\n",t->left.v);
        printtree (t->right, level+step); 
        break;
     }
 }

 	edge e;
 %}

%union {
   char a_char;
   tree* a_tree;
 }
 %start Graph
 %token <a_char> CHAR
 %type <a_char> W
 %token OPENBRACKET COMMA OPENBRACE CLOSEBRACE CLOSEBRACKET
 %type <a_tree> Graph Vertices MoreVertices Edges MoreEdges
 %%
Graph 			: OPENBRACKET OPENBRACE Vertices CLOSEBRACE COMMA OPENBRACE Edges CLOSEBRACE CLOSEBRACKET
					{$$=make_tree($3,$7);
		  			  printtree($$,0);
		  			  exit(0);	
		  			}
		  		;
Vertices 		:  {
					$$=NULL;
					}
				   |
		 		   W MoreVertices {
		  				$$=make_vertex($2,$1);
		  			}
		  		;
MoreVertices 	: 	
					{
						$$=NULL;
					}
					|

				    MoreVertices COMMA W  {
				    	$$=make_vertex($1,$3);
				    }

				;

Edges 			:  {
					$$=NULL;
					}
				   |
		 		   OPENBRACKET W COMMA W CLOSEBRACKET MoreEdges {		 		   		
		 		   		e.v1=$2;
		 		   		e.v2=$4;
		  				$$=make_edge($6,e);
		  			}
		  		;

MoreEdges 		:  {
						$$=NULL;
					}
					|
					COMMA OPENBRACKET W COMMA W CLOSEBRACKET MoreEdges {
		 		   		e.v1=$3;
		 		   		e.v2=$5;
		  				$$=make_edge($7,e);
		  			}
		  		;

W				: CHAR 	{
						$$=$1;
					}
				;
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






