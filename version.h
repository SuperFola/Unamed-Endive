#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 22;
	static const long REVISION  = 1097;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 258;
	#define RC_FILEVERSION 1,0,22,1097
	#define RC_FILEVERSION_STRING "1, 0, 22, 1097\0"
	static const char FULLVERSION_STRING [] = "1.0.22.1097";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 22;
	

}
#endif //VERSION_H
