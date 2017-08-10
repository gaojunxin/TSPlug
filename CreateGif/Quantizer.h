#if !defined(QUANTIZER_H_)
#define QUANTIZER_H_

struct NODE
{
	BOOL bIsLeaf;
	long nPixelCount;
	long nRedSum;
	long nGreenSum;
	long nBlueSum;
	UINT nColorIndex;
	NODE *pChild[8];
	NODE *pNext;
};

class CQuantizer
{
public:
	CQuantizer(UINT nMaxColors, UINT nColorBits);
	~CQuantizer();
	
	BOOL ProcessImage (HANDLE hImage);
	
	UINT GetColorCount ();

	void SetColorTable (RGBQUAD* prgb);

	BOOL GetColorIndex(BYTE r,BYTE g,BYTE b,UINT *pColorIndex);

protected:
	
	void AddColor (NODE** ppNode, BYTE r, BYTE g, BYTE b,
		UINT nColorBits, UINT nLevel, UINT*	pLeafCount,	NODE** pReducibleNodes);
	
	void* CreateNode (UINT nLevel, UINT	nColorBits,	UINT* pLeafCount,
			NODE** pReducibleNodes);
	
	void ReduceTree	(UINT nColorBits, UINT*	pLeafCount,
			NODE** pReducibleNodes);

	void DeleteTree	(NODE**	ppNode);

	void GetPaletteColors (NODE* pTree,	RGBQUAD* prgb, UINT* pIndex);

	BOOL FindColorIndex(NODE *pNode,BYTE r,BYTE g,BYTE b,int nLevel,UINT *pColorIndex);

	BYTE GetPixelIndex(long x, long y, int nbit, long effwdt, BYTE *pimage);

	int m_nColorBits;
	UINT m_nLeafCount;
	int m_nMaxColors;
	NODE *m_pTree;
	NODE *m_pReducibleNodes[9];
};

class CRGBQuantizer: public CQuantizer
{
public:
	CRGBQuantizer(UINT nMaxColors, UINT nColorBits);
	BOOL ProcessImageRGB (BYTE *pRGBData,UINT nWidth,UINT nHeight);
};

class CBitmapQuantizer: public CQuantizer
{
public:
	CBitmapQuantizer(UINT nMaxColors, UINT nColorBits);
	BOOL ProcessImageBitmap (HBITMAP hBmp);
};


#endif