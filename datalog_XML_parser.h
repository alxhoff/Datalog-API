
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

#include "datalog_parser_types.h"

/**
* @typedef dl_parser_metadata_t
* @brief Typdef for struct dl_parser_metadata
*/
/**
* @struct dl_parser_metadata
* @brief 
*/
typedef struct dl_parser_metadata{

    char* description;  /*!< description of the document */
    char* author;       /*!< author of the document */
    char* device_name;  /*!< device's name */
    char* device_type;  /*!< device's type */
    char* device_manufacturer;  /*!< devices's manufacturer*/
    char* device_contact;   /*!< contact responsible for device */
    char* device_model;     /*!< device's model */
    char* device_serial;    /*!< device's serial */
    char* device_year;      /*!< device's year */
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
* @typedef dl_parser_return_doc_t
* @brief Typdef for struct dl_parser_return_doc
*/
/**
* @struct dl_parser_return_doc
* @brief 
*/
typedef struct dl_parser_return_doc{
    char* filename;

    dl_parser_literal_t* literals_head; /*!< pointer to the list of literals */
    dl_parser_rule_t* rules_head;       /*!< pointer to the list of rules */
    dl_parser_fact_t* facts_head;       /*!< pointer to the list of facts */

    dl_parser_metadata_t* metadata;     /*!< pointer to the documents parsed metadata */

}dl_parser_return_doc_t;

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
* @brief parses a <device> node of the metadata tree in
* the XML document
* 
* @param doc pointer to parser document stuct
* @param dev_node pointer to the <device> XML node
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_metadata_device(dl_parser_doc_t* doc,
        xmlNode* dev_node);

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
* @brief Frees a term list
* 
* @param Pointer to the term list head
* @return void
*/
void datalog_parser_free_term_list(dl_parser_term_t** term_head);

/**
* @brief Frees a parser literal object
* 
* @param Double pointer to the literal object to be freed
* @return void
*/
void datalog_parser_free_literal(dl_parser_literal_t** lit);

/**
* @brief Frees a parser clause body object
* 
* @param Double pointer to the clause body object
* @return void
*/
void datalog_parser_free_clause_body(dl_parser_clause_body_t** body);

/**
* @brief Frees a parser fact object
* 
* @param Double pointer to the fact object
* @return void
*/
void datalog_parser_free_fact(dl_parser_fact_t** fact);

/**
* @brief Frees a fact list
* 
* @param Double pointer to the fact list's head
* @return void
*/
void datalog_parser_free_fact_list(dl_parser_fact_t** fact_head);

/**
* @brief Frees a rule object 
* 
* @param Double pointer to the rule object
* @return void
*/
void datalog_parser_free_rule(dl_parser_rule_t** rule);

/**
* @brief Frees a rule list
* 
* @param Double pointer to the rule list's head
* @return void
*/
void datalog_parser_free_rule_list(dl_parser_rule_t** rule_head);

/**
* @brief Deinitialises a return document, freeing memory resources
* 
* @param Double pointer to the return document object to be deinitialised
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_deinit_return_doc(dl_parser_return_doc_t** doc);

/**
* @brief Deinitialises a document object whilst preserving the processed data.
* Important if the processed data is being pointed to from a return doc object
* 
* @param Double pointer to doc object to be freed
* @return DL_PARSER_ERR_t error message
*/
DL_PARSER_ERR_t dl_parser_deinit_with_data(dl_parser_doc_t** doc);

/**
* @brief runtime funciton that takes a XML filename and returns the parsed
* document
* 
* @param filename spcified the filename of the XML file to be parsed
* @return DL_PARSER_ERR_t error message
*/
dl_parser_return_doc_t* dl_parser_runtime(char* filename);

#endif
