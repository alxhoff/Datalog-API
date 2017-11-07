/**
 * @file keyboard.h
 * @author Alex Hoffman
 * @date 11 October 2017
 * @brief Data types and functions for performing classical keyboard functions
 *
 * While the keyboard offers more advanced features, the classical keyboard I/O
 * features of the keyboard are implemented within this file
 *
 * @mainpage Datalog API
 * @section intro_sec Introduction
 * The original implementation of the LUA/C library is not the easiest to work 
 * with and does not lead to logical or easy to follow code. I have quickly 
 * thrown this API together to make the coding datalog coding style similar to 
 * that of the Python implementation. 
 * @ection prereq_sec Prerequisites 
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
 * @section building_sec Building
 * I have included a demo main.c as well as a demo CMake that will build the 
 * API as a shared library that can then be linked into a exsisting project.<br> 
 * @subsection build_cmds How to build
 * It's pretty tough.... cd into the root director and create a build dir, 
 * or don't, it'll be your mess.
 * @verbatim
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
 * @section progress_sec Work in progress
 * I wrote this quickly and dirtily, so excuse the mess. 
 * @subsection todo_sec To-Do
 * - Clauses with bodies
 * - Error checking on literal types
 * - Check that all methods are implemented for all objects, both with and 
 * without structs.
 * @section objects_sec Objects 
 * The API revolves around a struct objects to enable a more logical way or 
 * representing datalog commands rather than the push and pop confusion found
 * in the library. <br><br>
 * Literals are represented by the object datalog_literal_t, queries by 
 * datalog_query_t and clauses by datalog_clause_t. <br><br>
 * I may of missed a few methods to be implemented but I should get these 
 * done as I actually use this API for other code. 
 */

#ifndef __DATALOG_API_H__
#define __DATALOG_API_H__

#include "datalog.h"

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
    DATALOG_TYPE
}DATALOG_ERR_t;

/**
* @typedef datalog_query_answer_pair_t
* @brief Typdef for datalog_query_answer_pair
*/
typedef struct datalog_query_answer_pair datalog_query_answer_pair_t;

/**
* @struct datalog_query_answer_pair
* @brief Struct used to represent individual query answers in the API
*/
struct datalog_query_answer_pair{
    char* arg1;
    char* arg2;
};

/**
* @typedef datalog_query_answer_t
* @brief Typdef for datalog_query_answer
*/
typedef struct datalog_query_answer datalog_query_answer_t;

/**
* @struct datalog_query_answer
* @brief Struct used to represent query answers in the API
*/
struct datalog_query_answer{
    char* predic;
    int argc;
    datalog_query_answer_pair_t** answer_pairs;
};

/**
* @typedef datalog_literal_t
* @brief Typdef for datalog_literal
*/
typedef struct datalog_literal datalog_literal_t;

/**
* @struct datalog_literal
* @brief Struct to represent a datalog literal
*/
struct datalog_literal{
    char* predicate; /**< Literal's precidate string represenation*/
    char* arg1;      /**< String representation of the literals first arg*/
    char* arg2;      /**< String representation of the literals second arg*/
    DATALOG_LIT_t lit_type; /** Type of literal */
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
   datalog_query_answer_t* processed_answer;
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
* @brief Initialises the GPIO pins for the SR
*
* Takes the answers struct provided by the LUA/C library and exports it
* into an struct that this API uses to process query answers.
* 
* @param a The queries returned answers
* @return Pointer to an API query answer struct
*/
datalog_query_answer_t* datalog_process_answer(dl_answers_t a);

/**
* @brief Prints a visual representation of a query answer struct
*
* @param a API datalog query answers struct to be printed 
* @return void
*/
void datalog_print_answers(datalog_query_answer_t* a);

/**
* @brief Initialises a datalog query struct from the given arguements 
*
* @param predicate string literal of the literal's predicate
* @param arg1 string literal for the literal's first term
* @param arg2 string literal for the literal's second term
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @return datalog_query_t* pointer to created query object. NULL on error 
*/
datalog_query_t* datalog_query_init(char* predicate, char* arg1, char* arg2,
        DATALOG_LIT_t lit_type);

/**
* @brief Initialises a datalog query struct from a literal struct
*
* @param lit pointer to literal struct to be used in the query object
* @return datalog_query_t* pointer to created query object. NULL on error 
*/
datalog_query_t* datalog_query_init_s(datalog_literal_t* lit);

/**
* @brief Issues a query 
*
* The datalog database is queried using the predicate and args passed
* to the function.
* 
* @param predicate string literal of the literal's predicate
* @param arg1 string literal for the literal's first term
* @param arg2 string literal for the literal's second term
* @param lit_type specifies the types of arguments to be created in the
* literal. 
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_query(char* predicate, char* const1, 
        char* const2, DATALOG_LIT_t lit_type);

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
DATALOG_ERR_t datalog_query_s(datalog_query_t* query);

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
datalog_literal_t* datalog_init_literal(char* predicate, char* arg1,
        char* arg2, DATALOG_LIT_t lit_type);

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
* @brief Creates and assers a literal from a literal struct 
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
DATALOG_ERR_t datalog_print_clause(datalog_clause_t* clause);
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
datalog_clause_t* datalog_init_clause(char* head_predicate,
        char* head_arg1, char* head_arg2, DATALOG_LIT_t head_lit_type);

/**
* @brief Initialises a clause structure, initialising the clause's
* head literal with the values passed to the funciton.
*
* @param head_predicate string literal of the literal's predicate
* @param head_arg1 string literal for the literal's first term
* @param head_arg2 string literal for the literal's second term
* @param head_lit_type specifies the types of arguments to be created in the
* literal. 
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_add_literal(datalog_clause_t* clause,
        char* predicate, char* arg1, char* arg2, DATALOG_LIT_t lit_type);

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
* @param literal pointer to the literal object that will be copied in as
* the clause's head literal
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_clause_add_literal_s(datalog_clause_t* clause,
        datalog_literal_t* literal);

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
* @brief Asserts a clause that does not have a body
*
* Pops a literal off the stack to create and assert a clause.
* Literal must be already on the stack. Ordered appropriatley.
* 
* @param literal_count Number of literals in the body of the clause
* @return DATALOG_ERR_t error message
*/
DATALOG_ERR_t datalog_assert_clause(int literal_count);

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
