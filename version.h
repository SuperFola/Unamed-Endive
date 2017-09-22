#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.09";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 8;
	static const long BUILD  = 13;
	static const long REVISION  = 9503;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1258;
	#define RC_FILEVERSION 1,8,13,9503
	#define RC_FILEVERSION_STRING "1, 8, 13, 9503\0"
	static const char FULLVERSION_STRING [] = "1.8.13.9503";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 6;
	

}
#endif //VERSION_H
