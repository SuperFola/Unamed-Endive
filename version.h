#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 4;
	static const long BUILD  = 20;
	static const long REVISION  = 15288;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1600;
	#define RC_FILEVERSION 2,4,20,15288
	#define RC_FILEVERSION_STRING "2, 4, 20, 15288\0"
	static const char FULLVERSION_STRING [] = "2.4.20.15288";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 3;
	

}
#endif //VERSION_H
