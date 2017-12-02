
 
#include "getword.h"
 
/* Whitespace characters */
#define TAB '\t'      /* Tab character */
#define SPACE ' '     /* Space character */
#define NEW_LINE '\n' /* New line character */
 
/* Metacharacters */
#define F_SLASH 92    /* Forward Slash Character */
#define L_THAN '<'    /* Less Than Character */
#define G_THAN '>'    /* Greater Than Character */
#define PIPE '|'      /* Pipe Character */
#define SEMI_COL 59   /* Semi Colon */
#define QUOTE 39      /* Single Quote */
#define ANPER '&'     /* Ampersand */
 
 
/*
 * FUNCTION: insertNull(char *w)
 *
 * INPUTS: char *w
 * Pointer to an array of characters
 *
 * OUTPUTS: none
 *
 * DESCRIPTION:
 * This function inserts a null character
 * into the character array where *w is pointing
 */
void insertNull(char *w)
{
    *w = (int)'\0';
}
 
/* FUNCTION: isMeta(char letter)
 *
 * INPUTS: char letter
 * Character from stdin
 *
 * OUTPUTS: 1 or 0
 * 0 indicates false
 * 1 indicataes true
 *
 * DESCRIPTION:
 * This function is passed a character being read
 * in from the file, if this chracter is a metacharacter
 * NOT handled by the switch statement in getword() the
 * function returns a 1 for true. Otherwise it returns 0
 */
int isMeta(char letter)
{
    if ( letter == L_THAN ||
         letter == PIPE   ||
         letter == ANPER ) {
        return 1;
    }
    return 0;
}
 
/*
 * FUNCTION: quoteEnder(char letter)
 *
 * INPUTS: char letter
 * A character from stdin
 *
 * OUTPUTS: 0 or 1
 * 0 indicates false
 * 1 indicates true
 *
 * DESCRIPTION:
 * This function is passed a character taken from stdin
 * and determines if that letter is a QUOTE, NEW_LINE,
 * or EOF. If the function returns a 1 the character passed in
 * will signify the end of the special QUOTE case for parsing  
 */
int quoteEnder(char letter)
{
    return ( letter == QUOTE ||
             letter == NEW_LINE ||
             letter == EOF );
}
 
/*
 * FUNCTION: getword(char *w)
 *
 * INPUTS: char *w
 * Pointer to a character array
 *
 * OUTPUTS: int
 * The number of characters (count) in the word captured
 * or -1 for EOF or 0 for the empty string
 *
 * DESCRIPTION:
 * This function reads one character at a time from stdin. It fills
 * an array of characters one character at a time untill the end of
 * a word has been detected by a metacharacter, NEW_LINE, or EOF.
 *
 * Note:
 * Metacharacters are "<", ">", "'", ";","\", ";",">!"
 *
 * ";" acts as a new line character
 * "\" acts as an escape key
 * "'" captures all characters until the next "'" or NEW_LINE or EOF
 *
 * Note:
 * There are many calls to the function ungetc(char, stdin)
 * This library function places the specified char back on stdin to be read
 * on the next call to getword() or the next iteration of a loop.
 */
 
int getword(char *w)
{
    int iochar;
    int nextChar;
    int count = 0;
    /* Read one letter from stdin until the end of the file */
    while( (iochar = getchar()) != EOF)
    {
        /* If character array is full */
        if ( count == STORAGE - 1 )
        {
            insertNull(w);
            ungetc(iochar, stdin);
            return count;
        }
 
        /* Handling of metacharacters and exceptions */
        switch (iochar) {
 
            case TAB:
            case SPACE:
                /* skip leading tabs or spaces */
                if(count == 0)
                    continue;
                /* trailing tab or space signifies end of word */
                insertNull(w);
                return count;
                break;
 
                /*
                 * If the character is '\' move to the next character and
                 * insert it into the array
                 */
            case F_SLASH:
                nextChar = getchar();
                if( nextChar == NEW_LINE )
                {
                    /*
                     * Place NEW_LINE back on stdin to be
                     * handled on next iteration of the loop
                     */
                    ungetc(nextChar, stdin);
                    continue;
                }
 
                /* Add the character after '\' to the array */
                *w++ = nextChar;
                count++;
                break;
 
            case QUOTE:
                nextChar = getchar();
                /*
                 * Continue pulling characters from
                 * stdin until a QUOTE, NEW_LINE, or EOF
                 * is found
                 */
                while(!quoteEnder(nextChar))
                {
                    if(nextChar == F_SLASH)
                    {
                        if ( (iochar = getchar()) == QUOTE)
                        {
                            /*
                             * Previous two characters recieved are "\'"
                             * only place the "'" into the array
                             */
                            *w++ = iochar;
                            count++;
                            nextChar = getchar();
                            continue;
                        }
                        /* First character was "\" second character was NOT "'"
                         * place the second character back on stdin and continue
                         * looking for quoteEnder
                         */
                        ungetc(iochar, stdin);
                    }
                    /* Continue adding to character array until stopping condition */
                    *w++ = nextChar;
                    count++;
                    nextChar = getchar();
                }
                /* If the last char recieved was a QUOTE this does NOT signify
                 * the end of a word, continue pulling characters until EOF
                 */
                if(nextChar == QUOTE)
                    continue;
                else
                    return -2; /* Returning a unique -2 value to signal quote ended illegally */
 
                /* If the last char revieved was a NEW_LINE place back on stdin
                 * to be handled on next iteration of the loop
                 */
                if(nextChar == NEW_LINE)
                    ungetc(nextChar, stdin);
                insertNull(w);
                return count;
 
            case SEMI_COL:
            case NEW_LINE:
 
                insertNull(w);
                /* If this is the first char recieved return 0 and the empty string */
                if( count == 0 )
                    return 0;
                /* Previous characters have been captured, this is the end of a word */
                if ( iochar == NEW_LINE )
                    ungetc(NEW_LINE, stdin);
                else
                    ungetc(SEMI_COL, stdin);
 
                return count;
 
            case G_THAN:
                /* Check if next character is a '!' */
                if( (nextChar = getchar()) == '!')
                {
                    /*
                     * If previous characters have been recieved
                     * this is the end of the previous word
                     */
                    if( count != 0 )
                    {
                        insertNull(w);
                        /*
                         * Place the "!" back on stdin first
                         * then the ">" to be handled by the
                         * next call of getword()
                         */
                        ungetc(nextChar, stdin);
                        ungetc(iochar, stdin);
                        return count;
                    }
 
                    /*
                     * The first two characters recieved from stdin
                     * were ">!" this is considered a metacharacter.
                     * Place them into the array in proper order and
                     * return 1 since this is one word
                     */
                    *w++ = iochar;
                    *w++ = nextChar;
                    insertNull(w);
                    return -5;
                }
                ungetc(nextChar, stdin);
                /* A ">" was recieved and the next character is not "!" */
                if( count != 0 )
                {
                    insertNull(w);
                    ungetc(iochar, stdin);
                    return count;
                }
 
                /* ">" is the first character from stdin which
                 * means it is a word on its own */
                *w++ = iochar;
                insertNull(w);
                return 1;
 
 
            default:
                /*
                 * if the character is a metacharacter
                 * that has not been handled by above cases,
                 * it is either the end of the previous word or
                 * the metacharacter is a word by itself
                 */
                if( isMeta(iochar) )
                {
                    /*
                     * If previous characters have been recieved
                     * this metacharacter signifies the end of the
                     * word. Insert a null character and place
                     * the metacharacter back on stdin to be handled
                     * by the next call of getword()
                     */
                    if( count != 0 )
                    {
                        insertNull(w);
                        ungetc(iochar, stdin);
                        return count;
                    }
 
                    /* Metacharacter is a word  */
                    *w++ = iochar;
                    insertNull(w);
                    return 1;
                }
                /*Chacter is non metacharacter and gets added to the array */
                *w++ = iochar;
                count++;
                break;
        } /* switch(iochar)*/
    } /* while iochar != EOF */
    /* End of file detected, return length of the word or -1 */
    insertNull(w);
    return count != 0 ? count : -1;
}
