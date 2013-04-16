#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <jpeglib.h>

#include <setjmp.h>

#include "error.h"
#include "optimize.h"

struct jpeg_decompress_struct dinfo;
struct jpeg_compress_struct cinfo;


int save_com 	= 0;
int save_iptc 	= 0;
int save_exif 	= 0;
int save_icc 	= 0;

#define EXIF_JPEG_MARKER   JPEG_APP0+1
#define EXIF_IDENT_STRING  "Exif\000\000"
#define EXIF_IDENT_STRING_SIZE 6

#define IPTC_JPEG_MARKER   JPEG_APP0+13

#define ICC_JPEG_MARKER   JPEG_APP0+2
#define ICC_IDENT_STRING  "ICC_PROFILE\0"
#define ICC_IDENT_STRING_SIZE 12



struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


int optimize(const char* inputfilename, const char* outputfilename, int quality, int verbose) 
{
	JSAMPARRAY buf = NULL; // decoding buffer
	FILE *infile = NULL, *outfile = NULL;

	unsigned int i, j; // for for statement

	
	struct my_error_mgr jderr;
	struct my_error_mgr jcerr;
	//struct jpeg_error_mgr dpub;
	//struct jpeg_error_mgr cpub;


	//printf("inputfilename: %s\n", inputfilename);
	//printf("outputfilename: %s\n", outputfilename);


	//step 0. check input file exist .
	infile = fopen(inputfilename, "r");
	if(infile == NULL) {
		if(verbose) {
			printf("%s file is not exist\n", inputfilename);
		}
		return NODE_E_NOEXIST;
	}


	//step 1. read jpeg file
	dinfo.err = jpeg_std_error(&jderr.pub);
	jpeg_create_decompress(&dinfo);

	jderr.pub.error_exit = my_error_exit;
	jcerr.pub.error_exit = my_error_exit;



	//src file read
	//if(save_com)		jpeg_save_markers(&dinfo, JPEG_COM, 0xffff);
	//if(save_iptc)	jpeg_save_markers(&dinfo, IPTC_JPEG_MARKER, 0xffff);
	//if(save_exif)	jpeg_save_markers(&dinfo, EXIF_JPEG_MARKER, 0xffff);
	//if(save_icc)		jpeg_save_markers(&dinfo, ICC_JPEG_MARKER, 0xffff);

	if (setjmp(jderr.setjmp_buffer)) {
		jpeg_abort_decompress(&dinfo);
		fclose(infile);
		if (buf) {
			for (i=0;i<dinfo.output_height;i++) {
				free(buf[i]);
			}
			free(buf);
			buf=NULL;
		}
		return NODE_E_ERROR;
	}

	jpeg_stdio_src(&dinfo, infile);
	jpeg_read_header(&dinfo, TRUE);

	if(verbose) {
		printf("input file name: %s\t", inputfilename);
		printf("%dx%d %dbit %c \n",(int)dinfo.image_width, (int)dinfo.image_height, (int)dinfo.num_components*8, (dinfo.progressive_mode?'P':'N'));
	}

	jpeg_start_decompress(&dinfo);

	//printf("input jpeg file info: width: %d, height %d, color: %d\n",dinfo.output_width, dinfo.output_height, dinfo.out_color_components);

	buf = (JSAMPARRAY)malloc(sizeof(JSAMPROW)*dinfo.output_height);
	if (!buf) {
		if(verbose) {
			printf("not enough memory error 1\n");
		}
		jpeg_abort_decompress(&dinfo);
		fclose(infile);

		return NODE_E_NOMEMORY;
	}

	for (j=0;j<dinfo.output_height;j++) {
		buf[j] = (JSAMPROW)malloc(sizeof(JSAMPLE)*dinfo.output_width * dinfo.out_color_components);
		if (!buf[j]) {
			if(verbose) { printf("not enough memory error 2\n"); }
			jpeg_abort_decompress(&dinfo);
			fclose(infile);

			return NODE_E_NOMEMORY;
		}
	}

	while (dinfo.output_scanline < dinfo.output_height) {
		jpeg_read_scanlines(&dinfo, &buf[dinfo.output_scanline], dinfo.output_height-dinfo.output_scanline);
	}


	cinfo.err = jpeg_std_error(&jcerr.pub);
	jpeg_create_compress(&cinfo);


	// write compressed file
	outfile = fopen(outputfilename, "wb");

	jpeg_stdio_dest(&cinfo, outfile);

	if (setjmp(jcerr.setjmp_buffer)) {
		jpeg_abort_compress(&cinfo);
		jpeg_abort_decompress(&dinfo);
		fclose(outfile);
		outfile=NULL;

		if (infile) {
			fclose(infile);
		}

		if (buf) {
			for (i=0;i<dinfo.output_height;i++) {
				free(buf[j]);
			}
			free(buf);
			buf=NULL;
		}

		//if (file_exists(outfname)) {
		//	delete_file(outfname);
		//}
		return NODE_E_ERROR;
	}


	cinfo.in_color_space			= dinfo.out_color_space;
	cinfo.input_components		= dinfo.output_components;
	cinfo.image_width			= dinfo.image_width;
	cinfo.image_height			= dinfo.image_height;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	cinfo.optimize_coding = TRUE;
	jpeg_start_compress(&cinfo,TRUE);

	while (cinfo.next_scanline < cinfo.image_height) {
		jpeg_write_scanlines(&cinfo, &buf[cinfo.next_scanline], dinfo.output_height);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_finish_decompress(&dinfo);

	fflush(outfile);

	jpeg_destroy_decompress(&dinfo);
	jpeg_destroy_compress(&cinfo);

	if(infile) { fclose(infile); }
	if(outfile) { fclose(outfile); }

	if (buf) {
		for (j=0;j<dinfo.output_height;j++){
			 free(buf[j]);	
		}
		free(buf);
		buf=NULL;
	}

	return 0;
}



int optimize_loss_less(const char* inputfilename, const char* outputfilename) 
{
	jvirt_barray_ptr *coef_arrays = NULL;
	FILE *infile = NULL, *outfile = NULL;
	int verbose = 1;
	//int all_progressive = 0;
	//int all_normal = 0;

	struct my_error_mgr jderr;
	struct my_error_mgr jcerr;
	//struct jpeg_error_mgr dpub;
	//struct jpeg_error_mgr cpub;



	//step 0. check input file exist .
	infile = fopen(inputfilename, "r");
	if(infile == NULL) {
		if(verbose) {
			printf("%s file is not exist\n", inputfilename);
		}
		return NODE_E_NOEXIST;
	}


	//step 1. read jpeg file
	dinfo.err = jpeg_std_error(&jderr.pub);
	jpeg_create_decompress(&dinfo);

	jderr.pub.error_exit = my_error_exit;
	jcerr.pub.error_exit = my_error_exit;

	//printf("inputfilename: %s\n", inputfilename);
	//printf("outputfilename: %s\n", outputfilename);

	if (setjmp(jderr.setjmp_buffer)) {
		jpeg_abort_decompress(&dinfo);
		fclose(infile);
		return NODE_E_ERROR;
	}

	jpeg_stdio_src(&dinfo, infile);
	jpeg_read_header(&dinfo, TRUE);

	if(verbose) {
		printf("input file name: %s\t", inputfilename);
		printf("%dx%d %dbit %c \n",(int)dinfo.image_width, (int)dinfo.image_height, (int)dinfo.num_components*8, (dinfo.progressive_mode?'P':'N'));
	}

	coef_arrays = jpeg_read_coefficients(&dinfo);

	cinfo.err = jpeg_std_error(&jcerr.pub);
	jpeg_create_compress(&cinfo);


	// write compressed file
	outfile = fopen(outputfilename, "wb");

	jpeg_stdio_dest(&cinfo, outfile);

	if (setjmp(jcerr.setjmp_buffer)) {
		jpeg_abort_compress(&cinfo);
		jpeg_abort_decompress(&dinfo);
		fclose(outfile);
		outfile=NULL;

		if (infile) {
			fclose(infile);
		}
		return NODE_E_ERROR;
	}

	jpeg_copy_critical_parameters(&dinfo, &cinfo);

	jpeg_simple_progression(&cinfo);
	cinfo.optimize_coding = TRUE;

	/* write image */
	jpeg_write_coefficients(&cinfo, coef_arrays);

	jpeg_finish_compress(&cinfo);
	jpeg_finish_decompress(&dinfo);

	fflush(outfile);

	jpeg_destroy_decompress(&dinfo);
	jpeg_destroy_compress(&cinfo);

	if(infile) { fclose(infile); }
	if(outfile) { fclose(outfile); }



	return 0;
}

