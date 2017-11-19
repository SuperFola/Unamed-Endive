#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.11";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 3;
	static const long BUILD  = 13;
	static const long REVISION  = 14909;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1583;
	#define RC_FILEVERSION 2,3,13,14909
	#define RC_FILEVERSION_STRING "2, 3, 13, 14909\0"
	static const char FULLVERSION_STRING [] = "2.3.13.14909";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 15;
	

}
#endif //VERSION_H
