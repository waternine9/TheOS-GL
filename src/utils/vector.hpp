#ifndef H_TOS_VECTOR
#define H_TOS_VECTOR

#include <cstddef>

typedef struct
{
	void* Data;
	int Cap;
	int Size;
	size_t ElemSize;
} _Vector;

_Vector NewVector(size_t _ElemSize);

void _VectorVerify(_Vector* _Vec);

void VectorPushBack(_Vector* _Vec, void* _Data);

void VectorPopBack(_Vector* _Vec);

void VectorRead(_Vector* _Vec, void* _Out, int _Index);

void VectorWrite(_Vector* _Vec, void* _Data, int _Index);

void VectorFree(_Vector* _Vec);

void VectorCopy(_Vector* _Dest, _Vector* _Src);

#endif // H_TOS_VECTOR