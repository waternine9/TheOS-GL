#include <cstdint>
#include <cstddef>
#include "vector.hpp"
#include "../memory.hpp"

_Vector* NewVector(size_t _ElemSize)
{
	_Vector* _Vec = (_Vector*)malloc(sizeof(_Vector));
	_Vec->ElemSize = _ElemSize;
	_Vec->Cap = 128;
	_Vec->Size = 0;
	_Vec->Data = malloc(_Vec->ElemSize * _Vec->Cap);
	return _Vec;
}

void _VectorVerify(_Vector* _Vec)
{
	if (_Vec->Size >= _Vec->Cap)
	{
		_Vec->Cap += 128;
		void* NewData = malloc(_Vec->ElemSize * _Vec->Cap);
		memcpy(NewData, _Vec->Data, _Vec->ElemSize * _Vec->Size);
		free(_Vec->Data);
		_Vec->Data = NewData;
	}
}

void VectorPushBack(_Vector* _Vec, void* _Data)
{
	memcpy((uint8_t*)_Vec->Data + _Vec->ElemSize * _Vec->Size, _Data, _Vec->ElemSize);
	_Vec->Size++;
	_VectorVerify(_Vec);
}

void VectorPopBack(_Vector* _Vec)
{
	_Vec->Size--;
	if (_Vec->Size < 0)
	{
		_Vec->Size = 0;
	}
}

void VectorRead(_Vector* _Vec, void* _Out, int _Index)
{
	memcpy(_Out, (uint8_t*)_Vec->Data + _Vec->ElemSize * _Index, _Vec->ElemSize);
}

void VectorWrite(_Vector* _Vec, void* _Data, int _Index)
{
	memcpy((uint8_t*)_Vec->Data + _Vec->ElemSize * _Index, _Data, _Vec->ElemSize);
}

void VectorFree(_Vector* _Vec)
{
	free(_Vec->Data);
	free(_Vec);
}

void VectorCopy(_Vector* _Dest, _Vector* _Src)
{
	free(_Dest->Data);
	_Dest->Cap = _Src->Cap;
	_Dest->Size = _Src->Size;
	_Dest->ElemSize = _Src->ElemSize;
	_Dest->Data = malloc(_Dest->Cap * _Dest->ElemSize);
	memcpy(_Dest->Data, _Src->Data, _Src->Size * _Src->ElemSize);
}