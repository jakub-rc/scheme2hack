#include "scanner.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

Scanner::Scanner(std::string sourceFile) : sourceFile(sourceFile) {}

Scanner::~Scanner() {}

bool Scanner::hasNext() {

  if (tokensBuffer.size() > 0) {

    return true;
  } else {
    std::string line;

    while (std::getline(sourceFile, line)) {

      if (tokenizeChunk(line)) {
        return true;
      }
    }
  }
  return false;
}

bool Scanner::tokenizeChunk(std::string line) {

  tokensBuffer.clear();

  auto line_begin = std::sregex_iterator(line.begin(), line.end(), tokenRegex);
  auto line_end = std::sregex_iterator();

  bool commentStart = false;
  std::sregex_iterator regexIterator = line_begin;

  // FIXME: check based on match group what kind of token this is
  while (regexIterator != line_end) {
    std::smatch match = *regexIterator;

    std::string token = match.str();
    if (token == ";") {
      // comment only line
      if (tokensBuffer.size() == 0) {
        return false;
      } else {
        break; // end line comment, finish parsing
      }
    } else {
      std::cout << " TOKEN " << token << "\n";
      tokensBuffer.push_back(token);
    }

    regexIterator++;
  }

  bufferIterator = tokensBuffer.begin();

  return tokensBuffer.size() > 0;
}

std::string Scanner::nextToken() {
  std::string token = *bufferIterator++;

  if (bufferIterator == tokensBuffer.end()) {
    tokensBuffer.clear();
  }

  return token;
}

TokenType Scanner::getTokenType(std::string token) {
  if (token[0] == '"') {
    return TokenType::stringVal;
  } else if (!token.empty() &&
             token.find_first_not_of("0123456789") == std::string::npos) {
    return TokenType::numVal;
  }
  // else if (token.size() == 1) {

  //  char t = token[0];

  //  for (int i = 0; i < SYMBOLS_SIZE; i++) {
  //    if (t == SYMBOLS[i]) {
  //      return TokenType::symbol;
  //    }
  //  }
  //  return TokenType::identifier;
  //} else if (binary_search(KEYWORDS_SORTED.begin(), KEYWORDS_SORTED.end(),
  //                         token)) {
  //  //return TokenType::keyWord;
  //}
  else {
    return TokenType::identifier;
  }
}
