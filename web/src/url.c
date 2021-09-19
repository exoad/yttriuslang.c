#include "uriparser/Uri.h"

#include "url.h"

/* make_absolute: 
 * 
 * Tries to make an absolute URL from a base url + relative url
 * If the relative url is already absolute it is returned.
 * 
 * E.g. http://google.com/index.html + /foo/bar.html  = http://google.com/foo/bar.html
 * 
 * Returns -1 if either URL is invalid. 
 */


int make_absolute(char *buffer, int buffer_len, const char *base, const char *relative)
{
    UriParserStateA state;
    UriUriA base_uri;
    UriUriA rel_uri;

    state.uri = &base_uri;

    if (uriParseUriA(&state, base) != URI_SUCCESS) {
        return -1;
    }
    
    state.uri = &rel_uri;
    if (uriParseUriA(&state, relative) != URI_SUCCESS) {
        uriFreeUriMembersA(&base_uri);
        return -1;
    }
    

    UriUriA result;
    if (uriAddBaseUriA(&result, &rel_uri, &base_uri) != URI_SUCCESS)
    {
        uriFreeUriMembersA(&result);
        return -1;
    }
    uriFreeUriMembersA(&base_uri);
    uriFreeUriMembersA(&rel_uri);

    uriNormalizeSyntaxA(&result);
    
    uriToStringA(buffer, &result, buffer_len, NULL);    
    uriFreeUriMembersA(&result);
    return 0;
}


void rangeString(char *buffer, int buffer_len, UriTextRangeA *range) {
  int len = range->afterLast - range->first;
  len = buffer_len < len ? buffer_len : len;
  
  strncpy(buffer, range->first, len);
  buffer[len] = '\0'; 
}


int rangeLen(UriTextRangeA *range) {
    if(range) {
       int len = range->afterLast - range->first;
       return len;
    }
    
    return 0;
}

int hasScheme(const char *url) {
  UriParserStateA state;
  UriUriA uri;

  state.uri = &uri;

  if (uriParseUriA(&state, url) != URI_SUCCESS) {
      return 0;
  }

  int b = rangeLen(&uri.scheme) > 0;
  uriFreeUriMembersA(&uri);
  
  return b;
}



void add_scheme(char *buffer, int buffer_len, const char *url_)  {

  char path[128];
  
  if(!hasScheme(url_))  {
    
    get_path(path, 128, url_);
    snprintf(buffer, buffer_len, "http://%s", path);
    
  } else {
    strncpy(buffer, url_, buffer_len);
  }
}




int get_path(char *buffer, int buffer_len, const char *url) { 
    UriParserStateA state;
    UriUriA uri;

    state.uri = &uri;

    if (uriParseUriA(&state, url) != URI_SUCCESS) {
        return -1;
    }
    
    uri.scheme.first = NULL;
    uri.hostText.first = NULL ;
  
    int rc = uriToStringA(buffer, &uri, buffer_len, NULL);    
    uriFreeUriMembersA(&uri);
    return rc;
}



int get_host(char *buffer, int buffer_len, const char *url) { 
    UriParserStateA state;
    UriUriA uri;

    state.uri = &uri;

    if (uriParseUriA(&state, url) != URI_SUCCESS)
    {
        return -1;
    }

    // if there's no scheme then uriParser thinks the host is part of the path
    if(rangeLen(&uri.scheme)) {
      rangeString(buffer, buffer_len, &uri.hostText);
    } else {
      rangeString(buffer, buffer_len, &uri.pathHead->text);
    }

    uriFreeUriMembersA(&uri);
    return 0;
}


int match_hosts(const char *url1, const char *url2) {
    UriParserStateA state;
    UriUriA uri1;
    UriUriA uri2;

    state.uri = &uri1;

    if (uriParseUriA(&state, url1) != URI_SUCCESS) {
      uriFreeUriMembersA(&uri1);
      return -1;
    }
    
    state.uri = &uri2;
    if (uriParseUriA(&state, url2) != URI_SUCCESS) {
        uriFreeUriMembersA(&uri1);
        uriFreeUriMembersA(&uri2);        
        return -1;
    }
    
    UriPathSegmentA* h1 = uri1.pathHead;
    UriPathSegmentA* h2 = uri2.pathHead;
    
    uri1.pathHead = NULL;
    uri2.pathHead = NULL;
    
    int result = uriEqualsUriA(&uri1, &uri2);

    uri1.pathHead = h1;
    uri2.pathHead = h2;

    
    uriFreeUriMembersA(&uri1);
    uriFreeUriMembersA(&uri2);

    return result;
}




// int main(int argv, char **argc) {
//  
//   char abs[128], host[128], path[128];
//   
//   make_absolute(abs, 128, "http://google.com/foo/index", "test/bar.html?asd=4");
//   getPath(path, 128, abs);
//   getHost(host, 128, abs);
//   
//   printf("%s %s %s\n", abs, host, path);  
//   
// }
