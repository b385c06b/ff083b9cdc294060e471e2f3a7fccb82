#ifndef HGE_DEFINES_H
#define HGE_DEFINES_H

#define __WIN32

#ifndef __WIN32

//typedef unsigned __int64	QWORD;

#ifndef HINSTANCE
#define unsigned long		HINSTANCE
#endif

#ifndef HWND
#define unsigned long		HWND
#endif

// Flexible vertex format bits
//
#define D3DFVF_RESERVED0        0x001
#define D3DFVF_POSITION_MASK    0x400E
#define D3DFVF_XYZ              0x002
#define D3DFVF_XYZRHW           0x004
#define D3DFVF_XYZB1            0x006
#define D3DFVF_XYZB2            0x008
#define D3DFVF_XYZB3            0x00a
#define D3DFVF_XYZB4            0x00c
#define D3DFVF_XYZB5            0x00e
#define D3DFVF_XYZW             0x4002

#define D3DFVF_NORMAL           0x010
#define D3DFVF_PSIZE            0x020
#define D3DFVF_DIFFUSE          0x040
#define D3DFVF_SPECULAR         0x080

#define D3DFVF_TEXCOUNT_MASK    0xf00
#define D3DFVF_TEXCOUNT_SHIFT   8
#define D3DFVF_TEX0             0x000
#define D3DFVF_TEX1             0x100
#define D3DFVF_TEX2             0x200
#define D3DFVF_TEX3             0x300
#define D3DFVF_TEX4             0x400
#define D3DFVF_TEX5             0x500
#define D3DFVF_TEX6             0x600
#define D3DFVF_TEX7             0x700
#define D3DFVF_TEX8             0x800

#define D3DFVF_LASTBETA_UBYTE4   0x1000
#define D3DFVF_LASTBETA_D3DCOLOR 0x8000

#define D3DFVF_RESERVED2         0x6000  // 2 reserved bits

typedef enum _D3DTRANSFORMSTATETYPE {
	D3DTS_VIEW          = 2,
	D3DTS_PROJECTION    = 3,
	D3DTS_TEXTURE0      = 16,
	D3DTS_TEXTURE1      = 17,
	D3DTS_TEXTURE2      = 18,
	D3DTS_TEXTURE3      = 19,
	D3DTS_TEXTURE4      = 20,
	D3DTS_TEXTURE5      = 21,
	D3DTS_TEXTURE6      = 22,
	D3DTS_TEXTURE7      = 23,
	D3DTS_FORCE_DWORD     = 0x7fffffff, /* force 32-bit size enum */
} D3DTRANSFORMSTATETYPE;

#ifndef D3DMATRIX_DEFINED
typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;
#define D3DMATRIX_DEFINED
#endif

#endif

#endif