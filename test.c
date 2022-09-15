 
#include <stdio.h>

/**
 * PROVES that in 2022, %ms (the m modifer) does NOT work in C on macos
 * Ugh.
 */
//#define NULL ((void *) 0)

int main(int argc, char **argv)
{
    char *superclassName[512];
    char *instClassName[512];
    char *metaclassName[512];
    char *instVars[512];
    char *classVars[512];

  /* parse lines like:
     * +SuperClass subclass: #NewClass variables: #( instVars ) classVariables: #( classVars )
     * +nil subclass: #Object variables: #( ) classVariables: #( )
     */
    char* inputBuffer = "+nil subclass: #Object variables: #( ) classVariables: #( )";

    int foo = sscanf(inputBuffer, "+%[a-zA-Z] subclass: #%[a-zA-Z] variables: #(%[ a-zA-Z]) classVariables: #(%[ a-zA-Z])",
                            &superclassName,      &instClassName,         &instVars,                     &classVars);
    printf("%d %s %s %s %s\n", foo, superclassName, instClassName, instVars, classVars );
    
}
