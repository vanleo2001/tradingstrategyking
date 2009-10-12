/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		class	IStStore;
*/

#include	"StdAfx.h"
#include	"../Include/Database.h"
#include	"../Include/SpString.h"
#include	"QianLong.h"
#include	"ShengLong.h"
#include	"SelfDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//	class	CDBType
CDBType::CDBType( )
{
	type = IStStore::dbtypeUnknown;
	memset(name,0,sizeof(name));
}

////////////////////////////////////////////////////////////////////////////////////////////
//	class	IStStore

BOOL IStStore::IsValidDataType( int nType )
{
	return ( dbtypeSelfDB == nType || dbtypeQianlong == nType
		|| dbtypeShenglong == nType );
}

int	IStStore::GetSupportedDataType ( CDBType * pdbtype, int maxsize )
{
	SP_ASSERT( pdbtype );
	if( NULL == pdbtype || maxsize < 5 )
		return -1;

	pdbtype[0].type	=	CSelfDB::GetType();
	strncpy( pdbtype[0].name, CSelfDB::GetName(), min(sizeof(pdbtype[0].name)-1,strlen(CSelfDB::GetName())) );
	pdbtype[1].type	=	CQianlong::GetType();
	strncpy( pdbtype[1].name, CQianlong::GetName(), min(sizeof(pdbtype[1].name)-1,strlen(CQianlong::GetName())) );
//	pdbtype[2].type	=	CShenglong::GetType();
//	strncpy( pdbtype[2].name, CShenglong::GetName(), min(sizeof(pdbtype[2].name)-1,strlen(CShenglong::GetName())) );

	return 2;
}

IStStore * IStStore::CreateStore( const char * rootpath, int nDBType )
{
	IStStore	* pRet	=	NULL;

	// argument test
	if( ! rootpath || strlen(rootpath)<=0 || strlen(rootpath)>1023 || 0 != access(rootpath,0) )
		return NULL;

	// test rootpath type one by one
	char accurateroot[1024];
	memset( accurateroot, 0, sizeof(accurateroot) );
	switch( nDBType )
	{
	case dbtypeUnknown:
		if( CSelfDB::GetAccurateRoot( rootpath, accurateroot, 1024 ) )
			pRet	=	new CSelfDB( accurateroot, TRUE );
		else if( CQianlong::GetAccurateRoot( rootpath, accurateroot, 1024 ) )
			pRet	=	new CQianlong( accurateroot, TRUE );
		else if( CShenglong::GetAccurateRoot( rootpath, accurateroot, 1024 ) )
			pRet	=	new CShenglong( accurateroot, TRUE );
		else
			return NULL;
		break;
	case dbtypeSelfDB:
		if( CSelfDB::GetAccurateRoot( rootpath, accurateroot, 1024 ) )
			pRet	=	new CSelfDB( accurateroot, TRUE );
		break;
	case dbtypeQianlong:
		if( CQianlong::GetAccurateRoot( rootpath, accurateroot, 1024 ) )
			pRet	=	new CQianlong( accurateroot, TRUE );
		break;
	case dbtypeShenglong:
		if( CShenglong::GetAccurateRoot( rootpath, accurateroot, 1024 ) )
			pRet	=	new CShenglong( accurateroot, TRUE );
		break;
	default:
		return NULL;
	}
	
	return pRet;
}

const char * IStStore::GetRootPath( )
{
	return m_szRootPath;
}

