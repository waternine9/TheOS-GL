#ifndef H_TOS_STRING
#define H_TOS_STRING

typedef struct
{
	char* Data;
	int Cap;
	int Size;
} _String;

_String* NewString();

char StringGet(_String* _Str, int i);

void StringPush(_String* _Str, char c);

void StringPop(_String* _Str);

void StringAppend(_String* _Str, _String* _ToAppend);

char* String2CString(_String* _Str);

_String* CString2String(const char* _Str);

uint8_t StringEquals(_String* _A, const char* _B);

void StringCopy(_String* _Dst, _String* _Src);

#endif // H_TOS_STRING