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
	static const long BUILD  = 8;
	static const long REVISION  = 9232;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1244;
	#define RC_FILEVERSION 1,8,8,9232
	#define RC_FILEVERSION_STRING "1, 8, 8, 9232\0"
	static const char FULLVERSION_STRING [] = "1.8.8.9232";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
