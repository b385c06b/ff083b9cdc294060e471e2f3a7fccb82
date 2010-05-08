#include "hge_impl.h"

D3DXMATRIX * CALL HGE_Impl::Math_MatrixIdentity(D3DXMATRIX * pOut)
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixIdentity(pOut);
	return pOut;
}

D3DXMATRIX * CALL HGE_Impl::Math_MatrixTranslation( D3DXMATRIX *pOut, float x, float y, float z )
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixTranslation(pOut, x, y, z);
	return pOut;
}

D3DXMATRIX * CALL HGE_Impl::Math_MatrixRotationX( D3DXMATRIX *pOut, float angle )
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixRotationX(pOut, angle);
	return pOut;
}

D3DXMATRIX * CALL HGE_Impl::Math_MatrixRotationY( D3DXMATRIX *pOut, float angle )
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixRotationY(pOut, angle);
	return pOut;
}

D3DXMATRIX * CALL HGE_Impl::Math_MatrixRotationZ( D3DXMATRIX *pOut, float angle )
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixRotationZ(pOut, angle);
	return pOut;
}

D3DXMATRIX * CALL HGE_Impl::Math_MatrixScaling( D3DXMATRIX *pOut, float sx, float sy, float sz )
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixScaling(pOut, sx, sy, sz);
	return pOut;
}

D3DXMATRIX * CALL HGE_Impl::Math_MatrixMultiply( D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2 )
{
	if (!pOut || !pM1 || ! pM2)
	{
		return NULL;
	}
	D3DXMatrixMultiply(pOut, pM1, pM2);
	return pOut;
}

D3DXMATRIX*	CALL HGE_Impl::Math_MatrixOrthoOffCenterLH(D3DXMATRIX *pOut, float l, float r, float b, float t, float zn, float zf)
{
	if (!pOut)
	{
		return NULL;
	}
	D3DXMatrixOrthoOffCenterLH(pOut, l, r, b, t, zn, zf);
	return pOut;
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