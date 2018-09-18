#include <stdio.h>

int analisador(char* entrada){

q0:
    if(*entrada=='i') goto q1;
    else if(*entrada=='p') goto q7;
    else if(*entrada=='v') goto q19;
    else if(*entrada=='t') goto q24;
    else if(*entrada=='b') goto q29;
    else if(*entrada=='f') goto q34;
    else if(*entrada=='w') goto q40;
    else if(*entrada=='>') goto q46;
    else if(*entrada=='<') goto q50;
    return 0;

q1:
    entrada++;
    if(*entrada=='f') goto q2;
    else if(*entrada=='n') goto q4;
    return 0;
    
q2:
    entrada++;
    if(*entrada==' ') goto q3;
    return 0;

q3:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q4:
    entrada++;
    if(*entrada=='t') goto q5;
    return 0;

q5:
    entrada++;
    if(*entrada==' ') goto q6;
    return 0;

q6:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q7:
    entrada++;
    if(*entrada=='r') goto q8;
    return 0;

q8:
    entrada++;
    if(*entrada=='o') goto q9;
    else if(*entrada=='i') goto q15;
    return 0;

q9:
    entrada++;
    if(*entrada=='g') goto q10;
    return 0;
    
q10:
    entrada++;
    if(*entrada=='r') goto q11;
    return 0;
    
q11:
    entrada++;
    if(*entrada=='a') goto q12;
    return 0;
    
q12:
    entrada++;
    if(*entrada=='m') goto q13;
    return 0;

q13:
    entrada++;
    if(*entrada==' ') goto q14;
    return 0;
    
q14:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q15:
    entrada++;
    if(*entrada=='n') goto q16;
    return 0;
    
q16:
    entrada++;
    if(*entrada=='t') goto q17;
    return 0;
    
q17:
    entrada++;
    if(*entrada==' ') goto q18;
    return 0;
    
q18:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q19:
    entrada++;
    if(*entrada=='o') goto q20;
    return 0;

q20:
    entrada++;
    if(*entrada=='i') goto q21;
    return 0;
    
q21:
    entrada++;
    if(*entrada=='d') goto q22;
    return 0;
    
q22:
    entrada++;
    if(*entrada==' ') goto q23;
    return 0;
    
q23:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q24:
    entrada++;
    if(*entrada=='r') goto q25;
    return 0;
    
q25:
    entrada++;
    if(*entrada=='u') goto q26;
    return 0;
    
q26:
    entrada++;
    if(*entrada=='e') goto q27;
    return 0;
    
q27:
    entrada++;
    if(*entrada==' ') goto q28;
    return 0;
    
q28:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q29:
    entrada++;
    if(*entrada=='o') goto q30;
    return 0;
    
q30:
    entrada++;
    if(*entrada=='o') goto q31;
    return 0;
    
q31:
    entrada++;
    if(*entrada=='l') goto q32;
    return 0;
    
q32:
    entrada++;
    if(*entrada==' ') goto q33;
    return 0;
    
q33:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q34:
    entrada++;
    if(*entrada=='a') goto q35;
    return 0;
    
q35:
    entrada++;
    if(*entrada=='l') goto q36;
    return 0;
    
q36:
    entrada++;
    if(*entrada=='s') goto q37;
    return 0;
    
q37:
    entrada++;
    if(*entrada=='e') goto q38;
    return 0;
    
q38:
    entrada++;
    if(*entrada==' ') goto q39;
    return 0;
    
q39:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q40:
    entrada++;
    if(*entrada=='h') goto q41;
    return 0;
    
q41:
    entrada++;
    if(*entrada=='i') goto q42;
    return 0;
    
q42:
    entrada++;
    if(*entrada=='l') goto q43;
    return 0;
    
q43:
    entrada++;
    if(*entrada=='e') goto q44;
    return 0;
    
q44:
    entrada++;
    if(*entrada==' ') goto q45;
    return 0;
    
q45:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q46:
    entrada++;
    if(*entrada=='=') goto q47;
    else if(*entrada==' ') goto 49;
    return 0;
    
q47:
    entrada++;
    if(*entrada==' ') goto q48;
    return 0;
    
q48:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q49:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q50:
    entrada++;
    if(*entrada==' ') goto q51;
    else if(*entrada=='=') goto q52;
    return 0;
    
q51:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
    
q52:
    entrada++;
    if(*entrada==' ') goto q53;
    return 0;
    
q53:
    entrada++;
    if(*entrada=='\0') return 1;
    return 0;
}

int main(){
    char palavra[50] = ">= ";
    if(analisador(&palavra[0])==1) printf("Foi\n");
    else printf("Não foi\n");
    
    return 0;
}
