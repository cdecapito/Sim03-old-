// Program Information ////////////////////////////////////
/**
  * @file simFuncs.cpp
  *
  * @brief implementation of simulation, simulates 
  *        processes and threading    
  * 
  * @details splits meta data into processes and outputs data
  *
  * @version 1.01 Carli DeCapito
  * 		 Timer Functionality ( 10/7/17 )
  *
  * 		 1.00 Carli DeCapito
  *		     Original Document (10/6/17)
  *
  * @note None
  */

// Header Files
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "data.h"
#include "process.h"
#include <semaphore.h>

using namespace std;

//global variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//function definitions
void splitMetaData( vector<process> &pdata, 
					vector<metaData> mdata );

void logData( configData cdata, 
			   vector<process> pdata );

bool logProcessingData( process &proc, 
						configData cdata, 
						bool monitor, 
						bool file, 
						double &time, 
						ofstream &fout );

bool getStartString( metaData mdata, 
					 char string[], 
					 double &time, 
					 process &proc,
					 configData &cdata );

void getEndString( metaData mdata, 
				   char string[], 
				   double &time,
				   process &proc,
				   configData &cdata );

void timer( double &time,
			int timeInterval );

void* runner( void *param );

void addDelay( double &time,
			   int threadTime );

int getWaitTime( timeval startTime );

void allocateMemory( configData &cdata, 
					 char string[] );

void decToHex( int num, 
			   char string[] );



//function implementation /////////////////////////////////
/**
 * @brief splitMetaData 
 *
 * @details splits metaData into separate processes
 *          
 * @pre meta data must have a start(S & A) and end (A) to be a process
 *
 * @post data is stored into vector
 *
 * @par Algorithm 
 *      use for loop to get data
 *      
 * @exception None
 *
 * @param [in] mdata provies vector of metadata
 *
 * @param [out] pdata provides vector of processes
 *
 * @return Void
 *
 * @note None
 */
void splitMetaData( vector<process> &pdata, 
					vector<metaData> mdata )
{
	queue<metaData> tempData;
	process tempProcess;
	metaData tempMeta;
	int index;
	int processIndex = 1;
	int metaSize = mdata.size();

	//for size of vector, push data into queue
	for ( index = 0; index < metaSize; index++ )
	{
		//extract data and push onto queue
		tempMeta = mdata[ index ];
		tempData.push( tempMeta );

		//check if new process start or end
		if ( tempMeta.metaCode == 'S' || tempMeta.metaCode == 'A' )
		{
			//push onto process vector
			if( tempMeta.metaCode == 'S' &&
				strcmp( tempMeta.metaDescriptor, "end" ) == 0 )
			{
				pdata[ pdata.size() - 1 ].metadata.push( tempMeta );
			}
			//do nothing since last process
			if( tempMeta.metaCode == 'A' &&
				strcmp( tempMeta.metaDescriptor, "end" ) == 0 )
			{
				tempProcess.changeState( NEW );
				tempProcess.processNum = processIndex;
				processIndex++;
				tempProcess.metadata = tempData;

				pdata.push_back( tempProcess );

				while( !tempData.empty() )
				{
					tempData.pop();
				}
			}
		}
	}
}

/**
 * @brief logData
 *
 * @details outputs data to file/monitor/both
 *          
 * @pre processes and cdata must exist
 *
 * @post data is log to designated spot
 *
 * @par Algorithm 
 *      using checks, output data
 *      
 * @exception None
 *
 * @param [in] cdata provides class of configuration data
 *
 * @param [in] pdata provides vector of proesses
 *
 * @return Void
 *
 * @note None
 */
void logData( configData cdata, 
			  vector<process> pdata )
{
	bool file = false;
	bool monitor = false;
	bool processingValid = false;
	int index;
	int processSize = pdata.size();
	double time = 0.0;
	ofstream fout;

	if( strcmp( cdata.logInfo, "Log to Both" ) == 0 )
	{
		file = monitor = true;
	}
	else if ( strcmp( cdata.logInfo, "Log to File" ) == 0 )
	{
		file = true;
	}
	else if ( strcmp( cdata.logInfo, "Log to Monitor" ) == 0 )
	{
		monitor = true;
	}
	else
	{
		cout << "Error. Invalid Logging Information: " 
			 << cdata.logInfo << endl; 
	}
	//check if logging to file
	if( file == true )
	{
		//if file path was not given, report error and return
		if ( cdata.logFilePath[ 0 ] == '\0' )
		{
			cout << "Error. Missing File Path" << endl;
			return;
		}
		//otherwise open file
		fout.open( cdata.logFilePath );
	}

	for ( index = 0; index < processSize; index++ )
	{
		//change state to running
		pdata[ index ].changeState( RUNNING );

		//check that processing is valid
		processingValid = logProcessingData( pdata[ index ], cdata, monitor, file, time, fout );

		//pdata[ index ].changeState( READY );
		if ( !processingValid )
		{
			//close file if cannot process data
			fout.close();
			return;
		}
		//change state to terminated
		pdata[ index ].changeState( TERMINATED );
	}
	fout.close();
}


/**
 * @brief logProcessingData
 *
 * @details logs data for single process
 *          
 * @pre mdata must be in queue
 *
 * @post data is logged
 *
 * @par Algorithm 
 *      Loop until queue is empty
 *      
 * @exception None
 *
 * @param [out] proc provides current process
 *
 * @param [in] cdata provides class of configuration data
 *
 * @param [in] monitor provides bool if logging to monitor
 *
 * @param [in] file provides bool if logging to file
 *
 * @param [out] time provides total time of process
 *
 * @param [out] fout provides ofstream variable to output to file
 *
 * @return bool
 *
 * @note None
 */
bool logProcessingData( process &proc, 
						configData cdata, 
						bool monitor, 
						bool file, 
						double &time, 
						ofstream &fout )
{
	metaData currMetaData;
	bool dataValid;
	bool check = false;
	char startStr[ STR_LEN ];
	char endStr[ STR_LEN ];
	char errorStr[ STR_LEN ];
	queue<metaData> mdata = proc.metadata;

	//while no more metadata in queue
	while( !mdata.empty() )
	{
		//get current metadata
		currMetaData = mdata.front();
		//pop it off the queue
		mdata.pop();

		//check that the current metadata is valid
		dataValid = currMetaData.errorCheck( errorStr, cdata );

		//check that metadata is valid
		if( dataValid )
		{
			//get start string
			check = getStartString( currMetaData, startStr, time, proc, cdata );

			//output to monitor
			if( monitor ) 
			{
				cout << startStr << endl;
			}
			//output to file
			if( file )
			{
				fout << startStr << endl;
			}
			//if could get start string. then get end string
			if ( check )
			{

				getEndString( currMetaData, endStr, time, proc, cdata );

				//output to monitor
				if ( monitor )
				{
					cout << endStr << endl;
				}
				//output to file
				if ( file )
				{
					fout << endStr << endl;
				}
			}
		}
		//else error occured
		else 
		{
			cout << errorStr << endl;
			return false;
		}
	}
	return true;
}


/**
 * @brief getStartString
 *
 * @details returns starting string of output
 *          
 * @pre cdata and mdata must be valid
 *
 * @post string is passed by reference to print
 *
 * @par Algorithm 
 *      Does checks and function calls
 *      
 * @exception None
 *
 * @param [in] mdata provides metaData
 *
 * @param [in] string provides cstring to output
 *
 * @param [out] time provides total time to output
 *
 * @param [out] proc provides process class obj
 *
 * @return bool
 *
 * @note None
 */
bool getStartString( metaData mdata, 
					 char string[], 
					 double &time, 
					 process &proc,
					 configData &cdata )
{
	//initalize functions and variables
	char tempStr[ STR_LEN ];
	char timeStr[ STR_LEN ];
	pthread_t tid;
	int metaTime = mdata.time;
	int* threadTime = &metaTime;
	sem_t semaphore;
	
	//get mutex lock
	pthread_mutex_lock( &mutex );

	//get time
	sprintf( timeStr, "%f", time );
	strcpy( string, timeStr );
	strcat( string, " - " );

	//convert process number to string
	sprintf( tempStr, "%d", proc.processNum );

	//get string to output process number
	if( mdata.metaCode != 'S' && mdata.metaCode != 'A' )
	{
		strcat ( string, "Process " );
		strcat ( string, tempStr );
		strcat ( string, ": " );
	}

	//get rest of string
	mdata.getStartString( string, tempStr );

	//check for I/O metadata
	if ( mdata.metaCode == 'I' || mdata.metaCode == 'O' )
	{
		//change PCB state to waiting
		proc.changeState ( WAITING );

		//initalize semaphore
		sem_init( &semaphore, 0, 1 );

		//take semaphore
		sem_wait( &semaphore );

		//create thread to do IO 
		pthread_create( &tid, NULL, runner, threadTime );

		//wait for thread to finish
		pthread_join( tid, NULL );

		//release semaphore
		sem_post( &semaphore );

		//add delay
		addDelay( time, *threadTime );

		if ( strcmp( mdata.metaDescriptor, "printer" )  == 0 )
		{
			if( cdata.currPrinter == cdata.numPrinter + 1 )
			{
				cdata.currPrinter = 0;
			}
			strcat( string, " on PRNTR " );
			sprintf( tempStr, "%d", cdata.currPrinter );
			strcat( string, tempStr );
			cdata.currPrinter++;
		}
		else if( strcmp( mdata.metaDescriptor, "hard drive" ) == 0 )
		{
			if( cdata.currHardDrive == cdata.numHardDrive + 1 )
			{
				cdata.currHardDrive = 0;
			}
			strcat( string, " on HDD " );
			sprintf( tempStr, "%d", cdata.currHardDrive );
			strcat( string, tempStr );
			cdata.currHardDrive++;
		}
		//change pcb state to running
		proc.changeState ( RUNNING );

		//release mutex
		pthread_mutex_unlock( &mutex );

		return true;
	}

	timer( time, mdata.time );

	if ( ( mdata.metaCode == 'S' ) ||
		 ( mdata.metaCode == 'A' && 
		 strcmp( mdata.metaDescriptor, "end" ) == 0 ) )
	{
		//realease mutex
		pthread_mutex_unlock( &mutex );

		return false;
	}

	//release mutex
	pthread_mutex_unlock( &mutex );
	return true;
}


/**
 * @brief getEndString
 *
 * @details gets string to output to file/monitor
 *          
 * @pre cdata must be valid and startstring must have outputed
 *
 * @post string is passed by reference to output
 *
 * @par Algorithm 
 *      completes various checks and calls to get appropriate string
 *      
 * @exception None
 *
 * @param [in] mdata provides metaData obj
 *
 * @param [in] string provides cstring to output
 *
 * @param [out] time provides total time elapsed for process
 *
 * @param [out] proc provides process class obj
 *
 * @param [in] cdata provides configuration data obj
 *
 * @return None
 *
 * @note None
 */
void getEndString( metaData mdata, 
				   char string[], 
				   double &time,
				   process &proc,
				   configData &cdata )
{
	char tempStr[ STR_LEN ];
	char timeStr[ STR_LEN ];
	char memStr[ STR_LEN ];
	

	//get time in string 
	sprintf( timeStr, "%f", time );
	strcpy( string, timeStr );
	strcat( string, " - " );

	//convert process number to string
	sprintf( tempStr, "%d", proc.processNum );

	//check that code is not S or A
	if ( mdata.metaCode != 'S' && mdata.metaCode != 'A' )
	{
		strcat( string, "Process " );
		strcat( string, tempStr );
		strcat( string, ": " );

	}
	//allocate in memory
	if( mdata.metaCode == 'M' )
	{
		//memory allocation
		allocateMemory( cdata, memStr );
	}

	//get rest of string data
	mdata.getEndString( string, tempStr, memStr );
}

/**
 * @brief timer
 *
 * @details waits for amount of time requested, and updates total time
 *          
 * @pre timeInterval must be nonnegative
 *
 * @post time is passed and then time total updated to output
 *
 * @par Algorithm 
 *      Use sys/time.h timeval structure
 * 		Use while loop until pass time. 
 *      
 * @exception None
 *
 * @param [in] timeInterval provides int with requested time to wait
 *
 * @param [out] time provides double of total time to output
 *
 * @return None
 *
 * @note None
 */
void timer( double &time,
			int timeInterval )
{
	timeval startTime, endTime; 
	int secs, usecs;
	double tempTime;

	//get current time
	gettimeofday( &startTime, NULL );

	//wait until allotted time has passed
	while( getWaitTime( startTime ) < timeInterval );

	//get time after passing time
	gettimeofday( &endTime, NULL );

	//calculate time elapsed
	//time in secs and usecs
	secs = endTime.tv_sec - startTime.tv_sec;
	usecs = endTime.tv_usec - startTime.tv_usec;

	if( usecs < 0 )
	{
		//add 1 sec worth of microsecs
		usecs += 1000000;

		//take one sec away from secs
		secs -= 1;
	}
	//if usecs is greater than 1 sec, convert to 1 sec
	if( usecs > 999999 )
	{
		secs += 1;
		usecs -= 1000000;
	}

	//convert into double
	tempTime = ( double ) usecs / 1000 / 1000;
	time = time + ( double ) secs + tempTime; 
}

/**
 * @brief runner
 *
 * @details runner function to wait for thread creation
 *          
 * @pre param is time to elapse
 *
 * @post param is updated with time in usecs
 *
 * @par Algorithm 
 *      use timeval from sys/time.h library
 *      
 * @exception None
 *
 * @param [in] param provides int ptr with time to wait/ returns microsecs
 *
 * @return Void*
 *
 * @note None
 */
void* runner( void *param )
{
	int secs, usecs;
	timeval startTime, endTime;

	gettimeofday( &startTime, NULL );

	while( getWaitTime( startTime ) < * ( int* ) param );
	
	gettimeofday( &endTime, NULL );

	secs = endTime.tv_sec - startTime.tv_sec;
	usecs = endTime.tv_usec - startTime.tv_usec;

	if ( usecs < 0 )
	{
		//add 1 sec
		usecs += 1000000;

		//subtract 1 sec
		secs -= 1;
	}
	if ( usecs > 999999 )
	{
		//add 1 sec
		usecs -= 1000000;

		//subtract 1 sec
		secs += 1;	
	}

	*( int* )param = secs * ( 1000000 ) + usecs;

	pthread_exit( 0 );
}


/**
 * @brief addDelay
 *
 * @details adds delay from thread to total time
 *          
 * @pre threadTime contains time thread elapsed
 *
 * @post time is updated
 *
 * @par Algorithm 
 *      increment time with time of thread
 *      
 * @exception None
 *
 * @param [in] threadTime provides integer value returned from runner
 *
 * @param [out] time provides int value of total time
 *
 * @return Void
 *
 * @note None
 */
void addDelay( double &time,
			   int threadTime )
{
	double temp = ( double ) threadTime / 1000 / 1000; 

	time += temp;
}


/**
 * @brief getWaitTime
 *
 * @details helper functions that gets time to wait
 *          
 * @pre startTime must be valid
 *
 * @post return time to elapse
 *
 * @par Algorithm 
 *      use timeval from sys/time.h
 *      
 * @exception None
 *
 * @param [in] startTime provides timeval of start time
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
int getWaitTime( timeval startTime )
{
	timeval time;
	int usecs, secs;

	//get current time
	gettimeofday( &time, NULL );

	//extract secs and microsecs
	secs = time.tv_sec - startTime.tv_sec;
	usecs = time.tv_usec - startTime.tv_usec;

	if ( usecs < 0 )
	{
		// add 1 sec
		usecs += 1000000;
		//subtract 1 sec
		secs -= 1;
	}
	if( secs >  0 )
	{
		usecs = usecs + ( secs * 1000000 );
	}
	return usecs;
}

void allocateMemory( configData &cdata, 
					 char string[] )
{
	int address;
	char hexAddress[ STR_LEN ];
	int numZeros, length, index;

	if ( cdata.firstAlloc == true )
	{
		cdata.firstAlloc = false;
		address = 0;
	}
	else
	{
		address = cdata.lastAddUsed + cdata.memoryBlockSize;
		if( address >= cdata.sysMemory )
		{
			address = 0;
		}

		cdata.lastAddUsed = address;
	}

	decToHex( address, hexAddress );
	
	length = strlen( hexAddress );

	numZeros = 8 - length;

	for( index = 0; index < numZeros; index++ )
	{
		string[ index ] = '0';
	}

	string[ index ] = '\0';
	strcat( string, hexAddress );


}

void decToHex( int num, 
			   char string[] )
{
	int quotient = num / 16;
	int remainder = num % 16;
	int other, index;
	int inserted = 0;
	char letter;
	char temp[ STR_LEN ];

	if ( num == 0 )
	{
		string[ 0 ] = '0';
		string[ 1 ] = '\0';
		return; 
	}

	if ( remainder < 10 )
	{
		sprintf( temp, "%d", remainder );
		letter = temp[ 0 ];
	}
	else
	{
		switch( remainder )
		{
			case 10: 
				letter = 'A';
				break;

			case 11:
				letter = 'B';
				break;

			case 12:
				letter = 'C';
				break;

			case 13:
				letter = 'D';
				break;

			case 14: 
				letter = 'E';
				break;

			case 15:
				letter = 'F';
				break;
		}
	}

	string[ 0 ] = letter;
	inserted++;

	while( quotient != 0 )
	{
		other = quotient;
		quotient = quotient / 16;
		remainder = other % 16;

		if ( remainder < 10 )
		{
			sprintf( temp, "%d", remainder );
			letter = temp[ 0 ];
		}
		
		switch( remainder )
		{
			case 10: 
				letter = 'A';
				break;

			case 11:
				letter = 'B';
				break;

			case 12:
				letter = 'C';
				break;

			case 13:
				letter = 'D';
				break;

			case 14: 
				letter = 'E';
				break;

			case 15:
				letter = 'F';
				break;
		}

		for( index = inserted - 1; index >= 0; index-- )
		{
			string[ index + 1 ] = string[ index ];
		}

		string[ 0 ] = letter;
		inserted++;
	}
	string[ inserted ] = '\0';

}
