#ifndef URL_H
#define URL_H

/* make_absolute: 
 * 
 * Tries to make an absolute URL from a base url + relative url
 * If the relative url is already absolute it is returned.
 * 
 * E.g. http://google.com/index.html + /foo/bar.html  = http://google.com/foo/bar.html
 * 
 * Returns -1 if either URL is invalid, or the concatenation fails.
 * Returns 0 on success.
 * 
 */

int make_absolute(char *buffer, int buffer_len, const char *base, const char *relative);

/* 
 * add_scheme:
 *
 * add http:// if no scheme is specified. The uriparser does not identify the host properly
 * without it.
 * 
 * e.g. google.com ->  http://google.com/index.html
 * 
 * Returns -1 if either URL is invalid. 
 */

void add_scheme(char *buffer, int buffer_len, const char *url);


/* get_path, get_host: 
 * 
 * Get components of a url.
 * e.g. google.com/foo/index.html  -> google.com   /foo/index.html
 * 
 */

int get_path(char *buffer, int buffer_len, const char *url);
int get_host(char *buffer, int buffer_len, const char *url);

/* Check to see if urls match except for the paths.
 * 
 * Returns -1 if either URL is invalid, 1 for equal, 0 not equal.
 */

int match_hosts(const char *url1, const char *url2);



#endif