#include <node.h>
#include <v8.h>

#include <setjmp.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include <jpeglib.h>

#include "optimize.h"
#include "jpeginfo.h"

using namespace v8;
using namespace node;


#define TOSTR(obj) (*String::Utf8Value((obj)->ToString()))





Handle<Value> Optimize(const Arguments& args)
{
	HandleScope scope;

	//args[0] : inputfile
	//args[1] : outputfile
	//args[2] : quality
	//args[3] : verbose

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(String::New("At least 3 arguments required - file path and quality")));
	}

	//if(args[0].Length() < 1 || !args[0]->IsString()) {
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(String::New("first arguments must be file path")));
	}

	if(!args[1]->IsString()) {
		return ThrowException(Exception::TypeError(String::New("second arguments must be file path")));
	}

	if(!args[2]->IsNumber()) {
		return ThrowException(Exception::TypeError(String::New("third arguments must be image quaility 0~100")));
	}

	std::string inputfile = TOSTR(args[0]);
	std::string outputfile = TOSTR(args[1]);
	int quality = args[2]->NumberValue();

	if(quality < 0) {
		quality = 10;
	}

	if(quality > 100) {
		quality = 100;
	}


	// TODO plz adjust quality
	int verbose = 1;
	int ret = optimize(inputfile.c_str(), outputfile.c_str(), quality, verbose);

	if(ret < 0) {
		return ThrowException(Exception::TypeError(String::New("optimize error")));
	}

	return scope.Close(Number::New(ret));
}

Handle<Value> LossLess(const Arguments& args)
{
	HandleScope scope;

	//args[0] : inputfile
	//args[1] : outputfile
	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(String::New("At least 3 arguments required - file path and quality")));
	}

	//if(args[0].Length() < 1 || !args[0]->IsString()) {
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(String::New("first arguments must be file path")));
	}

	if(!args[1]->IsString()) {
		return ThrowException(Exception::TypeError(String::New("second arguments must be file path")));
	}

	
	std::string inputfile = TOSTR(args[0]);
	std::string outputfile = TOSTR(args[1]);

	int ret = optimize_loss_less(inputfile.c_str(), outputfile.c_str());

	if(ret < 0) {
		return ThrowException(Exception::TypeError(String::New("lossless error")));
	}

	return scope.Close(Number::New(ret));
}

Handle<v8::Value> GetInfo(const v8::Arguments &args)
{
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("At least 1 arguments required - file path")));
	}

	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(String::New("first arguments must be file path")));
	}

	std::string inputfile = TOSTR(args[0]);

	struct jpeg_decompress_struct dinfo;
	int ret = getJpegHeaderInfo(inputfile.c_str(), &dinfo);

	if(ret == 0) {
		Local<Object> image = Object::New();

		image->Set(String::New("width"), Integer::New(dinfo.image_width));
		image->Set(String::New("height"), Integer::New(dinfo.image_height));
		image->Set(String::New("colordepth"), Integer::New(dinfo.num_components * 8));
		return scope.Close(image);
	} else {
		return scope.Close(Undefined());
	}

}

void init(Handle<Object> exports)
{
	exports->Set(String::NewSymbol("optimize"),FunctionTemplate::New(Optimize)->GetFunction());
	exports->Set(String::NewSymbol("lossless"),FunctionTemplate::New(LossLess)->GetFunction());
	exports->Set(String::NewSymbol("getInfo"),FunctionTemplate::New(GetInfo)->GetFunction());
}

NODE_MODULE(jpegoptim, init);

