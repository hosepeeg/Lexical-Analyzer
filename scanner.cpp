#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

using namespace std;

enum tokenType {
    AND, BEGIN, END, FOR, IF, NOT, OR, READ,WHILE, WRITE, COMMENT, ID, REAL, STRING,
    PLUS, MULTIPLICATION, ASSIGNMENT, EQUAL, GREATERTHAN, LESSTHAN, LEFTP, COMMA,
    RIGHTP, SEMICOLON, INVALID, DIVISION, INTEGER
};

char tokenBuffer[80];

/* Function prototypes */
void skipSpaces(void);
void clear_buf(void);
void display_token(tokenType code);
tokenType lexical_error(void);
tokenType scanner(void);
tokenType getId(void);
tokenType getComment(void);
tokenType getReal(void);
tokenType getString(void);
tokenType getAssign(void);
tokenType getPlus(void);
tokenType getMul(void);
tokenType getEqual(void);
tokenType getGreater(void);
tokenType getLess(void);
tokenType getLP(void);
tokenType getRP(void);
tokenType getComma(void);
tokenType getSColon(void);

int main()
{
    cout << "TOKENS/TYPES        \tWORDS/LEXEMES" << endl;
    cout << "------------        \t-------------" << endl;
    tokenType code;
    
    while ((int)code != EOF)
    {
        clear_buf();
        code = scanner();
        
        if ((int)code != EOF)
            display_token(code);
    }
    
    return 0;
}

/* IT keeps reading every character that is a space
 * When it finds a character that is not a space, it puts it
 * back and returns
 */
void skipSpaces(void)
{
    int ch = cin.get();
    while (isspace(ch)) ch = cin.get();
    cin.putback(ch);
}

/* Clears the buffer by making null in every index */
void clear_buf(void)
{
    int i;
    for (i = 0; i < 80; i++)
        tokenBuffer[i] = '\0';
}

/* Displays the token type and the word in a table-like manner */
void display_token(tokenType code)
{
    static char message[][20] =  { "And", "Begin", "End", "For", "If", "Not",
    "Or", "Read", "While", "Write", "Comment", "Identifier",  "Real Constant",
    "String", "Plus", "Multiplication", "Assignment", "Equal", "Greater than",
    "Less than", "Left parenthesis", "Comma", "Right parenthesis", "Semicolon",
    "Invalid", "Division", "Integer"
    };
    
    cout << setw(20) << left << message[(int)code] << "\t" << tokenBuffer << endl;
}

/* Scanner: Calls a functino depending on the read character.
 * If the character is not recognized, lexical_error is called.
 * Otherwise, depending on the character read, it calls the function
 * to extract the word, classify it, and add it to the buffer.
 */
tokenType scanner(void)
{
    skipSpaces();
    int character = cin.get();
    
    if (character == EOF)
        return (tokenType)EOF;
    
    if (character == '/')
    {
        cin.putback(character);
        return getComment();
    }
    
    if (isalpha(character))
    {
        cin.putback(character);
        return getId();
    }
    
    if (isdigit(character))
    {
        cin.putback(character);
        return getReal();
    }
    
    if (character == '\"')
    {
        cin.putback(character);
        return getString();
    }
    
    if (character == '+')
    {
        cin.putback(character);
        return getPlus();
    }
    
    if (character == ':')
    {
        cin.putback(character);
        return getAssign();
    }
    
    if (character == '>')
    {
        cin.putback(character);
        return getGreater();
    }
    
    if (character == '<')
    {
        cin.putback(character);
        return getLess();
    }
    
    if (character == '(')
    {
        cin.putback(character);
        return getLP();
    }
    
    if (character == ',')
    {
        cin.putback(character);
        return getComma();
    }
    
    if (character == ')')
    {
        cin.putback(character);
        return getRP();
    }
    
    if (character == ';')
    {
        cin.putback(character);
        return getSColon();
    }
    
    /* If it gets to this point, it can only be a lexical error */
    cin.putback(character);
    return lexical_error();
}

/* read next character, and return INVALID token code */
tokenType lexical_error(void)
{
    tokenBuffer[0] = cin.get();
    return INVALID;
}

tokenType getId(void)
{
    static char reservedWord[][10] = {"AND", "BEGIN", "END", "FOR", "IF", "NOT",
        "OR", "READ", "WHILE", "WRITE"};
    
    tokenType code;
    int ch, i = 0;
    ch = cin.get(  );
    if (isalpha(ch))
    {
        tokenBuffer[i++]  = ch;
        ch = cin.get();
        while (isalnum(ch))
        {
            tokenBuffer[i++] = ch;
            ch = cin.get();
        }
        cin.putback(ch);
        
        /*---- find out if it is a reserved word using the binary search--*/
        int first = 0, mid, last = 9;
        bool notFound  = true;
        while (first <= last && notFound)
        {
            mid = (first + last) / 2;
            int  answer =  strcmp( tokenBuffer, reservedWord[ mid ] );
            if(  answer == 0 )
                notFound = false;
            else if ( answer > 0 )
                first = mid + 1;
            else
                last  =  mid - 1;
        }
        if( notFound )
            code = ID;
        else
            code = (tokenType)mid;
    }
    else      // it is not a letter
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );

}

tokenType getComment(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if(  ch == '/'  &&  cin.peek( ) == '*')
    {
        tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
        
        // read the second symbol and store it into the buffer
        ch  =  cin.get(  );
        tokenBuffer[ i++ ]  = ch;
        
        ch  = cin.get(  );
        while( !( ch ==  '*'  &&  cin.peek( ) == '/' )  &&   cin.peek( ) != EOF)
        {
            tokenBuffer[i++] = ch;   // store the first symbol into the buffer
            
            // read the second symbol and store it into the buffer
            ch  =  cin.get(  );
        }
        if(cin.peek( ) == EOF)
            code  =  INVALID;
        else
        {
            tokenBuffer[i++]  = ch;  // store the first symbol into the buffer
            
            // read the second symbol and store it into the buffer
            ch  =  cin.get(  );
            tokenBuffer[ i++ ]  = ch;
            code  =  COMMENT;
        }
    }
    else            // it is not "/*"
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );

}

tokenType getReal(void)
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get(  );
    if (isdigit(ch))
    {
        tokenBuffer[ i++ ]  = ch;
        ch  =  cin.get(  );
        while( isdigit( ch ))
        {
            tokenBuffer[ i++ ]  = ch;
            ch  =  cin.get(  );
        }
        if(ch == '.' )
        {
            tokenBuffer[ i++ ]  = ch;
            ch  =  cin.get(  );
            if(isdigit(ch))
            {
                tokenBuffer[i++]  = ch;
                ch = cin.get();
                while( isdigit( ch ))
                {
                   tokenBuffer[ i++ ]  = ch;
                   ch  =  cin.get(  );
                }
                cin.putback( ch );
                code  =  REAL;
            }
            else
            {
                cin.putback( ch );
                code  =  INVALID;
            }
        }
        else      // it is not a period
        {
            cin.putback( ch );
            code  =  INVALID;
        }
    }
    else      // it is not a digit
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}

tokenType getString(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if(  ch == '\"' )
    {
        tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
        
        ch  = cin.get(  );
        while( !( ch ==  '\"' ) &&  ch != EOF)
        {
            tokenBuffer[ i++ ]  = ch;        // store the symbol into the buffer
            
            // read the second symbol and store it into the buffer
            ch  =  cin.get(  );
        }
        if(ch == EOF)
            code  =  INVALID;
        else
        {
            tokenBuffer[i++]  = ch;  // store the first symbol into the buffer
            code  =  STRING;
        }
    }
    else            // it is not "
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}

tokenType getAssign(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == ':' && cin.peek( ) == '=')
    {
        tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
        
        // read the second symbol and store it into the buffer
        ch = cin.get();
        tokenBuffer[i]  = ch;
        code = ASSIGNMENT;
    }
    else            // it is not an assignment
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}

tokenType getPlus(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '+')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = PLUS;
    }
    else            // it is not the operator +
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}

tokenType getMul(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '*')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = MULTIPLICATION;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}

tokenType getEqual(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '=')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = EQUAL;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}

tokenType getGreater(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '>')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = GREATERTHAN;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}

tokenType getLess(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '<')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = LESSTHAN;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}
               
tokenType getLP(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '(')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = LEFTP;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );

}
               
tokenType getRP(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == ')')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = RIGHTP;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}

tokenType getComma(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == ',')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = COMMA;
    }
    else
    {
        cin.putback( ch );
        code = INVALID;
    }
    return( code );
}
               
tokenType getSColon(void)
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == ';')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = SEMICOLON;
    }
    else
    {
        cin.putback(ch);
        code = INVALID;
    }
    return( code );
}

