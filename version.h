#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 4;
	static const long BUILD  = 18;
	static const long REVISION  = 6362;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 833;
	#define RC_FILEVERSION 1,4,18,6362
	#define RC_FILEVERSION_STRING "1, 4, 18, 6362\0"
	static const char FULLVERSION_STRING [] = "1.4.18.6362";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 5;
	

}
#endif //VERSION_H
