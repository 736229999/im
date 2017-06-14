#include <windows.h>

// 65001 is utf-8.
wchar_t *CodePageToUnicode(int codePage, const char *src);
char *UnicodeToCodePage(int codePage, const wchar_t *src);
BOOL is_utf8(const char * string);

//const char *text = "Sôn bôn de magnà el véder, el me fa minga mal.";
//
//// Convert ANSI (Windows-1252, i.e. CP1252) to utf-8:
//wchar_t *wText = CodePageToUnicode(1252,text);
//
//char *utf8Text = UnicodeToCodePage(65001,wText);
//
//FILE *fp = fopen("utf8File.txt","w");
//fprintf(fp,"%s\n",utf8Text);
//fclose(fp);
//
//// Now convert utf-8 back to ANSI:
//wchar_t *wText2 = CodePageToUnicode(65001,utf8Text);
//
//char *ansiText = UnicodeToCodePage(1252,wText2);
//
//fp = fopen("ansiFile.txt","w");
//fprintf(fp,"%s\n",ansiText);
//fclose(fp);
//
//delete [] ansiText;
//delete [] wText2;
//delete [] wText;
//delete [] utf8Text;