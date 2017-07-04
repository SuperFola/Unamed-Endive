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
	static const long MINOR  = 6;
	static const long BUILD  = 21;
	static const long REVISION  = 8760;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1184;
	#define RC_FILEVERSION 1,6,21,8760
	#define RC_FILEVERSION_STRING "1, 6, 21, 8760\0"
	static const char FULLVERSION_STRING [] = "1.6.21.8760";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 12;
	

}
#endif //VERSION_H
