
/**
 * @file datalog_parser.h 
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief       A Datalog parser to extract Datalog rules and facts from
 *              XML documents.
 @verbatim
   ----------------------------------------------------------------------
    Copyright (C) Alexander Hoffman, 2017
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */

#ifndef __DATALOG_PARSER_H__
#define __DATALOG_PARSER_H__

#include <stdlib.h>
#include <stdint.h>

#include "libxml/parser.h"

/**
 * @enum DL_PARSER_ERR_t
 * @brief error messages
 * */
typedef enum{
    DL_PARSER_OK,       /*!< no errors*/
    DL_PARSER_MEM,      /*!< memory error*/
    DL_PARSER_DOC,      /*!< document error*/
    DL_PARSER_FILENAME, /*!< document filename error*/
    DL_PARSER_FILETYPE, /*!< document filetype error*/
    DL_PARSER_EMPTY,    /*!< variable empty*/
    DL_PARSER_NO_NODE   /*!< no node mathing criteria*/
}DL_PARSER_ERR_t;

#ifndef __DATALOG_API_H__
/**
* @enum DATALOG_LIT_t
* @brief used to specify the type of literal to be created
*/
typedef enum{
    DL_CC,  /*!< constant constant */
    DL_CV,  /*!< constant variable */
    DL_VV,  /*!< variable variable */
    DL_VC   /*!< variaben constant */
}DATALOG_LIT_t;
#endif

/**
* @enum DL_PARSER_ARG_TYPE_t 
* @brief used to specify the type of the first argument in a literal
*/
typedef enum{
    DL_PARSER_CONST = 1,    /*!< constant*/
    DL_PARSER_VAR = 2      /*!< variable*/
}DL_PARSER_ARG_TYPE_t;

/**
* @typedef dl_parser_literal_t
* @brief Typdef for struct dl_parser_literal
*/
typedef struct dl_parser_literal dl_parser_literal_t;

/**
* @struct dl_parser_literal
* @brief 
*/
struct dl_parser_literal{
    char* predicate;    /*!< string literal of the literal's predicate */
    char* arg1;         /*!< string literal of the literal's arg1 */
    char* arg2;         /*!< string literal of the literal's arg2 */

    DATALOG_LIT_t lit_type; /*!< literal's type */

    dl_parser_literal_t* next; /*!< next literal in a linked list */
};

/**
* @typedef dl_parser_fact_t
* @brief Typdef for struct dl_parser_fact
*/
typedef struct dl_parser_fact dl_parser_fact_t;

/**
* @struct dl_parser_fact
* @brief 
*/
struct dl_parser_fact{
    dl_parser_literal_t* literal;   /*!< fact's literal */

    xmlNode* node;                  /*!< node location in the XML tree */

    dl_parser_fact_t* next;         /*!< next fact in a linked list */
};

/**
* @typedef dl_parser_clause_body_t
* @brief Typdef for struct dl_parser_clause_body
*/
/**
* @struct dl_parser_clause_body
* @brief 
*/
typedef struct dl_parser_clause_body{
    dl_parser_literal_t** literals; /*!< array of literals comprising the clause's body */

    uint8_t literal_count;          /*!< number of literals within the clause's body */
}dl_parser_clause_body_t;

/**
* @typedef dl_parser_rule_t
* @brief Typdef for struct dl_parser_rule
*/
typedef struct dl_parser_rule dl_parser_rule_t;

/**
* @struct dl_parser_rule
* @brief 
*/
struct dl_parser_rule{
    dl_parser_literal_t* head;      /*!< pointer to the head of the rule */
    dl_parser_clause_body_t* body;  /*!< pointer to the body of the rule */

    xmlNode* node;                  /*!< XML node */

    dl_parser_rule_t* next;         /*!< next rule in a linked list */
};

/**
* @typedef dl_parser_metadata_t
* @brief Typdef for struct dl_parser_metadata
*/
/**
* @struct dl_parser_metadata
* @brief 
*/
typedef struct dl_parser_metadata{
    char* description;              /*!< description of the document */
    char* author;                   /*!< author of the document */
}dl_parser_metadata_t;

/**
* @typedef dl_parser_doc_t
* @brief Typdef for struct dl_parser_doc
*/
/**
* @struct dl_parser_doc
* @brief 
*/
typedef struct dl_parser_doc{
    char* filename;

    dl_parser_literal_t* literals_head; /*!< pointer to the list of literals */
    dl_parser_rule_t* rules_head;       /*!< pointer to the list of rules */
    dl_parser_fact_t* facts_head;       /*!< pointer to the list of facts */

    dl_parser_metadata_t* metadata;     /*!< pointer to the documents parsed metadata */
        
    xmlNode* root_node;                 /*!< XML document's root node */
    xmlNode* mappings_node;             /*!< XML document's mappings node */
    xmlNode* script_node;               /*!< XML document's script node */
    xmlNode* metadata_root;             /*!< XML document's metadata node */    

    xmlDoc* document;                   /*!< XML document structure used by libxml */
}dl_parser_doc_t;   

/**
* @brief prints the root node's name
*
* @param doc pointer to parser document stuct
* @return void
*/
void dl_parser_print_root(dl_parser_doc_t* doc);

/**
* @brief prints the XML documents metadata
* 
* @param doc pointer to parser document stuct
* @return void
*/
void dl_parser_print_metadata(dl_parser_doc_t* doc);

/**
* @brief prints a Datalog style representation of a literal
* 
* Prints with the style:  predicate(arg1, arg2)
* 
* @param literal pointer to the literal object that is to be printed
* @return void
*/
void dl_parser_print_literal(dl_parser_literal_t* literal);

/**
* @brief prints a the literals that make up a clause body
* 
* Iterates through the linked list of a clause bodies literals and prints them
* in the style: predicate(arg1, arg2), predicate(arg1, arg2)... 
* 
* @param clause_body pointer to the clause body object that is to be printed
* @return void
*/
void dl_parser_print_clause_body(dl_parser_clause_body_t* clause_body);

/**
* @brief prints a rule in a datalog format.
* 
* Prints the head literal of the rule followed by the clause body (literal list)
* 
* @param rule pointer to a parser rule object that is to be printed
* @return void
*/
void dl_parser_print_rule(dl_parser_rule_t* rule);

/**
* @brief prints a list of all the rules parsed from the XML document
* 
* Prints all the entries in the linked list of rules parsed from the
* mappings portion of the XML document
* 
* @param doc pointer to parser document stuct
* @return void
*/
void dl_parser_print_rule_list(dl_parser_doc_t* doc);

/**
* @brief prints a face in a datalog format
* 
* Prints the literal that makes up the fact
* 
* @param fact pointer to a parser fact object that is to be printed
* @return void
*/
void dl_parser_print_fact(dl_parser_fact_t* fact);

/**
* @brief prints a list of all the facts parsed from the XML document
* 
* Prints all the entries in the linked list of facts parsed from the
* mappings portion of the XML document
* 
* @param doc pointer to parser document stuct
* @return void
*/
void dl_parser_print_fact_list(dl_parser_doc_t* doc);

/**
* @brief gets the last dl_parser_rule_t* object in the linked list of parsed rules
* 
* @param doc pointer to parser document stuct
* @return dl_parser_rule_t* pointer to the last fact in the linked list of parsed rules
*/
dl_parser_rule_t* dl_parser_get_last_rule(dl_parser_doc_t* doc);

/**
* @brief adds a rule to the end of the linked list of parsed rules
* 
* @param doc pointer to parser document stuct
* @param rule pointer to a parser rule object that is to be added to the rule list
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_add_rule(dl_parser_doc_t* doc, dl_parser_rule_t* rule);

/**
* @brief gets the last dl_parser_fact_t* object in the linked list of parsed facts
* 
* @param doc pointer to parser document stuct
* @return dl_parser_fact_t* pointer to the last fact in the linked list of parsed facts
*/
dl_parser_fact_t* dl_parser_get_last_fact(dl_parser_doc_t* doc);

/**
* @brief adds a fact to the end of the linked list of parsed rules
* 
* @param doc pointer to parser document stuct
* @param fact pointer to a parser fact object that is to be added to the fact list
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_add_fact(dl_parser_doc_t* doc, dl_parser_fact_t* fact);

/**
* @brief retrieves the literal terms from a <terms> node and saves them into a 
* given literal object
* 
* @param doc pointer to parser document stuct
* @param terms_node terms_node pointer to the <terms> node that is to be parsed
* @param literal pointer to the literal where the parsed terms shall be saved
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_terms(dl_parser_doc_t* doc, xmlNode* terms_node, 
        dl_parser_literal_t* literal);

/**
* @brief processes a <literal> node into a dl_parser_literal_t object
* 
* Walks the subtree under the <literal> node and extracts all relevant information
* 
* @param doc pointer to parser document stuct
* @param literal_node pointer to the <literal> node that is to be processed into a dl_parser_literal_t
* struct
* @return dl_parser_literal_t* pointer to the resulting literal object
*/
dl_parser_literal_t* dl_parser_process_literal(dl_parser_doc_t* doc, xmlNode* literal_node);

/**
* @brief returns a pointer to the <literal> node contained within the <head> node
* 
* @param head_node pointer to the <head> node from which the literal is to be extracted
* @return xmlNode* the <literal> node contained within the <head> node
*/
xmlNode* dl_parser_get_head_literal_node(xmlNode* head_node);

/**
* @brief retrieved all the literal nodes within the subtree of a <body> node, saving them into
* a dl_parser_clause_body_t struct
* 
* @param doc pointer to parser document stuct
* @return dl_parser_clause_body_t* parser object representation of the parsed <body> node
*/
dl_parser_clause_body_t* dl_parser_body_get_literal_nodes(dl_parser_doc_t* doc,
        xmlNode* body_node);

/**
* @brief processes a rule object, placing it on the linked list of parsed rules
* 
* @param doc pointer to parser document stuct
* @param rule pointer to a parser rule object that is to be processed
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_process_rule(dl_parser_doc_t* doc, dl_parser_rule_t* rule);

/**
* @brief processes a fact object, placing it on the linked list of parsed facts
* 
* @param doc pointer to parser document stuct
* @param fact pointer to a parser fact object that is to be processed
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_process_fact(dl_parser_doc_t* doc, dl_parser_fact_t* fact);

/**
* @brief !TODO! parses the <script> subtree of the XML document
* 
* @param doc pointer to parser document stuct
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_script(dl_parser_doc_t* doc);

/**
* @brief parses the metadata of the XML document
* 
* @param doc pointer to parser document stuct
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_metadata(dl_parser_doc_t* doc);

/**
* @brief initilises the parser 
* 
* allocates all the required data structs required by libxml as well as
* opening and reading the XML file, creating the XML tree in memory.
* 
* @param filename spcified the filename of the XML file to be parsed
* @return dl_parser_doc_t* pointer to the document struct used by the parser
*/
dl_parser_doc_t* dl_parser_init(char* filename);

/**
* @brief where all parser logic that the user needs can be written
* 
* @param filename spcified the filename of the XML file to be parsed
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_runtime(char* filename);

#endif
