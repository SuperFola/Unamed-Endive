#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 0;
	static const long BUILD  = 53;
	static const long REVISION  = 2707;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 398;
	#define RC_FILEVERSION 1,0,53,2707
	#define RC_FILEVERSION_STRING "1, 0, 53, 2707\0"
	static const char FULLVERSION_STRING [] = "1.0.53.2707";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 53;
	

}
#endif //VERSION_H
