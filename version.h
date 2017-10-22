#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.10";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 0;
	static const long BUILD  = 4;
	static const long REVISION  = 11331;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1357;
	#define RC_FILEVERSION 2,0,4,11331
	#define RC_FILEVERSION_STRING "2, 0, 4, 11331\0"
	static const char FULLVERSION_STRING [] = "2.0.4.11331";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
