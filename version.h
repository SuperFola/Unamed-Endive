#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 5;
	static const long BUILD  = 4;
	static const long REVISION  = 8010;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1023;
	#define RC_FILEVERSION 1,5,4,8010
	#define RC_FILEVERSION_STRING "1, 5, 4, 8010\0"
	static const char FULLVERSION_STRING [] = "1.5.4.8010";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 15;
	

}
#endif //VERSION_H
