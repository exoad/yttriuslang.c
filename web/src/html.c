#include <regex.h>    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int num_matches(regex_t *regex, const char *str) {
  regmatch_t match[2];
  int matches = 0;
  
  while(1) {
    int rc = regexec(regex, str, 2, match, 0);

    if(rc == 0) {
      str += match[1].rm_eo;
      matches++;
      
    } else {
     return matches; 
    }
    
  }
}

int get_matches(char **out, int max_matches, regex_t *regex, const char *str) {
  regmatch_t match[2];
  int matches = 0;
  
  while(matches < max_matches) {
    int rc = regexec(regex, str, 2, match, 0);

    if(rc == 0) {
      const char *start = str + match[1].rm_so;
      int len = match[1].rm_eo - match[1].rm_so;
      
      char *buffer = (char*)malloc(len + 1);
      strncpy(buffer, start, len);
      buffer[len] = '\0';
      
      *out = buffer;
            
      str += match[1].rm_eo;
      matches++;
      out++;
      
    } else {
     *out = NULL;
     return matches; 
    }  
  }
  
  *out = NULL;
  return matches;
}

char** match_many(const char *str, const char *pattern) {
  

  regex_t regex;

  /* Compile regular expression */
  int rc = regcomp(&regex, pattern, REG_EXTENDED);
  if (rc) {
      fprintf(stderr, "Could not compile regex\n");
      exit(1);
  }

  int n = num_matches(&regex, str);
  
  int size = (n + 1) * sizeof(char*);
  char **matches = (char**)malloc(size);
  memset(matches, 0, size);
  
  get_matches(matches, n, &regex, str);
  regfree(&regex);
  
  return matches;
}

void print_strings(char **strings) {
    
  while(*strings) {
    printf("%s\n", *strings); 
    strings++;
  }
}



char** extract_links(const char *html) {

  const char *link_pat = "href[:space:]*=[:space:]*\"([^[:space:]\"]+)\"";
  return match_many(html, link_pat);
  
}


void free_strings(char **strings) {
  
  char **iter = strings;
  while(*iter) {
    free(*iter); 
    iter++;
  }
  
  free(strings);
}