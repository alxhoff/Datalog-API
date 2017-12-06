/**
 * @file datalog_api.h
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief API functions to use the LUA/C datalog implementation found
 *        found at http://datalog.sourceforge.net
 *
 * @mainpage TARS Assistant
 * @section datalogapi_sec Datalog API
 * @subsection intro_sec Introduction
 * The original implementation of the LUA/C library is not the easiest to work 
 * with and does not lead to logical or easy to follow code. I have quickly 
 * thrown this API together to make the coding datalog coding style similar to 
 * that of the Python implementation. 
 * @subsection prereq_sec Prerequisites 
 * The LUA/C library can be found here http://datalog.sourceforge.net and must be 
 * downlaoded and placed into the libs folder such that the file tree looks like 
 * this <br><br>
 @verbatim
  |--- Datalog-API 
        | 
        |---bin
        |---build
        |---doc
        |
        |---libs
             |---libdatalog <---rename downloaded folder to this
                 |---source files
                 |---lua
                      |---lua source files
@endverbatim
@subsection building_sec Building
 * I have included a demo main.c as well as a demo CMake that will build the 
 * API as a shared library that can then be linked into a exsisting project.<br> 
@subsubsection build_cmds How to build
 * It's pretty tough.... cd into the root director and create a build dir, 
 * or don't, it'll be your mess.
@verbatim
 cd Datalog-API
 mkdir build
 cd build
@endverbatim
 * Generate CMake junk
 * @verbatim
 cmake ..
@endverbatim
 * and finally make
 * @verbatim
 make
@endverbatim
 * The executable can be found in the bin subdirectory in the root dir. <br><br>
 * Library objects in the build subdirectory.
@subsection progress_sec Work in progress
 * I wrote this quickly and dirtily, so excuse the mess. 
@subsubsection todo_sec To-Do
 * - Clauses with bodies
 * - Error checking on literal types
 * - Check that all methods are implemented for all objects, both with and 
 * without structs.
@section objects_sec Objects 
 * The API revolves around a struct objects to enable a more logical way or 
 * representing datalog commands rather than the push and pop confusion found
 * in the library. <br><br>
 * Literals are represented by the object datalog_literal_t, queries by 
 * datalog_query_t and clauses by datalog_clause_t. <br><br>
 * I may of missed a few methods to be implemented but I should get these 
 * done as I actually use this API for other code. 
@subsection literal_sec Literal objects
 * The parser represents literals using the dl_parser_literal_t object that stores the 
 * predicate and arguments for the literal. Literals are probably not used directly in
 * the parser.
@subsection fact_sec Fact objects
 * A fact in data log is a clause without a body, aka it's a standard literal. Facts are
 * essentailly a literal stored in a container allowing them to be used as elements of a
 * linked list as well as storing a reference to the XML node that corresponds to the fact.
 * The parser will scan the entire document for facts, creating a linked list. This linked
 * list is then traversed and each XML fact node is walked and processed so that each fact
 * is represented by a dl_parser_fact_t object which contains the fact's literal. <br>
 * The facts in the XML tree can be found by calling dl_parser_mappings as this will in
 * turn walk the XML tree and call dl_parser_add_fact on all found facts. Found facts are 
 * then processed using dl_parser_process_fact. After processing the found facts can be found
 * pointed to by the linked list head pointer dl_parser_fact_t* facts_head found within the
 * dl_parser_doc_t object.
@subsection rule_sec Rule objects
 * In Datalog rules are clauses that contain a body, the body being a potentially infinite 
 * length array of literals. In the XML parser rules are represented similarly to facts,
 * using the dl_parser_rule_t object, with the exception that the rules object represents
 * the body of the rule by an array of literal pointers. This list is dynamically allocated
 * to allow for potentially infinite length rules. Parsing of rules happens in the same 
 * fashion as facts, in that a call to dl_parser_mappings will walk the XML tree and call
 * dl_parser_add_rule on any found rules. These are then stores in a similar linked list 
 * fashion and processed through calls to dl_parser_process_rule whilst iterating over the
 * linked list. The linked list of processed rules can be found using the head pointer
 * dl_parser_rule_t* rules_head found in the dl_parser_doc_t object.
@section parser_sec XML Parser
 * The XML parser is designed to be used to be able parse XML files
 * to a datalog program to load rules and facts into the
 * datalog database. The parser is built around libxml2 and built into a shared library
 * through CMake to use
 * to ensure easier building and linking. 
@subsection parser_use_sec 
 * Parser use should pretty much only require the user to call 
@verbatim
 dl_parser_doc_t* dl_doc = dl_parser_init(filename);
@endverbatim
 * followed by
@verbatim
 dl_parser_mappings(dl_doc);
@endverbatim
 * but one can also print any number of the objects in the library though the number of 
 * print functions. Document metadata can also be parsed by calling dl_parser_metadata
 * which will populat a dl_parser_metadata_t object stored in the main dl_parser_doc_t
 * object. <br>
 * See included example for furthur usage.
@subsubsection parser_use_init_sec Initialisation and deinitialisation
 * The parser must be started by calling
 * the dl_parser_init function, specifying the file to be opened. This will initialise
 * the document object used by the parser. Calling dl_parser_deinit all objects used
 * by the parser are free'd and the parser deinit'd.
@subsubsection parser_use_parsing_sec Parsing a document
 * The function dl_parser_runtime can be called to completley pass a document, returning
 * a dl_parser_return_doc_t object that contains a linked list to all the parsed rules,
 * a linked list to all the parsed facts found within the document as well as populating
 * the dl_parser_metadata_t object within the return doc object. <br>
 * One can also parse the various types of data seperatley by running either:
 * dl_parser_metadata or dl_parser_mappings to parse only the metadata or the rules and 
 * facts respectivley. 
@subsubsection parser_use_assert_sec Asserting a parsed document
 * The dl_parser_return_doc_t object can have both it's rules list and facts lists asserted
 by calling the functions dl_assert_fact_list and dl_assert_rule_list respectivley.
@section xml_style_guide_sec XML Style Guide
* The XML documents to be parsed must adhere to a strict style guide. I would
* like to say I have been scrupulous in implementing error checking but as this
* is more a time>quality implementation, you will most likley crash your program
* by parsing an invalid XML document. More throrogh error checking is on the TODO.
@subsection XML_style_tags_sec Tags
@subsubsection XML_style_tags_document Document
* All datalog XML documents must have a root tag <datalog>
@subsubsection XML_style_tags_metadata Metadata
* - <name>: The name of the device
* - <type>: The type of device
* - <manufacturer>: Manufacturer of the device
* - <contact>: Contact reguarding the device, probably from supplier
* - <model>: Device model
* - <serial>: Device serial number
* - <year>: Year the device was produced
@subsubsection XML_style_tags_mappings Mappings
* The mappings tags houses the portion of the document where all rules and
* facts are to be represented.
@subsubsection XML_style_tags_facts Facts
* A fact is represented very similarly to a rule, except it does not contain a
* <body> tag. The <head> tag contains one literal.
@subsubsection XML_style_tags_rules Rules
* A rule is represented by two tags, a <head> tag and a <body> tag. The head
* can only contain one literal while the body can contain a theorerically
* infinite number of literals.
@subsubsection XML_style_tags_head Head
* <head> tags are used to represent the head of a clause and contain one literal.
@subsubsection XML_style_tags_body Body
* <body> tags are used to represent the body of a calsue can can contain a 
* theoretically infiinite number of literals.
@subsubsection XML_style_tags_literal Literal
* A literal is used to display datalog's most rudementary data type. One literal
* is comprised of a predicate and the corresponsing terms. <br>
* a literal such as test(foo, BAR). would be represented using the following
@verbatim
<literal>
    <predicate>test</predicate>
    <terms>
        <constant>foo</constant> <-- constant as the term has a lowercase letter
        <variable>BAR</variable> <-- Variable as the term has a capital letter
    </terms>
</literal>
@endverbatim
* - <predicate>: The literal's predicated
* - <terms>: Terms can have two types, the tags can be interchanged to modify the
* type of literal you wish to express
*   - <constant>: represents a constant term
*   - <variable>: represents a variable term
@subsection XML_style_example_sec Example XML
@verbatim
<?xml version="1.0" encoding="UTF-8"?>
<datalog>
    <metadata>
        <description>EDD to NAMUR mappings</description>
        <author>alex</author>
        <device>
            <name>test device</name>
            <type>test type</type>
            <manufacturer>siemens</manufacturer>
            <contact>alxhoff@gmail.com</contact>
            <model>test model</model>
            <serial>test serial</serial>
            <year>2017</year>
        </device>
    </metadata>
    <mappings>
        <fact>
            <head>
                <literal>
                    <predicate>test</predicate>
                    <terms>
                        <constant>hello</constant>
                        <constant>world</constant>
                    </terms>
                </literal>
            </head>
        </fact>
        <rule>
            <head>
                <literal>
                    <predicate>test</predicate>
                    <terms>
                        <constant>this</constant>
                        <variable>is</variable>
                    </terms>
                </literal>
            </head>
            <body>
                <literal>
                    <predicate>foo</predicate>
                    <terms>
                        <constant>a</constant>
                        <variable>rule</variable>
                    </terms>
                </literal>
                <literal>
                    <predicate>test</predicate>
                    <terms>
                        <constant>with two</constant>
                        <constant>literals</constant>
                    </terms>
                </literal>
            </body>
        </rule>
    </mappings>
</datalog>
@endverbatim
@section CLI_sec Command Line Interface
 * I have made a small set of funtions that work on a loop to read commands from the
 * native CLI to the Datalog's CLI emulator. The CLI can be used to interact with
 * and previous datalog happenings run within the same program as it shares the same
 * global database as the other API modules. 
@subsection CLI_use_sec Using the CLI
 * The CLI accepts 4 types of input: normal datalog statements, "help" to display help,
 * "clear" to clear the CLI and "exit" to exit the CLI function loop. <br>
 * Please see the inbuilt help function for information on how to express datalog
 * statements.<br>
 * The CLI can be embedded into a program via it's main runtime fuction 
 * datalog_command_line_run. The function runs on a goto loop and will run until the
 * program is terminated or the "exit" command is given.
 */

#ifndef __DATALOG_API_H__
#define __DATALOG_API_H__

#include "datalog.h"
#include "datalog_api_types.h"

/**
* @enum DATALOG_ERR_t
* @brief error messages
* */
typedef enum{
    DATALOG_OK = 0,
    DATALOG_ASRT,
    DATALOG_INIT,
    DATALOG_MEM,
    DATALOG_LIT,
    DATALOG_QUE,
    DATALOG_TYPE,
    DATALOG_TERM,
}DATALOG_ERR_t;

/**
* @typedef datalog_query_answers_t
* @brief Typdef for datalog_query_answers
*/
typedef struct datalog_query_answers datalog_query_answers_t;

/**
* @struct datalog_query_answers
* @brief Struct used to represent individual query answers in the API
*/
struct datalog_query_answers{
    int term_count;
    char** term_list;
};

/**
* @typedef datalog_query_processed_answers_t
* @brief Typdef for datalog_query_answer
*/
typedef struct datalog_query_answer datalog_query_processed_answers_t;

/**
* @struct datalog_query_answer
* @brief Struct used to represent query answers in the API
*/
struct datalog_query_answer{
    char* predic;
    int answer_term_count;
    int answer_count;
    datalog_query_answers_t** answers;
};

/**
* @typedef datalog_query_t
* @brief Typdef for datalog_query
*/
typedef struct datalog_query datalog_query_t;

/**
* @struct datalog_query
* @brief Struct used to represent send a query the API
*/
struct datalog_query{
   datalog_literal_t* literal;
   dl_answers_t* answer;
   datalog_query_processed_answers_t* processed_answer;
};

/**
* @typedef datalog_clause_t
* @brief Typdef for datalog_clause
*/
typedef struct datalog_clause datalog_clause_t;

/**
* @struct datalog_clause
* @brief Struct to represent a datalog clause
*/
struct datalog_clause{
    datalog_literal_t* head;        /**< Clause's head literal pointer*/
    int literal_count;              /**< Number of literals in the clause's body*/
    datalog_literal_t** body_list;  /**< List of pointers to the literals in the clause's body*/
};

/**
* @brief Global database used by datalog library
*/
extern dl_db_t datalog_db;

/**
* @brief Processes the answers returned from a query.
*
* Takes the answers struct provided by the LUA/C library and exports it
* into an struct that this API uses to process query answers.
* 
* @param a The queries returned answers
* @return Pointer to an API query answer struct
*/
datalog_query_processed_answers_t* datalog_process_answer(dl_answers_t a);

/**
* @brief Prints a visual representation of a query answer struct
*
* @param a API datalog query answers struct to be printed 
* @return void
*/
void datalog_print_answers(datalog_query_processed_answers_t* a);

/**
* @brief Initialises a datalog query struct from a literal struct
*
* @param lit pointer to literal struct to be used in the query object
* @return datalog_query_t* pointer to created query object. NULL on error 
*/
datalog_query_t* datalog_query_init(datalog_literal_t* lit);
DATALOG_ERR_t datalog_query_print(datalog_query_t* query);

/**
* @brief Issues a query and saves the answer 
*
* The datalog database is queried using the literal pointed to from within the
* query struct.
* 
* @param query struct holding a pointer to the literal to be used for the query
* and a pointer to where the answer should be stored.
* @return DATALOG_ERR_t error message 
*/
DATALOG_ERR_t datalog_query_ask(datalog_query_t* query);

DATALOG_ERR_t datalog_literal_add_term(datalog_literal_t* lit, char* value, 
        DATALOG_TERM_t type);

/**
* @brief Initialises a datalog literal struct, does not create the literal
* in the database.
*
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @param predicate string literal of the literal's predicate
* @param arg1 string literal for the literal's first term
* @param arg2 string literal for the literal's second term
* @return datalog_literal_t* pointer to created literal object. NULL on error 
*/
datalog_literal_t* datalog_literal_init(char* predicate);

DATALOG_ERR_t datalog_literal_print(datalog_literal_t* lit);
DATALOG_ERR_t datalog_literal_create(datalog_literal_t* lit);
DATALOG_ERR_t datalog_literal_create_and_assert(datalog_literal_t* lit);

//TODO lit type error checking. can all types be directly asserted?
//
/**
* @brief Updates the values of a preexsisting literal structure and 
* asserts the literal.
*
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @param predicate string literal of the literal's predicate
* @param arg1 string literal for the literal's first term
* @param arg2 string literal for the literal's second term
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_update_and_assert_literal(datalog_literal_t* lit,
        char* predicate, char* arg1, char* arg2, DATALOG_LIT_t lit_type);

/**
* @brief Updates the values of a preexsisting literal structure
*
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @param predicate string literal of the literal's predicate
* @param arg1 string literal for the literal's first term
* @param arg2 string literal for the literal's second term
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_update_literal(datalog_literal_t* lit,
        char* predicate, char* arg1, char* arg2, DATALOG_LIT_t lit_type);

/**
* @brief Creates and asserts a literal from a literal struct 
*
* Creates a literal from the given struct, of a specified type,
* instead of leaving the literal on top of the stack the literal
* is asserted into the database.
* 
* @param literal struct containing the information required to
* create the literal. 
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_create_and_assert_literal_s(datalog_literal_t* lit);

/**
* @brief Creates a literal from the given arguments
*
* Creates a literal from the given terms, of a specified type,
* and leaves the literal on top of the stack.
* 
* @param predicate string literal of the literal's predicate
* @param arg1 string literal for the literal's first term
* @param arg2 string literal for the literal's second term
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_create_literal(char* predicate, char* var1, 
    char* var2, DATALOG_LIT_t lit_type);

/**
* @brief Creates a literal from a literal struct 
*
* Creates a literal from the given struct, of a specified type,
* and leaves the literal on top of the stack.
* 
* @param literal struct containing the information required to
* create the literal. 
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_create_literal_s(datalog_literal_t* literal);

/**
* @brief Pushes the given string on to the top of the datalog stack
*
* @param string string to be pushed onto the stack
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_push_string(char* string);

/**
* @brief Adds either a variable or constant to the current literal
*
* Whilst a literal is in creation (whilst it is on top of the stack)
* this function takes the string ontop of the stack and adds it to the
* literal as either a variable or constant.
* 
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @param index specifies if the variable or constant to be created is the
* 0th term or the 1st term of the literal
* @return 0 on success
*/
int datalog_add_var_const(DATALOG_LIT_t lit_type, int index);

/**
* @brief Prints a clause structure in a datalog representation 
*
* @param clause pointer to the clause object to be printed
* @return DATALOG_ERR_t 
*/
DATALOG_ERR_t datalog_clause_print(datalog_clause_t* clause);

/**
* @brief Initialises a clause structure, populating the head literal
* with the arguments parsed to the function.
*
* @param head_predicate string literal of the literal's predicate
* @param head_arg1 string literal for the literal's first term
* @param head_arg2 string literal for the literal's second term
* @param head_lit_type specifies the types of arguments to be created in the
* literal. 
* @return datalog_clause_t* pointer to the clause struct created 
*/
datalog_clause_t* datalog_clause_init(datalog_literal_t* lit);

/**
* @brief Initialises a clause structure, but instead of creating a copy
* of the passed literal as the clause's head literal, the clause struct
* only points to it.
*
* One must be careful that the literal is never free'd as this would 
* lead to undefined behavious.
*
* @param clause pointer to the clause object to which the literal shall
* be added
* @param lit pointer to the literal object that will be copied in as
* the clause's head literal
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_add_literal(datalog_clause_t* clause, 
        datalog_literal_t* lit);
DATALOG_ERR_t datalog_clause_create_and_assert(datalog_clause_t* clause);

/**
* @brief Initialises a clause structure, the clause's head literal
* is created as a copy of the literal that is passed to the function
*
* With this function one can free the initialising head literal
*
* @param clause pointer to the clause object to which the literal shall
* be added
* @param literal pointer to the literal object that will be copied in as
* the clause's head literal
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_add_literal_s_copy(datalog_clause_t* clause,
        datalog_literal_t* literal);

/**
* @brief Pushes a clause structure onto the stack then asserts the 
* terms as a clause.
*
* @param clause pointer to the clause object to which the literal shall
* be asserted
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_create_and_assert(datalog_clause_t* clause);

/**
* @brief Asserts a clause that has its head and body literals already 
* pushed onto the stack
*
* Pops a literal off the stack to create and assert a clause.
* Literal must be already on the stack. Ordered appropriatley.
* 
* @param literal_count Number of literals in the body of the clause
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_assert(int literal_count);

/**
* @brief Pushes a clause structure onto the stack then retracts the 
* terms as a clause.
*
* @param clause pointer to the clause object to which the literal shall
* be retracted
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_create_and_retract(datalog_clause_t* clause);

/**
* @brief Retracts a clause that does not have a body
*
* Pops a literal off the stack to create and retracts a clause.
* Literal must be already on the stack. Ordered appropriatley.
* 
* @param literal_count Number of literals in the body of the clause
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_retract_clause(void);

/**
* @brief Initialises the datalog database
*
* @param none
* @return DATALOG_ERR_t error message 
*/
DATALOG_ERR_t datalog_engine_db_init(void);

/**
* @brief Deinitialises the datalog database
*
* @param none
* @return DATALOG_ERR_t error message 
*/
DATALOG_ERR_t datalog_engine_db_deinit(void);

#endif
