#include <cstdint>
#include <cstddef>
#include "string.hpp"
#include "../memory.hpp"

_String* NewString()
{
	_String* String = (_String*)malloc(sizeof(_String));
	String->Cap = 128;
	String->Size = 0;
	String->Data = (char*)malloc(String->Cap);
	memset(String->Data, 0, String->Cap);
	return String;
}

char StringGet(_String* _Str, int i)
{
	return _Str->Data[i];
}

void StringPush(_String* _Str, char c)
{
	_Str->Data[_Str->Size++] = c;
	if (_Str->Size >= _Str->Cap - 2)
	{
		_Str->Cap += 128;
		char* NewData = (char*)malloc(_Str->Cap);
		memset(NewData, 0, _Str->Cap);
		memcpy(NewData, _Str->Data, _Str->Size);
		free(_Str->Data);
		_Str->Data = NewData;
	}
}

void StringPop(_String* _Str)
{
	if (_Str->Size == 0) return;
	_Str->Size--;
}

void StringAppend(_String* _Str, _String* _ToAppend)
{
	for (int i = 0; i < _ToAppend->Size; i++)
	{
		StringPush(_Str, _ToAppend->Data[i]);
	}
}

char* String2CString(_String* _Str)
{
	return _Str->Data;
}

_String* CString2String(const char* _Str)
{
	_String* String = NewString();
	while (*_Str)
	{
		StringPush(String, *_Str);
		_Str++;
	}
	return String;
}

uint8_t StringEquals(_String* _A, const char* _B)
{
	if (!_A) return 0;
	for (int i = 0; i < _A->Size; i++)
	{
		if (!_B[i]) return 0;
		if (_A->Data[i] != _B[i]) return 0;
	}
	if (_B[_A->Size]) return 0;
	return 1;
}

void StringCopy(_String* _Dst, _String* _Src)
{
	free(_Dst->Data);
	_Dst->Cap = _Src->Cap;
	_Dst->Size = _Src->Size;
	_Dst->Data = (char*)malloc(_Dst->Cap);
	memcpy(_Dst->Data, _Src->Data, _Src->Cap);
}