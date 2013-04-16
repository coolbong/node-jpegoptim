#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <jpeglib.h>

#include <setjmp.h>

#include "error.h"
#include "jpeginfo.h"

//struct jpeg_decompress_struct dinfo;

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;


static void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

int verbose = 0;


int getJpegHeaderInfo(const char *inputfilename, struct jpeg_decompress_struct *dinfo) 
{
	
	FILE *infile = NULL;
	struct my_error_mgr jderr;

	//step 0. check input file exist .
	infile = fopen(inputfilename, "r");
	if(infile == NULL) {
		if(verbose) {
			printf("%s file is not exist\n", inputfilename);
		}
		return NODE_E_NOEXIST;
	}


	//step 1. read jpeg file
	dinfo->err = jpeg_std_error(&jderr.pub);
	jpeg_create_decompress(dinfo);

	jderr.pub.error_exit = my_error_exit;

	if (setjmp(jderr.setjmp_buffer)) {
		jpeg_abort_decompress(dinfo);
		fclose(infile);
		return NODE_E_ERROR;
	}

	jpeg_stdio_src(dinfo, infile);
	jpeg_read_header(dinfo, TRUE);

	if(verbose) {
		printf("input file name: %s\t", inputfilename);
		printf("%dx%d %dbit %c \n",(int)dinfo->image_width, (int)dinfo->image_height, (int)dinfo->num_components*8, (dinfo->progressive_mode?'P':'N'));
	}

	return 0;
}

int getWidth(const char* inputfilename)
{
	struct jpeg_decompress_struct dinfo;
	int ret = NODE_E_ERROR;

	ret = getJpegHeaderInfo(inputfilename, &dinfo);

	if(ret < 0){
		return ret;
	}

	return dinfo.image_width;
}


int getHeight(const char* inputfilename)
{
	struct jpeg_decompress_struct dinfo;
	int ret = NODE_E_ERROR;

	ret = getJpegHeaderInfo(inputfilename, &dinfo);

	if(ret < 0){
		return ret;
	}

	return dinfo.image_height;

}

int getBitDepth(const char* inputfilename)
{
	struct jpeg_decompress_struct dinfo;
	int ret = NODE_E_ERROR;

	ret = getJpegHeaderInfo(inputfilename, &dinfo);

	if(ret < 0){
		return ret;
	}

	return dinfo.num_components;
}