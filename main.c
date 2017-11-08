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

#include "datalog_api.h"
#include "datalog_parser.h"

//global datalog database
dl_db_t datalog_db;

int main(void)
{
    DATALOG_ERR_t ret = DATALOG_OK;

    ret =  datalog_engine_db_init();

    if(ret != DATALOG_OK) return -1;
  
    datalog_literal_t* test_lit = datalog_init_literal("test", "hello", "world", DL_CC);

    if(test_lit == NULL) return -1;

    if(datalog_create_and_assert_literal_s(test_lit) != DATALOG_OK)
        return -1;

    if(datalog_update_and_assert_literal
        (test_lit, "test", "hello", "universe", 0) != DATALOG_OK)
        return -1;
    
    datalog_query_t* test_query = datalog_query_init_s(test_lit);

    if(test_query == NULL) return -1;

    if(datalog_query( "test", "hello", "X", DL_CV) != DATALOG_OK)
        return -1;
    
    ret = datalog_engine_db_deinit();

    if(ret != DATALOG_OK) return -1;

    dl_parser_runtime("text_xml.xml");

    return 0;
}

