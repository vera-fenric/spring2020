#include <stdio.h> 
#include <stdlib.h>

int main () 
{
    printf("Content-type: text/html\n\n");
    printf ( "Arguments: %s", getenv ( "QUERY_STRING" )); 
    return 0; 
}
