/**
	ESP limit.
*/

#include <stdio.h>
#include <math.h>
#include <vector>
#include "DataValue.hpp"

#include <hiredis/hiredis.h>

#include "limit.h"
#include "hierarchie.h"

using namespace std;

extern "C" {
	///DOXYS_OFF
	static float getLimit( const char *);
	static void setLimit( const char *, float);
	static void setLimit0( const char *, float);
	static float getLimit0( const char *);
	static float getLimit1( const char *);
	static float getLimit2( const char *);
	///DOXYS_ON

	/**
	  set Limit.
	 */
	int32_t h_setLimit( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisContext *pContext;
		redisReply *pReply;
		int i = 0;
		const char *pId = NULL;
		float fLimit = 0.0;
		float fLimit0 = 0.0;

		DataTypes::DataValue *pCurr = (nextArgs != NULL) ? nextArgs : top;

		pId = (nextArgs + i++)->val.stringv;
		fLimit = (nextArgs + i++)->val.doublev;
		fLimit0 = top->val.doublev;

		pContext = GetContext();

		/* multi.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "MULTI");
		freeReplyObject( pReply);                                                                                                           

		/* set limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "HMSET Hierarchie:Limit:%s limit %g limit0 %g", pId, fLimit, fLimit0);

		//Return value
		top->setInt64( pReply->integer);

		freeReplyObject( pReply);                                                                                                           

		/* exec.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "EXEC");
		freeReplyObject( pReply);                                                                                                           

		return( 0);
	}

	/**
	  set Limit1.
	 */
	int32_t h_setLimit1( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisContext *pContext;
		redisReply *pReply;
		int i = 0;
		const char *pId = NULL;
		float fLimit = 0.0;

		DataTypes::DataValue *pCurr = (nextArgs != NULL) ? nextArgs : top;

		pId = (nextArgs + i++)->val.stringv;
		fLimit = top->val.doublev;

		pContext = GetContext();

		/* multi.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "MULTI");
		freeReplyObject( pReply);                                                                                                           

		/* set limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "HSET Hierarchie:Limit:%s limit1 %g", pId, fLimit);

		//Return value
		top->setInt64( pReply->integer);

		freeReplyObject( pReply);                                                                                                           

		/* exec.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "EXEC");
		freeReplyObject( pReply);                                                                                                           

		return( 0);
	}

	/**
	  set Limit2.
	 */
	int32_t h_setLimit2( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisContext *pContext;
		redisReply *pReply;
		int i = 0;
		const char *pId = NULL;
		float fLimit = 0.0;

		DataTypes::DataValue *pCurr = (nextArgs != NULL) ? nextArgs : top;

		pId = (nextArgs + i++)->val.stringv;
		fLimit = top->val.doublev;

		pContext = GetContext();

		/* multi.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "MULTI");
		freeReplyObject( pReply);                                                                                                           

		/* set limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "HSET Hierarchie:Limit:%s limit2 %g", pId, fLimit);

		//Return value
		top->setInt64( pReply->integer);

		freeReplyObject( pReply);                                                                                                           

		/* exec.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "EXEC");
		freeReplyObject( pReply);                                                                                                           

		return( 0);
	}

	/**
	  get Limit.
	 */
	int32_t h_getLimit( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisContext *pContext;
		redisReply *pReply;

		pContext = GetContext();

		/* get limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "HGET Hierarchie:Limit:%s limit0", top->val.stringv);

		//Return value
		top->setDouble( (pReply->str != NULL) ? atof( pReply->str) : 0.0);

		freeReplyObject( pReply);                                                                                                           

		return( 0);
	}

	/**
		calculate hierarchie.
	*/
	int32_t h_calcHierarchie( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		int nRetCode = 0;
		int nLevel = 0;
		redisContext *pContext;
		redisReply *pReply;
		redisReply *pMember;
		redisReply **ppElement;
		redisReply **ppMember;
		float fLimitParent = 0;
		float fLimit = 0;
		float fLimit0 = -1;
		float fCurrLimit = 0;
		int nPrevScore = -1;
		float fSumParent = 0;

		pContext = GetContext();

		pReply = (redisReply *) redisCommand( pContext, "ZRANGE Hierarchie:Tree %d -1", nLevel);

		/* go top to down.*/
		for( int nElement = 0; nElement < pReply->elements; nElement++) {
			ppElement = pReply->element + nElement;

			/* get top limit.*/                                                                                                                          
			fCurrLimit = getLimit( (*ppElement)->str);
			fLimit0 = getLimit0( (*ppElement)->str);

			/* set current limit.*/
			fLimitParent = (fLimit0 != -1) ? fLimit0 : fCurrLimit;
			setLimit( (*ppElement)->str, fLimitParent);

			/* members.*/
			pMember = (redisReply *) redisCommand( pContext, "SMEMBERS Hierarchie:Set:%s", (*ppElement)->str);

			for( int nMember = 0; nMember < pMember->elements; nMember++) {
				ppMember = pMember->element + nMember;

				/* get child limit.*/                                                                                                                          
				fLimit = getLimit0( (*ppMember)->str);

				/* set parent limit.*/
				if( fLimitParent > 0) {
					if( fLimit == -1) {
						setLimit( (*ppMember)->str, fLimitParent);
					}
					else {
						setLimit( (*ppMember)->str, fLimit);
					}
				}
				else {
					if( fLimit == -1) {
						setLimit( (*ppMember)->str, 0.0);
					}
					else {
						setLimit( (*ppMember)->str, fLimit);
					}
				}
			}

			freeReplyObject( pMember);
		}

		freeReplyObject( pReply);                                                                                                           

		/* go down to top.*/
		pReply = (redisReply *) redisCommand( pContext, "ZREVRANGE Hierarchie:Tree 0 -1");

		for( int nElement = 0; nElement < pReply->elements; nElement++) {
			redisReply *pMember;
			char *pKey = (* (pReply->element + nElement))->str;

			/* get current limit.*/                                                                                                                          
			fCurrLimit = getLimit( pKey);
			fLimit0 = getLimit0( pKey);

			/* find next set.*/
			for( int nMember = nElement + 1; nMember < pReply->elements; nMember++) {
				char *pParentKey = (* (pReply->element + nMember))->str;

				pMember = (redisReply *) redisCommand( pContext, "SISMEMBER Hierarchie:Set:%s %s", pParentKey, pKey);

				if( pMember != NULL) {

					switch( pMember->integer) {
						case 0:
							break;
						case 1:
							fSumParent = getLimit( pParentKey);

							/* add if limit or previous limit.*/
							if( fLimit0 != -1) {
								if( getLimit0( pParentKey) == -1) {
									fSumParent = fLimit0;
								}
 								else {
									fSumParent += fLimit0;
								}
							}
							else {
								if( fSumParent != fCurrLimit) {
									fSumParent += fCurrLimit;
								}
							}

							/* score changed update limit.*/
							setLimit( pParentKey, fSumParent);
							break;
						default:
							break;
					}
				}

				freeReplyObject( pMember);                                                                                                           
			}
		}

		freeReplyObject( pReply);                                                                                                           

		return( nRetCode);
	}

	/**
		set limit.
	*/
	static void setLimit( const char *pVal, float fLimit)
	{
		redisReply *pReply;

		/* set limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( GetContext(), "HSET Hierarchie:Limit:%s limit %f", pVal, fLimit);

		freeReplyObject( pReply);                                                                                                           
	}

	/**
		set limit0.
	*/
	static void setLimit0( const char *pVal, float fLimit)
	{
		redisReply *pReply;

		/* set limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( GetContext(), "HSET Hierarchie:Limit:%s limit0 %f", pVal, fLimit);

		freeReplyObject( pReply);                                                                                                           
	}


	/**
		get limit.
	*/
	static float getLimit( const char *pVal)
	{
		float fLimit = 0.0;
		redisReply *pReply;

		/* get limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( GetContext(), "HGET Hierarchie:Limit:%s limit", pVal);

		//Return value
		fLimit = (pReply->str != NULL) ? atof( pReply->str) : 0.0;

		freeReplyObject( pReply);                                                                                                           

		return( fLimit);
	}

	/**
		get limit0.
	*/
	static float getLimit0( const char *pVal)
	{
		float fLimit = 0.0;
		redisReply *pReply;

		/* get limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( GetContext(), "HGET Hierarchie:Limit:%s limit0", pVal);

		//Return value
		fLimit = (pReply->str != NULL) ? atof( pReply->str) : 0.0;

		freeReplyObject( pReply);                                                                                                           

		return( fLimit);
	}

	/**
		get limit1.
	*/
	static float getLimit1( const char *pVal)
	{
		float fLimit = 0.0;
		redisReply *pReply;

		/* get limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( GetContext(), "HGET Hierarchie:Limit:%s limit1", pVal);

		//Return value
		fLimit = (pReply->str != NULL) ? atof( pReply->str) : 0.0;

		freeReplyObject( pReply);                                                                                                           

		return( fLimit);
	}

	/**
		get limit2.
	*/
	static float getLimit2( const char *pVal)
	{
		float fLimit = 0.0;
		redisReply *pReply;

		/* get limit.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( GetContext(), "HGET Hierarchie:Limit:%s limit2", pVal);

		//Return value
		fLimit = (pReply->str != NULL) ? atof( pReply->str) : 0.0;

		freeReplyObject( pReply);                                                                                                           

		return( fLimit);
	}
} 

