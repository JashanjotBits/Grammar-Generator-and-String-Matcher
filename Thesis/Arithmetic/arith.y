%{
  #include <stdio.h>
  #include <stdlib.h>
  #define SIZE 100
  enum nodeType {Number,oper};
  typedef struct Tree tree;
  char in[SIZE],pre[SIZE];
  FILE *i,*p;
  int id;
  struct Tree {
    enum nodeType type;
    union{
      struct {
        tree *left,*right;
        char op;
      }opNode;
      int number;
    }body;
    int nodeId;
  };

  static tree *make_operator(tree *l,tree *r,char op){
        tree *t= (tree*) malloc (sizeof(tree));
        t->type=oper;
        t->body.opNode.left=l;
        t->body.opNode.right=r;
        t->body.opNode.op=op;
        t->nodeId=id++;
        return t;
  }

  static tree *make_number(int num){
      tree *t= (tree*) malloc (sizeof(tree));
      t->type=Number;
      t->nodeId=id++;
      t->body.number=num;
      return t;
  }

  static void printtree (tree *t, int level) {
      int i;
      #define step 4
        if (t){
          
          switch (t->type){
              case Number:
                //for(i=0;i<level;i++)
                //printf(" ");
                printf("%d\n",t->body.number);
                break;
              case oper:
                //printf("I am here\n");                   
                printtree (t->body.opNode.left, level+step);
                //for(i=0;i<level;i++)
                //printf(" ");  
                printf("%c\n",t->body.opNode.op);               
                printtree (t->body.opNode.right, level+step); 
                break;
        
          }


      }
  }

  static void printtreetofileIn (tree *t, FILE *fp) {
        if (t){
          switch (t->type){
              case Number:
                fprintf(fp,"%d %d\n",t->body.number,t->nodeId);
                break;
              case oper:                  
                printtreetofileIn (t->body.opNode.left, fp);
                //if(strcmp(t->body.opNode.op," "))
                fprintf(fp,"%c %d\n",t->body.opNode.op,t->nodeId);               
                printtreetofileIn (t->body.opNode.right, fp); 
                break;
          } 
      }

  }

  static void printtreetofilePre (tree *t, FILE *fp) {
        
        if (t){
          switch (t->type){
              case Number:

                fprintf(fp,"%d %d\n",t->body.number,t->nodeId);
                break;
              case oper:
                //if(strcmp(t->body.opNode.op," "))
                fprintf(fp,"%c %d\n",t->body.opNode.op,t->nodeId);                          
                printtreetofilePre (t->body.opNode.left, fp);                          
                printtreetofilePre (t->body.opNode.right, fp); 
                break;
          } 
      }

  }
 %}

 %union {
   int a_number;
   tree* a_tree;
   char an_op;
 }

 %start input
 %token <an_op> operator 
 %token <a_number> number
 %token OPENBRACKET CLOSEBRACKET
 %type <a_tree> exp input
 %token SEMICOLON

 %%
 input :  exp SEMICOLON { printf("Enter the name of the files for storing inorder and preorder traversal rep\n");scanf("%s %s",in,pre);i=fopen(in,"w");printtreetofileIn($1,i);p=fopen(pre,"w");printtreetofilePre($1,p);exit(0); };
 exp: 
    number{
     $$=make_number($1);
    //printtree($$,0);
    }

    |

    exp operator exp {
      
      $$ =make_operator($1,$3,$2);
      //printf(" The operator is %c\n",$2);
    }
 
    
    |

    OPENBRACKET exp CLOSEBRACKET {
      $$=make_operator($2,NULL,' '); 
      //printtree($$,0);      
    }  

    ;


 %%
 #include "lex.yy.c"

main()
{

  id=0;
 return yyparse();
}

int yyerror( char *s ) { fprintf( stderr, "Can't parse the string\n"); }
    

yywrap()
{
  return(1);
}


