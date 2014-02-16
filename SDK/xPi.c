/*
xPi V2000 for Calculator
Prizm SDK Version

Copyleft (c) 2014

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <display.h>
#include <keyboard_syscalls.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "keybios.h"

char sResult[2002];

char* strcat(char* strDest,const char* strSrc)
{
    char* address=strDest;

    while (*strDest)
      strDest++;
    while(*strDest++=*strSrc++);
    return address;
}

char* strcpy(char* strDest,const char* strSrc)
{
    char* address=strDest;

    while (*strDest++=*strSrc++);
    return address;
}


void Bdisp_AreaReverseVRAM(int x1,int y1,int x2,int y2)
{
    int i,j;
	for (i=x1;i<x2;++i)
	    for (j=y1;j<y2;++j)
		    Bdisp_SetPoint_VRAM(i,j,Bdisp_GetPoint_VRAM(i,j) ^ 0xFFFF);
}

int atoi(const char* p)
{
    int neg_flag=0;
    int res=0;
	
    if (p[0]=='+'||p[0]=='-')
        neg_flag=(*p++!='+');
    while (*p>=0x30&&*p<=0x39) res=res*10+(*p++-'0');
    return neg_flag?0-res:res;
}

char* itoa(int val,char* buf,unsigned radix)
{
    char* p;             
    char* firstdig;      
    char temp;           
    unsigned digval;     

    p=buf;

    if (val<0)
    {
        *p++='-';
        val=(unsigned long)(-(long)val);
    }

    firstdig=p;

    do
	{
        digval=(unsigned)(val%radix);
        val/=radix;

        if (digval>9)
            *p++=(char)(digval-10+'a'); 
        else
            *p++=(char)(digval+'0');      
    }
	while (val>0);

    *p--='\0';         

    do
	{
        temp=*p;
        *p=*firstdig;
        *firstdig=temp;
        --p;
        ++firstdig;        
    }
	while(firstdig<p);  

    return buf;
}

static FONTCHARACTER* char_to_font(const char* cfname,FONTCHARACTER* ffname)
{
    int i,len=strlen(cfname);

	for(i=0;i<len;++i)
		ffname[i]=cfname[i];

	ffname[i]=0;
	return ffname;
}

int PiCalc(int c)
{
    int a=10000, b,d,e,f[7001],g,h=0;
	int i;

    char sBuf[10];

    strcpy(sResult,"3.141");
	
	for(i=0;i<c;i++)
        f[i]=a/5;
	
	//iElapsedTime=RTC_GetTicks();
	
    for (i=0;c!=0;++i)
    { 
        d=0;
        g=c*2;
        b=c;
        while (1) 
        { 
            d=d+f[b]*a;
            g--;
            f[b]=d%g;
            d=d/g;
            g--;
            b--;
            if(b==0) break; 
            d=d*b; 
        }
        c=c-14;
		
		memset(sBuf,0,10);
		
		if (i>0) {
            itoa(e+d/a,sBuf,10);
			if (e+d/a>1000);
			else {if (e+d/a>100)
			    strcat(sResult,"0");
			else {if (e+d/a>10)
			    strcat(sResult,"00");
			else {
			    strcat(sResult,"000");
			}}}
		    strcat(sResult,sBuf);
        }
		
        e=d%a;
	}
	
	//iElapsedTime=RTC_GetTicks()-iElapsedTime;
	//iElapsedTime*=0.015625;
}

int xPi(int c)
{
    int iHandle;
	int iSize=0;
	int i=0;
	int key;
	
	char* __sResult;
	char sTmpBuf[20];
	FONTCHARACTER ffname[32];
	
	MsgBoxPop();MsgBoxPush(1);
	locate_OS(4,4);Print_OS("Calculating...",0,0);
	Bdisp_PutDisp_DD();
	
	memset(sResult,0,2002);
	PiCalc(c);
	
	char_to_font("\\\\fls0\\PI.txt",ffname);
	Bfile_DeleteEntry(ffname);
	Bfile_CreateEntry_OS(ffname,1,&iSize);
	
	iHandle=Bfile_OpenFile_OS(ffname,2);
    if (iHandle<0) return 0;
	
	Bfile_WriteFile_OS(iHandle,sResult,strlen(sResult));
    Bfile_CloseFile_OS(iHandle);
	
	MsgBoxPop();MsgBoxPush(4);
	locate_OS(4,3);Print_OS("Complete!",0,0);
	//locate_OS(4,3);Print_OS("Time elapsed:",0,0);
	locate_OS(6,5);Print_OS("Press:[EXIT]",0,0);
	
	//memset(sTmpBuf,0,20);
	//sprintf(sTmpBuf,"%.2fs",iElapsedTime);
	//locate_OS(4,4);Print_OS(sTmpBuf,0,0);
	
	while (1)
	{
	    GetKey(&key);
		if (key==KEY_CTRL_EXIT)
		    return 0;
	}
}

void Disp_About()
{
	redraw:
	
	MsgBoxPop();MsgBoxPush(5);
	
	locate_OS(3,2);
	Print_OS("xPi V2000",0,0);
	locate_OS(3,3);
	Print_OS("Prizm SDK Version",0,0);
	locate_OS(3,4);
	Print_OS("Copyleft (c) 2014",0,0);
	locate_OS(3,5);
	Print_OS("Licensed under",0,0);
	locate_OS(3,6);
	Print_OS("GNU GPL v2",0,0);
	
	while (1)
	{
	    int key;
	    GetKey(&key);
	    switch (key)
		{
		    case KEY_CTRL_EXIT:
				return;break;
	    }
	}
}

int EnterDigits()
{
    int iInsPos;
	int key;
	
	char keybuff[5];

	memset(keybuff,0,5);
	
redraw:
	MsgBoxPop();MsgBoxPush(2);
	
	locate_OS(3,3);Print_OS("Enter digits:",0,0);
	locate_OS(3,4);Print_OS("[    ]",0,0);
	locate_OS(4,4);Print_OS(keybuff,0,0);
	
	while (1)
	{
	    GetKey(&key);
		switch (key)
		{
        case KEY_CTRL_DEL:
			if (iInsPos>0)
		    {
				keybuff[--iInsPos]=0;
				goto redraw;
			}
			break;
		case KEY_CTRL_EXIT:
		    return 0;
		case KEY_CTRL_F1:
		case KEY_CTRL_EXE:
			if (iInsPos==0 || atoi(keybuff)>2000 || atoi(keybuff)<4 || atoi(keybuff)%4!=0) break;
			return atoi(keybuff);
		default:
			if (key>=0x30&&key<=0x39)
			{
				if (iInsPos<=3)
				{
					if (key==0x30&&iInsPos==0) break;
				    keybuff[iInsPos++]=key;
					goto redraw;
				}
			}
		}
	}
}

void main()
{
    unsigned int key;
	int iPiDigits=800;
	int iCursorPos=0;

	char cDispTmpBuf[20];
	char sTmp[20];
	int iTmpBuf;
	
redraw:
    Bdisp_AllClr_VRAM();
	
	locate_OS(1,1);Print_OS("Digits",0,0);
    
	memset(cDispTmpBuf,0,20);
	strcpy(cDispTmpBuf,":");
	memset(sTmp,0,20);
	itoa(iPiDigits,sTmp,10);
	strcat(cDispTmpBuf,sTmp);
	locate_OS(13,1);Print_OS(cDispTmpBuf,0,0);
	
	locate_OS(1,2);Print_OS("Start calculation",0,0);
	locate_OS(1,3);Print_OS("About xPi V2000",0,0);
	
	Bdisp_AreaReverseVRAM(0,25+24*iCursorPos,384,47+24*iCursorPos);
	
	while (1)
	{
	    GetKey(&key);
		switch (key)
		{
		case KEY_CTRL_UP:
		    if (iCursorPos>0) --iCursorPos;
			goto redraw;break;
		case KEY_CTRL_DOWN:
		    if (iCursorPos<2) ++iCursorPos;
			goto redraw;break;
        case KEY_CTRL_EXE:
		    switch (iCursorPos)
			{
			case 0:
			    iTmpBuf=EnterDigits();
				if (iTmpBuf) iPiDigits=iTmpBuf;
				goto redraw;break;
			case 1:
                xPi(iPiDigits/4*14);
			    goto redraw;break;
			case 2:
			    Disp_About();
				goto redraw;break;
			}
        }
    }

    return;
}