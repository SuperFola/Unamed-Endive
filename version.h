#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 5;
	static const long BUILD  = 3;
	static const long REVISION  = 7970;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1001;
	#define RC_FILEVERSION 1,5,3,7970
	#define RC_FILEVERSION_STRING "1, 5, 3, 7970\0"
	static const char FULLVERSION_STRING [] = "1.5.3.7970";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
