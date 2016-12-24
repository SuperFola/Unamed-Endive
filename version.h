#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 30;
	static const long REVISION  = 1482;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 280;
	#define RC_FILEVERSION 1,0,30,1482
	#define RC_FILEVERSION_STRING "1, 0, 30, 1482\0"
	static const char FULLVERSION_STRING [] = "1.0.30.1482";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 30;
	

}
#endif //VERSION_H
