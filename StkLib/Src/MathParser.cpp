//unit Parser;

/*==========================================================================*/
/* Expression Evaluator v1.4 for Delphi                                     */
/* (16 & 32 bits)                                                           */
/*                                                                          */
/* Copyright © 1997 by BitSoft Development, L.L.C.                          */
/* All rights reserved                                                      */
/*                                                                          */
/* Web:     http://www.bitsoft.com                                          */
/* E-mail:  info@bitsoft.com                                                */
/* Support: tech-support@bitsoft.com                                        */
/*--------------------------------------------------------------------------*/
/* Portions Copyright © 1992 by Borland International, Inc.                 */
/* All rights reserved                                                      */
/*--------------------------------------------------------------------------*/
/* This file is distributed as freeware and without warranties of any kind. */
/* You can use it in your own applications at your own risk.                */
/* See the License Agreement for more information.                          */
/*==========================================================================*/

/*===========PASCAL => C++*       WUZHIQIANG 1999/2/8 ======================*/


#include "stdafx.h"
#include "../Include/MathParser.h"
#include <math.h>

CMathParser::CMathParser( )
{
	Clear( );
}

CMathParser::CMathParser(LPCTSTR s, int maxVarCount )
{
	SetParserString( s );
	InitVarMap( maxVarCount );
}

CMathParser::~CMathParser()
{
	Clear( );
	ClearVarMap( );
}

void CMathParser::Clear( )
{
	StackTop	=	-1;
	TokenLen	=	0;
	Position	=	0;//1
	
	ErrorCode	=	MP_ErrSuccess;
	ParseValue	=	0;
}

void CMathParser::ClearVarMap( )
{
	SPPOSITION	pos	=	m_VarMap.GetStartPosition();
	while( pos )
	{
		CSPString	rKey;
		void	*	ptr;
		m_VarMap.GetNextAssoc( pos, rKey, ptr );
		if( ptr )
			delete	(VARSTRU *)ptr;
	}
	m_VarMap.RemoveAll();
}

void CMathParser::SetParserString( LPCTSTR s )
{
	Clear( );
	FInput	=	s;
}

void CMathParser::InitVarMap( int maxVarCount )
{
	ClearVarMap( );
	m_VarMap.InitHashTable( maxVarCount + maxVarCount + 10, FALSE );
}

BOOL CMathParser::IsError( )
{
	return ErrorCode != MP_ErrSuccess;
}

int CMathParser::GetErrorCode( )
{
	return ErrorCode;
}

MPExtended CMathParser::GetResult( )
{
	if( IsError() )
		return 0;
	return ParseValue;
}

WORD CMathParser::GotoState(WORD Production)/* Finds the new state based on the just-completed production and the top state. */
{
	WORD  State;
	WORD  ResultGotoState;	

	State = Stack[StackTop].State;

	if (Production <= 3) 
	{
		switch(State)
		{
		case 0 : ResultGotoState = 1;
				break;	
		case 9 : ResultGotoState = 19;
				break;
		case 20 : ResultGotoState = 28;
				break;
		}
	}
	else if(Production <= 6)
	{
		switch(State)
		{
		case 0:
		case 9:
		case 20: 
			ResultGotoState = 2;
			break;
		case 12 : 
			ResultGotoState = 21;
			break;
		case 13 : 
			ResultGotoState = 22;
			break;
		}
	}
	else if( (Production <= 8) || (Production == 100))
	{ //begin
		switch(State)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 20:
			ResultGotoState = 3;
			break;
		case 14 : 
			ResultGotoState = 23;
			break;
		case 15 : 
			ResultGotoState = 24;
			break;
		case 16 : 
			ResultGotoState = 25;
			break;
		case 40 : 
			ResultGotoState = 80;
			break;
		}
	}
	else if (Production <= 10)
	{//  begin
		switch(State)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20:
		case 40:
			ResultGotoState = 4;
		}
	}
	else if( Production <= 12 )
	{
		switch(State)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20:
		case 40:
		ResultGotoState = 6;
		break;
		case 5:
		ResultGotoState = 17;
		}
	}
	else 
	{
		switch(State)
		{
		case 0:
		case 5:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20:
		case 40:
		  ResultGotoState = 8;
		}
	}

	return ResultGotoState;
} 

BOOL CMathParser::IsFunc(CSPString S)//{ Checks to see if the parser is about to read a function }
{
	WORD P, SLen;
	CSPString FuncName;
	BOOL bIsFunc;

	P = Position;
	FuncName = "";

	while (P < FInput.GetLength())// && (FInput[P] in ['A'..'Z', 'a'..'z', '0'..'9', '_']) 
	{
		if(!((FInput.GetAt(P) <= 'Z' && FInput.GetAt(P) >= 'A')
		  ||(FInput.GetAt(P) <= 'z' && FInput.GetAt(P) >= 'a')
		  ||(FInput.GetAt(P) <= '9' && FInput.GetAt(P) >= '0')
		  ||(FInput.GetAt(P) == '_')))
		  break;

		//FInput.SetAt(P, FInput.GetAt(P));
		FuncName = FuncName + FInput.GetAt(P);//[P];
		P++;//Inc(P);
	}//  end; { while }

	FuncName.MakeUpper();
	if (FuncName == S )//Uppercase(FuncName) = S
	{
		SLen = S.GetLength();
		CurrToken.FuncName = FInput.Mid(Position, SLen);
		CurrToken.FuncName.MakeUpper();
		Position += SLen;
		//Inc(Position, SLen);
		bIsFunc = TRUE;
	}
	else
		bIsFunc = FALSE;

	return bIsFunc;
}

BOOL CMathParser::IsVar(MPExtended &Value)
{
	CSPString VarName;
	BOOL VarFound;

	VarFound = FALSE;
	VarName = "";

	while ((Position < FInput.GetLength()) && (
		(FInput.GetAt(Position) >= 'A' && FInput.GetAt(Position) <= 'Z')
		||(FInput.GetAt(Position) >= 'a' && FInput.GetAt(Position) <= 'z')
		||(FInput.GetAt(Position) >= '0' && FInput.GetAt(Position) <= '9')
		||(FInput.GetAt(Position) == '_')
		))//	  in ['A'..'Z',  'a'..'z', '0'..'9', '_']) do
	{
		VarName = VarName + FInput.GetAt(Position);
		Position++;
	}

	if( GetVar(VarName, Value) == TRUE)
		return TRUE;

	return FALSE;
}

TokenTypes CMathParser::NextToken()//{ Gets the next Token from the Input stream }
{
	CSPString NumString;// : String[80];
	WORD TLen, NumLen; //, FormLen, Place;
	// int Check ;//: Integer;
	char Ch;//, FirstChar;
	BOOL Decimal ;
	TokenTypes ResultNextToken;

	while ((Position < FInput.GetLength()) && (FInput.GetAt(Position) == ' '))
		Position++;
	TokenLen = Position;
	if (Position >= FInput.GetLength())
	{
		ResultNextToken = EOL;
		TokenLen = 0;
		return ResultNextToken;
	}

	FInput.MakeUpper();

	Ch = FInput.GetAt(Position);
	if (Ch== '!')
	{
		ResultNextToken = ERR;
		TokenLen = 0;
		return ResultNextToken ;
	}

	if((Ch >= '0' && Ch <= '9') || Ch == '.')//   if Ch in ['0'..'9', '.'] then
	{
		NumString = "";
		TLen = Position;
		Decimal = FALSE;

		while ((TLen < FInput.GetLength()) &&
			((FInput.GetAt(TLen) >= '0' && FInput.GetAt(TLen) <= '9' ) ||
			((FInput.GetAt(TLen) == '.') && (!Decimal)))) 
		{
			NumString = NumString + FInput.GetAt(TLen);
			if (Ch == '.')// then
				Decimal = TRUE;
			TLen++;//Inc(TLen);
		}

		if ((TLen == 2) && (Ch == '.'))// then
		{
			ResultNextToken = BAD;
			TokenLen = 0;
			return ResultNextToken ;
		}

		if ((TLen < FInput.GetLength()) && ((FInput.GetAt(TLen)) == 'E'))// then
		{
			NumString = NumString + 'E';
			TLen++;
   
			if( FInput.GetAt(TLen) == '+' || FInput.GetAt(TLen) == '-')// in ['+', '-'] then
			{
				NumString.SetAt(TLen, FInput.GetAt(TLen));//= NumString + FInput[TLen];
				TLen++;
			}

			NumLen = 1;
			while ((TLen < FInput.GetLength()) && (FInput.GetAt(TLen) >= '0' && FInput.GetAt(TLen) <= '9') &&
				 (NumLen <= MP_MaxExpLen)) 
			{
				NumString = NumString + FInput.GetAt(TLen);
				NumLen++;//Inc(NumLen);
				TLen++;//Inc(TLen);
			}
		}

		if (NumString[0] == '.')// then
			NumString = '0' + NumString;

		CurrToken.Value = atof(NumString);
		//Val(NumString, CurrToken.Value, Check);

		/*//if (Check != 0 )
		{//    begin
			ErrorCode = MP_ErrMath;
			ErrorCode = ErrInvalidNum;
			Position += Pred(Check);
			//Inc(Position, Pred(Check));
		}//  end { if }
		else*/
		{//  begin
			ResultNextToken = NUM;
			Position += NumString.GetLength();
			//Inc(Position, System.Length(NumString));
			TokenLen = Position - TokenLen;
		}// end; { else }
		return ResultNextToken;
	}//end { if }
	else if ((Ch>='a' && Ch <= 'z')
	   || (Ch>='A' && Ch <= 'Z'))//in Letters then
	{//   begin
		if (IsFunc("ABS") ||
			IsFunc("ATAN") ||
			IsFunc("COS") ||
			IsFunc("EXP") ||
			IsFunc("LN") ||
			IsFunc("ROUND") ||
			IsFunc("SIN") ||
			IsFunc("SQRT") ||
			IsFunc("SQR") ||
			IsFunc("TRUNC")||
			IsFunc("NOT")||// then//EXPAND
			IsFunc("BOOL")||
			IsFunc("SGN")
			)
		{
			ResultNextToken = FUNC;
			TokenLen = Position - TokenLen;
			return ResultNextToken ;
		}

		if (IsFunc("MOD")) 
		{
			ResultNextToken = MODU;
			TokenLen = Position - TokenLen;
			return ResultNextToken ;
		}
		if (IsVar(CurrToken.Value))
		{
			ResultNextToken = NUM;
			TokenLen = Position - TokenLen;
			return ResultNextToken;
		}
		else 
		{
			ResultNextToken = BAD;
			TokenLen = 0;
			return ResultNextToken ;
		}
	}
	else 
	{//   begin
		switch(Ch)
		{
		case  '+' : ResultNextToken = PLUS; break;
		case  '-' : ResultNextToken = MINUS; break;
		case  '*' : ResultNextToken = TIMES; break;
		case  '/' : ResultNextToken = DIVIDE; break;
		case  '^' : ResultNextToken = EXPO; break;
		case  '(' : ResultNextToken = OPAREN; break;
		case  ')' : ResultNextToken = CPAREN; break;
		default:
		 ResultNextToken = BAD;
		 TokenLen = 0;
		 return ResultNextToken ;
	}//end; { case }
	Position++;
	TokenLen = Position - TokenLen;
	return ResultNextToken ;
	}//end; { else if }
}//end; { NextToken }

void CMathParser::Pop(TokenRec &Token)//{ Pops the top Token off of the stack }
{
	Token = Stack[StackTop];
	StackTop--;
}

void CMathParser::Push(TokenRec Token)//{ Pushes a new Token onto the stack }
{
	if (StackTop == MP_ParserStackSize-1)// then
		ErrorCode = MP_ErrParserStack;
	else 
	{
		StackTop++; 
		Stack[StackTop] = Token;
	}
}

void CMathParser::Parse()//{ Parses an input stream }
{
	TokenRec FirstToken ;
	BOOL Accepted;

	Position = 0;//1
	StackTop = -1;
	ErrorCode = 0;
	Accepted = FALSE;
	FirstToken.State = 0;
	FirstToken.Value = 0;

	Push(FirstToken);
	TokenType = NextToken();

	do{//repeat
		switch(Stack[StackTop].State)
		{
		case 0:
		case 9:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 20:
		case 40 : 
		{//begin
		if (TokenType == NUM)
		  Shift(10);
		else if (TokenType == FUNC)
		  Shift(11);
		else if (TokenType == MINUS)
		  Shift(5);
		else if (TokenType == OPAREN)
		  Shift(9);
		else if (TokenType == ERR)
		{//    begin
			 ErrorCode = MP_ErrMath;
			 Accepted = TRUE;
		}// end { else if }
		else 
		{//begin
		  ErrorCode = MP_ErrExpression;
		  Position -= TokenLen;
		  //Dec(Position, TokenLen);
		}//end; { else }
		break;
		}//end; { case of }
		case 1 : 
		{//begin
		if (TokenType == EOL)
		  Accepted = TRUE;
		else if (TokenType == PLUS)
		  Shift(12);
		else if (TokenType == MINUS)
		  Shift(13);
		else 
		{//begin
		  ErrorCode = MP_ErrOperator;
		  Position -= TokenLen;
		  //Dec(Position, TokenLen);
		}//end { else }
		break;
		}//end; { case of }
		case 2 : 
		{//begin
		if (TokenType == TIMES)
		  Shift(14);
		else if (TokenType == DIVIDE)
		  Shift(15);
		else
		  Reduce(3);
		break;
		}//end; { case of }
		case 3 :
		{
		if( TokenType == MODU)
		 Shift(40);
		else
		 Reduce(6);
		break;
		}//end; { case of }
		case 4 : 
		{//begin
		if (TokenType == EXPO)
		 Shift(16);
		else
		 Reduce(8);
		break;
		}//end; { case of }
		case 5 : 
		{//begin
		if (TokenType == NUM)
		  Shift(10);
		else if (TokenType = FUNC )
		  Shift(11);
		else if (TokenType = OPAREN)
		  Shift(9);
		else
		{
			ErrorCode = MP_ErrExpression;
			Position -= TokenLen;
			//Dec(Position, TokenLen);
		 }// end; { else }
		break;
		}//  end; { case of }
		case 6 : Reduce(10); break;
		case 7 : Reduce(13); break;
		case 8 : Reduce(12); break;
		case 10 : Reduce(15); break;
		case 11 : 
		{//begin
		if (TokenType == OPAREN)
		  Shift(20);
		else
		{//          begin
			ErrorCode = MP_ErrOpenParen;
			Position -= TokenLen;
			//Dec(Position, TokenLen);
		 }//end; { else }
		break;
		}//end; { case of }
		case 17 : Reduce(9); break;
		case  18 : break;//raise Exception.Create('Bad token state'); break;
		case  19 : 
		{//begin
		if (TokenType == PLUS)
		  Shift(12);
		else if (TokenType == MINUS)
		  Shift(13);
		else if (TokenType == CPAREN)
		  Shift(27);
		else
		{//begin
			ErrorCode = MP_ErrOpCloseParen;
			Position -= TokenLen;
			//Dec(Position, TokenLen);
		}// end;
		break;
		}//end; { case of }
		case 21 : 
		{// begin
		if (TokenType == TIMES)
		  Shift(14);
		else if (TokenType == DIVIDE)
		  Shift(15);
		else
		  Reduce(1);
		break;
		}//end; { case of }
		case 22 : //begi
		{
		if (TokenType == TIMES)
		  Shift(14);
		else if (TokenType == DIVIDE)
		  Shift(15);
		else
		  Reduce(2);
		break;
		}//end; { case of }
		case 23 : Reduce(4); break;
		case 24 : Reduce(5); break;
		case 25 : Reduce(7); break;
		case 26 : Reduce(11); break;
		case 27 : Reduce(14); break;
		case 28 : 
		{
		if (TokenType == PLUS)
		  Shift(12);
		else if (TokenType == MINUS)
		  Shift(13);
		else if (TokenType == CPAREN)
		  Shift(29);
		else
		{//  begin
			ErrorCode = MP_ErrOpCloseParen;
			Position -= TokenLen;
			//Dec(Position, TokenLen);
		 }// end; { else }
		break;
		}//end; { case of }
		case 29 : Reduce(16); break;
		case 80 : Reduce(100); break;
		}
	}while( !Accepted && (ErrorCode == 0));//end; { case }

	if (ErrorCode != 0 )
	{//  begin
		if (ErrorCode == MP_ErrBadRange )
			Position -= TokenLen;
		//Dec(Position, TokenLen);
		//if( Assigned(FOnParseError))
			//FOnParseError(Self, ErrorCode);
	}//end; { if }
	if (ErrorCode != MP_ErrSuccess )// then
	{//  begin
		ParseValue = 0;
		return;
	}//end; { if }
	ParseValue = Stack[StackTop].Value;
}//end; { Parse }

void CMathParser::Reduce(WORD  Reduction)//{ Completes a reduction }
{
	TokenRec Token1, Token2;

	switch(Reduction)
	{
	case 1 : 
	  {
	  Pop(Token1);
	  Pop(Token2);
	  Pop(Token2);
	  CurrToken.Value = Token1.Value + Token2.Value;
	  break;
	  }//  end;
	case 2 : {//begin
	  Pop(Token1);
	  Pop(Token2);
	  Pop(Token2);
	  CurrToken.Value = Token2.Value - Token1.Value;
	  break;
	}//    end;
	case 4 :// begin
	{
	  Pop(Token1);
	  Pop(Token2);
	  Pop(Token2);
	  CurrToken.Value = Token1.Value * Token2.Value;
	  break;
	}//    end;
	case 5 : //begin
	{
	  Pop(Token1);
	  Pop(Token2);
	  Pop(Token2);
	  if (fabs(Token1.Value) < 0.000001 )
		ErrorCode = MP_ErrMath;
	  else
		CurrToken.Value = Token2.Value / Token1.Value;
	  break;
	}//    end;

	//{ MOD operator }
	case 100 : //begin
	{
	  Pop(Token1);
	  Pop(Token2);
	  Pop(Token2);
	  if (fabs(Token1.Value) < 0.000001)
		ErrorCode = MP_ErrMath;
	  else
		CurrToken.Value = Round(Token2.Value) % Round(Token1.Value);
	  break;
	}//end;

	case 7 :
	{
	  Pop(Token1);
	  Pop(Token2);
	  Pop(Token2);
	  if (Token2.Value < 0.0)
		ErrorCode = MP_ErrMath;
	  else if ((Token1.Value * log(Token2.Value) < -MP_ExpLimit) ||
			  (Token1.Value * log(Token2.Value) > MP_ExpLimit)) 
		ErrorCode = MP_ErrMath;
	  else
		CurrToken.Value = exp(Token1.Value * log(Token2.Value));
	  break;
	}//end;
	case 9 : 
	{//begin
	  Pop(Token1);
	  Pop(Token2);
	  CurrToken.Value = -Token1.Value;
	  break;
	}//end;
	case 11 : break;//raise Exception.Create('Invalid reduction'); break;
	case 13 : break;//raise Exception.Create('Invalid reduction'); break;
	case 14 : 
	{//begin
	  Pop(Token1);
	  Pop(CurrToken);
	  Pop(Token1);
	  break;
	}//end;

	case 16 : 
	{//begin
		Pop(Token1);
		Pop(CurrToken);
		Pop(Token1);
		Pop(Token1);
		if( Token1.FuncName == "ABS")
		CurrToken.Value = fabs(CurrToken.Value);
		else if (Token1.FuncName == "ATAN")
		CurrToken.Value = atan(CurrToken.Value);
		else if (Token1.FuncName == "COS")
		{//      begin
		 if ((CurrToken.Value < -9E18) || (CurrToken.Value > 9E18))
			ErrorCode = MP_ErrMath;
		 else
			CurrToken.Value = cos(CurrToken.Value);
		}//end {...if Token1.FuncName = 'SIN' }
		else if (Token1.FuncName == "EXP")
		{//      begin
		if ((CurrToken.Value < -MP_ExpLimit) || (CurrToken.Value > MP_ExpLimit))// then
		  ErrorCode = MP_ErrMath;
		else
		  CurrToken.Value = exp(CurrToken.Value);
		}//end
		else if (Token1.FuncName == "LN")// then
		{//      begin
		if (CurrToken.Value <= 0)
		  ErrorCode = MP_ErrMath;
		else
		  CurrToken.Value = log(CurrToken.Value);
		}//end
		else if (Token1.FuncName == "ROUND")
		{//     begin
		if ((CurrToken.Value < -1E9) || (CurrToken.Value > 1E9))
		  ErrorCode = MP_ErrMath;
		else
		  CurrToken.Value = Round(CurrToken.Value);
		}//end
		else if (Token1.FuncName == "SIN")
		{//      begin
		 if( (CurrToken.Value < -9E18) || (CurrToken.Value > 9E18))
			ErrorCode = MP_ErrMath;
		 else
			CurrToken.Value = sin(CurrToken.Value);
		 }//end {...if Token1.FuncName = 'SIN' }
		else if (Token1.FuncName == "SQRT")
		{//begin
		if( CurrToken.Value < 0 )//then
		  ErrorCode = MP_ErrMath;
		else
		  CurrToken.Value = sqrt(CurrToken.Value);
		}//end
		else if (Token1.FuncName == "SQR")
		{//begin
		if ((CurrToken.Value < -MP_SQRLIMIT) || (CurrToken.Value > MP_SQRLIMIT))// then
		  ErrorCode = MP_ErrMath;
		else
		  CurrToken.Value = sqrt(CurrToken.Value);
		}//end
		else if (Token1.FuncName == "TRUNC")
		{//begin
		if ((CurrToken.Value < -1E9) || (CurrToken.Value > 1E9))
		  ErrorCode = MP_ErrMath;
		else
		  CurrToken.Value = (int)(CurrToken.Value);//Trunc
		}
		else if(Token1.FuncName == "NOT")//end;//EXPAND
		{
		if (CurrToken.Value < 0.001 && CurrToken.Value > -0.001)
			CurrToken.Value = 1.0;
		else 
			CurrToken.Value = 0.0;
		}
		else if(Token1.FuncName == "BOOL")//end;//EXPAND
		{
		if (CurrToken.Value < 0.001 && CurrToken.Value > -0.001)
			CurrToken.Value = 0.0;
		else 
			CurrToken.Value = 1.0;
		}
		else if(Token1.FuncName == "SGN")//end;//EXPAND
		{
		if (CurrToken.Value > 0.0)
			CurrToken.Value = 1.0;
		else 
			CurrToken.Value = 0;
		}
		break;
	}//end;
	case 3:
	case 6:
	case 8:
	case 10:
	case 12:
	case 15 : Pop(CurrToken); break;
	}//  end; { case }

	CurrToken.State = GotoState(Reduction);
	Push(CurrToken);
}

void CMathParser::Shift(WORD State)//{ Shifts a Token onto the stack }
{
	CurrToken.State = State;
	Push(CurrToken);
	TokenType = NextToken();
}


int CMathParser::Round(double value)
{
	int result1, result2;
	if(value > 0.0)
	{
		result1 = (int)value;
		result2 = result1 + 1;
		if( value - (double)result1 > (double)result2 - value)
			return result2;
		else result1;
	}
	else
	{
		result1 = (int)value;
		result2 = result1 - 1;
		if( value - (double)result2 > (double)result1 - value)
			return result1;
		else result2;
	}
	return 0;
}

void CMathParser::AddVar(MPExtended Value, LPCTSTR lpszVarName)
{
	if( NULL == lpszVarName || strlen(lpszVarName) == 0 )
		return;

	VARSTRU *pvar = new VARSTRU;
	CSPString VarName	=	lpszVarName;
	VarName.MakeUpper();

	pvar->Value = Value;
	strncpy( pvar->VarName, VarName.GetBuffer(MP_MaxVarNameLen-1), MP_MaxVarNameLen-1 );
	m_VarMap.SetAt( VarName, pvar );
}

BOOL CMathParser::GetVar( LPCTSTR lpszVarName, MPExtended &Value)
{
	if( m_VarMap.IsEmpty() || NULL == lpszVarName || strlen(lpszVarName) == 0 )
		return FALSE;

	CSPString	VarName	=	lpszVarName;
	VarName.MakeUpper();
	void	*	pvar	=	NULL;
	if( m_VarMap.Lookup( VarName, pvar ) )
	{
		Value = ((VARSTRU *)pvar)->Value;
		return TRUE;
	}

	return FALSE;
}
