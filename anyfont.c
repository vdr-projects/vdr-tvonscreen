/*
 * anyfont.h: Internationalization
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: anyfont.c,v 1.2 2004/03/04 23:16:22 schmitzj Exp $
 *
 */

#include "anyfont.h"
#include "magazine.h"

anyFont::anyFont(cOsdBase *_osd,const cFont::tPixelData *fd,int fheight,int transparent)
{
	osd=_osd;
	FontData=fd;
	FontHeight=fheight;
	trans=transparent;
}
int anyFont::Height(void)
{
	return FontHeight-2-2;
}
int anyFont::Width(const char *txt) 
{
	int w = 0;
	while (txt && *txt) w += Width(*txt++);
	return w;
}
int anyFont::LargeWidth(const char *txt) 
{
	int w = 0;
	while (txt && *txt) w += Width(*txt++);
	return w*2;
}
int anyFont::Width(char c) 
{
	if ((int)FontData[(((unsigned char)c)-32)*(FontHeight)]>100)
	{
		mzlog(1," big letter error %c: %d",c,(int)FontData[(((unsigned char)c)-32)*(FontHeight)]);
		return 100;
	}
	return (int)FontData[(((unsigned char)c)-32)*(FontHeight)];
}
int anyFont::LargeWidth(char c) 
{
	if ((int)FontData[(((unsigned char)c)-32)*(FontHeight)]>100)
	{
		mzlog(1," big letter error %c: %d",c,(int)FontData[(((unsigned char)c)-32)*(FontHeight)]);
		return 100;
	}
	return (int)FontData[(((unsigned char)c)-32)*(FontHeight)]*2;
}
int anyFont::Text(int x, int y, const char *txt, eDvbColor fg, eDvbColor bg, tWindowHandle wh) 
{
	unsigned int pxl;
	int row,col;

	while (txt && *txt)
	{
		for (row=0; row<Height(); row++) 
		{
			pxl = FontData[(((unsigned char)*txt)-32)*(FontHeight)+row+2];

			// XXX maybe increment col befor starting?
			for (col=Width(*txt); col>=0; col--) 
			{
				if (!trans || pxl&1)
					osd->Fill(x+col-1,y+row,x+col-1,y+row,(pxl&1) ? fg : bg,wh);
				pxl >>= 1; 
			}
		}
		x += Width(*txt++);
	}
	return x;
}
int anyFont::LargeText(int x, int y, const char *txt, eDvbColor fg, eDvbColor bg, tWindowHandle wh) 
{
	unsigned int pxl;
	int row,col;

	while (txt && *txt)
	{
		for (row=0; row<Height(); row++) 
		{
			pxl = FontData[(((unsigned char)*txt)-32)*(FontHeight)+row+2];

			// XXX maybe increment col befor starting?
			for (col=Width(*txt); col>=0; col--) 
			{
				if (!trans || pxl&1)
					osd->Fill(x+(col-1)*2,y+row,x+(col-1)*2+1,y+row,(pxl&1) ? fg : bg,wh);
				pxl >>= 1; 
			}
		}
		x += LargeWidth(*txt++);
	}
	return x;
}
int anyFont::Text(int x, int y, int w, int h, const char *txt, eDvbColor fg, eDvbColor bg, tWindowHandle wh) 
{
	if (txt==NULL)
		return x;
	char *memtxt=strdup(txt);
	char *mytxt=memtxt;
	char *t2=mytxt;
	char c;
	int x0=0;
	int y0=0;
//mzlog(0,"'%s'",t2);
	while (mytxt && *mytxt)
	{
		switch (*mytxt)
		{
			case ' ':
				c=*(mytxt);
				*(mytxt)=0;
				if (x0+Width(t2)>w && x0>0)
				{
					x0=0;
					y0+=Height();
				}
				if (y0<h*Height())
					x0+=Text(x+x0,y+y0,t2,fg,bg,wh)-x-x0+Width(c);
				*(mytxt)=c;
				t2=mytxt+1;
				break;
			case '-':
			case '.':
				c=*(mytxt+1);
				*(mytxt+1)=0;
				if (x0+Width(t2)>w && x0>0)
				{
					x0=0;
					y0+=Height();
				}
				if (y0<h*Height())
					x0+=Text(x+x0,y+y0,t2,fg,bg,wh)-x-x0;
				*(mytxt+1)=c;
				t2=mytxt+1;
				break;
		}
		mytxt++;
	}
	if (t2!=mytxt && *t2!=0)
	{
		if (x0+Width(t2)>w && x0>0)
		{
			x0=0;
			y0+=Height();
		}
		if (y0<h*Height())
			x0+=Text(x+x0,y+y0,t2,fg,bg,wh);
	}
	free(memtxt);
	return y0/Height()+1;
}
int anyFont::TextHeight(int w, const char *txt) 
{
	if (txt==NULL)
		return 0;
	char *memtxt=strdup(txt);
	char *mytxt=memtxt;
	char *t2=mytxt;
	char c;
	int x0=0;
	int y0=0;
	while (mytxt && *mytxt)
	{
		switch (*mytxt)
		{
			case ' ':
				c=*(mytxt);
				*(mytxt)=0;
				if (x0+Width(t2)>w && x0>0)
				{
					x0=0;
					y0+=Height();
				}
				x0+=Width(t2)+Width(c);
				*(mytxt)=c;
				t2=mytxt+1;
				break;
			case '-':
			case '.':
				c=*(mytxt+1);
				*(mytxt+1)=0;
				if (x0+Width(t2)>w && x0>0)
				{
					x0=0;
					y0+=Height();
				}
				x0+=Width(t2);
				*(mytxt+1)=c;
				t2=mytxt+1;
				break;
		}
		mytxt++;
	}
	if (t2!=mytxt && *t2!=0)
	{
		if (x0+Width(t2)>w && x0>0)
		{
			x0=0;
			y0+=Height();
		}
	}
	free(memtxt);
	return y0/Height()+1;
}