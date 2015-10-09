/**
	ESP Hierarchie.

	Setup connect to redis server.	
*/

#include <stdio.h>
#include <math.h>
#include <vector>
#include "DataValue.hpp"

#include <shmht.h>
#include <hiredis/hiredis.h>

#include "hierarchie.h"

static redisContext *g_pContext = NULL;                                                                                                   

///DOXYS_OFF
redisContext *GetContext();
///DOXYS_ON

using namespace std;

extern "C" {
	/**
		Get Context.
	*/
	redisContext *GetContext()
	{
		return( g_pContext);
	}

	/**
		Set Context.
	*/
	void SetContext( redisContext *pNew)
	{
		g_pContext = pNew;
	}

	/**
	  init library.

	 */
	int init() 
	{
		int nRetCode = 0;
		struct timeval timeout = { 1, 500000};                                                                                              

		const char *sHostname = "127.0.0.1";                                                                                                
		int nPort = 6379;

		printf( "HierarchieLib:init");

		g_pContext = redisConnectWithTimeout( sHostname, nPort, timeout);                                                                     

		if( g_pContext != NULL) {                                                                                                             
			if( g_pContext->err != 0) {
				printf( "Connection error: %s", g_pContext->errstr);                                                                          
			}                                                                                                                               
			else {
				printf( "Connected: %s", sHostname);                                                                          
			}
		}

		return( nRetCode);
	}

	/**
	  fini library.

	 */
	int fini() 
	{
		int nRetCode = 0;

		printf( "HierarchieLib:fini");

		if( g_pContext != NULL) {                                                                                                             
			redisFree( g_pContext);

			g_pContext = NULL;
		}

		return( nRetCode);

	}

	/**
	  getContext.
	 */
	int32_t h_getContext( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		int nRetCode = 0;
		struct timeval timeout = { 1, 500000};                                                                                              

		const char *sHostname = "127.0.0.1";                                                                                                
		int nPort = 6379;

		printf( "HierarchieLib:getContext");

		if( g_pContext == NULL) {
			g_pContext = redisConnectWithTimeout( sHostname, nPort, timeout);                                                                     

			if( g_pContext != NULL) {                                                                                                             
				if( g_pContext->err != 0) {
					printf( "Connection error: %s", g_pContext->errstr);                                                                          
				}                                                                                                                               
				else {
					printf( "Connected: %s", sHostname);                                                                          
				}
			}
		}

		return( nRetCode);
	}

	/**
	  releaseContext.
	 */
	int32_t h_releaseContext( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		int nRetCode = 0;
		struct timeval timeout = { 1, 500000};                                                                                              

		if( g_pContext != NULL) {                                                                                                             
			redisFree( g_pContext);

			g_pContext = NULL;
		}

		return( nRetCode);
	}


	/**
	  get Parent.
	 */
	int32_t h_getParent( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisReply *pReply;

		/* get parent.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( g_pContext, "HGET Hierarchie:Party:%s parentId", top->val.stringv);

		if( pReply != NULL) {
			/* get string data.*/
			top->setString( strdup( pReply->str));

			freeReplyObject( pReply);                                                                                                           
		}

		return( 0);
	}

	/**
	  get clientLegalId.
	 */
	int32_t h_getClientLegalId( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisReply *pReply;

		/* get parent.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( g_pContext, "HGET Hierarchie:Party:%s clientLegalId", top->val.stringv);

		if( pReply != NULL) {
			/* get string data.*/
			top->setString( strdup( pReply->str));

			freeReplyObject( pReply);                                                                                                           
		}

		return( 0);
	}


	/**
	  set Parent.
	 */
	int32_t h_setParent( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		//Processes the arguments from last to first.
		for( int i=numargs-2; i >= 0; i--) {
			//If any of the arguments is null result is null
			if(( nextArgs+i)->null) {
				top->null = true;

				return( 0);
			}

			/* get string data.*/
			top->setString( nextArgs->val.stringv);
		}

		return( 0);
	}
} 

