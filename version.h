#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 5;
	static const long BUILD  = 10;
	static const long REVISION  = 16954;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1688;
	#define RC_FILEVERSION 2,5,10,16954
	#define RC_FILEVERSION_STRING "2, 5, 10, 16954\0"
	static const char FULLVERSION_STRING [] = "2.5.10.16954";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 16;
	

}
#endif //VERSION_H
