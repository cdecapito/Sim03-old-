// Program Information /////////////////////////////////////////////////////////
/**
  * @file data.cpp
  *
  * @brief  implements class member functions    
  * 
  * @details defines class member functions
  *
  * @version 1.01 Carli DeCapito
  *			 Additional functionality (10/4/17)
  *			 
  *			 1.00 Carli DeCapito
  *			 Original Document (9/11/17)
  *
  * @note None
  */

//precompiler directives
#ifndef DATA_CPP
#define DATA_CPP

//header files
#include "data.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

//CONFIGDATA IMPLEMENTATION
/**
 * @brief constructor
 *
 * @details initializes data members
 *          
 * @pre none
 *
 * @post members are initialized
 *
 * @par Algorithm 
 *      Assignment operators
 *      
 * @exception None
 *
 * @param [in] none
 *
 * @param [out] none
 *
 * @return None
 *
 * @note None
 */
configData::configData()
{
	//initialize strings to null
	version[ 0 ] = '\0';
	filePath[ 0 ] = '\0';
	logInfo[ 0 ] = '\0';
	logFilePath[ 0 ] = '\0';

	//set numberical data to 0
	processorCT = 0;
	monitorDT = 0;
	hardDriveCT = 0;
	printerCT = 0;
	keyboardCT = 0;
	memoryCT = 0;
	mouseCT = 0;
	speakerCT = 0;
	sysMemory = 0;
	lastAddUsed = 0;
	currPrinter = 0;
	currHardDrive = 0;
	firstAlloc = true;
}


/**
 * @brief ~configData
 *
 * @details destructor
 *          
 * @pre does nothing
 *
 * @post none
 *
 * @par Algorithm 
 *      None
 *      
 * @exception None
 *
 * @param [in] None
 *
 * @param [out] None
 *
 * @return None
 *
 * @note None
 */	
configData::~configData()
{
	//nothing
}


//METADATA IMPLEMENTATION
/**
 * @brief metaData
 *
 * @details constructor 
 *          
 * @pre initializes data member 
 *
 * @post data members are initialized
 *
 * @par Algorithm 
 *      use assignment operator and strcpy function
 *      
 * @exception None
 *
 * @param [in] None
 *
 * @param [out] None
 *
 * @return None
 *
 * @note None
 */
metaData::metaData()
{
	metaCode = 'd'; //d for default
    strcpy( metaDescriptor, "default" );
    cycles = 0;
    time = 0;
    used = false;
}

/**
 * @brief ~metadata
 *
 * @details destructor
 *          
 * @pre none
 *
 * @post none
 *
 * @par Algorithm 
 *      none
 *      
 * @exception None
 *
 * @param [in] none
 *
 * @param [out] none
 *
 * @return None
 *
 * @note None
 */
metaData::~metaData()
{
	//nothing
}

/**
 * @brief isDescriptorValid
 *
 * @details determines if metadata descriptor is valid
 *          
 * @pre metaDescriptor is initialized
 *
 * @post returns true if valid or false otherwise
 *
 * @par Algorithm 
 *      compares strings and calculates time (usec)
 *      
 * @exception None
 *
 * @param [in] cData provides configuration data for calculation
 *
 * @param [out] None
 *
 * @return returns true if valid or false otherwise
 *
 * @note None
 */
bool metaData::isDescriptorValid( configData cData )
{
	if( strcmp( metaDescriptor, "end" ) == 0 )
	{
		time = cData.processorCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "hard drive" ) == 0 )
	{
		time = cData.hardDriveCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "keyboard" ) == 0 )
	{
		time = cData.keyboardCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "printer" )  == 0 )
	{
		time = cData.printerCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "monitor" )  == 0 )
	{
		time = cData.monitorDT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "run" )  == 0 )
	{
		time = cData.processorCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "start" )  == 0 )
	{
		time = cData.processorCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "allocate" )  == 0 )
	{
		time = cData.memoryCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "mouse" )  == 0 )
	{
		time = cData.mouseCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "speaker" )  == 0 )
	{
		time = cData.speakerCT * cycles * 1000;
		return true;
	}
	else if( strcmp( metaDescriptor, "block" )  == 0 )
	{
		time = cData.memoryCT * cycles * 1000;
		return true;
	}

	return false;
}


/**
 * @brief getEndString
 *
 * @details gets last string to print from data
 *          
 * @pre process must have valid string, memory and process must exist
 *
 * @post string is used to cout and fout
 *
 * @par Algorithm 
 *      checks and strcat
 *      
 * @exception None
 *
 * @param [in] process provides string containing process info
 *
 * @param [in] memory provides string containing memory name
 *
 * @param [in] string provides cstring to cout/fout
 *
 * @return void
 *
 * @note None
 */
void metaData::getEndString( char string[],
							 char process[],
							 char memory[] )
{
	if ( metaCode == 'S' )
	{
		strcat( string, "Simulator program ending" );
	}
	else if ( metaCode == 'A' )
	{
		if( strcmp( metaDescriptor, "start") == 0 )
		{
			strcat( string, "OS: starting process " );
			strcat( string, process );
		}
	}
	else if ( metaCode == 'M' )
	{
		if( strcmp( metaDescriptor, "allocate" ) == 0 )
		{
			strcat( string, "memory allocated at 0x" );
			strcat( string, memory );
		}
		else //block
		{
			strcat( string, "end memory blocking" );
		}
	}
	else if ( metaCode == 'P' )
	{
		strcat( string, "end processing action" );
	}
	else if ( metaCode == 'I' )
	{
		if( strcmp( metaDescriptor, "hard drive" ) == 0 )
		{
			strcat( string, "end hard drive input" );
		}
		else if( strcmp( metaDescriptor, "keyboard" ) == 0 )
		{
			strcat( string, "end keyboard input" );
		}
		else //mouse
		{
			strcat( string, "end mouse input" );
		}
	}
	else if ( metaCode == 'O' )
	{
		if( strcmp( metaDescriptor, "hard drive" ) == 0 )
		{
			strcat( string, "end hard drive input" );
		}
		else if ( strcmp( metaDescriptor, "monitor" ) == 0 )
		{
			strcat( string, "end monitor output" );
		}
		else if ( strcmp( metaDescriptor, "printer" ) == 0 )
		{
			strcat( string, "end printer output" );
		}
		else //speaker
		{
			strcat( string, "end speaker output" );
		}
	}
}


/**
 * @brief getStartString
 *
 * @details gets the first string to print to data
 *          
 * @pre process exist
 *
 * @post string contains data to fout/cout
 *
 * @par Algorithm 
 *      checks and string manipulation
 *      
 * @exception None
 *
 * @param [in] string provides cstring that holds data
 *
 * @param [in] process provides cstring of process data
 *
 * @return void
 *
 * @note None
 */
void metaData::getStartString( char string[],
							   char process[] )
{
	if ( metaCode == 'S' )
	{
		if( strcmp( metaDescriptor, "start" ) == 0 )
		{
			strcat( string, "Simulator program starting" );
		}
		else
		{
			strcat( string, "Simulator program ending" );
		}
	}
	else if ( metaCode == 'A' )
	{
		if ( strcmp( metaDescriptor, "start" ) == 0 )
		{
			strcat( string, "OS: preparing process " );
			strcat( string, process );
		}
		else
		{
			strcat( string, "OS: removing process " );
			strcat( string, process );
		}
	}
	else if ( metaCode == 'M' )
	{
		if( strcmp( metaDescriptor, "allocate" ) == 0 )
		{
			strcat( string, "allocating memory" );
		}
		else //block
		{
			strcat( string, "start memory blocking" );
		}
	}
	else if ( metaCode == 'P' )
	{
		strcat( string, "start processing action" );
	}
	else if ( metaCode == 'I' )
	{
		if( strcmp( metaDescriptor, "hard drive" ) == 0 )
		{
			strcat( string, "start hard drive input" );
		}
		else if ( strcmp( metaDescriptor, "keyboard" ) == 0 )
		{
			strcat( string, "start keyboard input" );
		}
		else //mouse
		{
			strcat( string, "start mouse input" );
		}
	}
	else if ( metaCode == 'O' )
	{
		if ( strcmp( metaDescriptor, "hard drive" ) == 0 )
		{
			strcat( string, "start hard drive output" );
		}
		else if ( strcmp( metaDescriptor, "monitor" ) == 0 )
		{
			strcat( string, "start monitor output" );
		}
		else if( strcmp( metaDescriptor, "printer" ) == 0 )
		{
			strcat( string, "start printer output" );
		}
		else //speaker
		{
			strcat( string, "start speaker output" );
		}
	}
}


/**
 * @brief error check
 *
 * @details returns an error message
 *          
 * @pre cData exists
 *
 * @post error holds error message
 *
 * @par Algorithm 
 *      check data
 *      
 * @exception None
 *
 * @param [in] error provides cstring of error message
 *
 * @param [in] cData provides configuration data 
 *
 * @return None
 *
 * @note None
 */
bool metaData::errorCheck( char error[],
						   configData cData )
{
	bool dataValid;
	char temp[ STR_LEN ];

	//check if code is invalid
	if ( metaCode != 'P' &&
		 metaCode != 'S' &&
		 metaCode != 'A' &&
		 metaCode != 'M' &&
		 metaCode != 'O' &&
		 metaCode != 'I' )
	{
		strcpy( error, "Error. Invalid Code: " );
		sprintf( temp, "%c", metaCode );
		strcat( error, temp );

		return false;
	}

	//check if descriptor is valid
	dataValid = isDescriptorValid( cData );

	//if not then report error
	if ( !dataValid )
	{
		strcpy( error, "Error. Invalid Action: " );
		strcat( error, metaDescriptor );
		return false;
	}

	if ( cycles <= 0 )
	{
		if( metaCode == 'S' || metaCode == 'A' )
		{
			return true;
		}
		strcpy( error, "Error. Invalid Cycles: " );
		sprintf( temp, "%d", cycles );
		strcat( error, temp );

		return false;
	}

	return true;
}

#endif
