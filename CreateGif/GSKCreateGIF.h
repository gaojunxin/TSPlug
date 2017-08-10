#if !defined(GSK_CREATEGIF_H_)
#define GSK_CREATEGIF_H_

BOOL GetData(HBITMAP hBmp,BYTE **ppPalette,BYTE **ppData,BYTE *pBitsPixel,int *pWidth,int *pHeight);

void CreateGIFHeard(HANDLE file,WORD nImageWidth,WORD nImageHeight,BYTE bitsPixel);

void AddImageToGIF(HANDLE file,BYTE *pData,BYTE *palette,WORD nImageWidth,WORD nImageHeight,BYTE bitsPixel,WORD nDelay,
				   short int nTransparentColorIndex);

void CloseGIF(HANDLE file);

#endif