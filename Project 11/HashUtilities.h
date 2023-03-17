#ifndef HASH_UTILITIES_H
#define HASH_UTILITIES_H

// header files
#include "StandardConstants.h"
#include <stdio.h>
#include <stdlib.h>

// constants
typedef enum { NO_PROBING, LINEAR_PROBING = 101, 
                                            QUADRATIC_PROBING = 202 } ProbeType;
extern const int MINIMUM_HASH_LETTER_COUNT;
extern const int ITEM_NOT_FOUND;
extern const bool USED_NODE;
extern const bool UNUSED_NODE;

// data structures
typedef struct StateStruct
   {
    char name[ STD_STR_LEN ];

    double averageTemp, lowestTemp, highestTemp;

    bool inUse;
   } StateDataType;

typedef struct HashStruct
   {
    StateDataType *array;

    int tableSize;

    ProbeType probeStrategy;
   } ProbingHashType;

// prototypes

/*
Name: addItemFromData
Process: adds item to hash table using data input,
         sets node to used state
Function input/parameters: hash data (ProbingHashType *), 
                           state name (const char *),
                           average, lowest, and highest temperatures (double),
Function output/parameters: updated hash table data (ProbingHashType *)
Function output/returned: result of operation (bool)
Device input/---: none
Device output/monitor: probing process displayed (in called function)
Dependencies: setHashNodeFromData, addItemFromStruct
*/
bool addItemFromData( ProbingHashType *hashTable, const char *stateName, 
                              double avgTemp, double lowTemp, double highTemp );

/*
Name: addItemFromStruct
Process: adds item to hash table using struct input,
         uses probing as specified in hash data,
         may probe as many as tableSize times
Function input/parameters: hash data (ProbingHashType *), 
                           new item (StateDataType)
Function output/parameters: updated hash table data (ProbingHashType *)
Function output/returned: result of operation (bool)
Device input/---: none
Device output/monitor: probing process displayed
Dependencies: getHashIndex, toPower, printf, dataToString,
              setHashNodeFromStruct
*/
bool addItemFromStruct( ProbingHashType *hashTable, StateDataType newItem );

/*
Name: clearHashTable
Process: clear hash table array, sets size to zero,
         sets probing to NO_PROBING, deallocates hash struct
Function input/parameters: hash data (ProbingHashType *)
Function output/parameters: updated hash table data (ProbingHashType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: free
*/
void clearHashTable( ProbingHashType *hashTable );

/*
Name: compareStates
Process: compares states by name
Function input/parameters: one state and other state (const StateDataType)
Function output/parameters: none
Function output/returned: value representing comparison,
                  greater than zero if first state is greater than second state,
                  less than zero if first state is less than second state,
                  zero if state names and lengths are the same,
                  otherwise, larger state name length is greater
Device input/---: none
Device output/---: none
Dependencies: getStringLength
*/
int compareStates( const StateDataType oneState, 
                                               const StateDataType otherState );

/*
Name: copyString
Process: copies string from source to destination
Function input/parameters: source string (const char *)
Function output/parameters: destination string (char *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: none
*/
void copyString( char *dest, const char *source );

/*
Name: dataToString
Process: creates string from StateDataType data
Function input/parameters: provided state data (const StateDataType)
Function output/parameters: output string (char *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: sprintf, or other options
*/
void dataToString( char *outStr, const StateDataType state );

/*
Name: displayHashTable
Process: array data dump of numbered data to screen, 
         shows data where used, shows "Unused" where not
Function input/parameters: provided hash data (const ProbingHashType *)
Function output/parameters: none
Function output/returned: none
Device input/---: none
Device output/monitor: displayed as specified
Dependencies: printf, dataToString
*/
void displayHashTable( const ProbingHashType *hashTable );

/*
Name: findItem
Process: finds item in hash table, returns
Function input/parameters: provided search data (const StateDataType),
                           hash (const ProbingHashType *)
Function output/parameters: none
Function output/returned: state data set to struct (StateDataType),
                          or search parameter set to empty node if not found
Device input/---: none
Device output/---: none
Dependencies: findItemIndex, setEmptyHashNode
*/
StateDataType findItem( const ProbingHashType *hashTable, StateDataType searchItem );

/*
Name: findItemIndex
Process: finds item index, using probing strategy provided in hash data,
         otherwise, 
         returns ITEM_NOT_FOUND if search fails after tableSize attempts,
         displays index probing attempts
Function input/parameters: provided search data (const StateDataType),
                           hash (const ProbingHashType *)
Function output/parameters: none
Function output/returned: index or ITEM_NOT_FOUND (int) as specified
Device input/---: none
Device output/monitor: displays probing action, provided in sample run file
Dependencies: getHashIndex, printf, compareStates, toPower, 
              setHashNodeFromStruct
*/
int findItemIndex( const ProbingHashType *hashTable, StateDataType searchItem );
    
/*
Name: getHashIndex
Process: finds hashed index for given data item,
         sums integer values of state name characters,
         if state name length is less than MINIMUM_HASH_LETTER_COUNT,
         repeats going over the state letters as needed to meet this minimum
Function input/parameters: provided search data (const StateDataType),
                           hash (const ProbingHashType)
Function output/parameters: none
Function output/returned: array index generated from state name (int)
Device input/---: none
Device output/---: none
Dependencies: getStringLength
*/
int getHashIndex( const ProbingHashType hashTable, const StateDataType state );

/*
Name: getStringLength
Process: utility for finding string length
Function input/parameters: given string (const char *)
Function output/parameters: none
Function output/returned: length of string (int)
Device input/---: none
Device output/---: none
Dependencies: none
*/
int getStringLength( const char *str );

/*
Name: initializeHash
Process: creates dynamically allocated hash, 
         creates dynamically allocated array of empty StateDataType items,
         initializes tableSize to given capacity,
         initializes probe strategy to given strategy
Function input/parameters: provided capacity (int),
                           provided probe strategy (ProbeType)
Function output/parameters: none
Function output/returned: pointer to created hash (ProbingHashType *)
Device input/---: none
Device output/---: none
Dependencies: malloc, sizeof, setEmptyHashNode, 
*/
ProbingHashType *initializeHashTable( int capacity, ProbeType probe );

/*
Name: removeState
Process: finds item in hash table, removes, 
         sets array location to unused (but does not set any other data),
         returns removed state
Function input/parameters: provided search data (const StateDataType),
                           hash (const ProbingHashType)
Function output/parameters: returned state (StateDataType *)
Function output/returned: Boolean result of action (bool)
Device input/---: none
Device output/---: none
Dependencies: findItemIndex, setHashNodeFromStruct
*/
bool removeState( StateDataType *removedState, 
             const StateDataType toBeRemoved, const ProbingHashType hashTable );

/*
Name: setEmptyHashNode
Process: sets given node values to empty string, zero values,
         and Boolean in use variable to false
Function input/parameters: none
Function output/parameters: updated state data struct (StateDataType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: copyString
*/
void setEmptyHashNode( StateDataType *node );

/*
Name: setHashNodeFromData
Process: sets given values into a given hash struct, 
         using individual struct data
Function input/parameters: provided name (const char *),
                           average, lowest, and highest temperatures (double),
                           and node in use state
Function output/parameters: updated state data pointer (StateDataType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: copyString
*/
void setHashNodeFromData( StateDataType *nodePtr, const char *name, 
              double avgTemp, double lowTemp, double highTemp, bool nodeState );

/*
Name: setHashNodeFromStruct
Process: sets given values into a given hash struct, 
         using another data struct
Function input/parameters: provided state data source struct 
                           (const StateDataType)
Function output/parameters: updated state data pointer (StateDataType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: setHashNodeFromData
*/
void setHashNodeFromStruct( StateDataType *nodePtr, 
                                                   const StateDataType source );

/*
Name: showHashTableStatus
Process: displays array <D>ata values and <U>unused values 
         as a string of letters,
         displays number of items in hash, 
         displays minimum number of contiguous data items,
         displays maximum number of contiguous data items,
         displays number of empty/unused array elements
Function input/parameters: hash (const ProbingHashType)
Function output/parameters: none
Function output/returned: none
Device input/---: none
Device output/monitor: hash data status displayed as specified
Dependencies: sprintf, printf
*/
void showHashTableStatus( const ProbingHashType hashTable );

/*
Name: toPower
Process: recursively calculates result of given base to given exponent,
         for exponents >= 0
Function input/parameters: base and exponent (int)
Function output/parameters: none
Function output/returned: result of specified calculations
Device input/---: none
Device output/---: none
Dependencies: toPower (recursively)
*/
int toPower( int base, int exponent );

#endif   // HASH_UTILITIES_H
