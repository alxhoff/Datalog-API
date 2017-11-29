/**
 * @file        datalog_opcua_generator.h
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       OPCUA Server XML Generator
 *
 * This OPC UA server XML generator is a dumb generator to assist in the
 * creation of XML representations of OPC UA servers. It revolves around
 * objective C styles where objects are created and a number of object
 * methods can be called to set various parameters within the object. 
 * Once an object is properly set up it can be turned into an XML 
 * representation of itself.
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
#include <stdint.h>

#ifndef bool
#define bool signed char
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

/*
* @brief Filename which will be used when outputting the generate OPC UA XML
*/
#define XML_FILENAME        "test_doc.xml"
/*
* @brief Version string which is attached to the OPC UA XML document upon creation
*/
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
    DL_OPC_VAR_TYPE,
    DL_OPC_REF_TYPE,
    DL_OPC_DATA_TYPE,
    DL_OPC_VIEW,
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
    char* name;                 /**< Alias pair name*/
    int integer;                /**< Alias pair integer value*/
}alias_pair_t;

/**
* @typedef opcua_container_t
* @brief Typdef for opcua_container
*/
typedef struct opcua_container opcua_container_t;
/**
* @struct opcua_container
* @brief Linked list container
*/
struct opcua_container{
    opcua_container_t* next;    /**< Linked list container*/
    void* contents;             /**< Pointer to contents of container, next_type specifies the cast of contents*/    
    DL_OPCUA_TYPE_t next_type;  /**< Type of object stored within the container*/
};

/**
* @typedef opcua_document_t
* @brief Typdef for opcua_document
*/
/**
* @struct opcua_document
* @brief Object representing a OPCUA XML document to be generated
*/
typedef struct opcua_document{
    xmlDocPtr document;         /**< Pointer to document data type used by libxml*/
    xmlNodePtr root_node;       /**< Pointer to the root XML node in the document, used to create root node children*/
    char* filename;             /**< Filename string to be used when saving the document*/
    char* version;              /**< Version string to be used when creating the document*/
}opcua_document_t;

/**
* @typedef opcua_node_id_t
* @brief Typdef for opcua_node_id
*/
typedef struct opcua_node_id opcua_node_id_t;

/**
* @struct opcua_node_id
* @brief Stores the attribute values for ID's such as ParentNodeId and
* NodeId
*/
struct opcua_node_id{
    int ns;         /**< NamespaceIndex*/
    int i;          /**< numeric identifier*/
    char* s;        /**< string identifier*/
    char* o;        /**< opaque identifier*/
    char* g;        /**< GUID identifier*/ 

    DL_OPCUA_ERR_t (*set_ns)(opcua_node_id_t*,int);
    DL_OPCUA_ERR_t (*set_i)(opcua_node_id_t*,int);
    DL_OPCUA_ERR_t (*set_s)(opcua_node_id_t*,char*);
    DL_OPCUA_ERR_t (*set_o)(opcua_node_id_t*,char*);
    DL_OPCUA_ERR_t (*set_g)(opcua_node_id_t*,char*);
};

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
    char* type;                 /**< Reference type*/
    opcua_node_id_t id;           /**< Reference ID attributes*/
    bool is_forward;            /**< Reference "IsForward" attribute*/

    opcua_reference_t* next;    /**< Next reference if the reference has already been added to an object's 
                                     linked list of references*/

    DL_OPCUA_ERR_t (*set_id_ns)(opcua_reference_t*,int);        /**< Sets the reference's ID ns attribute*/
    DL_OPCUA_ERR_t (*set_id_i)(opcua_reference_t*,int);         /**< Sets the reference's ID i attribute*/
    DL_OPCUA_ERR_t (*set_id_s)(opcua_reference_t*,char*);       /**< Sets the reference's ID s attribute*/
    DL_OPCUA_ERR_t (*set_type)(opcua_reference_t*,char*);       /**< Sets the reference's "ReferenceType"     
                                                                    attribute*/
    DL_OPCUA_ERR_t (*set_is_forward)(opcua_reference_t*,bool);  /**< Sets the reference's "IsForward" 
                                                                    attribute*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_reference_t*,void*,   /**< Adds the reference to the linked list of
                                                                    an object*/
        DL_OPCUA_TYPE_t);
    DL_OPCUA_ERR_t (*free_reference)(opcua_reference_t**);       /**< Frees the reference object from memory*/
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
    opcua_node_id_t parent_node_id;   /**< ID information for a node's attribute "ParentNodeId"*/
    opcua_node_id_t node_id;          /**< ID information for a node's attribute "NodeId"*/
    char* browse_name;              /**< Node's "BrowseNode" attribute string*/
    char* display_name;             /**< Node's "DisplayName" attribute string*/
    char* description;              /**< localized texual description of the node*/
    uint32_t user_access_level;     /**< "UserAccessLevel" attribute*/
    uint32_t access_level;          /**< "AccessLevel" attribute*/

    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_node_attributes_t*,int);   /**< Sets the node's parent ID ns 
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_node_attributes_t*,int);    /**< Sets the node's parent ID i
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_node_attributes_t*,char*);  /**< Sets the node's parent ID s
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_node_attributes_t*,int);     /**< Sets the node's node ID ns
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_node_attributes_t*,int);      /**< Sets the node's node ID i
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_node_attributes_t*,char*);    /**< Sets the node's node ID s
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_node_attributes_t*,int);                                                                        
    DL_OPCUA_ERR_t (*set_access_level)(opcua_node_attributes_t*,int);                                                                        
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_node_attributes_t*,char*);  /**< Sets the node's browse name
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_node_attributes_t*,char*); /**< Sets the node's display name
                                                                            attribute*/
    DL_OPCUA_ERR_t (*set_description)(opcua_node_attributes_t*,char*);  /**< Sets the attribute's description*/
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
    bool is_abstract; 
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
    int method_declaration_id;      /**< <UAMethod>'s method declaration ID attribute*/
}opcua_method_attributes_t;

typedef struct opcua_data_type opcua_data_type_t;

/**
* @typedef opcua_variable_attributes_t
* @brief Typdef for opcua_variable_attributes
*/
/**
* @struct opcua_variable_attributes
* @brief Attributes specific to a <UAVariable> node
*/
typedef struct opcua_variable_attributes{
    void* value;
    opcua_node_id_t data_type;     /**< Variable's "DataType" attribute*/ 
    uint32_t array_dimensions;      /**< Variable's "ArrayDimensions" attribute*/
    int32_t value_rank;             /**< Variable's "ValueRank" attribute*/
    bool is_abstract;               /**< */ 
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
    unsigned char event_notifier; 
}opcua_object_attributes_t;

/**
* @typedef 
* @brief Typdef for 
*/
/**
* @struct 
* @brief Attributes specific to a 
*/
typedef struct opcua_variable_type_attributes{
    void* value;
    opcua_node_id_t data_type;
    uint32_t array_dimensions;
    int32_t value_rank;
    bool is_abstract;
}opcua_variable_type_attributes_t;
    
/**
* @typedef 
* @brief Typdef for 
*/
/**
* @struct 
* @brief Attributes specific to a 
*/
typedef struct opcua_reference_type_attributes{
    bool is_abstract;
    bool symmetric; 
    char* inverse_name; 
}opcua_reference_type_attributes_t;

/**
* @typedef 
* @brief Typdef for 
*/
/**
* @struct 
* @brief Attributes specific to a 
*/
typedef struct opcua_data_type_attributes{
    bool is_abstract; 
}opcua_data_type_attributes_t;

/**
* @typedef 
* @brief Typdef for 
*/
/**
* @struct 
* @brief Attributes specific to a 
*/
typedef struct opcua_view_attributes{
    bool contains_no_loops; 
    unsigned char event_notifier; 
}opcua_view_attributes_t;

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
    xmlNodePtr node;                /**< Pointer to <UAObjectType> XML node*/
    xmlNodePtr references_node;     /**< Pointer to <References> XML node*/
    xmlNodePtr display_name_node;   /**< Pointer to <DisplayName> XML node*/

    opcua_node_attributes_t* attributes;                    /**< Pointer to the object's general attributes*/
    opcua_object_type_attributes_t* object_type_attributes; /**< Pointer to the object's specific attributes*/
    opcua_reference_t* reference_head;                      /**< Pointer to the head of the object's reference linked list*/
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_object_type_t*,int);       /**< Sets the object's parent ID ns attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_object_type_t*,int);        /**< Sets the object's parent ID i attirbute*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_object_type_t*,char*);      /**< Sets the object's parent ID s attribute*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_object_type_t*,int);         /**< Sets the object's node ID ns attribute*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_object_type_t*,int);          /**< Sets the object's node ID i attribute*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_object_type_t*,char*);        /**< Sets the object's node ID s attribute*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_object_type_t*,char*);      /**< Sets the object's browse name attribute*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_object_type_t*,char*);     /**< Sets the object's display name attribute*/
    DL_OPCUA_ERR_t (*set_description)(opcua_object_type_t*,char*);      /**< Sets the object type's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_object_type_t*,int);  /**< Sets the objects's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_object_type_t*,int);       /**< Sets the objects's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_object_type_t*);          /**< Creates <Reference> nodes from all the
                                                                            reference objects in the object's reference list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_object_type_t*,   
                                    opcua_reference_t*);                /**< Adds a reference object to the object's
                                                                            references linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_object_type_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_is_abstract)(opcua_object_type_t*,bool);       //TODO
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
    xmlNodePtr node;                                /**< Pointer to <UAMethod> XML node */
    xmlNodePtr references_node;                     /**< Pointer to <Reference> XML node*/
    xmlNodePtr display_name_node;                   /**< Pointer to <DisplayName> XML node*/

    opcua_node_attributes_t* attributes;            /**< Pointer to the method's general attributes*/
    opcua_method_attributes_t* method_attributes;   /**< Pointer to the method's specific attributes*/
    opcua_reference_t* reference_head;              /**< Pointer to the head of the object's reference linked list*/
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_method_t*,int);        /**< Sets the method's parent ID ns attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_method_t*,int);         /**< Sets the method's parent ID i attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_method_t*,char*);       /**< Sets the method's parent s attribute*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_method_t*,int);          /**< Sets the method's node ns attribute*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_method_t*,int);           /**< Sets the method's node i attribute*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_method_t*,char*);         /**< Sets the method's node s attribute*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_method_t*,char*);       /**< Sets the method's drowse name attribute*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_method_t*,char*);      /**< Sets the method's display name attribute*/
    DL_OPCUA_ERR_t (*set_description)(opcua_method_t*,char*);       /**< Sets the method's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_method_t*,int);   /**< Sets the method's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_method_t*,int);        /**< Sets the method's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_method_t*);           /**< Creates <Reference nodes from all the 
                                                                        reference objects in the object's reference list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_method_t*, 
                                    opcua_reference_t*);            /**< Add a reference object to the object's
                                                                        references linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_method_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_declaration_id)(opcua_method_t*,int);      /**< Sets the method's declaration ID attribute*/
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
    xmlNodePtr node;                                    /**< Pointer to <UAVariable> XML node */
    xmlNodePtr references_node;                         /**< Pointer to <Reference> XML node*/
    xmlNodePtr display_name_node;                       /**< Pointer to <DisplayName> XML node*/
        
    opcua_node_attributes_t* attributes;                /**< Pointer to the variable's general attributes*/
    opcua_variable_attributes_t* variable_attributes;   /**< Pointer to the variable's specific attributes*/
    opcua_reference_t* reference_head;                  /**< Pointer to the head of the object's reference linked list*/

    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_variable_t*,int);          /**< Sets the variable's parent ID ns attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_variable_t*,int);           /**< Sets the variable's parent ID i attribute*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_variable_t*,char*);         /**< Sets the variable's parent ID s attribute*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_variable_t*,int);            /**< Sets the variable's node ID ns attribute*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_variable_t*,int);             /**< Sets the variable's node ID i attribute*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_variable_t*,char*);           /**< Sets the variable's node ID s attribute*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_variable_t*,char*);         /**< Sets the variable's browse name attribute*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_variable_t*,char*);        /**< Sets the variable's display name attribute*/
    DL_OPCUA_ERR_t (*set_description)(opcua_variable_t*,char*);         /**< Sets the variable's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_variable_t*,int);     /**< Sets the variable's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_variable_t*,int);          /**< Sets the variable's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_variable_t*);             /**< Creates <Reference nodes from all the 
                                                                        reference objects in the object's reference list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_variable_t*, 
                                    opcua_reference_t*);                /**< Add a reference object to the object's
                                                                            references linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_variable_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_value)(opcua_variable_t*,void*);               //TODO
    DL_OPCUA_ERR_t (*set_data_type)(opcua_variable_t*,opcua_node_id_t*);           /**< Sets the variable's data type attribute*/
    DL_OPCUA_ERR_t (*set_array_dimensions)(opcua_variable_t*,int);      /**< Sets the variable's array dimensions attribute*/
    DL_OPCUA_ERR_t (*set_value_rank)(opcua_variable_t*,int32_t);            /**< Sets the variable's value rank attribute*/
    DL_OPCUA_ERR_t (*set_is_abstract)(opcua_variable_t*,bool);          //TODO
    
    DL_OPCUA_ERR_t (*free_variable)(opcua_variable_t**);
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
    xmlNodePtr node;                                /**< Pointer to <UAObject> node*/
    xmlNodePtr references_node;                     /**< Pointer to <References> node, this is the parent for <Reference> items*/
    xmlNodePtr display_name_node;                   /**< Pointer to <DisplayName> node*/
    
    opcua_node_attributes_t* attributes;            /**< <UAObject>'s general attributes*/
    opcua_object_attributes_t* object_attributes;   /**< <UAObject>'s type specific attributes*/
    opcua_reference_t* reference_head;              /**< Pointer to the head of the object's references linked list*/

    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_object_t*,int);            /**< Sets the ParentNodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_object_t*,int);             /**< Sets the ParentNodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_object_t*,char*);           /**< Sets the ParentNodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_object_t*,int);              /**< Sets the NodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_object_t*,int);               /**< Sets the NodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_object_t*,char*);             /**< Sets the NodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_object_t*,char*);           /**< Sets the BrowseName attribute's property*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_object_t*,char*);          /**< Sets the DisplayName node's contents*/
    DL_OPCUA_ERR_t (*set_description)(opcua_object_t*,char*);           /**< Sets the object's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_object_t*,int);       /**< Sets the objects's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_object_t*,int);            /**< Sets the objects's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_object_t*);               /**< Creates <Reference> nodes for each reference in 
                                                                            the object's references linked list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_object_t*, 
                                    opcua_reference_t*);                /**< Adds a reference object to the object's references 
                                                                                linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_object_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_event_notifier)(opcua_object_t*, unsigned char); //TODO
};

typedef struct opcua_variable_type opcua_variable_type_t;

struct opcua_variable_type{
    xmlNodePtr node;
    xmlNodePtr references_node;
    xmlNodePtr display_name_node;

    opcua_node_attributes_t* attributes;
    opcua_variable_type_attributes_t* variable_type_attributes;
    opcua_reference_t* reference_head;              /**< Pointer to the head of the variable type's references linked list*/
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_variable_type_t*,int);             /**< Sets the ParentNodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_variable_type_t*,int);              /**< Sets the ParentNodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_variable_type_t*,char*);            /**< Sets the ParentNodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_variable_type_t*,int);               /**< Sets the NodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_variable_type_t*,int);                /**< Sets the NodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_variable_type_t*,char*);              /**< Sets the NodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_variable_type_t*,char*);            /**< Sets the BrowseName attribute's property*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_variable_type_t*,char*);           /**< Sets the DisplayName node's contents*/
    DL_OPCUA_ERR_t (*set_description)(opcua_variable_type_t*,char*);            /**< Sets the variable type's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_variable_type_t*,int);        /**< Sets the variable type's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_variable_type_t*,int);             /**< Sets the variable type's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_variable_type_t*);                 /**< Creates <Reference> nodes for each reference in the 
                                                                                    variable type's references linked list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_variable_type_t*, 
                                    opcua_reference_t*);                        /**< Adds a reference variable_type to the variable type's 
                                                                                        references linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_variable_type_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_value)(opcua_variable_type_t*,void*);                  //TODO
    DL_OPCUA_ERR_t (*set_data_type)(opcua_variable_type_t*,opcua_node_id_t*);   //TODO
    DL_OPCUA_ERR_t (*set_array_dimensions)(opcua_variable_type_t*,uint32_t);    //TODO
    DL_OPCUA_ERR_t (*set_value_rank)(opcua_variable_type_t*,int32_t);           //TODO
    DL_OPCUA_ERR_t (*set_is_abstract)(opcua_variable_type_t*,bool);             //TODO
};

typedef struct opcua_reference_type opcua_reference_type_t;
    
struct opcua_reference_type{
    xmlNodePtr node;
    xmlNodePtr references_node;
    xmlNodePtr display_name_node;

    opcua_node_attributes_t* attributes;
    opcua_reference_type_attributes_t* reference_type_attributes;
    opcua_reference_t* reference_head;                                      /**< Pointer to the head of the reference type's references linked list*/
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_reference_type_t*,int);        /**< Sets the ParentNodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_reference_type_t*,int);         /**< Sets the ParentNodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_reference_type_t*,char*);       /**< Sets the ParentNodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_reference_type_t*,int);          /**< Sets the NodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_reference_type_t*,int);           /**< Sets the NodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_reference_type_t*,char*);         /**< Sets the NodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_reference_type_t*,char*);       /**< Sets the BrowseName attribute's property*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_reference_type_t*,char*);      /**< Sets the DisplayName node's contents*/
    DL_OPCUA_ERR_t (*set_description)(opcua_reference_type_t*,char*);       /**< Sets the reference type's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_reference_type_t*,int);   /**< Sets the reference type's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_reference_type_t*,int);        /**< Sets the reference type's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_reference_type_t*);           /**< Creates <Reference> nodes for each reference in the 
                                                                                reference type's references linked list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_reference_type_t*, 
                                    opcua_reference_t*);                    /**< Adds a reference reference_type to the reference 
                                                                                type's references linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_reference_type_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_is_abstract)(opcua_reference_type_t*,bool);        //TODO
    DL_OPCUA_ERR_t (*set_symmetric)(opcua_reference_type_t*,bool);          //TODO
    DL_OPCUA_ERR_t (*set_inverse_name)(opcua_reference_type_t*,char*);      //TODO
};

struct opcua_data_type{
    xmlNodePtr node;
    xmlNodePtr references_node;
    xmlNodePtr display_name_node;
    
    opcua_node_attributes_t* attributes;
    opcua_data_type_attributes_t* data_type_attributes;
    opcua_reference_t* reference_head;                                  /**< Pointer to the head of the data type's references linked list*/
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_data_type_t*,int);         /**< Sets the ParentNodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_data_type_t*,int);          /**< Sets the ParentNodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_data_type_t*,char*);        /**< Sets the ParentNodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_data_type_t*,int);           /**< Sets the NodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_data_type_t*,int);            /**< Sets the NodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_data_type_t*,char*);          /**< Sets the NodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_data_type_t*,char*);        /**< Sets the BrowseName attribute's property*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_data_type_t*,char*);       /**< Sets the DisplayName node's contents*/
    DL_OPCUA_ERR_t (*set_description)(opcua_data_type_t*,char*);        /**< Sets the data type's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_data_type_t*,int);    /**< Sets the data type's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_data_type_t*,int);         /**< Sets the data type's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_data_type_t*);            /**< Creates <Reference> nodes for each reference in the 
                                                                             data type's references linked list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_data_type_t*, 
                                    opcua_reference_t*);                /**< Adds a reference object to the data type's references 
                                                                            linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_data_type_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_is_abstract)(opcua_data_type_t*,bool);
};

typedef struct opcua_view opcua_view_t;
    
struct opcua_view{
    xmlNodePtr node;
    xmlNodePtr references_node;
    xmlNodePtr display_name_node;
    
    opcua_node_attributes_t* attributes;
    opcua_view_attributes_t* view_attributes;
    opcua_reference_t* reference_head;                          /**< Pointer to the head of the view's references linked list*/
    
    DL_OPCUA_ERR_t (*set_parent_id_ns)(opcua_view_t*,int);      /**< Sets the ParentNodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_parent_id_i)(opcua_view_t*,int);       /**< Sets the ParentNodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_parent_id_s)(opcua_view_t*,char*);     /**< Sets the ParentNodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_node_id_ns)(opcua_view_t*,int);        /**< Sets the NodeId attribute's ns property*/
    DL_OPCUA_ERR_t (*set_node_id_i)(opcua_view_t*,int);         /**< Sets the NodeId attribute's i property*/
    DL_OPCUA_ERR_t (*set_node_id_s)(opcua_view_t*,char*);       /**< Sets the NodeId attribute's s property*/
    DL_OPCUA_ERR_t (*set_browse_name)(opcua_view_t*,char*);     /**< Sets the BrowseName attribute's property*/
    DL_OPCUA_ERR_t (*set_display_name)(opcua_view_t*,char*);    /**< Sets the DisplayName node's contents*/
    DL_OPCUA_ERR_t (*set_description)(opcua_view_t*,char*);     /**< Sets the view's description attribute*/
    DL_OPCUA_ERR_t (*set_user_access_level)(opcua_view_t*,int); /**< Sets the view's user access level attribute*/
    DL_OPCUA_ERR_t (*set_access_level)(opcua_view_t*,int);      /**< Sets the view's access level attribute*/
    DL_OPCUA_ERR_t (*create_references)(opcua_view_t*);         /**< Creates <Reference> nodes for each reference in the 
                                                                     view's references linked list*/
    DL_OPCUA_ERR_t (*add_reference)(opcua_view_t*, 
                                    opcua_reference_t*);        /**< Adds a reference object to the view's references linked list*/
    DL_OPCUA_ERR_t (*create_node)(opcua_view_t*);                                                                                    
    DL_OPCUA_ERR_t (*set_contains_no_loops)(opcua_view_t*,bool);
    DL_OPCUA_ERR_t (*set_event_notifier)(opcua_view_t*,unsigned char);
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
DL_OPCUA_ERR_t self_set_variable_data_type(opcua_variable_t* self, opcua_node_id_t* ID);

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
DL_OPCUA_ERR_t self_set_variable_value_rank(opcua_variable_t* self, int32_t vr);

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
* @brief Adds a reference object to the references linked list of the object type object
* 
* @param object_t Pointer to the object type object to whom the reference shall be added
* @param reference Pointer to the reference object that is to be added
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_add_ref_object_type(opcua_object_type_t* object_t, 
        opcua_reference_t* reference);

/**
* @brief Adds a reference object to the references linked list of the method object 
* 
* @param object_t Pointer to the method object to whom the reference shall be added
* @param reference Pointer to the reference object that is to be added
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_add_ref_method(opcua_method_t* method,
        opcua_reference_t* reference);

/**
* @brief Adds a reference object to the references linked list of the variable object
* 
* @param object_t Pointer to the variable object to whom the reference shall be added
* @param reference Pointer to the reference object that is to be added
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_add_ref_variable(opcua_variable_t* variable,
        opcua_reference_t* reference);

/**
* @brief Adds a reference object to the references linked list of the object object
* 
* @param object_t Pointer to the object object to whom the reference shall be added
* @param reference Pointer to the reference object that is to be added
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t self_add_ref_object(opcua_object_t* object,
        opcua_reference_t* reference);
/**
* @brief Generalised function to set the browse name of an object
* 
* @param attribute Pointer to attribute object
* @param browse_name String representation of browse name
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_browse_name(opcua_node_attributes_t* attribute, 
char* browse_name);

/**
* @brief Generalised function to set the display name of an object
* 
* @param object Pointer to object
* @param type Object type
* @param display_name Display name string to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_display_name(opcua_node_attributes_t* attribute, 
        char* display_name);

/**
* @brief Sets the s value of an id object
* 
* @param id Pointer to ID object
* @param value String representation of the S to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_s(opcua_node_id_t* id, char* value);

/**
* @brief Sets the ns value of an id object
* 
* @param id Pointer to ID object
* @param ns NS value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_ns(opcua_node_id_t* id, int ns);

/**
* @brief Sets the i value of an id object
* 
* @param id Pointer to ID object
* @param i I value to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_set_id_i(opcua_node_id_t* id, int i);

DL_OPCUA_ERR_t datalog_opcua_set_description(opcua_node_attributes_t* attribute,
        char* description);

/**
* @brief Creates node attributes for a given XML node
* 
* @param node XML node that is to have the att
* @param attributes Attributes that are the be added to the XML node
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr node, 
        xmlNodePtr display_name_node, opcua_node_attributes_t* attributes);

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
* @brief Searches for and returns the first reference found with a matching
* i value
*
* @param object Pointer to object object
* @param type Type of object passed to function
* @param i ID i value that is to be searched for
* @return opcua_reference_t* Pointer to the matching reference object
*/
opcua_reference_t* datalog_opcua_find_reference_i(void* object,
        DL_OPCUA_TYPE_t type, int i);

/**
* @brief Searches for and returns the first reference found with a matching
* s string
* 
* @param object Pointer to object object
* @param type Type of object passed to function
* @param ID s string that is to be searched for
* @return opcua_reference_t* Pointer to the matching reference object
*/
opcua_reference_t* datalog_opcua_find_reference_s(void* object,
        DL_OPCUA_TYPE_t type, char* s);

/**
* @brief Searches for and returns the first reference found with a matching
* type string
* 
* @param object Pointer to object object
* @param type Type of object passed to function
* @param type_string Type string that is to be searched for
* @return opcua_reference_t* Pointer to the matching reference object
*/
opcua_reference_t* datalog_opcua_find_reference_type(void* object,
        DL_OPCUA_TYPE_t type, char* type_string);

/**
* @brief Searches and object's reference list for a reference with the specified
* ID information
* 
* @param object Pointer to object object
* @param type Type of object passed to function
* @param ID ID information that the returned reference should contain
* @return opcua_reference_t* Pointer to the matching reference object
*/
opcua_reference_t* datalog_opcua_find_reference(void* object,
        DL_OPCUA_TYPE_t type, opcua_node_id_t* ID);

/**
* @brief Frees all the references in an objects reference list 
* 
* @param object Pointer to object object
* @param type Type of object passed to function
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_free_reference_list(void** object, 
        DL_OPCUA_TYPE_t type);

/**
* @brief Frees a reference object
* 
* @param ref Pointer to the reference object pointer that is to be freed 
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_free_reference(opcua_reference_t** ref);

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
DL_OPCUA_ERR_t datalog_opcua_add_id_contents(xmlNodePtr node, opcua_node_id_t* id);

/**
* @brief Adds ID attributes to a XML node's attributes
* 
* @param node Pointer to XML node
* @param attribute Name of the attribute to have the ID added to
* @param id Pointer to ID object to be set
* @return DL_OPCUA_ERR_t error message
*/
DL_OPCUA_ERR_t datalog_opcua_add_id_attribute(xmlNodePtr node, char* attribute,
        opcua_node_id_t* id);

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
