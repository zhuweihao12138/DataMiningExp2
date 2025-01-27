
// Generated from src/cypher/grammar/Lcypher.g4 by ANTLR 4.13.0

#pragma once


#include "antlr4-runtime.h"


namespace parser {


class  LcypherLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    T__38 = 39, T__39 = 40, T__40 = 41, T__41 = 42, T__42 = 43, T__43 = 44, 
    T__44 = 45, EXPLAIN = 46, PROFILE = 47, UNION = 48, ALL = 49, OPTIONAL_ = 50, 
    MATCH = 51, UNWIND = 52, AS = 53, MERGE = 54, ON = 55, CREATE = 56, 
    SET = 57, DETACH = 58, DELETE_ = 59, REMOVE = 60, CALL = 61, YIELD = 62, 
    WITH = 63, DISTINCT = 64, RETURN = 65, ORDER = 66, BY = 67, L_SKIP = 68, 
    LIMIT = 69, ASCENDING = 70, ASC = 71, DESCENDING = 72, DESC = 73, USING = 74, 
    JOIN = 75, START = 76, WHERE = 77, OR = 78, XOR = 79, AND = 80, NOT = 81, 
    IN = 82, STARTS = 83, ENDS = 84, CONTAINS = 85, REGEXP = 86, IS = 87, 
    NULL_ = 88, COUNT = 89, ANY = 90, NONE = 91, SINGLE = 92, TRUE_ = 93, 
    FALSE_ = 94, EXISTS = 95, CASE = 96, ELSE = 97, END = 98, WHEN = 99, 
    THEN = 100, StringLiteral = 101, EscapedChar = 102, HexInteger = 103, 
    DecimalInteger = 104, OctalInteger = 105, HexLetter = 106, HexDigit = 107, 
    Digit = 108, NonZeroDigit = 109, NonZeroOctDigit = 110, OctDigit = 111, 
    ZeroDigit = 112, ExponentDecimalReal = 113, RegularDecimalReal = 114, 
    FILTER = 115, EXTRACT = 116, UnescapedSymbolicName = 117, CONSTRAINT = 118, 
    DO = 119, FOR = 120, REQUIRE = 121, UNIQUE = 122, MANDATORY = 123, SCALAR = 124, 
    OF = 125, ADD = 126, DROP = 127, IdentifierStart = 128, IdentifierPart = 129, 
    EscapedSymbolicName = 130, SP = 131, WHITESPACE = 132, Comment = 133
  };

  explicit LcypherLexer(antlr4::CharStream *input);

  ~LcypherLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace parser
