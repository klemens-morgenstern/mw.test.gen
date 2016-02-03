//============================================================================
// Name        : c++.cpp
// Author      : Klemens Morgenstern
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>

#include </boost/spirit/home/x3.hpp>
//#include <mw/test/parsers/objects.hpp>

using namespace std;

int main(int argc, char **argv)
{

	if (argc <= 1)
		return 1;

	ifstream fs(argv[1]);
	stringstream ss;
	ss << fs.rdbuf();

/*	string buf = ss.str();

	bool r = parse(buf.begin(), buf.end(), filename);

	if (r)
		cout << "Parse succeeded" << endl;
	else
		cout << "Parse failed" << endl;
*/
	return 0;
}
/** Language:
 * @code
 * <Id>   ::= [_A-Za-z] [_A-Za-z0-9]* ;
 * <File> ::=  "file" [A-Za-z0-9-.]* ";" ;
 *
 * <ParenthesisCode> ::= "(" <PlainCode> ")" ;
 * <BracketsCode>	 ::= "[" <PlainCode> "]" ;
 * <CurlyCode> 		 ::= "{" <PlainCode> "}" ;
 *
 * <PlainCode> ::= -("(" | ")" | "[" | "]" | "{" | "}")* ;
 *
 * <CodeContent> ::= <PlainCode> | <ParenthesisCode> | <BracketsCode> | <CurlyCode> | <Code> | ;
 *
 * <Code> ::= "code" <CodeContent> ";" ;
 * <Assert> ::= assert <CodeContent> ";" ;
 * <Expect> ::= expect <CodeContent> ";" ;
 *
 *
 *
 * @endcode
 */

/**
 * @startuml

class operation {
/' static_  : bool '/
 bitwise  : bool
 range    : bool
/' critical : bool '/
 data     : std::string
}
operation <|-- assertion
operation <|-- expectation
operation <|-- critical_assertion

operation <|-- static_assertion
operation <|-- static_expectation

 * @enduml
 */
