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

#define NAMESPACE_URI        "urn:UnifiedAutomation:CppDemoServer:BuildingAutomation"

typedef enum{
    DL_OPCUA_OK = 0,
    DL_OPCUA_MEM,
    DL_OPCUA_INVAL,
    DL_OPCUA_INIT,
    DL_OPCUA_ATTR,
}DL_OPCUA_ERR_t;

typedef struct alias_pair{
    char* name;
    int integer;
}alias_pair_t;

typedef enum{
    DL_OPC_VARIABLE,
    DL_OPC_METHOD,
    DL_OPC_OBJ
}DL_OPCUA_TYPE_t;

typedef struct opcua_container{
    void* next;
    DL_OPCUA_TYPE_t next_type;
}opcua_container_t;

typedef struct opcua_document{
    xmlDocPtr document;
    xmlNodePtr root_node;
    char* filename;
    char* version;
}opcua_document_t;

typedef struct opcua_ns_id{
    int ns;
    int i;
    char* s;
}opcua_ns_id_t;

typedef struct opcua_reference opcua_reference_t;

struct opcua_reference{
    char* type;
    opcua_ns_id_t id;
    bool is_forward;

    opcua_reference_t* next;
};

typedef struct opcua_node_attributes{
    opcua_ns_id_t parent_node_id;
    opcua_ns_id_t node_id;
    char* browse_name;
    char* display_name;
}opcua_node_attributes_t;

typedef struct opcua_method_attributes{
    int method_declaration_id;
}opcua_method_attributes_t;

typedef struct opcua_variable_attributes{
    char* data_type;
    int user_access_level;
    int access_level;
    int array_dimensions;
    int value_rank;
}opcua_variable_attributes_t;

typedef struct opcua_object_attributes{

}opcua_object_attributes_t;

typedef struct opcua_method opcua_method_t;

struct opcua_method{
    xmlNodePtr node;
    xmlNodePtr references_node;
    opcua_node_attributes_t* attributes;
    opcua_method_attributes_t* method_attributes;
    opcua_reference_t* reference_head;
};

typedef struct opcua_variable opcua_variable_t;

struct opcua_variable{
    xmlNodePtr node;
    xmlNodePtr references_node;
    opcua_node_attributes_t* attributes;
    opcua_variable_attributes_t* variable_attributes;

    char* description;
    opcua_reference_t* reference_head;
};

typedef struct opcua_object opcua_object_t;

struct opcua_object{
    xmlNodePtr node;
    xmlNodePtr references_node;
    opcua_node_attributes_t* attributes;
    opcua_object_attributes_t* object_attributes;
    opcua_reference_t* reference_head;
};

//SERVER CONFIG
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

void datalog_opcua_save_deinit_doc(void);
opcua_document_t* datalog_opcua_create_document(char* filename, char* version);
DL_OPCUA_ERR_t datalog_opcua_init_doc(void);
DL_OPCUA_ERR_t datalog_opcua_set_browse_name(void* object, DL_OPCUA_TYPE_t type,
        char* browse_name);
DL_OPCUA_ERR_t datalog_opcua_set_display_name(void* object, DL_OPCUA_TYPE_t type, 
        char* display_name);
DL_OPCUA_ERR_t datalog_opcua_set_variable_description(opcua_variable_t* variable,
        char* description);
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr parent_node, 
        opcua_node_attributes_t* attributes);
DL_OPCUA_ERR_t datalog_opcua_create_node_method_attributes(opcua_method_t* method);
DL_OPCUA_ERR_t datalog_opcua_create_node_variable_attributes(opcua_variable_t* variable);
DL_OPCUA_ERR_t datalog_opcua_create_node_object_attributes(opcua_object_t* object);
DL_OPCUA_ERR_t datalog_opcua_create_node_method(opcua_method_t* method);
DL_OPCUA_ERR_t datalog_opcua_create_node_variable(opcua_variable_t* variable);
DL_OPCUA_ERR_t datalog_opcua_create_node_object(opcua_object_t* object);
DL_OPCUA_ERR_t datalog_opcua_set_reference_type(opcua_reference_t* reference,
        char* type);
opcua_reference_t* datalog_opcua_create_reference(void);
DL_OPCUA_ERR_t datalog_opcua_add_reference(void* object, 
        DL_OPCUA_TYPE_t object_type, opcua_reference_t* reference);
DL_OPCUA_ERR_t datalog_opcua_add_reference_attributes(xmlNodePtr reference_node,
        opcua_reference_t* reference);
DL_OPCUA_ERR_t datalog_opcua_create_node_references(void* object, 
        DL_OPCUA_TYPE_t object_type);
DL_OPCUA_ERR_t datalog_opcua_add_id_contents(xmlNodePtr node, opcua_ns_id_t* id);
DL_OPCUA_ERR_t datalog_opcua_add_id_attribute(xmlNodePtr node, char* attribute,
        opcua_ns_id_t* id);
DL_OPCUA_ERR_t datalog_opcua_set_id_s(opcua_ns_id_t* id, char* value);
DL_OPCUA_ERR_t datalog_opcua_set_id_ns(opcua_ns_id_t* id, int ns);
DL_OPCUA_ERR_t datalog_opcua_set_id_i(opcua_ns_id_t* id, int i);
opcua_node_attributes_t* datalog_opcua_create_attributes(void);
DL_OPCUA_ERR_t datalog_opcua_set_attribute_browse_name(opcua_node_attributes_t* attributes, 
        char* browse_name);
opcua_method_attributes_t* datalog_opcua_create_method_attributes(void);
DL_OPCUA_ERR_t datalog_opcua_set_variable_data_type(opcua_variable_attributes_t* var,
        char* data_type);
opcua_variable_attributes_t* datalog_opcua_create_variable_attributes(void);
opcua_object_attributes_t* datalog_opcua_create_object_attributes(void);
opcua_method_t* datalog_opcua_create_method(void);
opcua_variable_t* datalog_opcua_create_variable(void);
opcua_object_t* datalog_opcua_create_object(void);
void datalog_opcua_runtime(void);

#endif
