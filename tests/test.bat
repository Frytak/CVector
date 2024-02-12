:: Turn off logging
@ECHO OFF

ECHO Compiling tests...

gcc -o test ^
.\..\vec.c ^
.\example_data\example_data.c ^
.\vec_drop\vec_drop_test.c ^
.\vec_get\vec_get_test.c ^
.\vec_new\vec_new_test.c ^
.\vec_push\vec_push_test.c ^
.\vec_push_multi\vec_push_multi_test.c ^
.\test.c

ECHO Launching tests...
ECHO:

test.exe
