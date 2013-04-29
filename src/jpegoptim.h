#ifndef _NODE_JPEGOPTIM_H_
#define _NODE_JPEGOPTIM_H_

#include <node.h>
#include <node_buffer.h>

#include <string>


class JpegOptim : public node::ObjectWrap {
    
    //pngtest.png PNG 91x69 100x100+0+0 8-bit DirectClass 8.61KB 0.000u 0:00.000

public:
    static void Initialize(v8::Handle<v8::Object> target);

    static v8::Handle<v8::Value> Optimize(const v8::Arguments &args);
    static v8::Handle<v8::Value> LossLess(const v8::Arguments &args);
    static v8::Handle<v8::Value> GetInfo(const v8::Arguments &args);

private:
    int width;
    int height;
    int colordepth;
    
    int fileSize;
    std::string fileName;
};

#endif //_NODE_JPEGOPTIM_H_

