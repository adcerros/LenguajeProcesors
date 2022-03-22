//Adrián Cerros Sánchez, Javier Rodriguez Macias,  Grupo 5  
//100405342@alumnos.uc3m.es, 100405873@alumnos.uc3m.es


#include <stdio.h>
#include <stdlib.h>

#define T_NUMBER 1001
#define T_OP     1002		

// Prototype for forward reference 
int ParseExpression_One () ;	
int ParseExpression_Two () ;
int ParseNumber() ;

int token ;			// Here we store the current token/literal 
int number ;		// and the value of the number 

int line_counter = 1 ;


int rd_lex ()
{
	int c ;
	
	do {
		c = getchar () ;
	} while (c == ' ' || c == '\t') ;
	
	if (isdigit (c)){
		ungetc (c, stdin) ;
		scanf ("%d", &number) ;
		token = T_NUMBER ;
		return (token) ;	// returns the Token for Number
	}

	if (c == '\n')
		line_counter++ ;	// info for rd_syntax_error ()
	
	token = c ;
	return (token) ;	// returns a literal
}


void rd_syntax_error (int expected, int token, char *output) 
{
	fprintf (stderr, "ERROR near line %d ", line_counter) ;
	fprintf (stderr, output, token, expected) ;
	
	exit (0);
}


void MatchSymbol (int expected_token)
{
	if (token != expected_token){
		rd_syntax_error (expected_token, token, "token %d expected, but %d was read") ;
	}
	rd_lex () ;
}


#define ParseLParen() 	MatchSymbol ('(') ; // More concise and efficient definitions
#define ParseRParen() 	MatchSymbol (')') ; // rather than using functions
											// This is only useful for matching Literals


// Sintax::= Expresion_1 ExpressionRest_1     +    ExpressionRest_1 ::= + Sintax | - Sintax | lambda 
int Sintax () {
    int val1;
    int val2;
    int operator;
    // Se procesa Expresion_1
    val1 = ParseExpression_One();
    // Para el resto de casos se devuelve el valor1
    if (token == '\n' || token == ')'){	
		return val1;
	}
    // Se deriva E y se trata el simbolo
    switch (token){		
        case '+' :  
            operator = token;	
            break;
        case '-' :
            operator = token;
            break;
        default :   
            rd_syntax_error (token, 0, "Token %d was read, but an Operator was expected");
            break;
    }
    // Se mueve la entrada y se procesa Sintax
    rd_lex () ;
    val2 = Sintax() ;
    // Acciones semanticas
    switch (operator){
        case '+' :  
            val1 += val2;
            break;
        case '-' :
            val1 -= val2;
            break;
    }
    return val1;
}

// Expresion_1 ::= Expresion_2 ExpressionRest_2     +    ExpressionRest_2 ::= * Expression_1 | / Expression_1 | lambda 
int ParseExpression_One() {
    int val1;
    int val2;
    int operator;
    // Se procesa B
    val1 = ParseExpression_Two();
    // Se deriva y se trata el simbolo
    if (token == '/' || token == '*'){
        switch (token){		
            case '/' :  
                operator = token;
                break;		
            case '*' :
                operator = token;
                break;
        }
        // Se mueve la entrada y se procesa Expresion_1
        rd_lex () ;
        val2 = ParseExpression_One();
        // Acciones semanticas
        switch (operator){
            case '/' :  
                val1 /= val2;
                break;
            case '*' :  
                val1 *= val2;
                break;
        return val1;
        }
    }
    // Para el resto de casos se devuelve el valor1
    else{
        return val1;
    }
}


// Expresion_2 ::= Number | (Sintax) | + Expresion_2 | -Expresion_2
int ParseExpression_Two(){
    int val1;
    // Si primero es un parentesis no encontramos en el caso (Sintax)
    if (token == '('){	
        // Se procesan los parentesis, se deriva Sintax y finalmente se retorna val1
		ParseLParen();
		val1 = Sintax();
		ParseRParen();	
        return val1;
	}
    // Si primero es un numero no encontramos en el caso Number
    else if (token == T_NUMBER){	
        // Se devuelve el numero	
        return ParseNumber();
    }
    // Para el resto de casos se trata un unario
    char simbol1;	
    simbol1 = token;
    rd_lex () ;
    // Acciones semanticas
    switch (simbol1){
        // Cuando finalmente se recibe un numero (caso base) se realizan sobre el todos los cambios de signo sin resolver en las capas superiores de la recursion
        // Por ultimo,  se devuelve el numero
        case '+' :  
            val1 = ParseExpression_Two();
            return val1;
            break;
        case '-' :  
            val1 = ParseExpression_Two();
            val1 = -val1;
            return val1;
            break;	
	}
}




int ParseNumber () 		
{						
	MatchSymbol (T_NUMBER) ;
	return number;
}


int main (void){

	while (1){
		rd_lex () ;
		printf ("Valor %d\n", Sintax()) ;
	}	
	
	system ("PAUSE") ;
}
