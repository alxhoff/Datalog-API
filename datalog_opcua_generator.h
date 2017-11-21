#ifndef __DATALOG_OPCUA_GENERATOR_H__
#define __DATALOG_OPCUA_GENERATOR_H__

#include "libxml/parser.h"
#include "libxml/tree.h"

#ifndef bool
#define bool unsigned char
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef struct alias_pair{
    char* name;
    int integer;
}alias_pair_t;

typedef struct opcua_reference opcua_reference_t;

struct opcua_reference{
    char* type;
    int ns;
    int i;
    bool is_forward;
    
    opcua_reference_t* next;
};

typedef struct opcua_node_attribute{
    int parent_node_id;
    int i;
    int node_id;
    char* browse_name;
    int user_access_level;
    int access_level;
    char* data_type;
}opcua_node_attribute_t;

typedef struct opcua_method opcua_method_t;

struct opcua_method{
    xmlNodePtr node;

    opcua_node_attribute_t* attributes;
    char* display_name;
    opcua_reference_t* reference_head;

    opcua_method_t* next;
};

typedef struct opcua_variable opcua_variable_t;

struct opcua_variable{
    xmlNodePtr node;

    opcua_node_attribute_t* attributes;
    char* display_name;
    opcua_reference_t* reference_head;

    opcua_variable_t* next;
};

typedef struct opcua_UAObject opcua_UAObject_t;

struct opcua_UAObject{
    xmlNodePtr node;

    opcua_node_attribute_t* attributes;
    char* display_name;
    opcua_reference_t* reference_head;

    opcua_UAObject_t* next;
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

void datalog_opcua_runtime(void);

#endif
