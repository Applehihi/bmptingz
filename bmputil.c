#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmpdef.h"
/*Functions*/

void lendint32(byte** destination, int32_t number)
{
	*destination = (byte*)calloc(4, 1);
	if (*destination)
	{
		(*destination)[0] = (byte)number;
		(*destination)[1] = (byte)(number >> 8);
		(*destination)[2] = (byte)(number >> 16);
		(*destination)[3] = (byte)(number >> 24);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}

void lendint16(byte** destination, int16_t number)
{
	*destination = (byte*)calloc(2, 1);
	if (*destination)
	{
		(*destination)[0] = (byte)number;
		(*destination)[1] = (byte)(number >> 8);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}

void onebitbmp(char* filename, int32_t width, int32_t height, int32_t hres, int32_t vres, byte* pixel_array)
{
	strcat(filename, ".bmp");
	FILE* file = fopen(filename, "wb");
	/*BMP Header*/
	/*Magic number*/
	int16_t id = 0x4D42;
	byte* lid;
	lendint16(&lid, id);
	fwrite(lid, 1, 2, file);

	/*File size*/
	int32_t filesize = 14 + 40 + 8 + height * (int)ceil((double)width / 32) * 4;
	byte* lfilesize;
	lendint32(&lfilesize, filesize);
	fwrite(lfilesize, 1, 4, file);

	/*Reserved*/
	int32_t zero = 0x00000000;
	fwrite(&zero, 4, 1, file);

	/*Pixel array offset*/
	int32_t pxoffset = 14 + 40 + 8;
	byte* lpxoffset;
	lendint32(&lpxoffset, pxoffset);
	fwrite(lpxoffset, 1, 4, file);

	/*DIB Header*/
	/*DIB Header size*/
	int32_t dibhsize = 40;
	byte* ldibhsize;
	lendint32(&ldibhsize, dibhsize);
	fwrite(ldibhsize, 1, 4, file);

	/*Width*/
	byte* lwidth;
	lendint32(&lwidth, width);
	fwrite(lwidth, 1, 4, file);

	/*Height*/
	byte* lheight;
	lendint32(&lheight, height);
	fwrite(lheight, 1, 4, file);

	/*Colour planes*/
	int16_t colourplanes = 0x0001;
	byte* lcolourplanes;
	lendint16(&lcolourplanes, colourplanes);
	fwrite(lcolourplanes, 1, 2, file);

	/*Bits per pixel*/
	int16_t bpp = 0x0001;
	byte* lbpp;
	lendint16(&lbpp, bpp);
	fwrite(lbpp, 1, 2, file);

	/*Compression*/
	int32_t compression = 0x00000000;
	byte* lcompression;
	lendint32(&lcompression, compression);
	fwrite(lcompression, 1, 4, file);

	/*Size of data*/
	int32_t datasize = filesize - 14 - 40 - 8;
	byte* ldatasize;
	lendint32(&ldatasize, filesize);
	fwrite(ldatasize, 1, 4, file);

	/*Horizontal resolution*/
	byte* lhres;
	lendint32(&lhres, hres);
	fwrite(lhres, 1, 4, file);

	/*Vertical resolution*/
	byte* lvres;
	lendint32(&lvres, vres);
	fwrite(lvres, 1, 4, file);

	/*Colours in palette*/
	fwrite(&zero, 4, 1, file);

	/*Important colours*/
	fwrite(&zero, 4, 1, file);
	

	/*Colour table*/
	int32_t colour1 = 0x00000000;
	byte* lcolour1;
	lendint32(&lcolour1, colour1);
	fwrite(lcolour1, 1, 4, file);
	int32_t colour2 = 0x00FFFFFF;
	byte* lcolour2;
	lendint32(&lcolour2, colour2);
	fwrite(lcolour2, 1, 4, file);

	/*Pixel array*/
	int32_t filledbytesperrow = (int32_t)floor((double)width / 8);
	int32_t paddingbytes = 0;
	printf("%d", width);
	if (width % 32 - width % 8 > 0)
	{
		paddingbytes = 4 - (width % 32 - width % 8) / 8;
	}
	else if (width % 32 == width % 8)
	{
		paddingbytes = 3;
	}
	printf("%d", paddingbytes);
	for (int heightindex = 0; heightindex < height; heightindex++) 
	{
		printf("height ");
		for (int byteindex = 0; byteindex < filledbytesperrow; byteindex++) //Complete sets of bytes
		{
			byte buffer = 0;
			for (int bitindex = 0; bitindex < 8; bitindex++) 
			{
				buffer = buffer | (pixel_array[heightindex * width + byteindex * 8 + bitindex] << (7 - bitindex)); //Arrange the bits from left to right
			}
			fwrite(&buffer, 1, 1, file);
		}
		
		byte buffer = 0;
		for (int bitindex = filledbytesperrow * 8; bitindex < width; bitindex++) //Remaining bits
		{
			buffer = buffer | (pixel_array[heightindex * width + filledbytesperrow * 8 + (bitindex - filledbytesperrow * 8)] << (7 - (bitindex - filledbytesperrow * 8))); //Arrange the bits from left to right
		}
		fwrite(&buffer, 1, 1, file);
		
		byte bufferzero = 0x00;
		
		for (int paddingindex = 0; paddingindex < paddingbytes; paddingindex++) //Padding
		{
			fwrite(&bufferzero, 1, 1, file);
		}
		
	}
	

}



int main(void)
{
	byte* bruh = (byte*)malloc(1);
	bruh[0] = 1;
	onebitbmp("bruh", 1, 1, 100, 100, bruh);
}