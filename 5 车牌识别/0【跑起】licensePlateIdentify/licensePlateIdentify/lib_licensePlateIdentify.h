#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "licensePlateIdenify.h"

extern "C"__declspec(dllexport)  uint licensePlate_recognize(void*, int );
extern "C"__declspec(dllexport)  uint licensePlate_identify(void*, int );
extern "C"__declspec(dllexport)  uint licensePlate_identify_file(void*, int );