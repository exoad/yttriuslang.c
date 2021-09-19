#include <stdio.h>
#include <stdlib.h>

#include "http.h"

/*
./http_test www.thomas-bayer.com /sqlrest/CUSTOMER/3/
Header:
HTTP/1.1 200 OK
Server: Apache-Coyote/1.1
Content-Type: application/xml
Date: Tue, 02 Sep 2014 04:47:16 GMT
Connection: close
Content-Length: 235

Content:
<?xml version="1.0"?><CUSTOMER xmlns:xlink="http://www.w3.org/1999/xlink">
    <ID>3</ID>
    <FIRSTNAME>Michael</FIRSTNAME>
    <LASTNAME>Clancy</LASTNAME>
    <STREET>542 Upland Pl.</STREET>
    <CITY>San Francisco</CITY>
</CUSTOMER>
*/

int main(int argc, char **argv) {

  
  if(argc != 3){
    fprintf(stderr, "usage: ./http_test host page\n");
    exit(1);
  }  
  
  char *response = http_query(argv[1], argv[2], 80);
  if(response) {
    char *content = http_split_content(response);
    
    printf("Header:\n%s\n", response);
    printf("Content:\n%s\n", content);
  }
  
  free(response);
  return 0;
}
  