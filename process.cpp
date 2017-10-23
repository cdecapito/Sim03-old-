// Program Information /////////////////////////////////////////////////////////
/**
  * @file process.cpp
  *
  * @brief implements PCB and process class   
  * 
  * @details defines member functions
  *
  * @version 1.00 Carli DeCapito
  *	  		 Original Document (10/4/17)
  *
  * @note None
  */
//precompiler directives
#ifndef PROCESS_CPP
#define PROCESS_CPP

//constants
static const int NEW = 0;
static const int READY = 1;
static const int RUNNING = 2;
static const int WAITING = 3;
static const int TERMINATED = 4;

//header files
#include "process.h"
#include <iostream>

using namespace std;

//process implementation///////////////////////////////////
/**
 * @brief constructor
 *
 * @details initalizes process
 *          
 * @pre None
 *
 * @post state is set to ready
 *
 * @par Algorithm 
 *      function call
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
process::process()
{
	pcb.processState = NEW;
}

/**
 * @brief destructor
 *
 * @details resets pcb to terminate
 *          
 * @pre none
 *
 * @post pcb state is set to terminate
 *
 * @par Algorithm 
 *      Function call
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
 
process::~process()
{
	pcb.processState = TERMINATED;
}


/**
 * @brief changeState
 *
 * @details assigns pcb state to state
 *          
 * @pre none
 *
 * @post pcb state is set to state 
 *
 * @par Algorithm 
 *      function call
 *      
 * @exception None
 *
 * @param [in] state provides int value of new state
 *
 * @param [out] None
 *
 * @return None
 *
 * @note None
 */
void process::changeState( int state )
{
	pcb.processState  = state;
}


#endif