/*******************************************************************************
* Copyright (C) 2009-2017 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*
* @author: Yann Loisel <yann.loisel@maximintegrated.com>
* @author: Benjamin VINOT <benjamin.vinot@maximintegrated.com>
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include <ca_sign_build.h>
#include <maxim_c_utils.h>


int process_string(char *output,char *reference,char *line,int fgets_correction)
{
	int i,j;
	char dupline[MAXLINE];
	char dupreference[MAXLINE];
	for(i=0;i<(int)strlen(line);i++)
		dupline[i]=(char)toupper((int)line[i]);
	dupline[strlen(line)]='\0';
	
	for(i=0;i<(int)strlen(reference);i++)
		dupreference[i]=(char)toupper((int)reference[i]);
	
	dupreference[strlen(reference)]='\0';
	
	if(strstr(dupline,dupreference)==dupline)
	{
		found=1;
		for(j=-1,i=0;i<(int)strlen(line)-fgets_correction;i++){
			if(line[i]=='='){
				j=i;
			}
		}
	
		if(strlen(line)>MAXLINE){
			return EXIT_FAILURE;
		}
	
		if(j!=-1){
			for(i=j+1;i<(int)strlen(line)-fgets_correction;i++)
			{
				output[i-j-1]=line[i];
			}
		}else {
			return EXIT_FAILURE;
		}

		output[i-j-1]='\0';


	}

	return EXIT_SUCCESS;
}

int process_hexvalue(u8 *value,char *reference,char *line,int limit,int fgets_correction)
{
  int i,j;
  char dupline[MAXLINE];
  char dupreference[MAXLINE];
  for(i=0;i<(int)strlen(line);i++)
    dupline[i]=(char)toupper((int)line[i]);
  dupline[strlen(line)]='\0';
  for(i=0;i<(int)strlen(reference);i++)
    dupreference[i]=(char)toupper((int)reference[i]);
  dupreference[strlen(reference)]='\0';
  if(strstr(dupline,dupreference)==dupline)
    {
      found=1;
      for(j=-1,i=0;i<(int)strlen(line)-1;i++){
    if(line[i]=='='){
      j=i;
      }}
      if(j!=-1){
    for((*value)=0,i=j+1;i<(int)strlen(line)-fgets_correction;i+=2)
      (*value)=((*value)*256)+hex(line[i],line[i+1]);
      }
      if((limit!=-1)&&(*value)>=limit)
    {
      printf("ERROR: %s shall be less than %d\n",reference,limit);
      return EXIT_FAILURE;
    }
    }
  return EXIT_SUCCESS;
}

int process_value (uint8_t * value, char *reference, char *line, int limit, int fgets_correction)
{
	int i, j;
	char dupline[MAXLINE];
	char dupreference[MAXLINE];
	for (i = 0; i < (int) strlen (line); i++)
		dupline[i] = (char) toupper ((int) line[i]);
	dupline[strlen (line)] = '\0';
	for (i = 0; i < (int) strlen (reference); i++)
		dupreference[i] = (char) toupper ((int) reference[i]);
	dupreference[strlen (reference)] = '\0';
	if (strstr (dupline, dupreference) == dupline)
	{
		found = 1;
		for (j = -1, i = 0; i < (int) strlen (line) - fgets_correction; i++)
			if (line[i] == '=')
				j = i;
		if (j != -1)
			for ((*value) = 0, i = j + 1; i < (int) strlen (line) - fgets_correction; i++)
			{
				(*value) = ((*value) * 10) + (int) line[i] - (int) '0';
				printf ("<%c>", line[i]);
			}
		if ((*value) >= limit)
		{
			printf ("ERROR: %s shall be less than %d\n", reference, limit);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int process_intvalue (int *value, char *reference, char *line, int limit, int fgets_correction)
{
	int i, j;
	char dupline[MAXLINE];
	char dupreference[MAXLINE];
	
	for (i = 0; i < (int) strlen (line); i++)
		dupline[i] = (char) toupper ((int) line[i]);
	dupline[strlen (line)] = '\0';
	
	for (i = 0; i < (int) strlen (reference); i++)
		dupreference[i] = (char) toupper ((int) reference[i]);
	dupreference[strlen (reference)] = '\0';
	

	if (strstr (dupline, dupreference) == dupline)
	{
		found = 1;
		for (j = -1, i = 0; i < (int) strlen (line) - fgets_correction; i++)
			if (line[i] == '=')
				j = i;
		if (j != -1)
			for ((*value) = 0, i = j + 1; i < (int) strlen (line) - fgets_correction; i++)
			{
				(*value) = ((*value) * 10) + (int) line[i] - (int) '0';
			}
		if ((*value) >= limit)
		{
			printf ("ERROR: %s shall be less than %d (%d submitted)\n", reference, limit, *value);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int process_hexint (int *value, char *reference, char *line, int fgets_correction)
{
	int i, j, k;
	char dupline[MAXLINE];
	char dupreference[MAXLINE];
	for (i = 0; i < (int) strlen (line); i++)
		dupline[i] = (char) toupper ((int) line[i]);
	dupline[strlen (line)] = '\0';
	for (i = 0; i < (int) strlen (reference); i++)
		dupreference[i] = (char) toupper ((int) reference[i]);
	dupreference[strlen (reference)] = '\0';
	k = -1;
	*value = -1;
	if (strstr (dupline, dupreference) == dupline)
	{
		found = 1;
		for (j = -1, i = 0; i < (int) strlen (line) - fgets_correction; i++)
			if (line[i] == '=')
				j = i;
		if (j != -1)
			for ((*value) = 0, k = 0, i = j + 1; i < (int) strlen (line) - fgets_correction; i += 2, k++)
			{
				(*value) = ((*value) << 8) ^ (hex (line[i], line[i + 1]));
			}
	}
	return (EXIT_SUCCESS);
}



int process_hextab(u8 *value,char *reference,char *line,int fgets_correction)
{
  int i,j,k;
  char dupline[MAXLINE];
  char dupreference[MAXLINE];
  for(i=0;i<(int)strlen(line);i++)
    dupline[i]=(char)toupper((int)line[i]);
  dupline[strlen(line)]='\0';
  for(i=0;i<(int)strlen(reference);i++)
    dupreference[i]=(char)toupper((int)reference[i]);
  dupreference[strlen(reference)]='\0';
  k=-1;
  if(strstr(dupline,dupreference)==dupline)
    {
      found=1;
      for(j=-1,i=0;i<(int)strlen(line)-1;i++)
	if(line[i]=='=')
	  j=i;
      if(j!=-1)
	for(k=0,i=j+1;i<(int)strlen(line)-fgets_correction;i+=2,k++)
	  {
	    if(EXIT_SUCCESS==test_hex(line[i],line[i+1]))
	      value[k]=hex(line[i],line[i+1]);
	    else
	      {
		printf("ERROR: non hexa char detected in string <%s> <%c%c>\n",line,line[i],line[i+1]);
		return EXIT_FAILURE;
	      }
	  }
    }
  return EXIT_SUCCESS;
}


int process_longvalue(int *value,char *reference,char *line,int limit,int fgets_correction)
{
  int i,j;
  char dupline[MAXLINE];
  char dupreference[MAXLINE];
  for(i=0;i<(int)strlen(line);i++)
    dupline[i]=(char)toupper((int)line[i]);
  dupline[strlen(line)]='\0';
  for(i=0;i<(int)strlen(reference);i++)
    dupreference[i]=(char)toupper((int)reference[i]);
  dupreference[strlen(reference)]='\0';
  if(strstr(dupline,dupreference)==dupline)
    {
      found=1;
      for(j=-1,i=0;i<(int)strlen(line)-1;i++)
	if(line[i]=='=')
	  j=i;
      if(j!=-1)
	for((*value)=0,i=j+1;i<(int)strlen(line)-fgets_correction;i++)
	  (*value)=((*value)*10)+(int)line[i]-(int)'0';
      if((limit!=-1)&&(*value)>=limit)
	{
	  printf("ERROR: %s shall be less than %d\n",reference,limit);
	  return EXIT_FAILURE;
	}
    }
  return EXIT_SUCCESS;
}

int process_longhexvalue(u32 *value,char *reference,char *line,int limit,int fgets_correction)
{
  int i,j;
  char dupline[MAXLINE];
  char dupreference[MAXLINE];
  for(i=0;i<(int)strlen(line);i++)
    dupline[i]=(char)toupper((int)line[i]);
  dupline[strlen(line)]='\0';
  for(i=0;i<(int)strlen(reference);i++)
    dupreference[i]=(char)toupper((int)reference[i]);
  dupreference[strlen(reference)]='\0';
  if(strstr(dupline,dupreference)==dupline)
    {
      found=1;
      for(j=-1,i=0;i<(int)strlen(line)-1;i++)
	if(line[i]=='=')
	  j=i;
      if(j!=-1)
	for((*value)=0,i=j+1;i<(int)(strlen(line)-fgets_correction);i+=2)
	  (*value)=((*value)*256)+hex(line[i],line[i+1]);
      if((limit!=-1)&&(int)(*value)>=limit)
	{
	  printf("ERROR: %s shall be less than %d\n",reference,limit);
	  return EXIT_FAILURE;
	}
    }
  return EXIT_SUCCESS;
}


