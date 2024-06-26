:: Turn off logging
@ECHO OFF

ECHO Compiling tests...

gcc -o test ^
.\..\vec.c ^
.\example_data\example_data.c ^
.\vec_drop\vec_drop_test.c ^
.\vec_reserve\vec_reserve_test.c ^
.\vec_get\vec_get_test.c ^
.\vec_new\vec_new_test.c ^
.\vec_is_partial_eq\vec_is_partial_eq_test.c ^
.\vec_push\vec_push_test.c ^
.\vec_push_multi\vec_push_multi_test.c ^
.\vec_insert\vec_insert_test.c ^
.\vec_insertion_sort\vec_insertion_sort_test.c ^
.\vec_remove\vec_remove_test.c ^
.\vec_remove_range\vec_remove_range_test.c ^
.\vec_remove_normalized_ranges\vec_remove_normalized_ranges_test.c ^
.\vec_binary_search\vec_binary_search_test.c ^
.\vec_find_first\vec_find_first_test.c ^
.\vec_copy\vec_copy_test.c ^
.\vec_fprint\vec_fprint_test.c ^
.\test.c

ECHO Launching tests...
ECHO:

test.exe

:: Return the tests' success value
EXIT /B %ERRORLEVEL%
