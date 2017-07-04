#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 4;
	static const long REVISION  = 9009;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1227;
	#define RC_FILEVERSION 1,7,4,9009
	#define RC_FILEVERSION_STRING "1, 7, 4, 9009\0"
	static const char FULLVERSION_STRING [] = "1.7.4.9009";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
