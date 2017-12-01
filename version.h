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
	static const long BUILD  = 8;
	static const long REVISION  = 15646;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1624;
	#define RC_FILEVERSION 2,4,8,15646
	#define RC_FILEVERSION_STRING "2, 4, 8, 15646\0"
	static const char FULLVERSION_STRING [] = "2.4.8.15646";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 12;
	

}
#endif //VERSION_H
