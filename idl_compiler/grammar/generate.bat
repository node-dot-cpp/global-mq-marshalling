flex global_mq_IDL.l
bison -dvt global_mq_IDL.y

del global_mq_IDL.tab.cpp
del lex.yy.cpp

ren global_mq_IDL.tab.c global_mq_IDL.tab.cpp
ren lex.yy.c lex.yy.cpp

sed -i.old s/"#include <unistd.h>"/\/\/\0/g lex.yy.cpp
del lex.yy.cpp.old

if not exist "./generated" mkdir "./generated"
move global_mq_IDL.tab.cpp ./generated/global_mq_IDL.tab.cpp
move global_mq_IDL.tab.h ./generated/global_mq_IDL.tab.h
move lex.yy.cpp ./generated/lex.yy.cpp

pause