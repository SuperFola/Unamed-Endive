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
	static const long MINOR  = 9;
	static const long BUILD  = 12;
	static const long REVISION  = 10610;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1330;
	#define RC_FILEVERSION 1,9,12,10610
	#define RC_FILEVERSION_STRING "1, 9, 12, 10610\0"
	static const char FULLVERSION_STRING [] = "1.9.12.10610";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 6;
	

}
#endif //VERSION_H
