#ifndef __DATALOG_PARSER_TYPES_H__
#define __DATALOG_PARSER_TYPES_H__

#include <stdlib.h>
#include <stdint.h>

#include "libxml/parser.h"
#include "datalog_api_types.h"

/**
* @enum DATALOG_ERR_t
* @brief error messages
* */
typedef enum{
    DL_PARSER_OK,
    DL_PARSER_MEM,
    DL_PARSER_DOC_,
    DL_PARSER_FILENAME,
    DL_PARSER_FILETYPE,
    DL_PARSER_EMPTY,
    DL_PARSER_NO_NODE
}DL_PARSER_ERR_t;

/**
* @enum DL_PARSER_ARG_TYPE_t 
* @brief used to specify the type of the first argument in a literal
*/
typedef enum{
    DL_PARSER_CONST = 1,    /*!< constant*/
    DL_PARSER_VAR = 2      /*!< variable*/
}DL_PARSER_ARG_TYPE_t;

typedef enum{
    DL_PARSE_TERM_C = 0x00,
    DL_PARSE_TERM_V = 0x01,
}DL_PARSER_TERM_t;

typedef struct dl_parser_term dl_parser_term_t;

struct dl_parser_term{
    char* value;
    DL_PARSER_TERM_t type;
    dl_parser_term_t* next;
};

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
    int term_count;
    dl_parser_term_t* term_head;

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

    void (*free)(dl_parser_rule_t**);
};

#endif
