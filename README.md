# Datalog API

_windows users you are on your own_

## Introduction
The original implementation of the LUA/C library is not the easiest to work with and does not lead to logical or easy to follow code. I have quickly thrown this API together to make the coding datalog coding style similar to that of the Python implementation. 

## Prerequisites 
The LUA/C library can be found [here](http://datalog.sourceforge.net) and must be downlaoded and placed into the libs folder such that the file tree looks like this  
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
+ Clauses with bodies
+ Error checking on literal types
+ Check that all methods are implemented for all objects, both with and without structs.
+ Lots more stuff I am to yet to discover

## Objects 
The API revolves around a struct objects to enable a more logical way or representing datalog commands rather than the push and pop confusion found in the library.  

Literals are represented by the object __datalog_literal_t__, queries by __datalog_query_t__ and clauses by __datalog_clause_t__. I may of missed a few methods to be implemented but I should get these done as I actually use this API for other code. 
 
