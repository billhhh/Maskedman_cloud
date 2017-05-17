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
	//isRun = false;
	isRun = true;
};

AlgorithmInterface::~AlgorithmInterface(){
	//destroy();
}

void AlgorithmInterface::lock(int& _mutex){
	//printf("Cur Lock.Mutex = %d \n", mutex);
	while( 0 != _mutex)
		sleep_ms(10);
	if( 0 == _mutex)
		++_mutex;
}

void AlgorithmInterface::unlock(int& _mutex){
	if( 1 == _mutex)
		--_mutex;
	//printf("Cur UnLock.Mutex = %d \n", mutex);
}