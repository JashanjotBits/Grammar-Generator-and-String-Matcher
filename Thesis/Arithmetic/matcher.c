#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

enum nodeType {Number,oper};
typedef struct Tree treeNode;
typedef treeNode *tree;
typedef union Node node;
union Node{
	struct {
			tree left,right;
			char op;
		}opNode;
		int number;
};

struct Tree {
	enum nodeType type;
	node body;
	int nodeId;
};

int preIndex;
tree makeTree(treeNode inOrder[],treeNode preOrder[],int start,int end){
	if(start>end)
		return NULL;
	int i,id;
	tree t;
	id=preOrder[preIndex++].nodeId;
	for(i=start;i<=end;i++){
		if(inOrder[i].nodeId==id)
			break;
	}
	t=&(inOrder[i]);
	if(t->type==oper){
		t->body.opNode.left=makeTree(inOrder,preOrder,start,i-1);
		t->body.opNode.right=makeTree(inOrder,preOrder,i+1,end);
	}	
	return t;
}

void print(tree t){
	if(t){
		if(t->type==oper){
			printf("%c\n",t->body.opNode.op );
			print(t->body.opNode.left);
			print(t->body.opNode.right);
		}
		else
			printf("%d\n",t->body.number);
	}
	return;
}

treeNode *populateArray(char file[],int *size){
	FILE *in;
	char temp[SIZE];
	char number[SIZE],id[SIZE];
	in=fopen(file,"r");
	int i=0;
	int k,l;
	treeNode *inOrder;
	inOrder=(treeNode*)malloc(sizeof(treeNode)*SIZE);
	while(fgets(temp,SIZE, in)!=NULL){
		if(temp[0]==' ')
			continue;
		k=0;
		l=0;
		while(temp[k]!=' '){
			number[k]=temp[k];
			k++;
		}

		number[k]='\0';
		while(temp[k]!='\0')
			id[l++]=temp[k++];

		if(!(temp[0]=='*' || temp[0]=='/' || temp[0]=='+' || temp[0]=='-')){
			inOrder[i].type=Number;
			inOrder[i].body.number=atoi(number);
		}

		else{
			inOrder[i].type=oper;
			inOrder[i].body.opNode.op=temp[0];
			inOrder[i].body.opNode.left=NULL;
			inOrder[i].body.opNode.right=NULL;
		}
		inOrder[i++].nodeId=atoi(id);
	}
	fclose(in);
	*size=i;
	return inOrder;
}

int compareTrees(tree t1,tree t2){
	if((t1&& !t2) || (!t1 && t2))
		return 0;
	if(t1){
		if(t1->type==Number){
			if(t2->type==oper)
				return 0;
			if(t1->body.number==t2->body.number)
				return 1;
			else
				return 0;
		}

		else{
			if(t1->body.opNode.op!=t2->body.opNode.op)
				return 0;
			if(compareTrees(t1->body.opNode.left,t2->body.opNode.right)){
				if(compareTrees(t1->body.opNode.right,t2->body.opNode.left) && ((t1->body.opNode.op=='+')|| (t1->body.opNode.op=='*')))
					return 1;
				printf("The subtrees emanating from %c don't match\n",t1->body.opNode.op );
				return 0;
			}

			else if(compareTrees(t1->body.opNode.left,t2->body.opNode.left)){
				if(compareTrees(t1->body.opNode.right,t2->body.opNode.right))
					return 1;
				printf("The subtrees emanating from %c don't match\n",t1->body.opNode.op );
				return 0;
			}

		}
	}
	return 1;
}

int main(){
	preIndex=0;
	tree t1,t2;
	char inFile[SIZE],preFile[SIZE];
	int i;
	int match;
	printf("Enter the names of files storing in order and pre order traversal respectively of the first tree\n");
	scanf("%s %s",inFile,preFile);
	treeNode *inOrder,*preOrder;
	inOrder=(treeNode*)malloc(sizeof(treeNode)*SIZE);
	preOrder=(treeNode*)malloc(sizeof(treeNode)*SIZE);
	inOrder=populateArray(inFile,&i);
	preOrder=populateArray(preFile,&i);
	t1=makeTree(inOrder,preOrder,0,i-1);
	printf("Enter the names of files storing in order and pre order traversal respectively of the second tree\n");
	scanf("%s %s",inFile,preFile);
	inOrder=(treeNode*)malloc(sizeof(treeNode)*SIZE);
	preOrder=(treeNode*)malloc(sizeof(treeNode)*SIZE);
	inOrder=populateArray(inFile,&i);
	preOrder=populateArray(preFile,&i);
	preIndex=0;
	t2=makeTree(inOrder,preOrder,0,i-1);
	match=compareTrees(t1,t2);
	if(match)
		printf("The trees represent same arithmetic exp\n");
	else
		printf("The trees don't match\n");
	
	return 0;
}


