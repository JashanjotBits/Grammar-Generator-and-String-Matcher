#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 100
#define MAX 100

typedef struct TreeNode node;
typedef node *tree;
struct TreeNode{
	char lexeme[SIZE];
	char token[SIZE];
	int numChild;
	tree children[MAX];
};

typedef float weight;

bool matchTrees(tree t1,tree t2){
	if((!t1 && t2) || (t1 && !t2))
		return false;
	if(t1){
		if(strcmp(t1->lexeme,t2->lexeme))
			return false;
		if(t1->numChild!= t2->numChild)
			return false;
		int i;
		for(i=0;i<(t1->numChild);i++){
			if(!matchTrees(t1->children[i],t2->children[i]))
				return false;
		}
	}
	return true;
}

/****************************************************************/
weight weightChildNode(tree parent,weight parentWeight){
	weight childWeight;
	if(!parent)
		return 1;
	return parentWeight/(parent->numChild);
}

weight partialWeightMatcher(tree t1,tree t2,weight w){
	if((!t1 && t2) || (t1 && !t2))
		return 0;
	if(t1){
		if(strcmp(t1->lexeme,t2->lexeme))
			return 0;
		if(t1->numChild!= t2->numChild)
			return 0;
		int i;
		weight matchRatio=0;
		weight childWeight=weightChildNode(t1,w);
		for(i=0;i<(t1->numChild);i++){
			matchRatio+=partialWeightMatcher(t1->children[i],t2->children[i],childWeight);
		}
		return matchRatio;
	}
	return 1;
}

/**************************************************************************/

void printTree(tree t,int offset){
	if(t){
		int o=4;
		int i;
		for(i=0;i<offset;i++)
			printf(" ");
		printf("%s\n",t->lexeme);
		int numChild=t->numChild;
		for(i=0;i<numChild;i++)
			printTree(t->children[i],offset+o);
	}
	return;
}

void partialMatcher(tree t1,tree t2){
	if(!t1 && t2){
		printf("There is no subtree in the first tree in comparison for the following subtree in the second tree:\n");
		printTree(t2,0);
		return;
	}
	if(t1 && !t2){
		printf("There is no subtree in the second tree in comparison for the following subtree in the first tree:\n");
		printTree(t1,0);
		return;
	}

	if(t1){
		if((strcmp(t1->lexeme,t2->lexeme)) || (t1->numChild!= t2->numChild)){
			printf("The following subtrees in the two trees don't match:\n");
			printf("\nThe subtree in first tree\n");
			printTree(t1,0);
			printf("\n\nThe subtree in second tree\n\n");
			printTree(t2,0);
			return;
		}

		int numChild=t1->numChild;
		int i;
		for(i=0;i<numChild;i++)
			partialMatcher(t1->children[i],t2->children[i]);
	}
	return;
}



/**************************************************************************/

int main(){
	return 0;
}