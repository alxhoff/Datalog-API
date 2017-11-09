# Datalog API

_windows users you are on your own_

## Introduction
The original implementation of the LUA/C library is not the easiest to work with and does not lead to logical or easy to follow code. I have quickly thrown this API together to make the datalog coding style similar to that of the Python implementation. 

## Prerequisites 
The LUA/C library can be found [here](http://datalog.sourceforge.net) and must be downloaded and placed into the libs folder such that the file tree looks like this  
```
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
```
## Building
I have included a demo main.c as well as a demo CMake that will build the API as a shared library that can then be linked into an exsisting project.  

### How to build
It's pretty tough....  
cd into the root director and create a build dir, or don't, it'll be your mess.  

```bash
 cd Datalog-API
 mkdir build
 cd build
```
 Generate CMake junk __or__ run ccmake to configure debugging options
```bash
 cmake ..
```
__or__
```bash
 ccmake ..
```
and finally make
```bash
 make
```
The executable can be found in the bin subdirectory in the root dir.  

Library objects in the build subdirectory.

## Documentation

To generate the documentation run the command

```bash
doxygen
```
from the doc folder. Then navigate to ./html/index.html 

## Work in progress
I wrote this quickly and dirtily, so excuse the mess.  

### To-Do  
+ ~~Fix demo project~~
+ ~~Integrate parser into API~~ Test parser wrapping
+ ~~Clauses with bodies~~
+ Error checking on literal types
+ Check that all methods are implemented for all objects, both with and without structs.
+ Lots more stuff I am to yet to discover

## Objects 
The API revolves around a struct objects to enable a more logical way or representing datalog commands rather than the push and pop confusion found in the library.  

Literals are represented by the object __datalog_literal_t__, queries by __datalog_query_t__ and clauses by __datalog_clause_t__. I may of missed a few methods to be implemented but I should get these done as I actually use this API for other code. 

# XML Parser (very Beta)

The XML parser is designed to be so that XML files can be parsed to a datalog program to load rules and facts into the datalog database. The parser is built around libxml2 and I built with gcc/CMake using stand alone libraries so as to ensure easier building. The parser works by calling the dl_parser_init function, specifying the file to be opened. This will initialise the document object used by the parser. 

## Literal objects
The parser represents literals using the dl_parser_literal_t object that stores the predicate and arguments for the literal. Literals are probably not used directly in the parser.

## Fact objects
 A fact in Datalog is a clause without a body, aka it's a standard literal. Facts are essentailly a literal stored in a container allowing them to be used as elements of a linked list as well as storing a reference to the XML node that corresponds to the fact. The parser will scan the entire document for facts, creating a linked list. This linked list is then traversed and each XML fact node is walked and processed so that each fact is represented by a dl_parser_fact_t object which contains the fact's literal.   

 The facts in the XML tree can be found by calling dl_parser_mappings as this will in turn walk the XML tree and call dl_parser_add_fact on all found facts. Found facts are then processed using dl_parser_process_fact. After processing the found facts can be found pointed to by the linked list head pointer dl_parser_fact_t* facts_head found within the dl_parser_doc_t object.

 ## Rule objects
 In Datalog rules are clauses that contain a body, the body being a potentially infinite length array of literals. In the XML parser rules are represented similarly to facts, using the dl_parser_rule_t object, with the exception that the rules object represents the body of the rule by an array of literal pointers. This list is dynamically allocated to allow for potentially infinite length rules. Parsing of rules happens in the same fashion as facts, in that a call to dl_parser_mappings will walk the XML tree and call dl_parser_add_rule on any found rules. These are then stores in a similar linked list fashion and processed through calls to dl_parser_process_rule whilst iterating over the linked list. The linked list of processed rules can be found using the head pointer dl_parser_rule_t* rules_head found in the dl_parser_doc_t object.

 ## Parser use
 Parser use should pretty much only require the user to call 

 ```c
 dl_parser_doc_t* dl_doc = dl_parser_init(filename);
 ```
 followed by   
 ```c
 dl_parser_mappings(dl_doc);
```

but one can also print any number of the objects in the library though the number of print functions. Document metadata can also be parsed by calling dl_parser_metadata which will populat a dl_parser_metadata_t object stored in the main dl_parser_doc_t object.   
 See included example for furthur usage.
