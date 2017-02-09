#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 1;
	static const long BUILD  = 3;
	static const long REVISION  = 3241;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 463;
	#define RC_FILEVERSION 1,1,3,3241
	#define RC_FILEVERSION_STRING "1, 1, 3, 3241\0"
	static const char FULLVERSION_STRING [] = "1.1.3.3241";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 7;
	

}
#endif //VERSION_H
