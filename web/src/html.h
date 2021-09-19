#ifndef HTML_H
#define HTML_H


/*
 * print_strings:
 * 
 * For debugging, print out a null terminated array of strings
 * 
 */


void print_strings(char **strings);


/*
 * free_strings:
 * 
 * Free memory (and strings) of a null terminated array of strings.
 * 
 */

void free_strings(char **strings);


/*
 * extract_links:
 * 
 * Extract any links found in an html string supplied, returns a null terminated
 * array of strings pointers.
 * 
 */

char** extract_links(const char *html);



#endif

