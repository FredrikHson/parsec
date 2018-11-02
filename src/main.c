#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clang-c/Index.h>
#include <unistd.h>

char srcfile[2048];
// get globals
enum CXChildVisitResult printchild(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    if(clang_getCursorKind(cursor) == CXCursor_VarDecl)
    {
        CXSourceLocation loc = clang_getCursorLocation(cursor);
        CXString filename;
        unsigned int line;
        unsigned int col;
        clang_getPresumedLocation(loc, &filename, &line, &col);
        const char* f = clang_getCString(filename);
        const char* p = clang_getCString(clang_getCursorSpelling(parent));
        size_t len = 0;

        if(strcmp(f, srcfile) == 0 && strcmp(p, srcfile) == 0)
        {
            if(clang_getNumElements(clang_getCursorType(cursor)) != -1)
            {
                len = clang_getNumElements(clang_getCursorType(cursor));
            }

            const char* typename = clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));
            size_t nlen = strlen(typename) + 1;
            char* modname = malloc(nlen);
            memcpy(modname, typename, nlen);

            for(int i = 1; i < nlen - 1; i++)
            {
                if(modname[i] == '[')
                {
                    modname[i] = 0;

                    if(modname[i - 1] == ' ')
                    {
                        modname[i - 1] = 0;
                    }
                }
            }

            printf("{\"type\":\"%s\",", modname);

            if(len != 0)
            {
                printf("\"array\": \"true\",");
                printf("\"length\":\"%zu\",", len);
            }
            else
            {
                printf("\"array\": \"false\",");
            }

            printf("\"name\":\"%s\",", clang_getCString(clang_getCursorSpelling(cursor)));
            printf("\"src\":\"%s\",\"line\":\"%i\",\"col\":\"%i\"}\n",
                   clang_getCString(filename), line, col
                  );
            free(modname);
        }
    }

    return CXChildVisit_Recurse;
}

int main(int argc, char* argv[])
{
    int c;

    while((c = getopt(argc, argv, "f:h")) != -1)
    {
        switch(c)
        {
            case 'f':
            {
                strcpy(srcfile, optarg);
                break;
            }

            case 'h':
            {
                printf("-f   \n");
                exit(1);
                break;
            }

            default:
                exit(1);
                break;
        }
    }

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
                                 index,
                                 srcfile, 0, 0,
                                 0, 0,
                                 CXTranslationUnit_None);

    if(unit == 0)
    {
        fprintf(stderr, "Unable to parse translation unit. Quitting.");
        exit(-1);
    }

    int level = 0;
    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, printchild, 0);
    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}
