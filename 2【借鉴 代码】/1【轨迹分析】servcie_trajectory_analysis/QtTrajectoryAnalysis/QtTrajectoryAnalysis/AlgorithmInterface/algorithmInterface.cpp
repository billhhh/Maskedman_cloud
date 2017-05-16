#include "algorithmInterface.h"

#ifdef WIN32
#include <windows.h>
void sleep_ms(int ms) {
	Sleep(ms);
}
#else
#include <unistd.h>
void sleep_ms(int ms) {
	usleep(ms*1000);
}
#endif

AlgorithmInterface::AlgorithmInterface(){
	//init();
	mutex = 0;
	pImg = &image;
	
	percent = 0;
	isRun = false;
};

AlgorithmInterface::~AlgorithmInterface(){
	//destroy();
}

void AlgorithmInterface::lock(){
	//printf("Cur Lock.Mutex = %d \n", mutex);
	while( 0 != mutex)
		sleep_ms(10);
	if( 0 == mutex)
		++mutex;
}

void AlgorithmInterface::unlock(){
	if( 1 == mutex)
		--mutex;
	//printf("Cur UnLock.Mutex = %d \n", mutex);
}