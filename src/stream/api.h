/**
 * @file api.h
 * @brief this must be include only once in source
 * 
 * 
 */
#ifndef DAKKU_STREAM_API_H_
#define DAKKU_STREAM_API_H_
#include <stream/fstream.h>

namespace dakku {

DAKKU_EXPORT_OBJECTS(Stream, FileOutputStream, FileInputStream);
}
#endif