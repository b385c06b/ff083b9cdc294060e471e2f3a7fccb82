#include "hge_impl.h"

D3DMATRIX * CALL HGE_Impl::Math_MatrixMakeIdentity(D3DMATRIX * mat)
{
	if (!mat)
	{
		return NULL;
	}
	Math_MatrixMakeIdentity(mat);
	return mat;
}

char * CALL HGE_Impl::Math_itoa(int ival, char * buffer)
{
	if (!buffer)
	{
		return NULL;
	}
	sprintf(buffer, "%d", ival);
	return buffer;
}

int CALL HGE_Impl::Math_atoi(const char * buffer)
{
	return atoi(buffer);
}

char * CALL HGE_Impl::Math_ftoa(float fval, char * buffer)
{
	if (!buffer)
	{
		return NULL;
	}
	sprintf(buffer, "%f", fval);
	return buffer;
}

float CALL HGE_Impl::Math_atof(const char * buffer)
{
	return atof(buffer);
}