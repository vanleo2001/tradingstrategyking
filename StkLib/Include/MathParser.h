#ifndef __STKLIB_MATHPARSER_H__
#define __STKLIB_MATHPARSER_H__
//#include "psms.h"

#define  MP_ParserStackSize	100
#define  MP_MaxVarNameLen	64
#define  MP_MaxFuncNameLen	5
#define  MP_ExpLimit		11356
#define  MP_SQRLIMIT		1E246
#define  MP_MaxExpLen		4
#define  MP_TotalErrors		8
#define  MP_ErrSuccess		0
#define  MP_ErrParserStack	1
#define  MP_ErrBadRange		2
#define  MP_ErrExpression	3
#define  MP_ErrOperator		4
#define  MP_ErrOpenParen	5
#define  MP_ErrOpCloseParen	6
#define  MP_ErrInvalidNum	7
#define  MP_ErrMath			8

//type  MP_ErrorRange = 0..MP_TotalErrors;

typedef enum 
{
	PLUS, MINUS, TIMES, DIVIDE, EXPO, OPAREN, CPAREN, NUM,
	FUNC, EOL, BAD, ERR, MODU
}TokenTypes;

typedef double MPExtended;

typedef struct STKLIB_API token_rec_t
{
	WORD State;//BYTE State;
	MPExtended Value;
	CSPString FuncName;//[MaxFuncNameLen];
}TokenRec;

typedef struct{
	MPExtended Value;	
	char VarName[MP_MaxVarNameLen];
}VARSTRU;

/***
	表达式分析类，可以有变量
*/
class STKLIB_API CMathParser
{
public:
	CMathParser( );
	CMathParser( LPCTSTR s, int maxVarCount = 10 );
	~CMathParser();
	void Clear( );
	void ClearVarMap( );
	void SetParserString( LPCTSTR s );
	void InitVarMap( int maxVarCount = 10 );
	void AddVar(MPExtended Value, LPCTSTR lpszVarName);

protected:
	CSPString	FInput;
	WORD		Position;

	TokenRec	CurrToken;
	TokenRec	Stack[MP_ParserStackSize];
	int		StackTop;// : 0..MP_ParserStackSize;
	WORD	TokenLen;// : Word;
	TokenTypes	TokenType;

	CSPMapStringToPtr m_VarMap;

	WORD GotoState(WORD Production);
	BOOL IsFunc(CSPString S);
	BOOL IsVar(MPExtended &Value);

	TokenTypes NextToken();
	void Push(TokenRec Token);
	void Pop(TokenRec &Token);
	void Reduce(WORD Reduction);
	void Shift(WORD State);
	int Round(double value);

	BOOL GetVar( LPCTSTR lpszVarName, MPExtended &Value);

	// result values
	int		ErrorCode;
	MPExtended ParseValue;

public://    { Public declarations }
	void	Parse();
	BOOL	IsError( );
	int		GetErrorCode( );
	MPExtended GetResult( );
};

#endif
