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
* @enum DL_OPCUA_ERR_t
* @brief Error messages 
*/
typedef enum{
    DL_OPCUA_OK = 0,        /**< No errors*/
    DL_OPCUA_MEM,           /**< Memory allocation error*/
    DL_OPCUA_INVAL,         /**< Invalid parameter*/
    DL_OPCUA_INIT,          /**< Initialisation error*/
    DL_OPCUA_ATTR,          /**< Attribute error*/
}DL_OPCUA_ERR_t;

/**
* @enum DL_OPCUA_TYPE_t
* @brief Types of XML node objects 
*/
typedef enum{
    DL_OPC_VARIABLE,        /**< <UAVariable>*/
    DL_OPC_METHOD,          /**< <UAMethod>*/
    DL_OPC_OBJ,             /**< <UAObject>*/
    DL_OPC_OBJ_TYPE,        /**< <UAObjectType>*/
    DL_OPC_ATTR
}DL_OPCUA_TYPE_t;

/**
* @typedef alias_pair_t
* @brief Typdef for alias_pair
*/
/**
* @struct alias_pair
* @brief Represents an <Alias> pair of alias name and i value
*/
typedef struct alias_pair{
    char* name;                 /**< */
    int integer;                /**< */
}alias_pair_t;

/**
* @typedef opcua_container_t
* @brief Typdef for opcua_container
*/
/**
* @struct opcua_container
* @brief Linked list container
*/
typedef struct opcua_container{
    void* next;                 /**< */
    DL_OPCUA_TYPE_t next_type;  /**< */
}opcua_container_t;

/**
* @typedef opcua_document_t
* @brief Typdef for opcua_document
*/
/**
* @struct opcua_document
* @brief Object representing a OPCUA XML document to be generated
*/
typedef struct opcua_document{
    xmlDocPtr document;         /**< */
    xmlNodePtr root_node;       /**< */
    char* filename;             /**< */
    char* version;              /**< */
}opcua_document_t;

/**
* @typedef opcua_ns_id_t
* @brief Typdef for opcua_ns_id
*/
/**
* @struct opcua_ns_id
* @brief Stores the attribute values for ID's such as ParentNodeId and
* NodeId
*/
typedef struct opcua_ns_id{
    int ns;         /**< */
    int i;          /**< */
    char* s;        /**< */
}opcua_ns_id_t;

/**
* @typedef opcua_reference_t
* @brief Typdef for opcua_reference
*/
typedef struct opcua_reference opcua_reference_t;

/**
* @struct opcua_reference
* @brief Object representing a <Reference> node
*/
struct opcua_reference{
    char* type;                 /**< */
    opcua_ns_id_t id;           /**< */
    bool is_forward;            /**< */

    opcua_reference_t* next;    /**< */

    DL_OPCUA_ERR_t (*set_id_ns)(opcua_reference_t*,int);        /**< */
    DL_OPCUA_ERR_t (*set_id_i)(opcua_reference_t*,int);         /**< */
    DL_OPCUA_ERR_t (*set_id_s)(opcua_reference_t*,char*);       /**< */
    DL_OPCUA_ERR_t (*set_type)(opcua_reference_t*,char*);       /**< */
    DL_OPCUA_ERR_t (*set_is_forward)(opcua_reference_t*,bool);  /**< */
    DL_OPCUA_ERR_t (*add_reference)(opcua_reference_t*,void*,   /**< */
        DL_OPCUA_TYPE_t);
};

/**
* @typedef opcua_node_attributes_t
* @brief Typdef for opcua_node_attributes
*/
typedef struct opcua_node_attributes opcua_node_attributes_t;

/**
* @struct opcua_node_attributes
* @brief Object representing a the attributes represented within a node
*
* Object nodes such as <UAObject>,<UAVariable>,<UAMethod> and <UAObjectType>
* have attributes within the tag head. These attributes are represented by
* this object.
*/
struct opcua_node_attributes{
    opcua_ns_id_t parent_node_id;   /**< */
    opcua_ns_id_t node_id;          /**< */
    char* browse_name;              /**< */
    char* display_name;             /**< */

    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_node_attributes_t*,int);   /**< */
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_node_attributes_t*,int);    /**< */
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_node_attributes_t*,char*);  /**< */
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_node_attributes_t*,int);     /**< */
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_node_attributes_t*,int);      /**< */
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_node_attributes_t*,char*);    /**< */
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_node_attributes_t*,char*);  /**< */
    DL_OPCUA_ERR_t (*set_display_name)(opcua_node_attributes_t*,char*); /**< */
};

/**
* @typedef opcua_object_type_attributes_t
* @brief Typdef for opcua_object_type_attributes
*/
/**
* @struct opcua_object_type_attributes
* @brief Attributes specific to a <UAObjectType> node
*/
typedef struct opcua_object_type_attributes{

}opcua_object_type_attributes_t;

/**
* @typedef opcua_method_attributes_t
* @brief Typdef for opcua_method_attributes
*/
/**
* @struct opcua_method_attributes
* @brief Attributes specific to a <UAMethod> node
*/
typedef struct opcua_method_attributes{
    int method_declaration_id;      /**< */
}opcua_method_attributes_t;

/**
* @typedef opcua_variable_attributes_t
* @brief Typdef for opcua_variable_attributes
*/
/**
* @struct opcua_variable_attributes
* @brief Attributes specific to a <UAVariable> node
*/
typedef struct opcua_variable_attributes{
    char* data_type;                /**< */
    int user_access_level;          /**< */
    int access_level;               /**< */
    int array_dimensions;           /**< */
    int value_rank;                 /**< */
}opcua_variable_attributes_t;

/**
* @typedef opcua_object_attributes_t
* @brief Typdef for opcua_object_attributes
*/
/**
* @struct opcua_object_attributes
* @brief Attributes specific to a <UAObject> node
*/
typedef struct opcua_object_attributes{

}opcua_object_attributes_t;

/**
* @typedef opcua_object_type_t
* @brief Typdef for opcua_object_type
*/
typedef struct opcua_object_type opcua_object_type_t;

/**
* @struct opcua_object_type
* @brief Object representing a <UAObjectType> node
*/
struct opcua_object_type{
    xmlNodePtr node;                                        /**< */
    xmlNodePtr references_node;                             /**< */
    xmlNodePtr display_name_node;                       /**< */

    opcua_node_attributes_t* attributes;                    /**< */
    opcua_object_type_attributes_t* object_type_attributes; /**< */
    opcua_reference_t* reference_head;                      /**< */
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_object_type_t*,int);   /**< */
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_object_type_t*,int);    /**< */
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_object_type_t*,char*);  /**< */
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_object_type_t*,int);     /**< */
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_object_type_t*,int);      /**< */
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_object_type_t*,char*);    /**< */
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_object_type_t*,char*);  /**< */
    DL_OPCUA_ERR_t (*set_display_name)(opcua_object_type_t*,char*); /**< */
    DL_OPCUA_ERR_t (*create_references)(opcua_object_type_t*);      /**< */
};

/**
* @typedef opcua_method_t
* @brief Typdef for opcua_method
*/
typedef struct opcua_method opcua_method_t;

/**
* @struct opcua_method
* @brief Object representing a <UAMethod> node
*/
struct opcua_method{
    xmlNodePtr node;                                /**< */
    xmlNodePtr references_node;                     /**< */
    xmlNodePtr display_name_node;                       /**< */

    opcua_node_attributes_t* attributes;            /**< */
    opcua_method_attributes_t* method_attributes;   /**< */
    opcua_reference_t* reference_head;              /**< */
    
    //functions
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_method_t*,int);    /**< */
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_method_t*,int);     /**< */
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_method_t*,char*);   /**< */
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_method_t*,int);      /**< */
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_method_t*,int);       /**< */
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_method_t*,char*);     /**< */
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_method_t*,char*);   /**< */
    DL_OPCUA_ERR_t (*set_display_name)(opcua_method_t*,char*);  /**< */
    DL_OPCUA_ERR_t (*set_declaration_id)(opcua_method_t*,int);  /**< */
    DL_OPCUA_ERR_t (*create_references)(opcua_method_t*);       /**< */
};

/**
* @typedef opcua_variable_t
* @brief Typdef for opcua_variable
*/
typedef struct opcua_variable opcua_variable_t;

/**
* @struct opcua_variable
* @brief Object representing a <UAVariable> node
*/
struct opcua_variable{
    xmlNodePtr node;                                    /**< */
    xmlNodePtr references_node;                         /**< */
    xmlNodePtr display_name_node;                       /**< */
        
    opcua_node_attributes_t* attributes;                /**< */
    opcua_variable_attributes_t* variable_attributes;   /**< */
    char* description;                                  /**< */
    opcua_reference_t* reference_head;                  /**< */

    //functions
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_variable_t*,int);      /**< */
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_variable_t*,int);       /**< */
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_variable_t*,char*);     /**< */
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_variable_t*,int);        /**< */
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_variable_t*,int);         /**< */
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_variable_t*,char*);       /**< */
    DL_OPCUA_ERR_t (*set_data_type)(opcua_variable_t*,char*);       /**< */
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_variable_t*,int); /**< */
    DL_OPCUA_ERR_t (*set_access_level)(opcua_variable_t*,int);      /**< */
    DL_OPCUA_ERR_t (*set_array_dimensions)(opcua_variable_t*,int);  /**< */
    DL_OPCUA_ERR_t (*set_value_rank)(opcua_variable_t*,int);        /**< */
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_variable_t*,char*);     /**< */
    DL_OPCUA_ERR_t (*set_display_name)(opcua_variable_t*,char*);    /**< */
    DL_OPCUA_ERR_t (*set_description)(opcua_variable_t*,char*);     /**< */
    DL_OPCUA_ERR_t (*create_references)(opcua_variable_t*);         /**< */
};

/**
* @typedef opcua_object_t
* @brief Typdef for opcua_object
*/
typedef struct opcua_object opcua_object_t;

/**
* @struct opcua_object
* @brief Object representing a <UAObject> node
*/
struct opcua_object{
    xmlNodePtr node;                                /**< */
    xmlNodePtr references_node;                     /**< */
    xmlNodePtr display_name_node;                       /**< */
    
    opcua_node_attributes_t* attributes;            /**< */
    opcua_object_attributes_t* object_attributes;   /**< */
    opcua_reference_t* reference_head;              /**< */

    //functions
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_object_t*,int);    /**< */
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_object_t*,int);     /**< */
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_object_t*,char*);   /**< */
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_object_t*,int);      /**< */
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_object_t*,int);       /**< */
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_object_t*,char*);     /**< */
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_object_t*,char*);   /**< */
    DL_OPCUA_ERR_t (*set_display_name)(opcua_object_t*,char*);  /**< */
    DL_OPCUA_ERR_t (*create_references)(opcua_object_t*);       /**< */
};

//SERVER CONFIG
opcua_reference_t object_type_array[] = {
    {.type = "HasSubtype",
        .id = {.i = 58},
        .is_forward = false
    },
    {.type = "HasComponent",
        .id = {.i = 6001,
            .ns = 1},
        .is_forward = -1
    },
    {.type = "HasComponent",
        .id = {.i = 7001,
            .ns = 1},
        .is_forward = -1
    },
    {.type = "HasComponent",
        .id = {.i = 6002,
            .ns = 1},
        .is_forward = -1
    },
    {.type = "HasComponent",
        .id = {.i = 5001,
            .ns = 1},
        .is_forward = -1
    },
    {.type = "HasComponent",
        .id = {.i = 7006,
            .ns = 1},
        .is_forward = -1
    },
    {.type = "HasComponent",
        .id = {.i = 6037,
            .ns = 1},
        .is_forward = -1
    },
    {.type = "HasComponent",
        .id = {.i = 6035,
            .ns = 1},
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
* @brief Saves the document and deinitialises the libxml2 parser and frees memory 
* used by the document
*
* The document is saved with the filename specified in the global opcua_document_t
* object. Global variables used by the parser are freed, the xmlDoc object as well 
* as the opcua_document_t objects are freed and the parser is deinitialised and 
* used memory is dumped.
* 
* @return void
*/
void datalog_opcua_save_deinit_doc(void);

/**
* @brief Allocates all memory for the document and libxml2 functionality
*
* Allocates the global opcua_document_t object as well as creating the 
* libxml2 document object. The root <UANodeSet> node is created and set into the
* xmlDoc. 
* 
* @param filename Filename to be assigned to the OPCUA XML document
* @param version Versions to be assigned to the OPCUA XML document
* @return opcua_document_t* Pointer to the datalog OPCUA generator document object
*/
opcua_document_t* datalog_opcua_create_document(char* filename, char* version);

/**
* @brief Initialises the libxml2 parser and document objects
*
* The opcua_document_t object is given the filename passed to the function
* as well as the version passed to the function. The required sections for the 
* OPCUA document are generated from the data arrays assigned in the header. 
* 
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_init_doc(void);

/**
* @brief Sets the parent node id ns value for object types
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_parent_node_id_ns(opcua_object_type_t* self, 
    int ns);

/**
* @brief Sets the parent node id i value for object type objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/

DL_OPCUA_ERR_t self_set_object_type_parent_node_id_i(opcua_object_type_t* self, int i);

/**
* @brief Sets the parent node id s value for object type objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_parent_node_id_s(opcua_object_type_t* self, 
    char* s);

/**
* @brief Sets the node id ns value for object type objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_node_id_ns(opcua_object_type_t* self, int ns);

/**
* @brief Sets the node id i value for object type objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_node_id_i(opcua_object_type_t* self, int i);

/**
* @brief Sets the node id s value for object type objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_node_id_s(opcua_object_type_t* self, char* s);

/**
* @brief Sets the browse name for object type objects
* 
* @param self pointer to object to be set
* @param browse_name String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_browse_name(opcua_object_type_t* self, 
        char* browse_name);

/**
* @brief Sets the dislay name value for object type objects
* 
* @param self pointer to object to be set
* @param display_name String representation of display name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_type_display_name(opcua_object_type_t* self, char* display_name);

/**
* @brief Sets the parent node id ns value for variable objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_parent_node_id_ns(opcua_variable_t* self, int ns);

/**
* @brief Sets the parent node id i value for variable objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_parent_node_id_i(opcua_variable_t* self, int i);

/**
* @brief Sets the parent node id s value for variable objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_parent_node_id_s(opcua_variable_t* self, char* s);

/**
* @brief Sets the node id ns value for variable objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_node_id_ns(opcua_variable_t* self, int ns);

/**
* @brief Sets the node id i value for variable objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_node_id_i(opcua_variable_t* self, int i);

/**
* @brief Sets the node id s value for variable objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_node_id_s(opcua_variable_t* self, char* s);

/**
* @brief Sets the datatype value for variable objects
* 
* @param self pointer to object to be set
* @param data_type Data type string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_data_type(opcua_variable_t* self, char* data_type);

/**
* @brief Sets the user access level value for variable objects
* 
* @param self pointer to object to be set
* @param ual User access level value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_user_access_level(opcua_variable_t* self, int ual);

/**
* @brief Sets the access level value for variable objects
* 
* @param self pointer to object to be set
* @param al Access level value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_access_level(opcua_variable_t* self, int al);

/**
* @brief Sets the array dimensions value for variable objects
* 
* @param self pointer to object to be set
* @param ad Array dimension value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_array_dimensions(opcua_variable_t* self, int ad);

/**
* @brief Sets the value rank value for variable objects
* 
* @param self pointer to object to be set
* @param vr Value rank value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_value_rank(opcua_variable_t* self, int vr);

/**
* @brief Sets the browse name for variable objects
* 
* @param self pointer to object to be set
* @param browse_name String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_browse_name(opcua_variable_t* self, 
    char* browse_name);

/**
* @brief Sets the display name for variable objects
* 
* @param self pointer to object to be set
* @param display_name String representation of display name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_display_name(opcua_variable_t* self, 
    char* display_name);

/**
* @brief Sets the description for variable objects
* 
* @param self pointer to object to be set
* @param description Description string
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_variable_description(opcua_variable_t* self,
        char* description);

/**
* @brief Sets the parent node id ns value for method objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_parent_node_id_ns(opcua_method_t* self, int ns);

/**
* @brief Sets the parent node id i value for method objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_parent_node_id_i(opcua_method_t* self, int i);

/**
* @brief Sets the parent node id s value for method objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_parent_node_id_s(opcua_method_t* self, char* s);

/**
* @brief Sets the node id ns value for method objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_node_id_ns(opcua_method_t* self, int ns);

/**
* @brief Sets the node id i value for method objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_node_id_i(opcua_method_t* self, int i);

/**
* @brief Sets the node id s value for method objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_node_id_s(opcua_method_t* self, char* s);

/**
* @brief Sets the method declaration id value for method objects
* 
* @param self pointer to object to be set
* @param id Declaration ID value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_declaration_id(opcua_method_t* self, int id);

/**
* @brief Sets the browse name value for method objects
* 
* @param self pointer to object to be set
* @param browse_name String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_browse_name(opcua_method_t* self, char* browse_name);

/**
* @brief Sets the display name value for method objects
* 
* @param self pointer to object to be set
* @param display_name String representation of display name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_method_display_name(opcua_method_t* self, char* display_name);

/**
* @brief Sets the parent node id ns value for object objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_parent_node_id_ns(opcua_object_t* self, int ns);

/**
* @brief Sets the parent node id i value for object objects
* 
* @param self pointer to object to be set
* @param I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_parent_node_id_i(opcua_object_t* self, int i);

/**
* @brief Sets the parent node id s value for object objects
* 
* @param self pointer to object to be set
* @param S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_parent_node_id_s(opcua_object_t* self, char* s);

/**
* @brief Sets the node id ns value for object objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_node_id_ns(opcua_object_t* self, int ns);

/**
* @brief Sets the node id i value for object objects
* 
* @param self pointer to object to be set
* @param I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_node_id_i(opcua_object_t* self, int i);

/**
* @brief Sets the node id s value for object objects
* 
* @param self pointer to object to be set
* @param S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_node_id_s(opcua_object_t* self, char* s);

/**
* @brief Sets the browse name value for object objects
* 
* @param self pointer to object to be set
* @param browse_name String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_browse_name(opcua_object_t* self, char* browse_name);

/**
* @brief Sets display name value for object objects
* 
* @param self pointer to object to be set
* @param display_name String representation of display name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_object_display_name(opcua_object_t* self, char* display_name);

/**
* @brief Sets the reference type for reference objects
* 
* @param self pointer to object to be set
* @param type String representation of reference type
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_reference_type(opcua_reference_t* self, char* type);

/**
* @brief Sets the id ns value for reference objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_reference_id_ns(opcua_reference_t* self, int ns);

/**
* @brief Sets the id i value for reference objects
* 
* @param self pointer to object to be set
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_reference_id_i(opcua_reference_t* self, int i);

/**
* @brief Sets the id s value for reference objects
* 
* @param self pointer to object to be set
* @param s S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_reference_id_s(opcua_reference_t* self, char* s);

/**
* @brief Sets the is forward value for reference objects
* 
* @param self pointer to object to be set
* @param val Boolean value
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_reference_is_forward(opcua_reference_t* self, bool val);

/**
* @brief Sets the parent node id ns value for attribute objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_parent_node_id_ns(
        opcua_node_attributes_t* self, int ns);

/**
* @brief Sets the parent node id i value for attribute objects
* 
* @param self pointer to object to be set
* @param I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_parent_node_id_i(opcua_node_attributes_t* self, int i);

/**
* @brief Sets the parent node id s value for attribute objects
* 
* @param self pointer to object to be set
* @param S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_parent_node_id_s(opcua_node_attributes_t* self, 
        char* s);

/**
* @brief Sets the node id ns value for attribute objects
* 
* @param self pointer to object to be set
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_node_id_ns(opcua_node_attributes_t* self, int ns);

/**
* @brief Sets the node id i value for attribute objects
* 
* @param self pointer to object to be set
* @param I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_node_id_i(opcua_node_attributes_t* self, int i);

/**
* @brief Sets the node id s value for attribute objects
* 
* @param self pointer to object to be set
* @param S string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_node_id_s(opcua_node_attributes_t* self, char* s);

/**
* @brief Sets the browse name value for attribute objects
* 
* @param self pointer to object to be set
* @param browse_name String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_browse_name(opcua_node_attributes_t* self, 
        char* browse_name);

/**
* @brief Sets display name value for attribute objects
* 
* @param self pointer to object to be set
* @param display_name String representation of display name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_set_attribute_display_name(opcua_node_attributes_t* self, 
        char* display_name);

/**
* @brief Creates a reference node from the reference object
* 
* @param object Pointer to object type object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_create_node_object_type_references(opcua_object_type_t* object);

/**
* @brief Creates a method node from the method object
* 
* @param method Pointer to method object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_create_node_method_references(opcua_method_t* method);

/**
* @brief Creates a variable node from the variable object
* 
* @param variable Pointer to variable object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_create_node_variable_references(opcua_variable_t* variable);

/**
* @brief Creates an object node from the object object
* 
* @param object Pointer to object object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_create_node_object_references(opcua_object_t* object);

/**
* @brief Generalised function to set the browse name of an object
* 
* @param object Pointer to object
* @param type Object type
* @param String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_browse_name(void* object, DL_OPCUA_TYPE_t type,
        char* browse_name);

/**
* @brief Generalised function to set the display name of an object
* 
* @param object Pointer to object
* @param type Object type
* @param display_name Display name string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_display_name(void* object, DL_OPCUA_TYPE_t type, 
        char* display_name);

/**
* @brief Sets the s value of an id object
* 
* @param id Pointer to ID object
* @param value String representation of the S to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_s(opcua_ns_id_t* id, char* value);

/**
* @brief Sets the ns value of an id object
* 
* @param id Pointer to ID object
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_ns(opcua_ns_id_t* id, int ns);

/**
* @brief Sets the i value of an id object
* 
* @param id Pointer to ID object
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_i(opcua_ns_id_t* id, int i);

/**
* @brief Creates node attributes for a given XML node
* 
* @param node XML node that is to have the att
* @param attributes Attributes that are the be added to the XML node
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr node, 
        opcua_node_attributes_t* attributes);

/**
* @brief Creates node type specific attributes for a given XML node
* 
* @param object_t pointer to object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object_type_attributes(
        opcua_object_type_t* object_t);

/**
* @brief Creates node type specific attributes for a given XML node
* 
* @param method pointer to object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_method_attributes(opcua_method_t* method);

/**
* @brief Creates node type specific attributes for a given XML node
* 
* @param variable pointer to object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_variable_attributes(opcua_variable_t* variable);

/**
* @brief Creates node type specific attributes for a given XML node
* 
* @param object pointer to object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object_attributes(opcua_object_t* object);

/**
* @brief Creates a <UAObjectType> node and populates it from an array
* 
* @param array Pointer to array storing initialisation references
* @param attributes Pointer to attributes object that stores the
* <UAObjectType> nodes initialisation attribute values
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object_type_from_array(
        opcua_reference_t* array, opcua_node_attributes_t* attributes);

/**
* @brief Creates a XML node from an object type object, also allocates object specific attributes
* and general attributes.
* 
* @param object Pointer to object type object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object_type(opcua_object_type_t* object);

/**
* @brief Creates a XML node from a method object, also allocates object specific attributes
* and general attributes.
* 
* @param method Pointer to method object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_method(opcua_method_t* method);

/**
* @brief Creates a XML node from a  variable, also allocates object specific attributes
* and general attributes.
* 
* @param variable Pointer to variable object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_variable(opcua_variable_t* variable);

/**
* @brief Creates a XML node from an object object, also allocates object specific attributes
* and general attributes.
* 
* @param object Pointer to object object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_object(opcua_object_t* object);

/**
* @brief Clears a reference object
* 
* @param ref Pointer to reference object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_clear_reference(opcua_reference_t* ref);

/**
* @brief Adds a reference object to a node object
* 
* @param reference Reference object to be added
* @param object Node object that the reference object should be added to
* @param type Object type
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_add_reference(opcua_reference_t* reference,
        void* object, DL_OPCUA_TYPE_t object_type);

/**
* @brief Adds attributes to a reference node
* 
* @param reference_node XML node that parent references node
* @param reference Pointer to reference object
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_add_reference_attributes(xmlNodePtr references_node,
        opcua_reference_t* reference);

/**
* @brief Creates XML nodes for all the reference objects stored in a node 
* objects reference linked list
* 
* @param object Pointer to node object
* @param type Object type
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_references(void* object, 
        DL_OPCUA_TYPE_t object_type);

/**
* @brief Adds ID attributes to a XML node's contents
* 
* @param node Pointer to XML node
* @param id Pointer to ID object to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_add_id_contents(xmlNodePtr node, opcua_ns_id_t* id);

/**
* @brief Adds ID attributes to a XML node's attributes
* 
* @param node Pointer to XML node
* @param attribute Name of the attribute to have the ID added to
* @param id Pointer to ID object to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_add_id_attribute(xmlNodePtr node, char* attribute,
        opcua_ns_id_t* id);

/**
* @brief Creates an opcua_node_attributes_t object
* 
* @return opcua_node_attributes_t* Pointer to newly created opcua_node_attributes_t object
*/
opcua_node_attributes_t* datalog_opcua_create_attributes(void);

/**
* @brief Creates an opcua_object_type_attributes_t object
* 
* @return opcua_object_type_attributes_t* Pointer to newly created 
* opcua_object_type_attributes_t object
*/
opcua_object_type_attributes_t* datalog_opcua_create_object_type_attributes(void);

/**
* @brief Creates an opcua_method_attributes_t object
* 
* @return opcua_method_attributes_t* Pointer to newly created opcua_method_attributes_t object
*/
opcua_method_attributes_t* datalog_opcua_create_method_attributes(void);

/**
* @brief Creates an opcua_variable_attributes_t object
* 
* @return opcua_variable_attributes_t* Pointer to newly created opcua_variable_attributes_t object
*/
opcua_variable_attributes_t* datalog_opcua_create_variable_attributes(void);

/**
* @brief Creates an opcua_object_attributes_t object
* 
* @return opcua_object_attributes_t* Pointer to newly created opcua_object_attributes_t object
*/
opcua_object_attributes_t* datalog_opcua_create_object_attributes(void);

/**
* @brief Creates an opcua_object_type_t object
* 
* @return opcua_object_type_t* Pointer to newly created opcua_object_type_t object
*/
opcua_object_type_t* datalog_opcua_create_object_type(void);

/**
* @brief Creates an opcua_method_t object
* 
* @return opcua_method_t* Pointer to newly created opcua_method_t object
*/
opcua_method_t* datalog_opcua_create_method(void);

/**
* @brief Creates an opcua_variable_t object
* 
* @return opcua_variable_t* Pointer to newly created opcua_variable_t object
*/
opcua_variable_t* datalog_opcua_create_variable(void);

/**
* @brief Creates an opcua_object_t object
* 
* @return opcua_object_t* Pointer to newly created opcua_object_t object
*/
opcua_object_t* datalog_opcua_create_object(void);

/**
* @brief Creates an opcua_reference_t object
* 
* @return opcua_reference_t* Pointer to newly created opcua_reference_t object
*/
opcua_reference_t* datalog_opcua_create_reference(void);

/**
* @brief Entry function to the OPCUA XML generator
* 
* @return void
*/
void datalog_opcua_runtime(void);

#endif
