#include <stdio.h>

#include "datalog_opcua_generator.h"

void datalog_opcua_save_deinit_doc(char* filename, xmlDocPtr doc)
{
    xmlSaveFormatFileEnc( filename, doc, "UTF-8", 1);

    xmlFree(doc);

    xmlCleanupParser();

    xmlMemoryDump();
}

xmlDocPtr datalog_opcua_init_doc(void)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, tmp_node = NULL,
               tmp_parent = NULL, tmp_child = NULL;
    char buff[256];
    int i = 0;

    //create doc
    doc = xmlNewDoc(BAD_CAST "1.0");
    //root node
    root_node = xmlNewNode(NULL, BAD_CAST "UANodeSet");
    xmlDocSetRootElement(doc, root_node);

    //DTD
    xmlCreateIntSubset(doc, BAD_CAST "UANodeSet", NULL, BAD_CAST "dtdhere");

    //create namespace Uris
    tmp_node = xmlNewChild(root_node, NULL, BAD_CAST "NamespaceUris", NULL);
    xmlNewChild(tmp_node, NULL, BAD_CAST "Uri", 
            BAD_CAST "urn:UnifiedAutomation:CppDemoServer:BuildingAutomation");

    //create aliases
    tmp_parent = xmlNewChild(root_node, NULL, BAD_CAST "Aliases", NULL);
    
    while(alias_array[i].integer != 0){
        sprintf(buff, "i=%d", alias_array[i].integer);
        printf("Current pair: %s,%d\n", alias_array[i].name, alias_array[i].integer);
        tmp_child = xmlNewChild(tmp_parent, NULL, BAD_CAST "Alias", BAD_CAST buff);
        xmlNewProp(tmp_child, BAD_CAST "Alias",  BAD_CAST alias_array[i].name);
        i++;
    }

    return doc;
}

void datalog_opcua_runtime(void)
{
    printf("opcua runtime\n");

    xmlDocPtr doc = datalog_opcua_init_doc();

    if(doc == NULL) return;

    datalog_opcua_save_deinit_doc("test_doc.xml", doc);
}

