/**
 * @file        datalog_opcua_generator.h
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       OPCUA Server XML Generator
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

#ifndef __DATALOG_OPCUA_GENERATOR_H__
#define __DATALOG_OPCUA_GENERATOR_H__

#include "libxml/parser.h"
#include "libxml/tree.h"

#ifndef bool
#define bool signed char
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define XML_FILENAME        "test_doc.xml"
#define XML_FILE_VERSION    "1.0"

#define NAMESPACE_URI       "urn:UnifiedAutomation:CppDemoServer:BuildingAutomation"

/**
* @enum 
* @brief 
*/
typedef enum{
    DL_OPCUA_OK = 0,
    DL_OPCUA_MEM,
    DL_OPCUA_INVAL,
    DL_OPCUA_INIT,
    DL_OPCUA_ATTR,
}DL_OPCUA_ERR_t;

/**
* @enum 
* @brief 
*/
typedef enum{
    DL_OPC_VARIABLE,
    DL_OPC_METHOD,
    DL_OPC_OBJ,
    DL_OPC_OBJ_TYPE
}DL_OPCUA_TYPE_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct alias_pair{
    char* name;
    int integer;
}alias_pair_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_container{
    void* next;
    DL_OPCUA_TYPE_t next_type;
}opcua_container_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_document{
    xmlDocPtr document;
    xmlNodePtr root_node;
    char* filename;
    char* version;
}opcua_document_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_ns_id{
    int ns;
    int i;
    char* s;
}opcua_ns_id_t;

/**
* @typedef 
* @brief 
*/
typedef struct opcua_reference opcua_reference_t;

/**
* @struct 
* @brief 
*/
struct opcua_reference{
    char* type;
    opcua_ns_id_t id;
    bool is_forward;

    opcua_reference_t* next;

    DL_OPCUA_ERR_t (*set_id_ns)(opcua_reference_t*,int);
    DL_OPCUA_ERR_t (*set_id_i)(opcua_reference_t*,int);
    DL_OPCUA_ERR_t (*set_id_s)(opcua_reference_t*,char*);
    DL_OPCUA_ERR_t (*set_type)(opcua_reference_t*,char*);
    DL_OPCUA_ERR_t (*set_is_forward)(opcua_reference_t*,bool);
    DL_OPCUA_ERR_t (*add_reference)(opcua_reference_t*,void*,DL_OPCUA_TYPE_t);
};

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_node_attributes{
    opcua_ns_id_t parent_node_id;
    opcua_ns_id_t node_id;
    char* browse_name;
    char* display_name;
}opcua_node_attributes_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_object_type_attributes{

}opcua_object_type_attributes_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_method_attributes{
    int method_declaration_id;
}opcua_method_attributes_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_variable_attributes{
    char* data_type;
    int user_access_level;
    int access_level;
    int array_dimensions;
    int value_rank;
}opcua_variable_attributes_t;

/**
* @typedef 
* @brief 
*/
/**
* @struct 
* @brief 
*/
typedef struct opcua_object_attributes{

}opcua_object_attributes_t;

/**
* @typedef 
* @brief 
*/
typedef struct opcua_object_type opcua_object_type_t;

/**
* @struct 
* @brief 
*/
struct opcua_object_type{
    xmlNodePtr node;
    xmlNodePtr references_node;

    opcua_node_attributes_t* attributes;
    opcua_object_type_attributes_t* object_type_attributes;
    opcua_reference_t* reference_head;
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_object_type_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_object_type_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_object_type_t*,char*);
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_object_type_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_object_type_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_object_type_t*,char*);
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_object_type_t*,char*);
    DL_OPCUA_ERR_t (*set_display_name)(opcua_object_type_t*,char*);
    DL_OPCUA_ERR_t (*create_references)(opcua_object_type_t*);
};

/**
* @typedef 
* @brief 
*/
typedef struct opcua_method opcua_method_t;

/**
* @struct 
* @brief 
*/
struct opcua_method{
    xmlNodePtr node;
    xmlNodePtr references_node;

    opcua_node_attributes_t* attributes;
    opcua_method_attributes_t* method_attributes;
    opcua_reference_t* reference_head;
    
    //functions
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_method_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_method_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_method_t*,char*);
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_method_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_method_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_method_t*,char*);
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_method_t*,char*);
    DL_OPCUA_ERR_t (*set_display_name)(opcua_method_t*,char*);
    DL_OPCUA_ERR_t (*set_declaration_id)(opcua_method_t*,int);
    DL_OPCUA_ERR_t (*create_references)(opcua_method_t*);
};

/**
* @typedef 
* @brief 
*/
typedef struct opcua_variable opcua_variable_t;

/**
* @struct 
* @brief 
*/
struct opcua_variable{
    xmlNodePtr node;
    xmlNodePtr references_node;
    
    opcua_node_attributes_t* attributes;
    opcua_variable_attributes_t* variable_attributes;
    char* description;
    opcua_reference_t* reference_head;

    //functions
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_variable_t*,char*);
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_variable_t*,char*);
    DL_OPCUA_ERR_t (*set_data_type)(opcua_variable_t*,char*);
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_access_level)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_array_dimensions)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_value_rank)(opcua_variable_t*,int);
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_variable_t*,char*);
    DL_OPCUA_ERR_t (*set_display_name)(opcua_variable_t*,char*);
    DL_OPCUA_ERR_t (*set_description)(opcua_variable_t*,char*);
    DL_OPCUA_ERR_t (*create_references)(opcua_variable_t*);
};

/**
* @typedef 
* @brief 
*/
typedef struct opcua_object opcua_object_t;

/**
* @struct 
* @brief 
*/
struct opcua_object{
    xmlNodePtr node;
    xmlNodePtr references_node;
    
    opcua_node_attributes_t* attributes;
    opcua_object_attributes_t* object_attributes;
    opcua_reference_t* reference_head;

    //functions
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_object_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_object_t*,int);
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_object_t*,char*);
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_object_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_object_t*,int);
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_object_t*,char*);
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_object_t*,char*);
    DL_OPCUA_ERR_t (*set_display_name)(opcua_object_t*,char*);
    DL_OPCUA_ERR_t (*create_references)(opcua_object_t*);
};

//SERVER CONFIG
opcua_reference_t object_type_array[] = {
    {.type = "hello",
        .id = {.i = 10,
            .ns = 11},
        .is_forward = false
    },
    {.type = "hello2",
        .id = {.i = 12,
            .ns = 13},
        .is_forward = true
    },
    {.type = "hello3",
        .id = {.s = "testS",
            .ns = 13},
        .is_forward = -1 
    },
    {.type = NULL}
};

alias_pair_t alias_array[] = {
    {"Boolean", 1},
    {"SByte", 2},
    {"Byte", 3},
    {"Int16", 4},
    {"UInt16", 5},
    {"Int32", 6},
    {"UInt32", 7},
    {"Int64", 8},
    {"UInt64", 9},
    {"Float", 10},
    {"Double", 11},
    {"String", 12},
    {"DateTime", 13},
    {"Guid", 14},
    {"ByteString", 15},
    {"XmlElement", 16},
    {"NodeId", 17},
    {"StatusCode", 19},
    {"QualifiedName", 20},
    {"LocalizedText", 21},
    {"Number", 26},
    {"Integer", 27},
    {"UInteger", 28},
    {"Organizes", 35},
    {"HasModellingRule", 37},
    {"HasTypeDefinition", 40},
    {"HasSubtype", 45},
    {"HasProperty", 46},
    {"HasComponent", 47},
    {"NodeClass", 257},
    {"Duration", 290},
    {"UtcTime", 294},
    {"Argument", 296},
    {"Range", 884},
    {"EUInformation", 887},
    {"EnumValueType", 7594},
    {"TimeZoneDataType", 8912},
    {NULL, 0},
};
//END SERVER CONFIG

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
void datalog_opcua_save_deinit_doc(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_document_t* datalog_opcua_create_document(char* filename, char* version);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_init_doc(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_browse_name(void* object, DL_OPCUA_TYPE_t type,
        char* browse_name);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_display_name(void* object, DL_OPCUA_TYPE_t type, 
        char* display_name);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_variable_description(opcua_variable_t* variable,
        char* description);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr parent_node, 
        opcua_node_attributes_t* attributes);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_method_attributes(opcua_method_t* method);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_variable_attributes(opcua_variable_t* variable);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object_attributes(opcua_object_t* object);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object_type(opcua_object_type_t* object);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_method(opcua_method_t* method);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_variable(opcua_variable_t* variable);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object(opcua_object_t* object);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_clear_reference(opcua_reference_t* ref);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_reference_type(opcua_reference_t* reference,
        char* type);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_reference_t* datalog_opcua_create_reference(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_add_reference(opcua_reference_t* reference,
        void* object, DL_OPCUA_TYPE_t object_type);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_add_reference_attributes(xmlNodePtr reference_node,
        opcua_reference_t* reference);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_references(void* object, 
        DL_OPCUA_TYPE_t object_type);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_add_id_contents(xmlNodePtr node, opcua_ns_id_t* id);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_add_id_attribute(xmlNodePtr node, char* attribute,
        opcua_ns_id_t* id);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_s(opcua_ns_id_t* id, char* value);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_ns(opcua_ns_id_t* id, int ns);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_i(opcua_ns_id_t* id, int i);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_node_attributes_t* datalog_opcua_create_attributes(void);
DL_OPCUA_ERR_t datalog_opcua_set_attribute_browse_name(opcua_node_attributes_t* attributes, 
        char* browse_name);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_method_attributes_t* datalog_opcua_create_method_attributes(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
DL_OPCUA_ERR_t datalog_opcua_set_variable_data_type(opcua_variable_attributes_t* var,
        char* data_type);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_variable_attributes_t* datalog_opcua_create_variable_attributes(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_object_attributes_t* datalog_opcua_create_object_attributes(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_object_type_t* datalog_opcua_create_object_type(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_method_t* datalog_opcua_create_method(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_variable_t* datalog_opcua_create_variable(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
opcua_object_t* datalog_opcua_create_object(void);

/**
* @brief 
* 
* @param 
* @param 
* @return 
*/
void datalog_opcua_runtime(void);

#endif
