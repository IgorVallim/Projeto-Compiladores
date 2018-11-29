/*
 Beatriz Godoy - 31612520
 Fernando Grangeiro - 31602843
 Igor Vallim Sordi - 31644961
 Lucas Barros - 31613144
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

//Lista encadeada, em que cada celula representa uma linha da tabela de simbolos.
struct Item{
	char *id; 
	int tipo; 
	struct Item *prox;
}; 
typedef struct Item item;

//Estrutura de dados representando um escopo.
struct Escopo{
	item *lista; //Lista de simbolos, de tamanho variavel.
	struct Escopo *anterior; //Referencia ao escopo pai.
};
typedef struct Escopo escopo;

escopo *escopoAtual; //Mantem o escopo atual em que o programa se encontra.
int lookahead;

//Prototipos das funcoes do programa
int programa();
int bloco();
int parteDeclaracaoDeVariaveis();
int declaracaoDeVariaveis();
int listaDeIdentificadores();
int parteDeclaracoesDeFuncoes();
int declaracaoDeFuncao();
int parametrosFormais();
int parametroFormal();
int comandoComposto();
int comando();
int atribuicaoOuChamadaDeProcedimento();
int atribuicao();
int chamadaDeProcedimento();
int listaDeParametros();
int comandoCondicional();
int comandoRepetitivo();
int expressao();
int relacao();
int expressaoSimples();
int termo();
int fator();
int booleano();
int match();
void imprimeErroSintatico();
void imprimeErroLexico();
int scanner();
int analisadorLexico();
void imprimeErroLexico();
char* leArquivo();
char* buscaToken();
void iniciaLista();
int vazia();
int insereSimbolo();
int criaEscopo();
int removeEscopo();
char* retornaUltimoId();
void imprimeErroDeclaracao();
void imprimeErroCompatibilidade();
int imprimeTabela();
int retornaTipo();

//Retorna o lexema equivalente ao ultimo identificador do programa, dada uma posicao de referencia.
char* retornaUltimoId(char programa[], int *pos){
	char *lexema = malloc(1000);
	int i = 0;
	int aux = *pos;
	//Retrocede uma posicao na String, ate encontrar o simbolo "_".
	while(programa[aux]!='_') aux--;
	//Avanca posicoes na String, gravando em uma variavel o identificador.
	for(; aux<=*pos; aux++){
		if(programa[aux+1]==' ' || iscntrl(programa[aux+1])) break;
		lexema[i] = programa[aux];
		i++;
	}
	lexema[i] = programa[aux];
	lexema[i+1] = '\0';
	return lexema;
}

//Le o arquivo contendo o programa fonte, e retorna uma String com seu conteudo.
char* leArquivo(FILE* arquivo, char nome[]){
	arquivo = fopen(nome, "r");
	if(!arquivo){
		printf("ERRO: arquivo nao encontrado!\n");
		exit(0);
	}
	
	//Aloca uma String com o tamanho do arquivo.
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

//Retorna um token (String), com base em seu identificador.
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

//------------------------------------ANALISADOR SEMANTICO-------------------------------------

//Inicializa uma lista de simbolos.
void iniciaLista(item *LISTA){
	LISTA->prox = NULL;
}

//Verifica se uma lista de simbolos esta vazia.
int vazia(item *LISTA){
	if(LISTA->prox == NULL)
		return 1;
	else
		return 0;
}

//Insere um novo simbolo na lista.
int insereSimbolo(char id[], int tipo){	
	item *tmp = escopoAtual->lista->prox;
		//Verifica se o simbolo ja existe no escopo atual.
		while(tmp != NULL){
			if(!strcmp(tmp->id, id)){
				return 0;
			} 
			tmp = tmp->prox;
		}
				
	//Cria uma nova celula na lista, com o conteudo dos parametros.					
	item*novo=(item*) malloc(sizeof(item));
	novo->id = id;
	novo->tipo = tipo;
	novo->prox = escopoAtual->lista->prox;
	
	//Insere a nova celula no começo da lista.
	escopoAtual->lista->prox = novo;
	
	return 1;
}

//Cria um novo escopo.
int criaEscopo(){
	escopo *ESCOPO = (escopo*) malloc(sizeof(escopo));
	//Cria uma lista de simbolos vazia para o novo escopo.
	item *LISTA = (item*) malloc(sizeof(item));
	iniciaLista(LISTA);
	ESCOPO->lista = LISTA;
	//Define o escopo atual como pai do novo escopo.
	ESCOPO->anterior = escopoAtual;
	//Define o novo escopo como escopo atual.
	escopoAtual = ESCOPO;
	return 1;
}

//Remove o ultimo escopo da lista (escopoAtual).
int removeEscopo(){
	escopoAtual = escopoAtual->anterior;
	return 1;
}

//Percorre todos os escopos da tabela de simbolos, imprimindo seus simbolos.
int imprimeTabela(){
	
	escopo *ESCOPO = escopoAtual;
	printf("----------Tabela de simbolos----------\n\n");
	if(ESCOPO!=NULL) printf("Identificador\tTipo\n\n");
	while(ESCOPO!=NULL){	
		if(!vazia(ESCOPO->lista)){
			item *tmp;
			tmp = ESCOPO->lista->prox;
			while( tmp != NULL){
				printf("%s\t\t", tmp->id);
				if(tmp->tipo==_INT_) printf("Int\n");
				else if(tmp->tipo==_BOOL_) printf("Bool\n");
				else printf("Void\n");
				tmp = tmp->prox;
			}
		
			printf("\n");
		}
		ESCOPO = ESCOPO->anterior; 	
	}
	return 1;
}

//Retorna o tipo de uma variavel, dado o seu identificador.
int retornaTipo(char id[]){
	escopo *ESCOPO = escopoAtual;
	
	while(ESCOPO!=NULL){	
		if(!vazia(ESCOPO->lista)){
			item *tmp;
			tmp = ESCOPO->lista->prox;
			while( tmp != NULL){
				if(!strcmp(tmp->id, id)) return tmp->tipo;
				tmp = tmp->prox;
			}
		}
		ESCOPO = ESCOPO->anterior; 	
	}
	return 0;
}

//Imprime os erros semanticos ligados a declaracao de novas variaveis.
void imprimeErroDeclaracao(char programa[], int *pos, char id[], int cod){
	int i, linhaNum = 1, comecoLinha = 0;
    //Percorre o vetor de entrada, do comeco ate a posicao atual, contando o numero de linhas.
    for(i=0;i<(*pos);i++){
        if(programa[i]=='\n'){
        	linhaNum++;
        	comecoLinha = i+1;
		} 
    }
    char* linha = malloc((*pos)-comecoLinha);
	int j = 0;
	i = comecoLinha;
    //Preenche a String linha com os caracteres da linha em que ocorreu o erro.
    while(programa[i]!='\n' && programa[i]!='\0'){
    	if(!iscntrl(programa[i])){
    		linha[j] = programa[i];
    		j++;	
		}
		i++;		
	}
	linha[j] = '\0';
	if(cod==1) printf("ERRO: identificador '%s' ja foi declarado nesse escopo!\n", id);
	else if(cod==2) printf("ERRO: identificador '%s' nao foi declarado!\n", id);
    printf("Linha %d: '%s'\n", linhaNum, linha);
}

//Imprime os erros semanticos ligados a imcompatibilidade de tipos.
void imprimeErroCompatibilidade(char programa[], int *pos, char id[], int cod){
	int i, linhaNum = 1, comecoLinha = 0;
    //Percorre o vetor de entrada, do comeco ate a posicao atual, contando o numero de linhas.
    for(i=0;i<(*pos);i++){
        if(programa[i]=='\n'){
        	linhaNum++;
        	comecoLinha = i+1;
		} 
    }
    char* linha = malloc((*pos)-comecoLinha);
    char* exp = malloc((*pos)-comecoLinha);
	int j = 0;
	i = comecoLinha;
	int k = 0, flag = 0, cont = 0;
    //Preenche a String linha com os caracteres da linha em que ocorreu o erro, e a String exp com a expressao de tipo conflitante.
    while(programa[i]!='\n' && programa[i]!='\0'){
		if((programa[i]==';') || (programa[i]==')' && cod==1 && cont==0)){
    		flag = 0;
		}
		if(!iscntrl(programa[i])){
    		printf("Flag: %d\n", flag);
			if(flag){
    			if(programa[i]=='(') cont++;
    			if(programa[i]==')') cont--;
    			exp[k] = programa[i];
    			k++;
			}
			linha[j] = programa[i];
    		j++;	
		}
		if((programa[i]=='=') || (programa[i]=='(' && cod==1)){
            flag = 1;
		}
		i++;		
	}
	exp[k] = '\0';
	linha[j] = '\0';

	if(cod==1) printf("ERRO: A expressao '%s' contem inteiros e booleanos!\n", exp);
	else if(cod==2) printf("ERRO: Tipos incompativeis entre o identificador '%s' e a expressao '%s'!\n", id, exp);
    printf("Linha %d: '%s'\n", linhaNum, linha);
}

//-------------------------------------ANALISADOR LEXICO---------------------------------------

//Funcao que faz a analise lexica de trecho do programa fonte.
int analisadorLexico(char programa[], int *pos){
	char lexema[100];
	int i = 0;
    //Pula todos os espcos em branco e caracteres de controle antes de comecar a ler o lexema.
	while(programa[*pos]==' ' || (iscntrl(programa[*pos]) && programa[*pos]!='\0')) (*pos)++;
    //Verifica se o lexema e um comentario (pode conter espacos em branco em seu interior).
	if(programa[(*pos)]=='/' && programa[(*pos)+1]=='*'){
		//Se for um comentario, percorre o programa ate encontrar a marcacao de final de comentario, ou final do programa.
        while(!(programa[(*pos)-2]=='*' && programa[(*pos)-1]=='/') && programa[*pos]!='\0'){
			lexema[i] = programa[*pos];
			(*pos)++;
			i++;
		}	
	}else{
        //Senao, percorre ate encontrar um espaco em branco ou quebra de linha.
		while(programa[*pos]!=' ' && !iscntrl(programa[*pos])  && programa[*pos]!='\0' ){
        	lexema[i] = programa[*pos];
			(*pos)++;
			i++;
		}	
	}
    //Se o lexema nao estiver vazio, adiciona um espaco em branco no final do mesmo.
	if(i!=0){
		lexema[i] = ' ';
		i++;	
	} 
	lexema[i] = '\0';
	int token = scanner(lexema);
    //Se o scanner retornar 0, e o lexema nao for vazio, imprime erro lexico.
	if(!token && i!=0) imprimeErroLexico(programa, pos, lexema);
	if(i!=0)return token;
	else return 1;
	
}

//Funcao que imprime erros lexicos.
void imprimeErroLexico(char programa[], int *pos, char lexema[]){
    int i, linhaNum = 1, comecoLinha = 0;
    //Percorre o vetor de entrada, do comeco ate a posicao atual, contando o numero de linhas.
    for(i=0;i<(*pos);i++){
        if(programa[i]=='\n'){
        	linhaNum++;
        	comecoLinha = i+1;
		} 
    }
    char* linha = malloc((*pos)-comecoLinha);
	int j = 0;
	i = comecoLinha;
    //Preenche a String linha com os caracteres da linha em que ocorreu o erro.
    while(programa[i]!='\n' && programa[i]!='\0'){
    	if(!iscntrl(programa[i])){
    		linha[j] = programa[i];
    		j++;	
		}
		i++;		
	}
	linha[j] = '\0';
    printf("ERRO: lexema '%s' nao reconhecido!\n", lexema);
    printf("Linha %d: '%s'\n", linhaNum, linha);
}

//Funcao que representa o afd do projeto, retornando um token correspondente a entrada.
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
    //Representaco de uma transicao.
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
	
q85: 
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

//-----------------------------------ANALISADOR SINTATICO--------------------------------------

//Funcao que confere se o token encontrado e o token esperado sao iguais.
int  match(int token, char programa[], int *pos){
    if (lookahead == token){
		lookahead = analisadorLexico(programa, pos);
		//Ignora todos os tokens de comentario, antes de comecar a leitura.
        while(lookahead==_COMENTARIO_) lookahead = analisadorLexico(programa, pos);
        if(lookahead) return 1;
    }else imprimeErroSintatico(programa, pos, &token);
    return 0;
}

//Funcao que imprime erros sintaticos.
void imprimeErroSintatico(char programa[], int* pos, int esperado[]){
    int i, comecoLinha = 0, linhaNum = 1;
    //Percorre o vetor de entrada, do comeco ate a posicao atual, contando o numero de linhas e armazenando a posicao do ultimo inicio de linha.
    for(i=0;i<(*pos);i++){
        if(programa[i]=='\n'){
        	linhaNum++;
        	comecoLinha = i+1;
		} 
    }
	char* linha = malloc((*pos)-comecoLinha);
	int j = 0;
	i = comecoLinha;
    //Preenche a String linha com os caracteres da linha em que ocorreu o erro, para ser imprimida posteriormente.
    while(programa[i]!='\n' && programa[i]!='\0'){
    	if(!iscntrl(programa[i])){
    		linha[j] = programa[i];
    		j++;	
		}
		i++;
    		
	}
	linha[j] = '\0';
    printf("ERRO: esperava token %s", buscaToken(esperado[0]));
    i = 1;
    //Existem situacoes onde mais de um token e esperado, imprime todos esses tokens.
    while(esperado[i]>=101 && esperado[i]<=403){
        printf(" ou %s",buscaToken(esperado[i]));
        i++;
    }
    printf(", encontrou %s\n", buscaToken(lookahead));
    printf("Linha %d: '%s'\n", linhaNum, linha);
    
}

//Regra #1:  <programa> ::=  program <identificador>  { <bloco> }	---MODIFICADA NA ULTIMA ENTREGA--- 
int programa(char programa[], int *pos){
	if(criaEscopo() && //Cria um novo escopo.
	   match(_PROGRAM_, programa, pos) &&
       match(_IDENTIFICADOR_, programa, pos) &&
       match(_ABRE_CHAVE_, programa, pos) &&
       bloco(programa, pos) &&
       match(_FECHA_CHAVE_, programa, pos) &&
       removeEscopo() //Remove o ultimo escopo da lista.
       ) return 1;
       
    return 0;
}

//Regra #2:  <bloco> ::= <parte declaracoes de variaveis> <parte declaracoes de funcoes> <comando composto> 
int bloco(char programa[], int *pos){
    if(parteDeclaracaoDeVariaveis(programa, pos) &&
       parteDeclaracoesDeFuncoes(programa, pos) &&
       comandoComposto(programa, pos) 
	   ) return 1;
    return 0;
}

//Regra #3:  <parte declaracoes de variaveis> ::= { <declaracao de variaveis> }
int parteDeclaracaoDeVariaveis(char programa[], int *pos){
    while(lookahead==_INT_ || lookahead==_BOOL_){
        if(!declaracaoDeVariaveis(programa, pos)) return 0;
    }
    return 1;
}

//Regra #4:  <declaracao de variaveis> ::= ( int | bool ) <lista de identificadores>  ;
int declaracaoDeVariaveis(char programa[], int *pos){
	if(lookahead==_INT_){
        int tipo = _INT_;
		if(
           match(_INT_, programa, pos) &&
           listaDeIdentificadores(programa, pos, tipo) &&
           match(_PONTO_E_VIRGULA_, programa, pos)
           ) return 1;
        return 0;
    }else if(lookahead==_BOOL_){
        int tipo = _BOOL_;
		if(
           match(_BOOL_, programa, pos) &&
           listaDeIdentificadores(programa, pos, tipo) &&
           match(_PONTO_E_VIRGULA_, programa, pos)
           ) return 1;
        return 0;
    }else{
        //Envia uma lista contendo todos os tokens esperados para serem imprimidos.
        int esperado[2] = {_INT_, _BOOL_};
        imprimeErroSintatico(programa, pos, esperado);
    }
    return 0;
}

//Regra #5:  <lista de identificadores> ::= <identificador> [ , <lista de identificadores> ]	---MODIFICADA NA ULTIMA ENTREGA---  
int listaDeIdentificadores(char programa[], int *pos, int tipo){
    if(!match(_IDENTIFICADOR_, programa, pos)) return 0;
    char *id = retornaUltimoId(programa, pos); 
    //Insere o ultimo identificador do programa na tabela de simbolos.
    if(!insereSimbolo(retornaUltimoId(programa, pos), tipo)){ 
    	imprimeErroDeclaracao(programa, pos, id, 1);
		return 0;	
	} 
    if(lookahead==_VIRGULA_){
        if(
           !match(_VIRGULA_, programa, pos) ||
           !listaDeIdentificadores(programa, pos, tipo)
           ) return 0;
    }
    return 1;
}

//Regra #6:  <parte declaracoes de funcoes> ::= { <declaração de funcao> ; }
int parteDeclaracoesDeFuncoes(char programa[], int *pos){
    while(lookahead==_VOID_){
        if(!(declaracaoDeFuncao(programa, pos) &&
           match(_PONTO_E_VIRGULA_, programa, pos))) return 0;
    }
    return 1;
}

//Regra #7:  <declaração de funcao> ::= void <identificador>  ( [ <parametros formais> ] ) { <bloco> }	---MODIFICADA NA ULTIMA ENTREGA--- 
int declaracaoDeFuncao(char programa[], int *pos){
	if(!match(_VOID_, programa, pos) ||
       !match(_IDENTIFICADOR_, programa, pos) ||
       !match(_ABRE_PARENTESE_, programa, pos)
       ) return 0;   
    char *id = retornaUltimoId(programa, pos);
    //Insere o ultimo identificador do programa na tabela de simbolos.
    if(!insereSimbolo(id, _VOID_)){
    	imprimeErroDeclaracao(programa, pos, id, 1);
		return 0;	
	} 
	if(!criaEscopo()) return 0; //Cria um novo escopo.
	if(lookahead==_INT_ || lookahead==_BOOL_) if(!parametrosFormais(programa, pos)) return 0;
	if(!match(_FECHA_PARENTESE_, programa, pos) ||
       !match(_ABRE_CHAVE_, programa, pos) ||
       !bloco(programa, pos) ||
       !match(_FECHA_CHAVE_, programa, pos) ||
       !removeEscopo() //Remove o ultimo escopo da lista.
       ) return 0;
    return 1;
}

//Regra #8:  <parametros formais> ::= <parametro formal> [ , <parametros formais> ]
int parametrosFormais(char programa[], int *pos){
    if(!parametroFormal(programa, pos)) return 0;
    if(lookahead==_VIRGULA_){
        if(
           !match(_VIRGULA_, programa, pos) ||
           !parametrosFormais(programa, pos)
           ) return 0;
    }
    return 1;
}

//Regra #9:  <parametro formal> ::= ( int | bool ) <identificador> ---MODIFICADA NA ULTIMA ENTREGA---  
int parametroFormal(char programa[], int *pos){
    if(lookahead==_INT_){
        if(!match(_INT_, programa, pos) ||
           !match(_IDENTIFICADOR_, programa, pos) 
           ) return 0;
		char *id = retornaUltimoId(programa, pos);
		//Insere o ultimo identificador do programa na tabela de simbolos.
		if(!insereSimbolo(id, _INT_)){
			imprimeErroDeclaracao(programa, pos, id, 1);
			return 0;
		}   
        return 1;
    }else if(lookahead==_BOOL_){
        if(!match(_BOOL_, programa, pos) ||
           !match(_IDENTIFICADOR_, programa, pos) 
           ) return 0;
        char *id = retornaUltimoId(programa, pos);
        //Insere o ultimo identificador do programa na tabela de simbolos.
		if(!insereSimbolo(id, _BOOL_)){
			imprimeErroDeclaracao(programa, pos, id, 1);
			return 0;
		}   
        return 1;
    }else{
        int esperado[2] = {_INT_, _BOOL_};
        imprimeErroSintatico(programa, pos, esperado);
    }
    return 0;
}

//Regra #10:  <comando composto> ::= <comando> [ <comando composto> ]
int comandoComposto(char programa[], int *pos){
    if(!comando(programa, pos)) return 0;
    if(lookahead==_IDENTIFICADOR_ || lookahead==_IF_ || lookahead==_PRINT_ || lookahead==_WHILE_){
        if(!comandoComposto(programa, pos)) return 0;
    }
    return 1;
}

/*Regra #11:  <comando> ::= <identificador> <atribuicao ou chamada de procedimento> | <comando condicional> 
                            | <comando repetitivo> | print ( <identificador> ) ;	---MODIFICADA NA ULTIMA ENTREGA---   */
int comando(char programa[], int *pos){
    if(lookahead==_IDENTIFICADOR_){
        if(match(_IDENTIFICADOR_, programa, pos) &&
           atribuicaoOuChamadaDeProcedimento(programa, pos)
           ) return 1;
    }else if(lookahead==_IF_){
        if(comandoCondicional(programa, pos)) return 1;
    }else if(lookahead==_WHILE_){
        if(comandoRepetitivo(programa, pos)) return 1;
    }else if(lookahead==_PRINT_){
        if(match(_PRINT_, programa, pos) &&
           match(_ABRE_PARENTESE_, programa, pos) &&
           match(_IDENTIFICADOR_, programa, pos) &&
           match(_FECHA_PARENTESE_, programa, pos) &&
           match(_PONTO_E_VIRGULA_, programa, pos) &&
           //Imprime tabela de simbolos.
		   imprimeTabela()
           ) return 1;
    }else{
        int esperado[4] = {_IDENTIFICADOR_, _IF_, _WHILE_, _PRINT_};
        imprimeErroSintatico(programa, pos, esperado);
    }
    return 0;
}

//Regra #12:  <atribuicao ou chamada de procedimento> ::= <atribuicao> | <chamada de procedimento> 
int atribuicaoOuChamadaDeProcedimento(char programa[], int *pos){
    if(lookahead==_ATRIBUICAO_){
        if(atribuicao(programa, pos)) return 1;
    }else if(lookahead==_ABRE_PARENTESE_){
        if(chamadaDeProcedimento(programa, pos)) return 1;
    }else{
        int esperado[2] = {_ATRIBUICAO_, _ABRE_PARENTESE_};
        imprimeErroSintatico(programa, pos, esperado);
    }
    return 0;
}

//Regra #13:   <atribuicao> ::= = <expressao> ;	---MODIFICADA NA ULTIMA ENTREGA---  
int atribuicao(char programa[], int *pos){
    char *id = retornaUltimoId(programa, pos);
    //Verifica se o identificador ja foi declarada previamente.
    if(!retornaTipo(id)){
    	imprimeErroDeclaracao(programa, pos, id, 2);
    	return 0;
	}
	int tipo;
	if(!match(_ATRIBUICAO_, programa, pos) ||
       !expressao(programa, pos, &tipo) 
       ) return 0;
    //Verifica se o tipo da variavel e igual ao da expressao atribuida a ela.
	if(tipo!=retornaTipo(id)){
    	imprimeErroCompatibilidade(programa, pos, id, 2);
		return 0;	
	} 
	if(match(_PONTO_E_VIRGULA_, programa, pos)) return 1;
    return 0;
}

//Regra #14:  <chamada de procedimento> ::= ( [ <lista de parametros> ] );	---MODIFICADA NA ULTIMA ENTREGA--- 
int chamadaDeProcedimento(char programa[], int *pos){
    char *id = retornaUltimoId(programa, pos);
    //Verifica se o identificador ja foi previamente declarado.
	if(!retornaTipo(id)){
    	imprimeErroDeclaracao(programa, pos, id, 2);
    	return 0;
	}
	if(!match(_ABRE_PARENTESE_, programa, pos)) return 0;
    if(lookahead==_IDENTIFICADOR_ || lookahead==_TRUE_ || lookahead==_FALSE_ || lookahead==_VALOR_){
        if(!listaDeParametros(programa, pos)) return 0;
    }
    if(!match(_FECHA_PARENTESE_, programa, pos) ||
       !match(_PONTO_E_VIRGULA_, programa, pos)
       ) return 0;
    return 1;
}

//Regra #15:  <lista de parametros> ::= ( <identificador> | <booleano> | <valor> ) [, <lista de parametros> ] ---MODIFICADA NA ULTIMA ENTREGA--- 
int listaDeParametros(char programa[], int *pos){
    if(lookahead==_IDENTIFICADOR_){
		if(!match(_IDENTIFICADOR_, programa, pos)) return 0;
		char *id = retornaUltimoId(programa, pos);
    	//Verifica se o identificador foi previamente declarado.
		if(!retornaTipo(id)){
    		imprimeErroDeclaracao(programa, pos, id, 2);
    		return 0;
		}
    }else if(lookahead==_TRUE_ || lookahead==_FALSE_){
        if(!booleano(programa, pos)) return 0;
    }else if(lookahead==_VALOR_){
        if(!match(_VALOR_, programa, pos)) return 0;
    }else{
        int esperado[4] = {_IDENTIFICADOR_, _TRUE_, _FALSE_, _VALOR_};
        imprimeErroSintatico(programa, pos, esperado);
        return 0;
    }
    if(lookahead==_VIRGULA_){
        if(!match(_VIRGULA_, programa, pos) ||
           !listaDeParametros(programa, pos)
           ) return 0;
    }
    return 1;
}

//Regra #16:  <comando condicional> ::= if ( <expressão> ) { <comando composto> } [ else { <comando composto> } ]
int comandoCondicional(char programa[], int *pos){
    int tipo;
	if(!match(_IF_, programa, pos) ||
       !match(_ABRE_PARENTESE_, programa, pos) ||
       !expressao(programa, pos, &tipo) ||
       !match(_FECHA_PARENTESE_, programa, pos) ||
       !match(_ABRE_CHAVE_, programa, pos) ||
       !comandoComposto(programa, pos) ||
       !match(_FECHA_CHAVE_, programa, pos)
       ) return 0;
    if(lookahead==_ELSE_){
        if(!match(_ELSE_, programa, pos) ||
           !match(_ABRE_CHAVE_, programa, pos) ||
           !comandoComposto(programa, pos) ||
           !match(_FECHA_CHAVE_, programa, pos)
           ) return 1;
    }
    return 1;
}

//Regra #17:  <comando repetitivo> ::= while ( <expressão> ) { <comando composto> }
int comandoRepetitivo(char programa[], int *pos){
    int tipo;
	if(match(_WHILE_, programa, pos) &&
       match(_ABRE_PARENTESE_, programa, pos) &&
       expressao(programa, pos, &tipo) &&
       match(_FECHA_PARENTESE_, programa, pos) &&
       match(_ABRE_CHAVE_, programa, pos) &&
       comandoComposto(programa, pos) &&
       match(_FECHA_CHAVE_, programa, pos)
       ) return 1;
    return 0;
}

//Regra #18: <expressao> ::= <expressao simples> [ <relacao> <expressao simples> ]	---MODIFICADA NA ULTIMA ENTREGA--- 
int expressao(char programa[], int *pos, int *tipo){
    if(!expressaoSimples(programa, pos, tipo)) return 0;
    int tipo2 = *tipo;
    if(lookahead==_MAIOR_IGUAL_ || lookahead==_MAIOR_ || lookahead==_MENOR_ || lookahead==_MENOR_IGUAL_ || lookahead==_IGUALDADE_ || lookahead==_DIFERENTE_){
        if(!relacao(programa, pos) ||
           !expressaoSimples(programa, pos, tipo)
           ) return 0;
        //Verifica se ambas as partes da expressao possuem o mesmo tipo.   
        if(*tipo!=tipo2){
        	imprimeErroCompatibilidade(programa, pos, "", 1);	
        	return 0;
		}     
    }
    return 1;
}

//Regra #19: <relacao> ::= == | != | < | <= | >= | >
int relacao(char programa[], int *pos){
    if(lookahead==_MAIOR_IGUAL_){
        if(match(_MAIOR_IGUAL_, programa, pos)) return 1;
    }else if(lookahead==_MAIOR_){
        if(match(_MAIOR_, programa, pos)) return 1;
    }else if(lookahead==_MENOR_){
        if(match(_MENOR_, programa, pos)) return 1;
    }else if(lookahead==_MENOR_IGUAL_){
        if(match(_MENOR_IGUAL_, programa, pos)) return 1;
    }else if(lookahead==_IGUALDADE_){
        if(match(_IGUALDADE_, programa, pos)) return 1;
    }else if(lookahead==_DIFERENTE_){
        if(match(_DIFERENTE_, programa, pos)) return 1;
    }else{
        int esperado[6] = {_MAIOR_IGUAL_, _MAIOR_, _MENOR_, _MENOR_IGUAL_, _IGUALDADE_, _DIFERENTE_};
        imprimeErroSintatico(programa, pos, esperado);
    }
    return 0;
}

//Regra #20: <expressao simples> ::= [ + | - ] <termo> [ <expressao simples> ] ---MODIFICADA NA ULTIMA ENTREGA--- 
int expressaoSimples(char programa[], int *pos, int *tipo){
    if(lookahead==_MAIS_){
        if(!match(_MAIS_, programa, pos)) return 0;
    }else if(lookahead==_MENOS_){
        if(!match(_MENOS_, programa, pos)) return 0;
    }
    if(!termo(programa, pos, tipo)) return 0;
    int tipo2 = *tipo;
    if(lookahead==_MAIS_ || lookahead==_MENOS_ || lookahead==_IDENTIFICADOR_ || lookahead==_VALOR_ || lookahead==_TRUE_ || lookahead==_FALSE_ || lookahead==_ABRE_PARENTESE_){
        if(!expressaoSimples(programa, pos, tipo)) return 0;
        //Verifica se ambas as partes da expressao possuem o mesemo tipo.
		if(*tipo!=tipo2){
        	imprimeErroCompatibilidade(programa, pos, "", 1);	
        	return 0;
		}  
    }
    return 1;
}


//Regra #21: <termo> ::= <fator> [ (  * | / ) <termo> ]	---MODIFICADA NA ULTIMA ENTREGA--- 
int termo(char programa[], int *pos, int *tipo){
    if(!fator(programa, pos, tipo)) return 0;
    int tipo2 = *tipo;
	if(lookahead==_DIVIDIDO_){
		if(!match(_DIVIDIDO_, programa, pos) ||
           !termo(programa, pos, tipo)
           ) return 0;
        //Verifica se termo e fator possuem o mesmo tipo.   
        if(*tipo!=tipo2){
        	imprimeErroCompatibilidade(programa, pos, "", 1);	
        	return 0;
		}    
        return 1;
    }else if(lookahead==_VEZES_){
        if(!match(_VEZES_, programa, pos) ||
           !termo(programa, pos, tipo)
           ) return 0;
        //Verifica se termo e fator possuem o mesmo tipo.     
        if(*tipo!=tipo2){
        	imprimeErroCompatibilidade(programa, pos, "", 1);	
        	return 0;
		}  
        return 1;
    }
    return 1;
}

//Regra #22:  <fator> ::= <valor> | <identificador> | <booleano> | ( <expressao simples> )	---MODIFICADA NA ULTIMA ENTREGA--- 
int fator(char programa[], int *pos, int *tipo){
    if(lookahead==_VALOR_){
        if(match(_VALOR_, programa, pos)){
			*tipo = _INT_;
			return 1;	
		} 
    }else if(lookahead==_IDENTIFICADOR_){
        if(match(_IDENTIFICADOR_, programa, pos)){
        	char *id = retornaUltimoId(programa, pos);
    		//Verifica se o identificador ja foi previamente declarado.
			if(!retornaTipo(id)){
    			imprimeErroDeclaracao(programa, pos, id, 2);
    			return 0;
			}
			//Armazena o tipo do identificador.
			*tipo = retornaTipo(id);
			return 1;	
		} 
    }else if(lookahead==_TRUE_ || lookahead==_FALSE_){
        if(booleano(programa, pos)){
        	*tipo = _BOOL_;
			return 1;	
		}
    }else if(lookahead==_ABRE_PARENTESE_){
        if(match(_ABRE_PARENTESE_, programa, pos) &&
           expressao(programa, pos, tipo) &&
           match(_FECHA_PARENTESE_, programa, pos)
           ) return 1;
    }else{
        int esperado[5] = {_VALOR_, _IDENTIFICADOR_, _TRUE_, _FALSE_, _ABRE_PARENTESE_};
        imprimeErroSintatico(programa, pos, esperado);
    }
   
    return 0;
}

//Regra #23:  <booleano> ::= true | false
int booleano(char programa[], int *pos){
    if(lookahead==_TRUE_){
        if(match(_TRUE_, programa, pos)) return 1;
    }else if(lookahead==_FALSE_){
        if(match(_FALSE_, programa, pos)) return 1;
    }else{
        int esperado[2] = {_TRUE_, _FALSE_};
        imprimeErroSintatico(programa, pos, esperado);
    }
    return 0;
}

//-------------------------------------------MAIN----------------------------------------------

int main(){
	FILE *arquivo;
	//Nome do arquivo a ser lido pelo programa.
	char nome[] = "entrada.txt"; 
    char* entrada = leArquivo(arquivo, nome);
	int pos = 0;
	escopoAtual = NULL;
    //Instancia o lookahead para o primeiro token do programa.
	lookahead = analisadorLexico(entrada, &pos);
    //Ignora todos os tokens de comentario, antes de comecar a leitura.
    while(lookahead==_COMENTARIO_) lookahead = analisadorLexico(entrada, &pos);
    if(programa(entrada, &pos)){
        printf("Analises lexica, sintatica e semantica efetuadas com sucesso!\n");
        printf("Nenhum erro encontrado!\n");
    }
    
    return 0;
}
