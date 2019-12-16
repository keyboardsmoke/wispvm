#include "shared/types.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"

using namespace compiler;

Parser::Parser(const char* sourceText) : m_lex(sourceText)
{

}

//