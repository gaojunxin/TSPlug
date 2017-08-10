//GIF编码
//编写者：高书克
//日  期：2004-12-6

#include <StdAfx.h>
#include <MATH.H>
#include "quantizer.h"
#include "GSKCreateGIF.h"

//13 byte
typedef struct _GIFHEADER
{
	BYTE Signature[6];
	WORD ScreenWidth;
	WORD ScreenHeight;
	BYTE GlobalFlagByte;
	BYTE BackGroundColor;
	BYTE AspectRatio;	
}GIFHEARD;

#define HASPalette 0x80

//10 byte
typedef struct _GIFDATAHEARD
{
	BYTE imageLabel;
	WORD imageLeft;
	WORD imageTop;
	WORD imageWidth;
	WORD imageHeight;
	BYTE localFlagByte;
}GIFDATAHEARD;


void addToBuf(HANDLE file,BYTE *buf,long &nUsedBits,BYTE data,int nBit)
{
	int nIndex,nCurByteUsedBits;
	
	nIndex = nUsedBits/8;
	nCurByteUsedBits = nUsedBits%8;

	if(nIndex >= 255)
	{//已经产生255个字节的数据
		BYTE bytes = 0xFF;
		DWORD DwWitten=0;
		::WriteFile(file,&bytes,1,&DwWitten,NULL);
		::WriteFile(file,buf,bytes,&DwWitten,NULL);
		//file.Write(&bytes,1);
		//file.Write(buf,bytes);

		buf[0] = buf[255];
		nIndex = 0;
		nUsedBits -= 255*8;
	}
	
	if(nCurByteUsedBits == 0)
	{
		buf[nIndex] = data;
	}
	else
	{
		buf[nIndex] |= data<<nCurByteUsedBits;
	
		if(nBit > 8-nCurByteUsedBits)
		{
			nIndex++;
			buf[nIndex] = data>>(8-nCurByteUsedBits);
		}
	}
		
	nUsedBits += nBit;
}

void WriteData(HANDLE file,WORD *pHeard,int nDataCount,BYTE bitsPixel,BOOL bEnd)
{
	BYTE initBits = bitsPixel+1;
	BYTE pByts[256];
	BYTE *pTmp = (BYTE*)pHeard;
	int nCurCodeBits=initBits;
	WORD nMaxCode=1;
	WORD nClearCode=1;
	WORD nEndCode;

	/*CStdioFile strFile;
	
	if(strFile.Open("d:\\writedata.txt",CFile::modeWrite|CFile::modeCreate|CFile::typeText))
	{
		CString sTmp;
		for(int i=0; i<nDataCount; i++)
		{
			sTmp.Format("%X ",pHeard[i]);
			strFile.WriteString(sTmp);
		}
	}
	*/

	nClearCode <<= nCurCodeBits-1;
	nMaxCode <<= nCurCodeBits;
	nEndCode = nClearCode+1;

	memset(pByts,0,256);
	
	long nUsedBits = 0;

	addToBuf(file,pByts,nUsedBits,nClearCode&0xFF,min(8,nCurCodeBits));
	if(nCurCodeBits > 8)
	{
		addToBuf(file,pByts,nUsedBits,(nClearCode&0xFF00)>>8,nCurCodeBits-8);
	}
	
	int nHeardCode = nClearCode+1;

	for(int i=0; i<nDataCount; i++)
	{
		if(nHeardCode++ >= nMaxCode)
		{
			nCurCodeBits++;
			if(nCurCodeBits == 13)
			{
				//CString sMsg;
				//sMsg.Format("[%X,%X]",pHeard[i-1],pHeard[i]);
				//AfxMessageBox(sMsg);

				addToBuf(file,pByts,nUsedBits,nClearCode&0xFF,8);
				addToBuf(file,pByts,nUsedBits,(nClearCode&0xFF00)>>8,4);
				nCurCodeBits = initBits;

				nHeardCode = nClearCode+2;
			}
			
			nMaxCode = (WORD)1<<nCurCodeBits;
		}
		
		addToBuf(file,pByts,nUsedBits,pHeard[i]&0xFF,min(8,nCurCodeBits));
		
		if(nCurCodeBits > 8)
		{
			addToBuf(file,pByts,nUsedBits,(pHeard[i]&0xFF00)>>8,nCurCodeBits-8);
		}
	}

	if(bEnd)
	{
		addToBuf(file,pByts,nUsedBits,nEndCode&0xFF,min(8,nCurCodeBits));
		if(nCurCodeBits > 8)
		{
			addToBuf(file,pByts,nUsedBits,(nEndCode&0xFF00)>>8,nCurCodeBits-8);
		}
	}

	long nByteCount = nUsedBits/8;
	
	if(nUsedBits%8 != 0)
	{
		nByteCount++;
	}
	
	BYTE bytes;
	int nIndex = 0;
	DWORD DwWitten=0;
	while(nByteCount>0)
	{
		bytes = min(0xFF,(BYTE)nByteCount);
		nByteCount -= bytes;
		
		
		::WriteFile(file,&bytes,1,&DwWitten,NULL);
		::WriteFile(file,pByts+nIndex,bytes,&DwWitten,NULL);
		//file.Write(&bytes,1);
		//file.Write(pByts+nIndex,bytes);
		
		nIndex += bytes;
	}

	bytes = 0x0;
	//file.Write(&bytes,1);
	::WriteFile(file,&bytes,1,&DwWitten,NULL);
}

BYTE GetColorIndex(RGBQUAD *pQuad, UINT nSize,BYTE r,BYTE g,BYTE b)
{
	BYTE nIndex;
	long nDist,nRDist,nGDist,nBDist,nMinDist = 999999999;

	
	for(UINT i=0; i<nSize; i++)
	{
		nRDist = pQuad[i].rgbRed - r;
		nGDist = pQuad[i].rgbGreen - g;
		nBDist = pQuad[i].rgbBlue - b;

		nDist = nRDist*nRDist + nGDist*nGDist + nBDist*nBDist;

		//nDist = 0.3*abs(nRDist) + 0.59*abs(nGDist) + 0.11*abs(nBDist);

		if(nDist < nMinDist)
		{
			nIndex = (BYTE)i;
			nMinDist = nDist;
			if(nDist == 0)
			{
				break;
			}
		}
	}
	
	return nIndex;
}

BOOL GetData(HBITMAP hBmp,BYTE **ppPalette,BYTE **ppData,BYTE *pBitsPixel,int *pWidth,int *pHeight)
{
	BITMAP bm;
	PBITMAPINFO bmpInf;
	
	if(GetObject(hBmp,sizeof(bm),&bm)==0)
		return FALSE;
	
	int nPaletteSize=0;
	
	if(bm.bmBitsPixel<16)
		nPaletteSize=(int)pow(2.0,bm.bmBitsPixel);
	
	bmpInf=(PBITMAPINFO)LocalAlloc(LPTR,sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*nPaletteSize+((bm.bmWidth*bm.bmBitsPixel+31)/32)*4*bm.bmHeight);
	
	BYTE* buf=((BYTE*)bmpInf) + 
		sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)*nPaletteSize;
	
	//-----------------------------------------------
	bmpInf->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInf->bmiHeader.biWidth = bm.bmWidth;
	bmpInf->bmiHeader.biHeight = bm.bmHeight;
	bmpInf->bmiHeader.biPlanes = bm.bmPlanes;
	bmpInf->bmiHeader.biBitCount = bm.bmBitsPixel;
	bmpInf->bmiHeader.biCompression = BI_RGB;
	bmpInf->bmiHeader.biSizeImage = (bm.bmWidth+7)/8*bm.bmHeight*bm.bmBitsPixel;
	//-----------------------------------------------
	
	HDC hDC = ::GetWindowDC(NULL);
	
	if(!::GetDIBits(hDC,hBmp,0,(UINT)bm.bmHeight,buf,bmpInf,DIB_RGB_COLORS))
	{
		::ReleaseDC(NULL,hDC);
		LocalFree(bmpInf);
		return FALSE;
	}
	
	::ReleaseDC(NULL,hDC);

	*pWidth = bm.bmWidth;
	*pHeight = bm.bmHeight;

	*ppData = new BYTE[bm.bmWidth*bm.bmHeight];


	CQuantizer quan(256,8);
	
	bmpInf->bmiHeader.biSize = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*nPaletteSize;
	quan.ProcessImage(bmpInf);

	int nColorCount = quan.GetColorCount();
	RGBQUAD *pQuad = new RGBQUAD[nColorCount];
	
	quan.SetColorTable(pQuad);
	
	int nLnBytes;
	
	//每行数据是4字节的整数倍
	nLnBytes = ((bm.bmWidth*bm.bmBitsPixel+31)/32)*4;

	int nOffset,i,nIndex=0;
	BYTE r,g,b;
	UINT nColorIndex;
	
	switch(bm.bmBitsPixel)
	{
	case 1:
	case 4:
	case 8:
		{
			delete []pQuad;
			nColorCount = (int)pow(2.0,bm.bmBitsPixel);
			pQuad = new RGBQUAD[nColorCount];
			memcpy(pQuad,(BYTE*)bmpInf+sizeof(BITMAPINFOHEADER),sizeof(RGBQUAD)*nColorCount);

			for(i=bm.bmHeight-1; i>=0; i--)
			{
				nOffset = i*nLnBytes;
				
				for(int j=0; j<bm.bmWidth; j++)
				{
					if(bm.bmBitsPixel==8)
					{
						(*ppData)[nIndex++] = buf[nOffset+j];
					} 
					else
					{
						BYTE pos;
						BYTE iDst= buf[nOffset + (j*bm.bmBitsPixel >> 3)];
						if (bm.bmBitsPixel==4)
						{
							pos = 4*(1-j%2);
							iDst &= (0x0F<<pos);
							(*ppData)[nIndex++] = iDst >> pos;
						}else if (bm.bmBitsPixel==1)
						{
							pos = 7-j%8;
							iDst &= (0x01<<pos);
							(*ppData)[nIndex++] = iDst >> pos;
						}
					}
				}
			}
		}
		break;
	case 15:
	case 16:
		for(i=bm.bmHeight-1; i>=0; i--)
		{
			nOffset = i*nLnBytes;
			
			for(int j=0; j<bm.bmWidth; j++)
			{
				b = buf[nOffset+j*2]&0x1F;
				g = buf[nOffset+j*2]>>5;
				g |= (buf[nOffset+j*2+1]&0x03)<<3;
				r = (buf[nOffset+j*2+1]>>2)&0x1F;
				
				r *= 8;
				b *= 8;
				g *= 8;

				quan.GetColorIndex(r,g,b,&nColorIndex);
				(*ppData)[nIndex++] = (BYTE)nColorIndex;
			}
		}
		break;
	case 24:
		for(i=bm.bmHeight-1; i>=0; i--)
		{
			nOffset = i*nLnBytes;
			
			for(int j=0; j<bm.bmWidth; j++)
			{
				b = buf[nOffset+j*3];
				g = buf[nOffset+j*3+1];
				r = buf[nOffset+j*3+2];

				quan.GetColorIndex(r,g,b,&nColorIndex);
				(*ppData)[nIndex++] = (BYTE)nColorIndex;
		
				//该算法效果好，但速度慢
				//pData[nIndex++]=GetColorIndex(pQuad,nColorCount,r,g,b);
			}
		}
		break;
	case 32:
		for(i=bm.bmHeight-1; i>=0; i--)
		{
			nOffset = i*nLnBytes;
			
			for(int j=0; j<bm.bmWidth; j++)
			{
				b = buf[nOffset+j*4];
				g = buf[nOffset+j*4+1];
				r = buf[nOffset+j*4+2];
				quan.GetColorIndex(r,g,b,&nColorIndex);
				(*ppData)[nIndex++] = (BYTE)nColorIndex;
			}
		}
		break;
	default:
		delete []pQuad;
		LocalFree(bmpInf);
		return FALSE;
		break;
	}

	*pBitsPixel = 8;

	if(nColorCount <= 8)
	{
		*pBitsPixel = 3;
	}
	else if(nColorCount <= 16)
	{
		*pBitsPixel = 4;
	}
	else if(nColorCount <= 32)
	{
		*pBitsPixel = 5;
	}
	else if(nColorCount <= 64)
	{
		*pBitsPixel = 6;
	}
	else if(nColorCount <= 128)
	{
		*pBitsPixel = 7;
	}
	
	int nGIFPaletteSize = (int)pow(2.0,(int)*pBitsPixel);
	*ppPalette = new BYTE[nGIFPaletteSize*3];
	memset(*ppPalette,0,nGIFPaletteSize*3);
	
	for(i=0; i<(int)nColorCount; i++)
	{
		(*ppPalette)[i*3] = pQuad[i].rgbRed;
		(*ppPalette)[i*3+1] = pQuad[i].rgbGreen;
		(*ppPalette)[i*3+2] = pQuad[i].rgbBlue;
	}

	delete []pQuad;
	LocalFree(bmpInf);

	return TRUE;
}

void EnCode(BYTE *pData,long nDataSize,BYTE bitsPixel,WORD **ppEnCodeData,long *pnSize)
{
	WORD *pHeard = new WORD[4097];
	WORD *pEnd = new WORD[4096];
	int nDataBufIndex=0;

	//每幅图片最大1000000个像素
	*ppEnCodeData = new WORD[1000000];

	int nIndex =0;
	int nDataIndex = 0;

	pHeard[nIndex] = pData[nDataIndex];
	pEnd[nIndex] = pData[nDataIndex+1];
	nIndex++;
	nDataIndex++;

	WORD wHeard,wEnd;
	BOOL bEndIsValide = TRUE;

	WORD wBeginCode = (((WORD)1)<<bitsPixel)+2;

	//编码
	for(; nDataIndex<nDataSize; )
	{
		wHeard = pData[nDataIndex];

		if(nDataIndex+1 >= nDataSize)
		{
			pHeard[nIndex] = wHeard;
			nIndex++;
			bEndIsValide = FALSE;
			break;
		}

		wEnd = pData[nDataIndex+1];
		nDataIndex++;

		for(int i=0; i<nIndex; i++)
		{
			if( (pHeard[i] == wHeard) && (pEnd[i] == wEnd) )
			{
				wHeard = wBeginCode+i;
				if(nDataIndex+1 >= nDataSize)
				{
					pHeard[nIndex] = wHeard;
					nIndex++;
					bEndIsValide = FALSE;
					break;
				}
				
				wEnd = pData[nDataIndex+1];
				nDataIndex++;
			}
		}
		
		pHeard[nIndex] = wHeard;
		
		if(bEndIsValide)
		{
			pEnd[nIndex] = wEnd;
			nIndex++;
		}
		else
		{
			break;
		}
		
		//达到4096(12位)后，重新开始编码
		if(wBeginCode+nIndex-1 == 4096)
		{
			memcpy( ((BYTE*)*ppEnCodeData)+nDataBufIndex,pHeard,nIndex*2);
			nDataBufIndex += nIndex*2;
			nIndex = 0;
			
			pHeard[nIndex] = pData[nDataIndex];
			pEnd[nIndex] = pData[nDataIndex+1];
			nIndex++;
			nDataIndex++;
		}
	}
	
	memcpy( ((BYTE*)*ppEnCodeData)+nDataBufIndex,pHeard,nIndex*2);
	nDataBufIndex += nIndex*2;

	*pnSize = nDataBufIndex/2;

	delete []pHeard;
	delete []pEnd;
}

typedef struct _GraphicController
{
	BYTE extensionIntroducer;
	BYTE graphicControlLabel;
	BYTE blockSize;
	BYTE packedField;
	WORD nDelayTime;
	BYTE transparentColorIndex;
	BYTE blockTerminator;
}GraphicController;

typedef struct _ApplicationExtension
{
	BYTE extensionIntroducer;
	BYTE applicationLabel;
	BYTE blockSize;
	char applicationId[8];
	char appAuthCode[3];
	char cAppData[4];
	BYTE blockTerminator;
}ApplicationExtension;

void CreateGIFHeard(HANDLE file,WORD nImageWidth,WORD nImageHeight,BYTE bitsPixel)
{
	//GIF文件头，89a格式
	GIFHEARD heard;
	memcpy(heard.Signature,"GIF89a",6);
	heard.ScreenWidth = nImageWidth;
	heard.ScreenHeight = nImageHeight;
	//heard.GlobalFlagByte = HASPalette|((bitsPixel-1)<<4)|0|(bitsPixel-1);全局调色板
	heard.GlobalFlagByte = ((bitsPixel-1)<<4);
	heard.BackGroundColor = 0;
	heard.AspectRatio = 0;

	DWORD DwWitten=0;
	::WriteFile(file,&heard,13,&DwWitten,NULL);
	//file.Write(&heard,13);//不要用sizeof(GIFHEARD)，除非把编译器设为1字节对齐
	
	//为了让IE循环播放，必须加入该应用程序块（如果只有一幅图片，可省略该块）
	ApplicationExtension appData;
	appData.extensionIntroducer = 0x21;
	appData.applicationLabel = 0xFF;
	appData.blockSize = 11;
	memcpy(appData.applicationId,"NETSCAPE",8);
	memcpy(appData.appAuthCode,"2.0",3);
	appData.cAppData[0] = 3;
	appData.cAppData[1] = 1;
	appData.cAppData[2] = 0;
	appData.cAppData[3] = 0;
	appData.blockTerminator = 0;
	
	
	::WriteFile(file,&appData,sizeof(ApplicationExtension),&DwWitten,NULL);
	//file.Write(&appData,sizeof(ApplicationExtension));
}

void AddImageToGIF(HANDLE file,BYTE *pData,BYTE *palette,WORD nImageWidth,WORD nImageHeight,BYTE bitsPixel,WORD nDelay,
				   short int nTransparentColorIndex)
{
	//控制块
	GraphicController control;
	
	control.extensionIntroducer = 0x21;
	control.graphicControlLabel = 0xF9;
	control.blockSize = 4;
	control.packedField = (nTransparentColorIndex==-1? 4:5);
	control.nDelayTime = nDelay;//延迟时间
	control.transparentColorIndex = (BYTE)(nTransparentColorIndex==-1? 0:nTransparentColorIndex);
	control.blockTerminator = 0;
	
	//file.Write(&control,sizeof(GraphicController));
	DWORD DwWitten=0;
	::WriteFile(file,&control,sizeof(GraphicController),&DwWitten,NULL);
	////file.Write(palette,nGIFPaletteSize*3);全局调色板
	//图片数据头
	GIFDATAHEARD dataHeard;
	dataHeard.imageLabel = 0x2c;
	dataHeard.imageLeft = 0;
	dataHeard.imageTop = 0;
	dataHeard.imageWidth = nImageWidth;
	dataHeard.imageHeight = nImageHeight;
	dataHeard.localFlagByte = HASPalette|(bitsPixel-1);
	
	::WriteFile(file,&dataHeard.imageLabel,1,&DwWitten,NULL);
	::WriteFile(file,&dataHeard.imageLeft,2,&DwWitten,NULL);
	::WriteFile(file,&dataHeard.imageTop,2,&DwWitten,NULL);
	::WriteFile(file,&dataHeard.imageWidth,2,&DwWitten,NULL);
	::WriteFile(file,&dataHeard.imageHeight,2,&DwWitten,NULL);
	::WriteFile(file,&dataHeard.localFlagByte,1,&DwWitten,NULL);

	//file.Write(&dataHeard.imageLabel,1);
	//file.Write(&dataHeard.imageLeft,2);
	//file.Write(&dataHeard.imageTop,2);
	//file.Write(&dataHeard.imageWidth,2);
	//file.Write(&dataHeard.imageHeight,2);
	//file.Write(&dataHeard.localFlagByte,1);
	
	//调色板
	//file.Write(palette,(int)pow(2,bitsPixel)*3);
	::WriteFile(file,palette,(int)pow(2.0,bitsPixel)*3,&DwWitten,NULL);

	//每像素占用位数
	//file.Write(&bitsPixel,1);
	::WriteFile(file,&bitsPixel,1,&DwWitten,NULL);
	
	WORD *pEnCodeData = NULL;
	long nSize = 0;

	//编码
	EnCode(pData,nImageWidth*nImageHeight,bitsPixel,&pEnCodeData,&nSize);
	
	//写入数据
	WriteData(file,pEnCodeData,nSize,bitsPixel,TRUE);
	
	delete []pEnCodeData;
}

void CloseGIF(HANDLE file)
{
	//写入结束标志
	BYTE bytes = 0x3B;
	//file.Write(&bytes,1);
	DWORD DwWitten=0;
	::WriteFile(file,&bytes,1,&DwWitten,NULL);
}