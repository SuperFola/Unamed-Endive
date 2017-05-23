#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "23";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 6;
	static const long BUILD  = 10;
	static const long REVISION  = 8267;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1065;
	#define RC_FILEVERSION 1,6,10,8267
	#define RC_FILEVERSION_STRING "1, 6, 10, 8267\0"
	static const char FULLVERSION_STRING [] = "1.6.10.8267";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
