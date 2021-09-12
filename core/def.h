
#ifndef __BLEP_DEF_H
#define __BLEP_DEF_H

#define ERROR__UNEXPECTED -1
#define ERROR__STACK -2    
#define ERROR__INTERNAL -3 
#define ERROR__TODO -4

#define TOKEN_EOF 0
#define TOKEN_LIT 1       
#define TOKEN_SEMICOLON 2 
#define TOKEN_OP 3        
#define TOKEN_COLON 4     
#define TOKEN_BRACE 5     
#define TOKEN_ARRAY 6     
#define TOKEN_PAREN 7     
#define TOKEN_TERNARY 8   
#define TOKEN_CLOSE 9     
#define TOKEN_STRING 10
#define TOKEN_REGEXP 11 
#define TOKEN_NUMBER 12
#define TOKEN_SYMBOL 13 
#define TOKEN_KEYWORD 14
#define TOKEN_LABEL 15 
#define TOKEN_BLOCK 16 
#define _TOKEN_MAX 16

#define SPECIAL__NEWLINE 1
#define SPECIAL__DECLARE 2 
#define SPECIAL__TOP 4     
#define SPECIAL__PROPERTY 8
#define SPECIAL__CHANGE 16        
#define SPECIAL__EXTERNAL 32      
#define SPECIAL__DESTRUCTURING 64 
#define SPECIAL__DEFAULT 128      
#define SPECIAL__LIT (1 << 30)

#define STACK__EXPR 1     
#define STACK__DECLARE 2  
#define STACK__CONTROL 3  
#define STACK__BLOCK 4    
#define STACK__FUNCTION 5 
#define STACK__CLASS 6    
#define STACK__MISC 7     
#define STACK__LABEL 8    
#define STACK__EXPORT 9   
#define STACK__MODULE 10  
#define STACK__INNER 11   
#define _STACK_MAX 11

#endif 
