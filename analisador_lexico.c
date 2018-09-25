#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define _IF_ 101 //Lista de constantes associadas a IDs de tokens.
#define _INT_ 102
#define _PROGRAM_ 103
#define _PRINT_ 104
#define _VOID_ 105
#define _TRUE_ 106
#define _BOOL_ 107
#define _FALSE_ 108
#define _WHILE_ 109
#define _ELSE_ 110

#define _MAIOR_IGUAL_ 201 
#define _MAIOR_ 202		  	
#define _MENOR_ 203
#define _MENOR_IGUAL_ 204
#define _ATRIBUICAO_ 205
#define _IGUALDADE_ 206
#define _DIVIDIDO_ 207
#define _VEZES_ 208
#define _DIFERENTE_ 209
#define _MAIS_ 210
#define _MENOS_ 211

#define _ABRE_CHAVE_ 301
#define _FECHA_CHAVE_ 302
#define _PONTO_E_VIRGULA_ 303
#define _VIRGULA_ 304
#define _ABRE_PARENTESE_ 305
#define _FECHA_PARENTESE_ 306

#define _COMENTARIO_ 401
#define _IDENTIFICADOR_ 402
#define _VALOR_ 403

/*
Beatriz Godoy 31612520
Fernando Grangeiro 31602843
Igor Vallim Sordi 31644961
Lucas Barros 31613144
*/

char** leArquivo(FILE *arquivo, char nome[]){

	char lexema[100], letra;
	memset(lexema, 0, sizeof(lexema));
	char** saida = malloc(10000);
	arquivo = fopen(nome, "r");
	int cont = 0, pos = 0;
	while((letra = fgetc(arquivo)) != EOF){
		if(letra==' ' || iscntrl(letra)){
			if(lexema[0]!=' ' && !iscntrl(lexema[0]) && lexema[0]!='\0'){
				lexema[cont] = ' ';
				saida[pos] = (char*)malloc(sizeof(lexema));
				strcpy(saida[pos], lexema);
				pos++;
				cont = 0;
				memset(lexema, 0, sizeof(lexema));
			}else memset(lexema, 0, sizeof(lexema));
			
		}else{
			lexema[cont] = letra;
			cont++;
		}
		
	}
	
	if(lexema[0]!=' ' && !iscntrl(lexema[0]) && lexema[0]!='\0'){
		lexema[cont] = ' ';
		saida[pos] = (char*)malloc(sizeof(lexema));
		strcpy(saida[pos], lexema);
	}
	
	fclose(arquivo);
	return saida;
}

char* buscaToken(int id){ //Funcao que retorna um token (string), com base em seu identificador.
	
	int i = 0;
	
	int ids[30] = {101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 301, 302, 303, 304, 305, 
	306, 401, 402, 403}; //Vetor contendo os identificadores dos tokens.
	
	char tokens[][30] = {"< if >", "< int >", "< program >", "< print >", "< void >", "< true >", "< bool >", "< false >", "< while >", "< else >", "< >= >",
"< > >", "< < >", "< <= >", "< = >", "< == >", "< / >", "< * >", "< != >", "< + >", "< - >", "< { >", "< } >", "< ; >", "< , >", "< ( >", "< ) >", 
"< comment >", "< id >", "< value >"}; //Vetor contendo os tokens (os tolkens possuem posições correspondentes aos seus IDs, encontrados no vetor "ids").
	
	for(i=0;i<30;i++){ //For que percorre o vetor "ids" até encontrar a posição do tolken correspondente ao ID passado como parametro.
        if(id==ids[i]){
        	char* resp = tokens[i];
            return resp;
        }
	}
	return 0;
	
}

int analisador(char* entrada){  //Funcao que representa o afd da etapa anterior do projeto, retornando um token correspondente a entrada.

q0: //Representação do estado inicial do automato.
    if(*entrada=='i') goto q1;
    else if(*entrada=='p') goto q7;
    else if(*entrada=='v') goto q19;
    else if(*entrada=='t') goto q24;
    else if(*entrada=='b') goto q29;
    else if(*entrada=='f') goto q34;
    else if(*entrada=='w') goto q40;
    else if(*entrada=='>') goto q46;
    else if(*entrada=='<') goto q50;
    else if(*entrada=='=') goto q54;
    else if(*entrada=='{') goto q58;
    else if(*entrada=='}') goto q60;
    else if(*entrada=='/') goto q62;
    else if(*entrada=='*') goto q66;
    else if(*entrada=='!') goto q70;
    else if(*entrada=='+') goto q73;
    else if(*entrada=='-') goto q75;
    else if(*entrada==';') goto q77;
    else if(*entrada==',') goto q78;
    else if(*entrada=='(') goto q81;
    else if(*entrada==')') goto q83;
    else if(*entrada=='_') goto q85;
    else if(isdigit(*entrada)) goto q87;
    else if(*entrada=='e') goto q90;
	return 0;

q1:
    entrada++; //Cada vez que ocorre uma transicao, e consumido um caractere da entrada.
    if(*entrada=='f') goto q2; //Representação de uma transição.
    else if(*entrada=='n') goto q4;
    return 0; //Função retorna 0, caso apareca um caractere nao previsto nesse estado.
    
q2:
    entrada++;
    if(*entrada==' ') goto q3;
    return 0;

q3: //Representação de um dos estados finais do automato.
    entrada++;
    if(*entrada=='\0') return _IF_; //Se a string de entrada estiver vazia, retorna o token(id) correspondente a esse estado final.
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
    if(*entrada=='\0') return _INT_;
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
    if(*entrada=='\0') return _PROGRAM_;
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
    if(*entrada=='\0') return _PRINT_;
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
    if(*entrada=='\0') return _VOID_;
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
    if(*entrada=='\0') return _TRUE_;
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
    if(*entrada=='\0') return _BOOL_;
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
    if(*entrada=='\0') return _FALSE_;
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
    if(*entrada=='\0') return _WHILE_;
    return 0;
    
q46:
    entrada++;
    if(*entrada=='=') goto q47;
    else if(*entrada==' ') goto q49;
    return 0;
    
q47:
    entrada++;
    if(*entrada==' ') goto q48;
    return 0;
    
q48:
    entrada++;
    if(*entrada=='\0') return _MAIOR_IGUAL_;
    return 0;
    
q49:
    entrada++;
    if(*entrada=='\0') return _MAIOR_;
    return 0;
    
q50:
    entrada++;
    if(*entrada==' ') goto q51;
    else if(*entrada=='=') goto q52;
    return 0;
    
q51:
    entrada++;
    if(*entrada=='\0') return _MENOR_;
    return 0;
    
q52:
    entrada++;
    if(*entrada==' ') goto q53;
    return 0;
    
q53:
    entrada++;
    if(*entrada=='\0') return _MENOR_IGUAL_;
    return 0;
    
q54:
    entrada++;
    if(*entrada==' ') goto q55;
    else if(*entrada=='=') goto q56;
    return 0;  
    
q55:
    entrada++;
    if(*entrada=='\0') return _ATRIBUICAO_;
    return 0;
    
q56:
    entrada++;
    if(*entrada==' ') goto q57;
    return 0;
    
q57:
    entrada++;
    if(*entrada=='\0') return _IGUALDADE_;
    return 0;
    
q58:
    entrada++;
    if(*entrada==' ') goto q59;
    return 0;
    
q59:
    entrada++;
    if(*entrada=='\0') return _ABRE_CHAVE_;
    return 0;
    
q60:
    entrada++;
    if(*entrada==' ') goto q61;
    return 0;
    
q61:
    entrada++;
    if(*entrada=='\0') return _FECHA_CHAVE_;
    return 0;
    
q62:
	entrada++;
	if(*entrada==' ') goto q63;
	else if(*entrada=='*') goto q64;
	return 0;
	
q63:
	entrada++;
	if(*entrada=='\0') return _DIVIDIDO_;
	return 0;
	
q64:
	entrada++;
	if(*entrada=='*') goto q65;
	else if(*entrada=='\0') return 0;
	goto q64;
	
q65:
	entrada++;
	if(*entrada=='/') goto q68;
	else if(*entrada=='\0') return 0;
	goto q64;
	
q66:
	entrada++;
	if(*entrada==' ') goto q67;
	return 0;
	
q67:
	entrada++;
	if(*entrada=='\0') return _VEZES_;
	return 0;
	
q68:
	entrada++;
	if(*entrada==' ') goto q69;
	return 0;

q69:
	entrada++;
	if(*entrada=='\0') return _COMENTARIO_;
	return 0;
	
q70:
	entrada++;
	if(*entrada=='=') goto q71;
	return 0;
	
q71:
	entrada++;
	if(*entrada==' ') goto q72;
	return 0;
	
q72:
	entrada++;
	if(*entrada=='\0') return _DIFERENTE_;
	return 0;	
	
q73:
	entrada++;
	if(*entrada==' ') goto q74;
	return 0;
	
q74:
	entrada++;
	if(*entrada=='\0') return _MAIS_;
	return 0;
	
q75:
	entrada++;
	if(*entrada==' ') goto q76;
	return 0;
	
q76:
	entrada++;
	if(*entrada=='\0') return _MENOS_;
	return 0;
	
q77:
	entrada++;
	if(*entrada==' ') goto q79;
	return 0;
	
q78:
	entrada++;
	if(*entrada==' ') goto q80;
	return 0;

q79:
	entrada++;
	if(*entrada=='\0') return _PONTO_E_VIRGULA_;
	return 0;	
	
q80:
	entrada++;
	if(*entrada=='\0') return _VIRGULA_;
	return 0;
	
q81:
	entrada++;
	if(*entrada==' ') goto q82;
	return 0;
	
q82:
	entrada++;
	if(*entrada=='\0') return _ABRE_PARENTESE_;
	return 0;		
		
q83:
	entrada++;
	if(*entrada==' ') goto q84;
	return 0;
	
q84:
	entrada++;
	if(*entrada=='\0') return _FECHA_PARENTESE_;
	return 0;		
	
q85:
	entrada++;
	if(isalpha(*entrada)) goto q85;
	else if(*entrada==' ') goto q86;
	return 0;
	
q86:
	entrada++;
	if(*entrada=='\0') return _IDENTIFICADOR_;
	return 0;
	
q87:
	entrada++;
	if(isdigit(*entrada)) goto q87;
	else if(*entrada=='.') goto q88;
	else if(*entrada==' ') goto q91;
	return 0;
	
q88:
	entrada++;
	if(isdigit(*entrada)) goto q89;
	return 0;	
	
q89:
	entrada++;
	if(isdigit(*entrada)) goto q89;
	else if(*entrada==' ') goto q91;
	return 0;
	
q90:
	entrada++;
	if(*entrada=='l') goto q92;
	return 0;	
	
q91:
	entrada++;
	if(*entrada=='\0') return _VALOR_;
	return 0;	
	
q92:
	entrada++;
	if(*entrada=='s') goto q93;
	return 0;			
			
q93:
	entrada++;
	if(*entrada=='e') goto q94;
	return 0;	
	
q94:
	entrada++;
	if(*entrada==' ') goto q95;
	return 0;
	
q95:
	entrada++;
	if(*entrada=='\0') return _ELSE_;
	return 0;			
}

int main(){
	FILE *arquivo;
	char nome[100] = "entrada.txt";
    char** lexemas = leArquivo(arquivo, nome);
    return 0;
}
