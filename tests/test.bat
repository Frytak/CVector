:: Turn off logging
@ECHO OFF

ECHO Compiling tests...

gcc -o test ^
.\..\vec.c ^
.\example_data\example_data.c ^
.\vec_drop\vec_drop_test.c ^
.\vec_get\vec_get_test.c ^
.\vec_new\vec_new_test.c ^
.\vec_is_partial_eq\vec_is_partial_eq_test.c ^
.\vec_push\vec_push_test.c ^
.\vec_push_multi\vec_push_multi_test.c ^
.\vec_binary_search\vec_binary_search_test.c ^
.\vec_find_first\vec_find_first_test.c ^
.\vec_copy\vec_copy_test.c ^
.\test.c

ECHO Launching tests...
ECHO:

test.exe

:: Return the tests' success value
EXIT /B %ERRORLEVEL%
