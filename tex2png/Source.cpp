#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TEXHDR_SIGNATURE "Texture"


struct TEXHDR
{
	char signature[8];
	unsigned long chunk_length;
};

struct TEXIMG
{
	unsigned char magic[4];	//81 01 40 00
	unsigned long image_length;
	unsigned long image_width;
	unsigned long image_height;
};

unsigned long bswap32(unsigned long i) {
	return ((i & 0xFF000000) >> 24) | ((i & 0xFF0000) >> 8) | ((i & 0xFF00) << 8) | ((i & 0xFF) << 24);
}

bool is_file_header(unsigned char *data){
	if (strncmp((const char *)data, "Texture", 7)) {
		if(!(data[0] == 0x81 && data[1] == 0x01 && data[2] == 0x40 && data[3] == 0x00)) {
			return false;
		}
	}
	return true;
}

unsigned char* f_map(const char *filename) {
	FILE *fp;
	long size;
	unsigned char *data;
	
	fp = fopen(filename,"rb");
	if(!fp) return 0;
	
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	
	data = (unsigned char*)malloc(size + sizeof(unsigned long));
	*(unsigned long*)data = size;
	data += sizeof(unsigned long);
	fread(data,size,1,fp);
	fclose(fp);
	return data;
}

unsigned long f_length(unsigned char *data){
	return *(unsigned long*)(data - sizeof(unsigned long));
}
void f_free(unsigned char *data){
	free((data - sizeof(unsigned long)));
}


void export_png(TEXIMG *img, const char *orgname){
	FILE *fp;
	char filename[256];
	strcpy(filename,orgname);
	if(strrchr(filename,'.')){
		*strrchr(filename,'.')=0;
	}
	strcat(filename,".png");
	fp = fopen(filename,"wb");
	if(fp)
	{
		fwrite(&img[1],bswap32(img->image_length),1,fp);
		fflush(fp);
		fclose(fp);
	}
}

int main(int argc, char*argv[]) {
	unsigned char *data;
	unsigned long length;
	if(argc < 2){
		printf("tex2png <input.tex>\n");
		return 0;
	}
	
	data = f_map(argv[1]);
	if(!data){
		printf("file open failed\n");
		return 0;
	}
	
	length = f_length(data);
	
	if(!is_file_header(data)){
		printf("invalid tex format\n");
		return 0;
	}
	
	if(strncmp((const char *)data, "Texture", 7) == 0){
		if(data[sizeof(TEXHDR) + 0] == 0x81 && data[sizeof(TEXHDR) + 1] == 0x01 && data[sizeof(TEXHDR) + 2] == 0x40 && data[sizeof(TEXHDR) + 3] == 0x00) {
			export_png((TEXIMG*)&data[sizeof(TEXHDR)], argv[1]);
		}
	}
	
	if(data[0] == 0x81 && data[1] == 0x01 && data[2] == 0x40 && data[3] == 0x00) {
		export_png((TEXIMG*)data, argv[1]);
	}
	
	
	return 0;
}