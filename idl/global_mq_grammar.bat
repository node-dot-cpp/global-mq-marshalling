flex global_mq_IDL.l
bison -dvt global_mq_IDL.y

del global_mq_IDL.tab.cpp
del lex.yy.cpp

ren global_mq_IDL.tab.c global_mq_IDL.tab.cpp
ren lex.yy.c lex.yy.cpp

sed -i.old s/"#include <unistd.h>"/\/\/\0/g lex.yy.cpp
del lex.yy.cpp.old

pause