#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.10";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 10;
	static const long BUILD  = 0;
	static const long REVISION  = 11117;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1349;
	#define RC_FILEVERSION 1,10,0,11117
	#define RC_FILEVERSION_STRING "1, 10, 0, 11117\0"
	static const char FULLVERSION_STRING [] = "1.10.0.11117";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 15;
	

}
#endif //VERSION_H
