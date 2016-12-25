#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 34;
	static const long REVISION  = 1668;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 291;
	#define RC_FILEVERSION 1,0,34,1668
	#define RC_FILEVERSION_STRING "1, 0, 34, 1668\0"
	static const char FULLVERSION_STRING [] = "1.0.34.1668";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 34;
	

}
#endif //VERSION_H
