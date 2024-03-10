#ifndef _FRYTAK_VEC
#include "../../vec.h"
#endif

#include <stdint.h>
const int INSERT_DATA1[] = {12, 64, 0, -23};
const int INSERT_DATA1_LEN = 4;
const int INSERT_DATA1_SIZE = sizeof(int);

const int DATA1[] = {5, 3, 98, 12, 52, 0, -12}; 
const int DATA1_LEN = 7;
const int DATA1_CAP = 8;
const int DATA1_SIZE = sizeof(int);

const int DATA2[] = {5, 3, 98, 12, 52, 1, -12, 0, 129}; 
const int DATA2_LEN = 9;
const int DATA2_CAP = 16;
const int DATA2_SIZE = sizeof(int);

const int DATA3[] = {}; 
const int DATA3_LEN = 0;
const int DATA3_CAP = 0;
const int DATA3_SIZE = sizeof(int);

const int DATA4[] = {-19, -12, -9, -2, -1, 0, 2, 4, 7, 7, 19, 20, 87}; 
const int DATA4_LEN = 13;
const int DATA4_CAP = 16;
const int DATA4_SIZE = sizeof(int);



const char INSERT_DATA2[][6] = {"TestA", "TestB", "TestC", "TestD"};
const int INSERT_DATA2_LEN = 4;
const int INSERT_DATA2_SIZE = sizeof(char[6]);

const char DATA5[][6] = {"Test1", "Test2", "Test3", "Test4", "Test5", "Test6", "Test7"}; 
const int DATA5_LEN = 7;
const int DATA5_CAP = 8;
const int DATA5_SIZE = sizeof(char[6]);

const char DATA6[][6] = {"Test1", "Test2", "Test3", "Test4", "Test5", "Test6", "Test7", "Test8", "Test9"}; 
const int DATA6_LEN = 9;
const int DATA6_CAP = 16;
const int DATA6_SIZE = sizeof(char[6]);

const char DATA7[][6] = {}; 
const int DATA7_LEN = 0;
const int DATA7_CAP = 0;
const int DATA7_SIZE = sizeof(char[6]);

const uint8_t DATA_U8[] = {255, 43, 0, 78, 12, 99};
const int DATA_U8_LEN = 6;
const int DATA_U8_CAP = 8;
const int DATA_U8_SIZE = sizeof(uint8_t);

const int8_t DATA_I8[] = {-128, 8, 34, 78, -78, 127};
const int DATA_I8_LEN = 6;
const int DATA_I8_CAP = 8;
const int DATA_I8_SIZE = sizeof(int8_t);

const uint16_t DATA_U16[] = {2555, 5394, 65535, 0};
const int DATA_U16_LEN = 4;
const int DATA_U16_CAP = 4;
const int DATA_U16_SIZE = sizeof(uint16_t);

const int16_t DATA_I16[] = {-5837, 0, 32767, -32768, -78, 99, 8957, 7283, -423};
const int DATA_I16_LEN = 9;
const int DATA_I16_CAP = 16;
const int DATA_I16_SIZE = sizeof(int16_t);

const uint32_t DATA_U32[] = {255894, 589454, 0, 78, 4294967295, 8888, 32, 999};
const int DATA_U32_LEN = 8;
const int DATA_U32_CAP = 8;
const int DATA_U32_SIZE = sizeof(uint32_t);

const int32_t DATA_I32[] = {-12833, 2585945, 3, 0, -789843, 324, 2147483647, -2147483648, 757};
const int DATA_I32_LEN = 9;
const int DATA_I32_CAP = 16;
const int DATA_I32_SIZE = sizeof(int32_t);

const uint64_t DATA_U64[] = {25885894, 5889454, 98888, 788943, 4294967295, 0, 398432, 999, 89843, 7777, 18446744073709551615};
const int DATA_U64_LEN = 11;
const int DATA_U64_CAP = 16;
const int DATA_U64_SIZE = sizeof(uint64_t);

const int64_t DATA_I64[] = {-9223372036854775808, 2585945, 9223372036854775807, 0, 2147483647, 8};
const int DATA_I64_LEN = 6;
const int DATA_I64_CAP = 8;
const int DATA_I64_SIZE = sizeof(int64_t);

const float DATA_FLOAT[] = {123.9, 88.0001, 93.5, -12.038, -12, 0, 23.999, 73.8};
const int DATA_FLOAT_LEN = 8;
const int DATA_FLOAT_CAP = 8;
const int DATA_FLOAT_SIZE = sizeof(float);

const double DATA_DOUBLE[] = {1.9999998, -88.00010001, 93.56, 12.08, -12, 0, 98323.999, 73.8};
const int DATA_DOUBLE_LEN = 8;
const int DATA_DOUBLE_CAP = 8;
const int DATA_DOUBLE_SIZE = sizeof(double);

const char DATA_CHAR[] = {'G', 't', 'f', '!', '\255', '"', 'i', '\n', '\0', 'Y', '\201', '\130', '\240', ' ', ':', 'D'};
const int DATA_CHAR_LEN = 16;
const int DATA_CHAR_CAP = 16;
const int DATA_CHAR_SIZE = sizeof(char);

const char STRING1[] = "Hello world!";
const char STRING2[] = "123, 123, 1234, is this working?";
const char STRING3[] = ":DDD \n \t Yoooo \n\201";
const char STRING4[] = "01000111 01001010";
const char STRING5[] = "Welcome from Rust!";
const char *DATA_STRING[] = {STRING1, STRING2, STRING3, STRING4, STRING5};
const int DATA_STRING_LEN = 5;
const int DATA_STRING_CAP = 8;
const int DATA_STRING_SIZE = sizeof(char*);

const char DATA_STRING_STATIC[][6] = {"Hello\0", "true\0", "false\0", "uUuU\0", "\n\n\255\240A\0"};
const int DATA_STRING_STATIC_LEN = 5;
const int DATA_STRING_STATIC_CAP = 8;
const int DATA_STRING_STATIC_SIZE = sizeof(char[6]);

const void *DATA_POINTER[] = {NULL, (void*)0x00007FF6A72CF500, (void*)0x00007FF6A72CF508, NULL, (void*)0x00007FF6A72CF510, (void*)0x00007FF6A72CF518};
const int DATA_POINTER_LEN = 6;
const int DATA_POINTER_CAP = 8;
const int DATA_POINTER_SIZE = sizeof(void*);

const Vector DATA_VECTOR_DUMMY[] = {
    { 5, 8, sizeof(char), (void*)0x00007FF6A72CF500},
    { 98, 128, sizeof(uint64_t), (void*)0x00007FF6A72CF508},
    { 12, 16, sizeof(char*), (void*)0x00007FF6A72CF510}
};
const int DATA_VECTOR_DUMMY_LEN = 3;
const int DATA_VECTOR_DUMMY_CAP = 4;
const int DATA_VECTOR_DUMMY_SIZE = sizeof(Vector);
