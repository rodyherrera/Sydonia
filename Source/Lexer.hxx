/***
 * Copyright (C) Rodolfo Herrera Hernandez. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root 
 * for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * For related information - https://github.com/CodeWithRodi/Sydonia/
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

#ifndef SYDONIA_LEXER_HXX
#define SYDONIA_LEXER_HXX

#include <cctype>
#include <locale>

#include "Configuration.hxx"
#include "Token.hxx"
#include "Utilities.hxx"

namespace Sydonia{
  // ! Class for lexing an inja Template.
  class Lexer{
    enum class State{
      Text,
      ExpressionStart,
      ExpressionStartForceLstrip,
      ExpressionBody,
      LineStart,
      LineBody,
      StatementStart,
      StatementStartNoLstrip,
      StatementStartForceLstrip,
      StatementBody,
      CommentStart,
      CommentStartForceLstrip,
      CommentBody,
    };

    enum class MinusState{
      Operator,
      Number,
    };

    const LexerConfiguration& LexerConfigurationInstance;

    State StateInstance;
    MinusState MinusStateInstance;
    std::string_view M_IN;
    size_t TokenStart;
    size_t Position;

    Token ScanBody(std::string_view Close, Token::Kind CloseKind, std::string_view CloseTrim = std::string_view(), bool Trim = false){
    Again:
      // ! Skip whitespace (except for \n as it might be a close)
      if(TokenStart >= M_IN.size())
        return MakeToken(Token::Kind::Eof);
      const char Character = M_IN[TokenStart];
      if(Character == ' ' || Character == '\t' || Character == '\r') {
        TokenStart += 1;
        goto Again;
      }

      // ! Check for close
      if(!CloseTrim.empty() && StringView::StartsWith(M_IN.substr(TokenStart), CloseTrim)){
        StateInstance = State::Text;
        Position = TokenStart + CloseTrim.size();
        const Token TokenInstance = MakeToken(CloseKind);
        SkipWhiteSpacesAndNewLines();
        return TokenInstance;
      }

      if(StringView::StartsWith(M_IN.substr(TokenStart), Close)){
        StateInstance = State::Text;
        Position = TokenStart + Close.size();
        const Token TokenInstance = MakeToken(CloseKind);
        if(Trim)
          SkipWhiteSpacesAndFirstNewLine();
        return TokenInstance;
      }

      // ! Skip \n
      if(Character == '\n'){
        TokenStart += 1;
        goto Again;
      }

      Position = TokenStart + 1;
      if(std::isalpha(Character)){
        MinusStateInstance = MinusState::Operator;
        return ScanID();
      }

      const MinusState CurrentMinusState = MinusStateInstance;
      if(MinusStateInstance == MinusState::Operator)
        MinusStateInstance = MinusState::Number;

      switch(Character){
        case '+':
          return MakeToken(Token::Kind::Plus);
        case '-':
          if(CurrentMinusState == MinusState::Operator)
            return MakeToken(Token::Kind::Minus);
          return ScanNumber();
        case '*':
          return MakeToken(Token::Kind::Times);
        case '/':
          return MakeToken(Token::Kind::Slash);
        case '^':
          return MakeToken(Token::Kind::Power);
        case '%':
          return MakeToken(Token::Kind::Percent);
        case '.':
          return MakeToken(Token::Kind::Dot);
        case ',':
          return MakeToken(Token::Kind::Comma);
        case ':':
          return MakeToken(Token::Kind::Colon);
        case '(':
          return MakeToken(Token::Kind::LeftParen);
        case ')':
          MinusStateInstance = MinusState::Operator;
          return MakeToken(Token::Kind::RightParen);
        case '[':
          return MakeToken(Token::Kind::LeftBracket);
        case ']':
          MinusStateInstance = MinusState::Operator;
          return MakeToken(Token::Kind::RightBracket);
        case '{':
          return MakeToken(Token::Kind::LeftBrace);
        case '}':
          MinusStateInstance = MinusState::Operator;
          return MakeToken(Token::Kind::RightBrace);
        case '>':
          if(Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::GreaterEqual);
          }
          return MakeToken(Token::Kind::GreaterThan);
        case '<':
          if (Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::LessEqual);
          }
          return MakeToken(Token::Kind::LessThan);
        case '=':
          if(Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::Equal);
          }
          return MakeToken(Token::Kind::Unknown);
        case '!':
          if(Position < M_IN.size() && M_IN[Position] == '='){
            Position += 1;
            return MakeToken(Token::Kind::NotEqual);
          }
          return MakeToken(Token::Kind::Unknown);
        case '\"':
          return ScanString();
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          MinusStateInstance = MinusState::Operator;
          return ScanNumber();
        case '_':
        case '@':
        case '$':
          MinusStateInstance = MinusState::Operator;
          return ScanID();
        default:
          return MakeToken(Token::Kind::Unknown);
      }
    }

    Token ScanID(){
      for(;;){
        if(Position >= M_IN.size())
          break;
        const char Character = M_IN[Position];
        if(!std::isalnum(Character) && Character != '.' && Character != '/' && Character != '_' && Character != '-')
          break;
        Position += 1;
      }
      return MakeToken(Token::Kind::Id);
    }

    Token ScanNumber(){
      for(;;){
        if(Position >= M_IN.size())
          break;
        const char Character = M_IN[Position];
        // ! Be very permissive in lexer (we'll catch errors when conversion happens)
        if(!(std::isdigit(Character) || Character == '.' || Character == 'e' || Character == 'E' || (Character == '+' && (Position == 0 || M_IN[Position - 1] == 'e' || M_IN[Position - 1] == 'E')) || (Character == '-' && (Position == 0 || M_IN[Position - 1] == 'e' || M_IN[Position - 1] == 'E'))))
          break;
        Position += 1;
      }
      return MakeToken(Token::Kind::Number);
    }

    Token ScanString(){
      bool Escape {false};
      for(;;){
        if(Position >= M_IN.size())
          break;
        const char Character = M_IN[Position++];
        if(Character == '\\')
          Escape = true;
        else if(!Escape && Character == M_IN[TokenStart])
          break;
        else
          Escape = false;
      }
      return MakeToken(Token::Kind::String);
    }

    Token MakeToken(Token::Kind KindLocal) const{
      return Token(KindLocal, StringView::Slice(M_IN, TokenStart, Position));
    }

    void SkipWhiteSpacesAndNewLines(){
      if(Position < M_IN.size())
        while(Position < M_IN.size() && (M_IN[Position] == ' ' || M_IN[Position] == '\t' || M_IN[Position] == '\n' || M_IN[Position] == '\r'))
          Position += 1;
    }

    void SkipWhiteSpacesAndFirstNewLine() {
      if(Position < M_IN.size())
        while(Position < M_IN.size() && (M_IN[Position] == ' ' || M_IN[Position] == '\t'))
          Position += 1;

      if(Position < M_IN.size()){
        const char Character = M_IN[Position];
        if(Character == '\n')
          Position += 1;
        else if(Character == '\r'){
          Position += 1;
          if(Position < M_IN.size() && M_IN[Position] == '\n')
            Position += 1;
        }
      }
    }

    static std::string_view ClearFinalLineIfWhiteSpace(std::string_view Text){
      std::string_view Result = Text;
      while(!Result.empty()){
        const char Character = Result.back();
        if (Character == ' ' || Character == '\t')
          Result.remove_suffix(1);
        else if (Character == '\n' || Character == '\r')
          break;
        else
          return Text;
      }
      return Result;
    }

  public:
    explicit Lexer(const LexerConfiguration& LexerConfigurationLocal)
      : LexerConfigurationInstance(LexerConfigurationLocal), StateInstance(State::Text), 
        MinusStateInstance(MinusState::Number){}

    SourceLocation CurrentPosition() const{
      return GetSourceLocation(M_IN, TokenStart);
    }

    void Start(std::string_view Input){
      M_IN = Input;
      TokenStart = 0;
      Position = 0;
      StateInstance = State::Text;
      MinusStateInstance = MinusState::Number;

      // ! Consume byte order mark (BOM) for UTF-8
      if(StringView::StartsWith(M_IN, "\xEF\xBB\xBF"))
        M_IN = M_IN.substr(3);
    }

    Token Scan(){
      TokenStart = Position;

    Again:
      if(TokenStart >= M_IN.size())
        return MakeToken(Token::Kind::Eof);

      switch(StateInstance){
        default:
        case State::Text: {
          // ! Fast-scan to first open character
          const size_t OpenStart = M_IN.substr(Position).find_first_of(LexerConfigurationInstance.OpenCharacters);
          if(OpenStart == std::string_view::npos){
            // ! Didn't find open, return remaining text as text token
            Position = M_IN.size();
            return MakeToken(Token::Kind::Text);
          }
          Position += OpenStart;

          // ! Try to match one of the opening sequences, and get the close
          std::string_view OpenString = M_IN.substr(Position);
          bool MustLstrip = false;
          if(StringView::StartsWith(OpenString, LexerConfigurationInstance.ExpressionOpen)){
            if(StringView::StartsWith(OpenString, LexerConfigurationInstance.ExpressionOpenForceLstrip)){
              StateInstance = State::ExpressionStartForceLstrip;
              MustLstrip = true;
            }else
              StateInstance = State::ExpressionStart;
          }else if(StringView::StartsWith(OpenString, LexerConfigurationInstance.StatementOpen)){
            if(StringView::StartsWith(OpenString, LexerConfigurationInstance.StatementOpenNoLstrip))
              StateInstance = State::StatementStartNoLstrip;
            else if(StringView::StartsWith(OpenString, LexerConfigurationInstance.StatementOpenForceLstrip)){
              StateInstance = State::StatementStartForceLstrip;
              MustLstrip = true;
            }else{
              StateInstance = State::StatementStart;
              MustLstrip = LexerConfigurationInstance.LstripBlocks;
            }
          }else if(StringView::StartsWith(OpenString, LexerConfigurationInstance.CommentOpen)){
            if(StringView::StartsWith(OpenString, LexerConfigurationInstance.CommentOpenForceLstrip)){
              StateInstance = State::CommentStartForceLstrip;
              MustLstrip = true;
            }else{
              StateInstance = State::CommentStart;
              MustLstrip = LexerConfigurationInstance.LstripBlocks;
            }
          }else if((Position == 0 || M_IN[Position - 1] == '\n') && StringView::StartsWith(OpenString, LexerConfigurationInstance.LineStatement))
            StateInstance = State::LineStart;
          else{
            // ! Wasn't actually an opening sequence
            Position += 1;
            goto Again;
          }

          std::string_view Text = StringView::Slice(M_IN, TokenStart, Position);
          if(MustLstrip)
            Text = ClearFinalLineIfWhiteSpace(Text);

          if(Text.empty()){
            // ! // don't generate empty token
            goto Again; 
          }
          return Token(Token::Kind::Text, Text);
        }
        case State::ExpressionStart: {
          StateInstance = State::ExpressionBody;
          Position += LexerConfigurationInstance.ExpressionOpen.size();
          return MakeToken(Token::Kind::ExpressionOpen);
        }
        case State::ExpressionStartForceLstrip: {
          StateInstance = State::ExpressionBody;
          Position += LexerConfigurationInstance.ExpressionOpenForceLstrip.size();
          return MakeToken(Token::Kind::ExpressionOpen);
        }
        case State::LineStart: {
          StateInstance = State::LineBody;
          Position += LexerConfigurationInstance.LineStatement.size();
          return MakeToken(Token::Kind::LineStatementOpen);
        }
        case State::StatementStart: {
          StateInstance = State::StatementBody;
          Position += LexerConfigurationInstance.StatementOpen.size();
          return MakeToken(Token::Kind::StatementOpen);
        }
        case State::StatementStartNoLstrip: {
          StateInstance = State::StatementBody;
          Position += LexerConfigurationInstance.StatementOpenNoLstrip.size();
          return MakeToken(Token::Kind::StatementOpen);
        }
        case State::StatementStartForceLstrip: {
          StateInstance = State::StatementBody;
          Position += LexerConfigurationInstance.StatementOpenForceLstrip.size();
          return MakeToken(Token::Kind::StatementOpen);
        }
        case State::CommentStart: {
          StateInstance = State::CommentBody;
          Position += LexerConfigurationInstance.CommentOpen.size();
          return MakeToken(Token::Kind::CommentOpen);
        }
        case State::CommentStartForceLstrip: {
          StateInstance = State::CommentBody;
          Position += LexerConfigurationInstance.CommentOpenForceLstrip.size();
          return MakeToken(Token::Kind::CommentOpen);
        }
        case State::ExpressionBody:
          return ScanBody(LexerConfigurationInstance.ExpressionClose, 
                    Token::Kind::ExpressionClose, LexerConfigurationInstance.ExpressionCloseForceRstrip);
        case State::LineBody:
          return ScanBody("\n", Token::Kind::LineStatementClose);
        case State::StatementBody:
          return ScanBody(LexerConfigurationInstance.StatementClose, 
                  Token::Kind::StatementClose, LexerConfigurationInstance.StatementCloseForceRstrip, LexerConfigurationInstance.TrimBlocks);
        case State::CommentBody: {
          // ! Fast-scan to comment close
          const size_t End = M_IN.substr(Position).find(LexerConfigurationInstance.CommentClose);
          if(End == std::string_view::npos){
            Position = M_IN.size();
            return MakeToken(Token::Kind::Eof);
          }

          // ! Check for trim pattern
          const bool MustRstrip = StringView::StartsWith(M_IN.substr(Position + End - 1), LexerConfigurationInstance.CommentCloseForceRstrip);

          // ! Return the entire comment in the close token
          StateInstance = State::Text;
          Position += End + LexerConfigurationInstance.CommentClose.size();
          Token TokenInstance = MakeToken(Token::Kind::CommentClose);
          if(MustRstrip || LexerConfigurationInstance.TrimBlocks) 
            SkipWhiteSpacesAndFirstNewLine();
          return TokenInstance;
        }
      }
    }

    const LexerConfiguration& GetConfiguration() const{
      return LexerConfigurationInstance;
    }
  };
}; // ! Namespace Sydonia

#endif // ! SYDONIA_LEXER_HXX
