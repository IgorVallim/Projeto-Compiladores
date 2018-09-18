#include <stdio.h>

int analisador(char* entrada){
    /*printf("%c\n",*entrada);
    entrada++;
    printf("%c\n",*entrada);*/
q0:
    if(*entrada=='i') goto q1;
q1:
    entrada++;
    if(*entrada=='f') goto q2;
    else if(*entrada=='n') goto q4;
    else return 0;
    
q2:
    entrada++;
    if(*entrada==' ') goto q3;
    else return 0;

q3:
    entrada++;
    if(*entrada=='\0') return 1;
    else return 0;
    
q4:
    entrada++;
    if(*entrada=='t') goto q5;
    else return 0;

q5:
    entrada++;
    if(*entrada==' ') goto q6;
    else return 0;

q6:
    entrada++;
    if(*entrada=='\0') return 1;
    else return 0;
}

int main(){
    char palavra[50] = "if  ";
    if(analisador(&palavra[0])==1) printf("Foi\n");
    else printf("Não foi\n");
    
    return 0;
}
