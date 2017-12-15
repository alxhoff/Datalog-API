/**
 * @file        main.c
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       Demo main.c for Datalog API
 *  
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

#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "datalog_api.h"
#include "datalog_parser.h"
#include "datalog_api_parser.h"
#include "datalog_cli.h"
#include "datalog_opcua_generator.h"

#define clear() printf("\033[H\033[J")

//global datalog database
dl_db_t datalog_db;

static int get_line (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return 1;

    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? 2 : 0;
    }

    buff[strlen(buff)-1] = '\0';
    return 0;
}

void datalog_splash()
{
    printf("*==========================================* \n");
    printf("        _       _        _             \n");
    printf("       | |     | |      | |            \n");
    printf("     __| | __ _| |_ __ _| | ___   __ _ \n");
    printf("    / _` |/ _` | __/ _` | |/ _ \\ / _` |\n");
    printf("   | (_| | (_| | || (_| | | (_) | (_| |\n");
    printf("    \\__,_|\\__,_|\\__\\__,_|_|\\___/ \\__, |\n");
    printf("                                  __/ |\n");
    printf("                                 |___/ \n");
    printf(" \n");
}

void datalog_command_line_splash(void)
{    
    printf("*==========================================* \n");
    printf(" \n");
}

void datalog_command_line_run(void)
{
    DATALOG_CLI_ERR_t ret = DATALOG_CLI_OK;
    char command_line[100];
    
datalog_cli_restart:    
    datalog_splash();
    datalog_command_line_splash();
    ret = get_line("\n-> ", command_line, 100);
    printf("input is: %s\n", command_line);  
    
    ret = dl_cli_parse_line(command_line);
    if(ret == DATALOG_CLI_EXIT){
        clear();
    }else if(ret == DATALOG_CLI_HELP){
        clear();
        dl_cli_print_help();
        goto datalog_cli_restart;
    }else
        goto datalog_cli_restart;
}

int main(void)
{
    DATALOG_ERR_t ret = DATALOG_OK;

    ret =  datalog_engine_db_init();

    if(ret != DATALOG_OK) return -1;

    datalog_literal_t* test_lit = datalog_literal_init("test");
    datalog_literal_add_term(test_lit, "hello", DL_TERM_C);
    datalog_literal_add_term(test_lit, "world", DL_TERM_C);
    datalog_literal_add_term(test_lit, "world", DL_TERM_C);
    datalog_literal_add_term(test_lit, "world", DL_TERM_C);
    datalog_literal_add_term(test_lit, "world", DL_TERM_C);
    datalog_literal_print(test_lit);
    datalog_literal_create_and_assert(test_lit);
    
    datalog_literal_t* test_lit_4 = datalog_literal_init("test");
    datalog_literal_add_term(test_lit_4, "hello", DL_TERM_C);
    datalog_literal_add_term(test_lit_4, "universe", DL_TERM_C);
    test_lit_4->print(test_lit_4);
    datalog_literal_create_and_assert(test_lit_4);

    datalog_literal_t* test_lit_2 = datalog_literal_init("test");
    test_lit_2->add_term(test_lit_2, "hello", DL_TERM_C);
    test_lit_2->add_term(test_lit_2, "X", DL_TERM_V);

    test_lit->free(&test_lit);

    datalog_query_t* test_query = datalog_query_init(test_lit_2);
    test_query->ask(test_query);
    test_query->print(test_query);
   
    test_query->free(&test_query);

    datalog_literal_t* clause_lit = datalog_literal_init("ancestor");
    clause_lit->add_term(clause_lit, "A", DL_TERM_V);
    clause_lit->add_term(clause_lit, "B", DL_TERM_C);
    datalog_clause_t* test_clause = datalog_clause_init(clause_lit);

    datalog_literal_t* clause_lit2 = datalog_literal_init("parent");
    clause_lit2->add_term(clause_lit2, "A", DL_TERM_V);
    clause_lit2->add_term(clause_lit2, "C", DL_TERM_C);
    
    datalog_literal_t* clause_lit3 = datalog_literal_init("ancestor");
    clause_lit3->add_term(clause_lit3, "C", DL_TERM_V);
    clause_lit3->add_term(clause_lit3, "B", DL_TERM_C);

    test_clause->add_literal(test_clause, clause_lit2);
    test_clause->add_literal(test_clause, clause_lit3);

    test_clause->assert(test_clause);

    test_clause->free(&test_clause);

    char* terms[] = {"first", "second", "third", "fourth"};
    char* test_predicate = "predicate";

    datalog_literal_stand_alone_create_and_assert(test_predicate, 4, terms, 0x00, true);

    datalog_query_processed_answers_t* answers = 
        datalog_query_stand_alone_create_and_ask(test_predicate, 4, terms, 0x00);

    datalog_processed_answers_print(answers);

    answers->free(&answers); 

    printf("parsing doc\n");

    dl_parser_return_doc_t* doc = dl_parser_runtime("query_test.xml");
    printf("parsing doc2\n");

    ret = datalog_parser_assert_doc(doc);
    printf("parsing doc3\n");

    datalog_opcua_runtime();
    
    datalog_command_line_run();

    ret = datalog_engine_db_deinit();

    if(ret != DATALOG_OK) return -1;

    return 0;
}

