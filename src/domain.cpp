/**
	ESP Domain.
*/

#include <stdio.h>
#include <math.h>
#include <vector>
#include "DataValue.hpp"

#include <shmht.h>
#include <hiredis/hiredis.h>

#include "domain.h"
#include "hierarchie.h"

///DOXYS_OFF
///DOXYS_ON

using namespace std;

extern "C" {
	/**
	  set Domain.
	 */
	int32_t h_setDomain( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		//Return value
		top->setString( "ALL");

		return( 0);
	}

	/**
	  get Domain.
	 */
	int32_t h_getDomain( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		//Return value
		top->setString( "ALL");

		return( 0);
	}

	/**
	  add Domain.
	 */
	int32_t h_addDomain( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisContext *pContext;
		redisReply *pReply;
		int i = 0;
		const char *pDomain = NULL;
		const char *pName = NULL;
		const char *pId = NULL;

		DataTypes::DataValue *pCurr = (nextArgs != NULL) ? nextArgs : top;

		pDomain = (nextArgs + i++)->val.stringv;
		pName = (nextArgs + i++)->val.stringv;
		pId = top->val.stringv;

		pContext = GetContext();

		/* multi .*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "MULTI");
		freeReplyObject( pReply);                                                                                                           

		/* add domainId.*/                                                                                                                          
		//pReply = (redisReply *) redisCommand( pContext, "HMSET Hierarchie:Location:%s name %s clientLegalId %s", pDomain, pName, pId);
		pReply = (redisReply *) redisCommand( pContext, "SADD Hierarchie:Location:ALL name %s", pName);
		freeReplyObject( pReply);                                                                                                           

		/* add domain set.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "SADD Hierarchie:Domain:%s %s", pName, pId);
		freeReplyObject( pReply);                                                                                                           

		/* exec .*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "EXEC");
		freeReplyObject( pReply);                                                                                                           
		return( 0);
	}
} 

