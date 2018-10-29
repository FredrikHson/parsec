#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <clang-c/Index.h>
struct foo
{
    int f;
    int d;
};

struct foo eeeew = {0};
int globalshit = 0;

enum CXChildVisitResult printchild(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    /*if(clang_getCursorKind(cursor) == CXCursor_VarDecl)*/
    {
        CXSourceLocation loc = clang_getCursorLocation(cursor);
        CXString filename;
        clang_getPresumedLocation(loc, &filename, 0, 0);
        const char* f = clang_getCString(filename);
        const char* p = clang_getCString(clang_getCursorSpelling(parent));

        if(strcmp(f, "main.c") == 0 && strcmp(p, "main.c") == 0)
        {
            printf("Cursor %s of kind %s %s parent:%s\n",
                   clang_getCString(clang_getCursorSpelling(cursor)),
                   clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(cursor))),
                   clang_getCString(filename),
                   clang_getCString(clang_getCursorSpelling(parent))
                  );
        }
    }
    return CXChildVisit_Recurse;
}

int main()
{
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
                                 index,
                                 "main.c", 0, 0,
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
