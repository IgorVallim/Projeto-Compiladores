/*
 Beatriz Godoy 31612520
 Fernando Grangeiro 31602843
 Igor Vallim Sordi 31644961
 Lucas Barros 31613144
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//Lista de constantes associadas a IDs de tokens.
#define _IF_ 101 
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

int lookahead;
int bloco(char programa[], int *pos);
int comandoComposto(char programa[], int *pos);
int expressaoSimples(char programa[], int *pos);

//Funcao que le o arquivo contendo o programa fonte, e retorna uma String com seu conteudo.
char* leArquivo(FILE* arquivo, char nome[]){
	arquivo = fopen(nome, "r");
	if(!arquivo){
		printf("ERRO: arquivo nao encontrado!\n");
		exit(0);
	}
	
	fseek(arquivo, 0, SEEK_END);
	int tamanho = ftell(arquivo);
	char* programa = malloc(tamanho*sizeof(char));
	fseek(arquivo, 0, SEEK_SET);
	char letra;
	int i = 0;
	
	while((letra = fgetc(arquivo)) != EOF){
		programa[i] = letra;
		i++;		
	}
	programa[i] = '\0';
	
	return programa;
}

//Funcao que retorna um token (String), com base em seu identificador.
char* buscaToken(int id){
	
    switch (id) {
        case 101:
            return "< if >";
        case 102:
            return "< int >";
        case 103:
            return "< program >";
        case 104:
            return "< print >";
        case 105:
            return "< void >";
        case 106:
            return "< true >";
        case 107:
            return "< bool >";
        case 108:
            return "< false >";
        case 109:
            return "< while >";
        case 110:
            return "< else >";
        case 201:
            return "< >= >";
        case 202:
            return "< > >";
        case 203:
            return "< < >";
        case 204:
            return "< <= >";
        case 205:
            return "< = >";
        case 206:
            return "< == >";
        case 207:
            return "< / >";
        case 208:
            return "< * >";
        case 209:
            return "< != >";
        case 210:
            return "< + >";
        case 211:
            return "< - >";
        case 301:
            return "< { >";
        case 302:
            return "< } >";
        case 303:
            return "< ; >";
        case 304:
            return "< , >";
        case 305:
            return "< ( >";
        case 306:
            return "< ) >";
        case 401:
            return "< comment >";
        case 402:
            return "< id >";
        case 403:
            return "< value >";
        default:
            return "Token nao encontrado!";
    }
}


//Funcao que representa o afd da etapa anterior do projeto, retornando um token correspondente a entrada.
int scanner(char* entrada){  

//Representacao do estado inicial do automato.
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
	//Cada vez que ocorre uma transicao, e consumido um caractere da entrada.
    entrada++; 
    //Representação de uma transicao.
    if(*entrada=='f') goto q2; 
    else if(*entrada=='n') goto q4;
    //Funcao retorna 0, caso apareca um caractere nao previsto nesse estado.
	return 0; 
    
q2:
    entrada++;
    if(*entrada==' ') goto q3;
    return 0;

//Representacao de um dos estados finais do automato.
q3: 
    entrada++;
    //Se a string de entrada estiver vazia, retorna o token(id) correspondente a esse estado final.
    if(*entrada=='\0') return _IF_; 
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
	
q85: //TODO: Arrumar automato
	entrada++;
	if(isalpha(*entrada)) goto q88;
	return 0;
	
q86:
	entrada++;
	if(*entrada=='\0') return _IDENTIFICADOR_;
	return 0;
	
q87:
	entrada++;
	if(isdigit(*entrada)) goto q87;
	else if(*entrada==' ') goto q91;
	return 0;
	
q88:
	entrada++;
	if(isalpha(*entrada)) goto q88;
	else if(*entrada==' ') goto q86;
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

//Funcao que imprime erros lexicos.
void imprimeErroLexico(char programa[], int *pos, char lexema[]){
    int i, linha = 1;
    for(i=0;i<(*pos);i++){
        if(programa[i]=='\n') linha++;
    }
    printf("ERRO: lexema '%s' nao reconhecido! (linha %d)\n", lexema, linha);
    exit(0);
}

//Funcao que faz a analise lexica de trecho do programa fonte.
int analisadorLexico(char programa[], int *pos){
	char lexema[100];
	int i = 0;
	while(programa[*pos]==' ' || (iscntrl(programa[*pos]) && programa[*pos]!='\0')) (*pos)++;
	while(programa[*pos]!=' ' && !iscntrl(programa[*pos])  && programa[*pos]!='\0' ){
        lexema[i] = programa[*pos];
		(*pos)++;
		i++;
	}
	if(i!=0){
		lexema[i] = ' ';
		i++;	
	} 
	lexema[i] = '\0';
	int token = scanner(lexema);
	if(!token && i!=0) imprimeErroLexico(programa, pos, lexema);
	if(i!=0)return token;
	else return 1;
	
}

void imprimeErroSintatico(char programa[], int* pos, int esperado[]){
    int i, linha = 1;
    for(i=0;i<(*pos);i++){
        if(programa[i]=='\n') linha++;
    }
    printf("ERRO: esperava token %s", buscaToken(esperado[0]));
    for(i=1;i<sizeof(*esperado)/sizeof(int);i++) printf(" ou %s",buscaToken(esperado[i]));
    printf(", encontrou %s (linha %d)\n", buscaToken(lookahead), linha);
}

//Funcao que confere se o token encontrado confere com o esperado.
int  match(int token, char programa[], int *pos){
    
    if (lookahead == token){
		lookahead = analisadorLexico(programa, pos);
		if(lookahead) return 1;
        
    }else imprimeErroSintatico(programa, pos, &token);
    return 0;
}

//---------------------------------------------------------Analisador Sintatico---------------------------------------------------------

int listaDeIdentificadores(char programa[], int *pos){
    if(!match(402, programa, pos)) return 0;
    if(lookahead==304){
        if(
           !match(304, programa, pos) ||
           !listaDeIdentificadores(programa, pos)
           ) return 0;
    }
    return 1;
}

int declaracaoDeVariaveis(char programa[], int *pos){
    if(lookahead==102){
        if(
           match(102, programa, pos) &&
           listaDeIdentificadores(programa, pos) &&
           match(303, programa, pos)
           ) return 1;
        return 0;
    }else if(lookahead==107){
        if(
           match(107, programa, pos) &&
           listaDeIdentificadores(programa, pos) &&
           match(303, programa, pos)
           ) return 1;
        return 0;
    }
    return 0;
}

int parteDeclaracaoDeVariaveis(char programa[], int *pos){
    while(lookahead==102 || lookahead==107){
        if(!declaracaoDeVariaveis(programa, pos)) return 0;
    }
    return 1;
}

int parametroFormal(char programa[], int *pos){
    if(lookahead==102){
        if(match(102, programa, pos) &&
           match(402, programa, pos)
           ) return 1;
        return 0;
    }else if(lookahead==107){
        if(match(107, programa, pos) &&
           match(402, programa, pos)
           ) return 1;
        return 0;
    }
    return 0;
}

int parametrosFormais(char programa[], int *pos){
    if(!parametroFormal(programa, pos)) return 0;
    if(lookahead==304){
        if(
           !match(304, programa, pos) ||
           !parametrosFormais(programa, pos)
           ) return 0;
    }
    return 1;
}

int declaracaoDeFuncao(char programa[], int *pos){
	if(!match(105, programa, pos) ||
       !match(402, programa, pos) ||
       !match(305, programa, pos)
       ) return 0;
	if(lookahead==102 || lookahead==107) if(!parametrosFormais(programa, pos)) return 0;
	if(!match(306, programa, pos) ||
       !match(301, programa, pos) ||
       !bloco(programa, pos) ||
       !match(302, programa, pos)
       ) return 0;
    return 1;
}

int parteDeclaracoesDeFuncoes(char programa[], int *pos){
    while(lookahead==105){
        if(!(declaracaoDeFuncao(programa, pos) &&
           match(303, programa, pos))) return 0;
    }
    return 1;
}

int booleano(char programa[], int *pos){
    if(lookahead==106){
        if(match(106, programa, pos)) return 1;
    }else if(lookahead==108){
        if(match(108, programa, pos)) return 1;
    }
    return 0;
}

int fator(char programa[], int *pos){
    if(lookahead==403){
        if(match(403, programa, pos)) return 1;
    }else if(lookahead==402){
        if(match(402, programa, pos)) return 1;
    }else if(lookahead==106 || lookahead==108){
        if(booleano(programa, pos)) return 1;
    }else if(lookahead==305){
        if(match(305, programa, pos) &&
           expressaoSimples(programa, pos) &&
           match(306, programa, pos)
           ) return 1;
    }
    return 0;
}

int termo(char programa[], int *pos){
    if(!fator(programa, pos)) return 0;
    if(lookahead==207){
        if(!match(207, programa, pos) ||
           !termo(programa, pos)
           ) return 0;
    }else if(lookahead==208){
        if(!match(208, programa, pos) ||
           !termo(programa, pos)
           ) return 0;
    }
    return 1;
}

int relacao(char programa[], int *pos){
    if(lookahead==201){
        if(match(201, programa, pos)) return 1;
    }else if(lookahead==202){
        if(match(202, programa, pos)) return 1;
    }else if(lookahead==203){
        if(match(203, programa, pos)) return 1;
    }else if(lookahead==204){
        if(match(204, programa, pos)) return 1;
    }else if(lookahead==206){
        if(match(206, programa, pos)) return 1;
    }else if(lookahead==209){
        if(match(209, programa, pos)) return 1;
    }
    return 0;
}

int expressaoSimples(char programa[], int *pos){
    if(lookahead==210){
        if(!match(210, programa, pos)) return 0;
    }else if(lookahead==211){
        if(!match(211, programa, pos)) return 0;
    }
    if(!termo(programa, pos)) return 0;
    if(lookahead==210 || lookahead==211 || lookahead==402 || lookahead==403 || lookahead==106 || lookahead==108 || lookahead==305){
        if(!expressaoSimples(programa, pos)) return 0;
    }
    return 1;
}

int expressao(char programa[], int *pos){
    if(!expressaoSimples(programa, pos)) return 0;
    if(lookahead==201 || lookahead==202 || lookahead==203 || lookahead==204 || lookahead==206 || lookahead==209){
        if(!relacao(programa, pos) ||
           !expressaoSimples(programa, pos)
           ) return 0;
    }
    return 1;
}

int atribuicao(char programa[], int *pos){
    if(match(205, programa, pos) &&
       expressao(programa, pos) &&
       match(303, programa, pos)
       ) return 1;
    return 0;
}

int listaDeParametros(char programa[], int *pos){
    if(lookahead==402){
        if(!match(402, programa, pos)) return 0;
    }else if(lookahead==106 || lookahead==108){
        if(!booleano(programa, pos)) return 0;
    }else if(lookahead==403){
        if(!match(403, programa, pos)) return 0;
    }else return 0;
    if(lookahead==304){
        if(!match(304, programa, pos) ||
           !listaDeParametros(programa, pos)
           ) return 0;
    }
    return 1;
}

int chamadaDeProcedimento(char programa[], int *pos){
    if(!match(305, programa, pos)) return 0;
    if(lookahead==402 || lookahead==106 || lookahead==108 || lookahead==403){
        if(!listaDeParametros(programa, pos)) return 0;
    }
    if(!match(306, programa, pos) ||
       !match(303, programa, pos)
       ) return 0;
    return 1;
}

int resto(char programa[], int *pos){
    if(lookahead==205){
        if(atribuicao(programa, pos)) return 1;
    }else if(lookahead==305){
        if(chamadaDeProcedimento(programa, pos)) return 1;
    }
    return 0;
}

int comandoRepetitivo(char programa[], int *pos){
    if(match(109, programa, pos) &&
       match(305, programa, pos) &&
       expressao(programa, pos) &&
       match(306, programa, pos) &&
       match(301, programa, pos) &&
       comandoComposto(programa, pos) &&
       match(302, programa, pos)
       ) return 1;
    return 0;
}

int comandoCondicional(char programa[], int *pos){
    if(!match(101, programa, pos) ||
       !match(305, programa, pos) ||
       !expressao(programa, pos) ||
       !match(306, programa, pos) ||
       !match(301, programa, pos) ||
       !comandoComposto(programa, pos) ||
       !match(302, programa, pos)
       ) return 0;
    if(lookahead==110){
        if(!match(110, programa, pos) ||
           !match(301, programa, pos) ||
           !comandoComposto(programa, pos) ||
           !match(302, programa, pos)
           ) return 1;
    }
    return 1;
}

int comando(char programa[], int *pos){
    if(lookahead==402){
        if(match(402, programa, pos) &&
           resto(programa, pos)
           ) return 1;
    }else if(lookahead==101){
        if(comandoCondicional(programa, pos)) return 1;
    }else if(lookahead==109){
        if(comandoRepetitivo(programa, pos)) return 1;
    }else if(lookahead==104){
        if(match(104, programa, pos) &&
           match(305, programa, pos) &&
           match(402, programa, pos) &&
           match(306, programa, pos) &&
           match(303, programa, pos)
           ) return 1;
    }
    return 0;
}

int comandoComposto(char programa[], int *pos){
    if(!comando(programa, pos)) return 0;
    if(lookahead==402 || lookahead==101 || lookahead==104 || lookahead==109){
        if(!comandoComposto(programa, pos)) return 0;
    }
    return 1;
}

int bloco(char programa[], int *pos){
    if(parteDeclaracaoDeVariaveis(programa, pos) &&
       parteDeclaracoesDeFuncoes(programa, pos) &&
       comandoComposto(programa, pos)
       ) return 1;
    return 0;
}

int programa(char programa[], int *pos){
	if(match(103, programa, pos) &&
       match(402, programa, pos) &&
       match(301, programa, pos) &&
       bloco(programa, pos) &&
       match(302, programa, pos)
       ) return 1;
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------

int main(){
	FILE *arquivo;
	//Nome do arquivo a ser lido pelo programa.
	char nome[] = "entrada.txt"; 
    char* entrada = leArquivo(arquivo, nome);
	int pos = 0;
	lookahead = analisadorLexico(entrada, &pos);
    if(programa(entrada, &pos)){
        printf("Analises lexica e semantica efetuadas com sucesso!\n");
        printf("Nenhum erro encontrado!\n");
    }
    return 0;
}
