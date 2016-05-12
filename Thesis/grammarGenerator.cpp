#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100

typedef struct HashTable hashTable;
typedef hashTable *link;
struct HashTable{
	char **Tokens;
	int occur[SIZE];
};

int numTokens[5];

char **tokeniseString(char s[],int n){
	char **tokens;
	tokens=(char**)malloc(sizeof(char*)*100);
	int i=0,j,k=0;
	for(j=0;j<100;j++)
		tokens[j]=(char*)malloc(sizeof(char)*100);
	while(s[k]!='\0'){
		j=0;
		while(s[k]!=' ' && s[k]!= '\0'){
			tokens[i][j++]=s[k];
			k++;
		}
		if(s[k]!='\0')
			k++;
		else
			tokens[i][j-1]='\0';
		i++;
	}
	numTokens[n]=i;
	return tokens;
}


int  hashString(char *s){
	int x=strlen(s);
	int i;
	long int hash=53;
	for(i=0;i<x;i++)
		hash+=hash*37+ s[i];
	return hash%SIZE;
}

hashTable hashTokens(char **tokens,int n){
	long int hash;
	int flag=0;
	int i,j,temp;
	hashTable table;
	table.Tokens=(char**)malloc(sizeof(char*)*SIZE);
	for(i=0;i<SIZE;i++){
		table.Tokens[i]=(char*)malloc(sizeof(char)*SIZE);
		table.occur[i]=0;
	}
	for(i=0;i<n;i++){
		flag=1;
		//printf("Now let's hash %s\n",tokens[i] );
		hash=hashString(tokens[i]);
		for(j=0;j<SIZE;j++){
			//printf("j is %d\n",j);
			temp=(hash+j)%SIZE;
			//printf("Temp is %d\n",temp );
			//printf("the temp is %d\n",temp );
			if(table.occur[temp]!=0){
				if(strcmp(tokens[i],table.Tokens[temp])==0){
					table.occur[temp]++;
					break;
				}
			}				
			else{
				//printf("Inserted %s at %d\n",tokens[i],temp);
				table.occur[temp]=1;
				table.Tokens[temp]=tokens[i];
				break;
			}	
		}
	}
	return table;
}

int isDecimalNum(char *s){
	int x=strlen(s);
	int decFlag=0;
	int i;
	if(s[0]<'0' || s[0] > '9')
		return 0;
	for(i=1;i<x;i++){
		if(s[i]=='.'){
			if(decFlag)
				return 0;
			else
				decFlag=1;
		}
		else if(s[i]<'0' || s[i] > '9')
				return 0;
	}
	if(decFlag)
		return 1;
	return 0;
}

int isNonDecNumber(char *s){
	int x=strlen(s);
	int decFlag=0;
	int i;
	for(i=0;i<x;i++)
		if(s[i]<'0' || s[i] > '9')
				return 0;
	return 1;
}

int detectANumList(int begin,int end,char **tokens){
	int i,j;
	if((end-begin)%2)
		return 0;
	char *separator;
	separator=(char*)malloc(sizeof(char)*3);
	if(begin+2<end){
		if(strcmp(tokens[begin+2],"+")==0 || strcmp(tokens[begin+2],",")==0)
			separator=tokens[begin+2];
		else
			return 0;
	}
	
	int isDeci=0;
	int k=1;
	for(i=begin+1;i<end;i++){
		if(k%2){
			if(isDecimalNum(tokens[i]))
				isDeci=1;
			else if(!isNonDecNumber(tokens[i]))
				return 0;
		}
		else if(strcmp(tokens[i],separator)!=0)
			return 0;
		k++;		
	}
	if(isDeci)
		return 1;
	else
		return 2;

}

int lookUp(hashTable table,char *s){
	int hash=hashString(s);
	int i;
	int temp;
	for(i=0;i<SIZE;i++){
		temp=(hash+i)%SIZE;
		if(table.occur[temp]){
			if(strcmp(table.Tokens[temp],s)==0){
				//printf("Lookup for %s positive\n",s);
				return table.occur[temp];
			}				
		}
		else
			return 0;
	}
	return 0;
}

int main(){
	char strings[SIZE];
	char **tokens[5];
	hashTable tables[5];
	int i,j,k,a;
	for(k=0;k<5;k++){
		numTokens[k]=0;
		tokens[k]=(char**)malloc(sizeof(char*)*SIZE);	
		tables[k].Tokens=(char**)malloc(sizeof(char*)*SIZE);
		for(j=0;j<SIZE;j++){
			tokens[k][j]=(char*)malloc(sizeof(char)*SIZE);
			tables[k].Tokens[j]=(char*)malloc(sizeof(char)*SIZE);
		}
		printf("Enter string number %d: ",k);
		fgets(strings,SIZE,stdin);
		tokens[k]=tokeniseString(strings,k);
		// printf("The tokenised string is :");
		// for(j=0;j<numTokens[k];j++)
		// 	printf("%s - ",tokens[k][j] );
		// printf("\n");
		tables[k]=hashTokens(tokens[k],numTokens[k]);
	}
	/*****************strings hashed*************/

	int firstMatch=0,lastMatch=0,listLen=0;
	char **matchedList;
	int matchedIndices[SIZE];
	matchedList=(char**)malloc(sizeof(char*)*SIZE);
	for(i=0;i<SIZE;i++)
		matchedList[i]=(char*)malloc(sizeof(char)*SIZE);
	for(i=0;i<numTokens[0];i++){
		for(j=1;j<5;j++){
			if(lookUp(tables[j],tokens[0][i])==0)
				break;
			if(j==4 && strcmp(tokens[0][i],",")){
				matchedIndices[listLen]=i;
				matchedList[listLen++]=tokens[0][i];
				//printf("Matched : %s\n",tokens[0][i] );
				if(i==0)
					firstMatch=1;
				if(i==numTokens[0]-1)
					lastMatch=1;
			}
		}
	}

	//printf("The length of the matched list is %d\n",listLen);

	/******************Common strings found**********/
	FILE *fp;
	char **listBuffer;
	int listBufferLen=0;
	char **unionBuffer;
	int unionSize=0;
	char input[SIZE];
	int bufferDecFlag=0;
	int bufferWholeFlag=0;
	int listFlag;
	
	listBuffer=(char**)malloc(sizeof(char*)*SIZE);
	unionBuffer=(char**)malloc(sizeof(char*)*SIZE);
	for(i=0;i<SIZE;i++)
		listBuffer[i]=(char*)malloc(sizeof(char)*SIZE);
	for(i=0;i<2*SIZE;i++)
		unionBuffer[i]=(char*)malloc(sizeof(char)*SIZE);
	fp= fopen("Grammar.txt","w");
	if(listLen==0){
		printf("No Common pattern found. Please write the template of the pattern you want to write Grammar for: \n");
		fprintf(fp, "Start : " );
		scanf("%s",input);
		if(strcmp(input,"list")==0 || strcmp(input,"List")==0){
			scanf("%s",input);
			scanf("%s",listBuffer[listBufferLen]);
			fprintf(fp, "%slist ",listBuffer[listBufferLen]);
			listBufferLen++;
		}

		else if(strcmp(input,"concatenation")==0){
			scanf("%s",input);   // inputs "of"
			scanf("%s",input);
			fprintf(fp, "%s ",input);
			scanf("%s",input);   //inputs "and"
			scanf("%s",input);
			fprintf(fp, "%s ",input);
		}

		else if(strcmp(input,"union")==0){
			scanf("%s",input);   // inputs "of"
			scanf("%s",unionBuffer[unionSize*2]);
			scanf("%s",input);   //inputs "and"
			scanf("%s",unionBuffer[unionSize*2+1]);
			unionSize++;
			fprintf(fp, "union%d ",unionSize );
		}

		else{
			while(strcmp(input,".")){
				fprintf(fp, "%s ",input );
				scanf("%s",input);
				}
		}
		fprintf(fp, "\n" );

	printf("Enter the grammar rules for non terminals(variables) you added.\n*Add a full stop after every rule and add '*' in the end\n");
	scanf("%s",input);
	while(strcmp(input,"*")){
		if(strcmp(input,".")==0)
			fprintf(fp, "\n");
		else
			fprintf(fp, "%s ",input);
		scanf("%s",input);
	}

	if(bufferDecFlag){
		fprintf(fp, "numberList : NUMBER moreNum\nmoreNum: , NUMBER moreNum\nmoreNum : E\n" );
	}

	if(bufferWholeFlag){
		fprintf(fp, "wholeNumberList : WHOLENUMBER moreWholeNum\nmoreWholeNum: , WHOLENUMBER moreWholeNum\nmoreWholeNum : E\n" );
	}

	
	for(i=0;i<listBufferLen;i++){
		fprintf(fp,"%sList : %s more%s\nmore%s: , %s more%s\nmore%s : E\n",listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i]);
	}

	for(i=0;i<unionSize;i++){
		fprintf(fp,"union%d : %s union%d\nunion%d : %s union%d\nunion%d: E\n",i+1,unionBuffer[i*2],i+1,i+1,unionBuffer[i*2+1],i+1,i+1);
	}
	return 0;
	}

	fprintf(fp, "Start : " );
	

	if(matchedIndices[0]>0){
		listFlag=0;
		int decFlag=0;
		int Temp;
		for(i=0;i<5;i++){
			Temp=detectANumList(-1,matchedIndices[0],tokens[i]);
			if(Temp==0){
				//printf("List not detected in %d\n",i);
				break;
			}				
			if(Temp==1)
				decFlag=1;
			if(i==4){
				listFlag=1;
				if(decFlag)
					bufferDecFlag=1;
				else
					bufferWholeFlag=1;
			}
		}
		if(decFlag && listFlag)
			fprintf(fp, "numberList ");
		else if(listFlag)
			fprintf(fp, "wholeNumberList ");
		else{
			printf("Write the grammar for what appears before %s. Enter a '.' in the end \n",tokens[0][matchedIndices[0]]);
			scanf("%s",input);
			if(strcmp(input,"list")==0 || strcmp(input,"List")==0){
					scanf("%s",input);
					scanf("%s",listBuffer[listBufferLen]);
					fprintf(fp, "%slist ",listBuffer[listBufferLen]);
					listBufferLen++;
				}

			else if(strcmp(input,"concatenation")==0){
					scanf("%s",input);   // inputs "of"
					scanf("%s",input);
					fprintf(fp, "%s ",input);
					scanf("%s",input);   //inputs "and"
					scanf("%s",input);
					fprintf(fp, "%s ",input);
				}

			else if(strcmp(input,"union")==0){
					scanf("%s",input);   // inputs "of"
					scanf("%s",unionBuffer[unionSize*2]);
					scanf("%s",input);   //inputs "and"
					scanf("%s",unionBuffer[unionSize*2+1]);
					unionSize++;;
					fprintf(fp, "union%d ",unionSize );

			}

			else{
				while(strcmp(input,".")){
					fprintf(fp, "%s ",input );
					scanf("%s",input);
				}
			}
		}
	}

	fprintf(fp, "%s ",matchedList[0]);


/****************************************************************************/
	for(i=1;i<listLen;i++){
		if(matchedIndices[i-1]+1==matchedIndices[i])
			fprintf(fp, "%s ",matchedList[i]);
		else{
			listFlag=0;
			int decFlag=0;
			int Temp;
			for(a=0;a<5;a++){
				Temp=detectANumList(matchedIndices[i-1],matchedIndices[i],tokens[a]);
				if(Temp==0)
					break;					
				if(Temp==1)
					decFlag=1;
				if(a==4){
					listFlag=1;
					if(decFlag)
						bufferDecFlag=1;
					else
						bufferWholeFlag=1;
				}
			}

			if(decFlag && listFlag)
				fprintf(fp, "numberList ");
			else if(listFlag)
				fprintf(fp, "wholeNumberList ");
			else{
				printf("Write the grammar for what appears between %s and %s. Enter a '.' in the end \n",matchedList[i-1],matchedList[i]);
				scanf("%s",input);
				if(strcmp(input,"list")==0 || strcmp(input,"List")==0){
					scanf("%s",input);
					scanf("%s",listBuffer[listBufferLen]);
					fprintf(fp, "%slist ",listBuffer[listBufferLen]);
					listBufferLen++;

				}

				else if(strcmp(input,"concatenation")==0){
					scanf("%s",input);   // inputs "of"
					scanf("%s",input);
					fprintf(fp, "%s ",input);
					scanf("%s",input);   //inputs "and"
					scanf("%s",input);
					fprintf(fp, "%s ",input);
				}

				else if(strcmp(input,"union")==0){
					scanf("%s",input);   // inputs "of"
					scanf("%s",unionBuffer[unionSize*2]);
					scanf("%s",input);   //inputs "and"
					scanf("%s",unionBuffer[unionSize*2+1]);
					unionSize++;;
					fprintf(fp, "union%d ",unionSize );

			}

				else{
					while(strcmp(input,".")){
						fprintf(fp, "%s ",input );
						scanf("%s",input);
					}
				}	
			} 

			fprintf(fp, "%s ",matchedList[i]);
			//printf("Printed %s to file for i=%d\n",matchedList[i],i);
		}
	}
	 

/****************************************************************************/
	if(matchedIndices[listLen-1]<numTokens[0]-1){
		listFlag=0;
		int decFlag=0;
		int Temp;
		for(i=0;i<5;i++){
			Temp=detectANumList(matchedIndices[listLen-1],numTokens[0],tokens[i]);
			if(Temp==0)
				break;
			if(Temp==1)
				decFlag=1;
			if(i==4){
				listFlag=1;
				if(decFlag)
					bufferDecFlag=1;
				else
					bufferWholeFlag=1;
			}
		}
		if(decFlag && listFlag)
			fprintf(fp, "numberList ");
		else if(listFlag)
			fprintf(fp, "wholeNumberList ");
		else{
			printf("Write the grammar for what appears after %s. Enter a '.' in the end \n",matchedList[listLen-1]);
			scanf("%s",input);
			if(strcmp(input,"list")==0 || strcmp(input,"List")==0){
					scanf("%s",input);
					scanf("%s",listBuffer[listBufferLen]);
					fprintf(fp, "%sList ",listBuffer[listBufferLen]);
					listBufferLen++;
			}

			else if(strcmp(input,"concatenation")==0){
					scanf("%s",input);   // inputs "of"
					scanf("%s",input);
					fprintf(fp, "%s ",input);
					scanf("%s",input);   //inputs "and"
					scanf("%s",input);
					fprintf(fp, "%s ",input);
				}

			else if(strcmp(input,"union")==0){
					scanf("%s",input);   // inputs "of"
					scanf("%s",unionBuffer[unionSize*2]);
					scanf("%s",input);   //inputs "and"
					scanf("%s",unionBuffer[unionSize*2+1]);
					unionSize++;;
					fprintf(fp, "union%d ",unionSize );

			}

			else{
				while(strcmp(input,".")){
					fprintf(fp, "%s ",input );
					scanf("%s",input);
				}
			}
		}
	}
	fprintf(fp, "\n" );

/****************************************************************************/
	if(bufferDecFlag){
		fprintf(fp, "numberList : NUMBER moreNum\nmoreNum: , NUMBER moreNum\nmoreNum : E\n" );
	}

	if(bufferWholeFlag){
		fprintf(fp, "wholeNumberList : WHOLENUMBER moreWholeNum\nmoreWholeNum: , WHOLENUMBER moreWholeNum\nmoreWholeNum : E\n" );
	}

	printf("Enter the grammar rules for non terminals(variables) you added.\n*Add a full stop after every rule and add '*' in the end\n");
	scanf("%s",input);
	while(strcmp(input,"*")){
		if(strcmp(input,".")==0)
			fprintf(fp, "\n");
		else
			fprintf(fp, "%s ",input);
		scanf("%s",input);
	}

	for(i=0;i<listBufferLen;i++){
		fprintf(fp,"%sList : %s more%s\nmore%s: , %s more%s\nmore%s : E\n",listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i],listBuffer[i]);
	}

	for(i=0;i<unionSize;i++){
		fprintf(fp,"union%d : %s union%d\nunion%d : %s union%d\nunion%d: E\n",i+1,unionBuffer[i*2],i+1,i+1,unionBuffer[i*2+1],i+1,i+1);
	}

/*****************************************************************************/
	fprintf(fp, "\n");
	fclose(fp);
	return 0;
}
