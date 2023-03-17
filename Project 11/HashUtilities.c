#include "HashUtilities.h"

// define constants
const int MINIMUM_HASH_LETTER_COUNT = 7;
const int ITEM_NOT_FOUND = -1;
const bool USED_NODE = true;
const bool UNUSED_NODE = false;

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
Dependencies: setHeapNodeFromData, addItemFromStruct
*/
bool addItemFromData( ProbingHashType *hash, const char *stateName, 
                              double avgTemp, double lowTemp, double highTemp )
  {
  // variables
  StateDataType newItem;
  
  // input data into new struct
  setHashNodeFromData( &newItem, stateName, avgTemp, 
                         lowTemp, highTemp, USED_NODE );   
  
  // pass to addItemFromStruct to prob and add at correct index
  return addItemFromStruct( hash, newItem );
  }

/*
Name: addItemFromStruct
Process: adds item to hash table using struct input,
         uses probing as specified in heap data,
         may probe as many as tableSize times
Function input/parameters: hash data (ProbingHashType *), 
                           new item (StateDataType)
Function output/parameters: updated hash table data (ProbingHashType *)
Function output/returned: result of operation (bool)
Device input/---: none
Device output/monitor: probing process displayed
Dependencies: getHashIndex, toPower, printf, dataToString,
              setHeapNodeFromStruct
*/
bool addItemFromStruct( ProbingHashType *hash, StateDataType newItem )
  {       
  // variables
  int hashIndex, index = 0, quadraticCounter = 0, 
                  linearCounter = 0, loopCounter = 0;
  char displayStr[ STD_STR_LEN ];

  // check for no prob strategy first
  if( hash->probeStrategy == NO_PROBING )
    {
    return false;
    }
  
  // get the hash index
  hashIndex = getHashIndex( *hash, newItem );
  
  // pre prime loop index
  index = hashIndex;

  // display indicies probed
  printf( "\nIndices probed: %d", index );    
        
  // loop while value not found at current index 
  // and value not equal to search value
  while( hash->array[ index ].inUse && loopCounter <= hash->tableSize) 
    {       
    // check for probing strategy
    // if linear
    if( hash->probeStrategy == LINEAR_PROBING )
      {
      // increment index
      linearCounter++;
       
      // update index 
      index = hashIndex + linearCounter;  
      }
  
    // otherwise assume quadratic
    else 
      {
      // increment quadraticCounter
      quadraticCounter++;
        
      // set index
      index = ( hashIndex + toPower( quadraticCounter, 2 ));
      }
      
    // mod the index, to verify within array
    index = index % hash->tableSize;
    
    // display index    
    printf( ", %d", index );
      
    // increment loopcounter
    loopCounter++;
    }
  
  // create display string
  dataToString( displayStr, newItem );
    
  // display string
  printf( "\n%s %d -> %d\n", displayStr, hashIndex, index );
  
  // add item at index found 
  setHashNodeFromStruct( &hash->array[ index ], newItem);

  // return sucess
  return true;
  }

/*
Name: clearHashTable
Process: clear hash table array, sets size to zero,
         sets probing to NO_PROBING
Function input/parameters: hash data (ProbingHashType *)
Function output/parameters: updated hash table data (ProbingHashType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: free
*/
void clearHashTable( ProbingHashType *hash )
  {
  // free memory of array
  free( hash->array );
  
  // set size of hash to 0
  hash->tableSize = 0;
  
  // set probing to NO_PROBING	
  hash->probeStrategy = NO_PROBING;
  
  // free the hash
  free( hash );
  }

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
                                               const StateDataType otherState )
  {
  // variables
  int oneIndex = 0, twoIndex = 0;
  
  // loop to iterate through both strings 
  while( oneState.name[ oneIndex ] && otherState.name[ twoIndex ] )
    {
    // compare character by character
    // if difference is found
    if( oneState.name[ oneIndex ] != otherState.name[ twoIndex ] )
      {
      // return difference
      return oneState.name[ oneIndex ] - otherState.name[ twoIndex];
      }
    
    // increment index's
    oneIndex++;
    twoIndex++;
    }
    
      
  // otherwise, if at end of shortest string
    // return difference in string lengths
  return getStringLength( oneState.name ) - 
             getStringLength( otherState.name );	
  }

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
void copyString( char *dest, const char *source )
  {
  // variables
  int index = 0;
  
  // iterate until hitting NULL_CHAR of source
  while( source[ index ] != NULL_CHAR )
    {
    // copy index to dest
	dest[index] = source[index];
	
	// increment index
	index++;
	
	// set dest at index to NULL_CHAR 	
    dest[index] = NULL_CHAR;
    }	
  }

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
void dataToString( char *outStr, const StateDataType state )
  {
  // copy string from stateDataType to outStr	
  sprintf( outStr, "%s, Avg: %5.2f, Low: %5.2f, High: %5.2f, ", 
    state.name, state.averageTemp, state.lowestTemp, state.highestTemp );	
  }

/*
Name: displayHashTable
Process: array data dump of numbered data to screen, 
         shows data where used, shows "Unused" where not
Function input/parameters: provided heap data (const ProbingHashType *)
Function output/parameters: none
Function output/returned: none
Device input/---: none
Device output/monitor: displayed as specified
Dependencies: printf, dataToString
*/
void displayHashTable( const ProbingHashType *hash )
  {
  // variables
  int index;
  char displayStr[ STD_STR_LEN ];
  
  // display title
  printf( "\nHASH TABLE ARRAY DUMP:\n" );
   
  // iterate thorugh array until hitting size
  for( index = 0; index < hash->tableSize; index++ )
    {
    printf( " %2d) ", index );    
        
    // check if data is in use
    if( hash->array[ index ].inUse )
      {
      dataToString( displayStr, hash->array[index] );    
          
      // display data at index
      printf( "%s\n", displayStr );
      }  
	  
    // otherwise
    else
      {
      // display not in use at index
      printf( "Unsused\n" );
      }	 	
    }  	
  }

/*
Name: findItem
Process: finds item in hash table, returns
Function input/parameters: provided search data (const StateDataType),
                           heap (const ProbingHashType *)
Function output/parameters: none
Function output/returned: state data set to struct (StateDataType),
                          or search parameter set to empty node if not found
Device input/---: none
Device output/---: none
Dependencies: findItemIndex, setEmptyHeapNode
*/
StateDataType findItem( const ProbingHashType *hash, StateDataType searchItem )
  {
  // variables
  int index;
  StateDataType emptyNode;
  
  // call findItemIndex to seach and find the wanted item
  index = findItemIndex( hash, searchItem );
  
  if( index != ITEM_NOT_FOUND )
    {
    // return data set to struct or search parameter set to empty node	
    return hash->array[index];
    }
  
  // otherwise item was not found
  // create empty node
  setEmptyHashNode( &emptyNode );
  
  // return set to empty node	
  return emptyNode;
  	
  }

/*
Name: findItemIndex
Process: finds item index, using probing strategy provided in heap data,
         otherwise, 
         returns ITEM_NOT_FOUND if search fails after tableSize attempts,
         displays index probing attempts
Function input/parameters: provided search data (const StateDataType),
                           heap (const ProbingHashType *)
Function output/parameters: none
Function output/returned: index or ITEM_NOT_FOUND (int) as specified
Device input/---: none
Device output/monitor: displays probing action, provided in sample run file
Dependencies: getHashIndex, printf, compareStates, toPower, 
              setHeapNodeFromStruct
*/
int findItemIndex( const ProbingHashType *hash, StateDataType searchItem )
  {
  // variables
  int hashIndex = 0, index = 0, iterations = 0;	
  ProbingHashType tempHash;
  	
  // get hash index
  hashIndex = getHashIndex( *hash, searchItem );
  
  // pre prime loop
  index = hashIndex;
  
  printf( "\nIndices probed: ", index );
  
  // loop 
  while( iterations != hash->tableSize )
    {       
    // check if in use
    if( hash->array[ index ].inUse && 
      compareStates( hash->array[ index ], searchItem )  == 0)
      {
      // go to new line
      printf( "\n" );      
              
      // return state index
      return index;  
      }
    
    else if( hash->probeStrategy == LINEAR_PROBING )
      {
      // else calculate new index
      index = hashIndex + iterations;
      }
      
    else 
      {
      // probing index squared + hash index modded by size
      index = hashIndex + toPower( iterations, 2 );
      }        
     
    // mod the index, to verify within array
    index = index % hash->tableSize;
    
    // display index    
    printf( "%d, ", index ); 
      
    // increment iterations
    iterations++;  
    }
   
  if( iterations == 0)
    {
    // go to next line
    printf( "%d" );    
    } 
    
  printf( "\n" );
  
  // return failure
  return ITEM_NOT_FOUND;
  }
    
/*
Name: getHashIndex
Process: finds hashed index for given data item,
         sums integer values of state name characters,
         if state name length is less than MINIMUM_HASH_LETTER_COUNT,
         repeats going over the state letters as needed to meet this minimum
Function input/parameters: provided search data (const StateDataType),
                           heap (const ProbingHashType)
Function output/parameters: none
Function output/returned: array index generated from state name (int)
Device input/---: none
Device output/---: none
Dependencies: getStringLength
*/
int getHashIndex( const ProbingHashType hash, const StateDataType state )
  {     
  // variables
  int sum = 0, stateIndex = 0, strLen = 0;
  int index = 0, loopCounter = MINIMUM_HASH_LETTER_COUNT;
    // set sum to zero
    // set stateIndex to zero
    
  // get string length
  strLen = getStringLength( state.name );
  
  // check for state name length less than min characters
  if( strLen > MINIMUM_HASH_LETTER_COUNT )
    {
    // set loop limit to min char constatnt
    loopCounter = strLen;
    }
       
  // loop up to min char/state name length
  while( index < loopCounter )
    {
    // add the integer value of the character to a sum
    sum += (int)( state.name[ stateIndex ] );   
    
    // increment my loop counter
    index++;
    
    // increment string index counter, and mod by string len
    stateIndex++;
    stateIndex = stateIndex % strLen;
    
    }
    // end loop
  
  // return sum mod size
  return sum % hash.tableSize;
  }

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
int getStringLength( const char *str )
  {
  // variables
  int index = 0;
  
  // find the string length
  while( str[ index ] != NULL_CHAR )
    {
    // increment index   
    index++;
    }
  
  // return string length	
  return index;	
  }

/*
Name: initializeHeap
Process: creates dynamically allocated heap, 
         creates dynamically allocated array of empty StateDataType items,
         initializes tableSize to given capacity,
         initializes probe strategy to given strategy
Function input/parameters: provided capacity (int),
                           provided probe strategy (ProbeType)
Function output/parameters: none
Function output/returned: pointer to created heap (ProbingHashType *)
Device input/---: none
Device output/---: none
Dependencies: malloc, sizeof, setEmptyHeapNode, 
*/
ProbingHashType *initializeHashTable( int capacity, ProbeType probe )
  {
  // variables	
  ProbingHashType *newHash;
  int index;
  	
  // allocate memory for heap
  newHash = ( ProbingHashType *)malloc( sizeof( ProbingHashType ) );
  
  // allocate memory for array of empty stateDataType items
  newHash->array = (StateDataType *)malloc( 
            capacity * sizeof( StateDataType ) );
  
  // set tableSize to given capacity
  newHash->tableSize = capacity;
  
  // set prob strategy	
  newHash->probeStrategy = probe;
  
  // set all index's to empty
  for( index = 0; index < capacity; index++ )
    {
    setEmptyHashNode( &newHash->array[index] );
    }
  
  return newHash;
  }

/*
Name: removeState
Process: finds item in hash table, removes, 
         sets array location to unused (but does not set any other data),
         returns removed state
Function input/parameters: provided search data (const StateDataType),
                           heap (const ProbingHashType)
Function output/parameters: returned state (StateDataType *)
Function output/returned: Boolean result of action (bool)
Device input/---: none
Device output/---: none
Dependencies: findItemIndex, setHeapNodeFromStruct
*/
bool removeState( StateDataType *removedState, 
                  const StateDataType toBeRemoved, const ProbingHashType hash )
  {
  // variables
  int index;
  
  // find the items index to be removed
  index = findItemIndex( &hash, toBeRemoved );
  
  // check if item was found
  if( index != ITEM_NOT_FOUND )
    {
    // move data to removedState
    setHashNodeFromStruct( removedState, hash.array[index] );
  
    // sets array location to unused
    hash.array[ index ].inUse = UNUSED_NODE;
  
    // return sucess	
    return true;
    }
  
  // otherwise return failure
  return false;
  }

/*
Name: setEmptyHeapNode
Process: sets given node values to empty string, zero values,
         and Boolean in use variable to false
Function input/parameters: none
Function output/parameters: updated state data struct (StateDataType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: copyString
*/
void setEmptyHashNode( StateDataType *node )
  {
  // set node string to empty
  copyString( node->name, "Unused" );
  
  // zero out values in node
  node->averageTemp = 0;
  node->highestTemp = 0;
  node->lowestTemp = 0;
  
  // set inuse to false	
  node->inUse = UNUSED_NODE;	
  }



/*
Name: setHeapNodeFromData
Process: sets given values into a given heap struct, 
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
void setHashNodeFromData( StateDataType *nodePtr, const char *name, double avgTemp, 
                          double lowTemp, double highTemp, bool nodeState )
  {    
  // copy string to nodePtr
  copyString( nodePtr->name, name );
  
  // copy data vales to nodePtr	
  nodePtr->averageTemp = avgTemp;
  nodePtr->highestTemp = highTemp;
  nodePtr->inUse = nodeState;
  nodePtr->lowestTemp = lowTemp;
  }

/*
Name: setHeapNodeFromStruct
Process: sets given values into a given heap struct, 
         using another data struct
Function input/parameters: provided state data source struct 
                           (const StateDataType)
Function output/parameters: updated state data pointer (StateDataType *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: setHeapNodeFromData
*/
void setHashNodeFromStruct( StateDataType *nodePtr, 
                                                   const StateDataType source )
  {
  setHashNodeFromData( nodePtr, source.name, source.averageTemp, 
                source.lowestTemp, source.highestTemp, source.inUse );
  }

/*
Name: showHashTableStatus
Process: displays array <D>ata values and <U>unused values 
         as a string of letters,
         displays number of items in heap, 
         displays minimum number of contiguous data items,
         displays maximum number of contiguous data items,
         displays number of empty/unused array elements
Function input/parameters: heap (const ProbingHashType)
Function output/parameters: none
Function output/returned: none
Device input/---: none
Device output/monitor: hash data status displayed as specified
Dependencies: sprintf, printf
*/
void showHashTableStatus( const ProbingHashType hash)
  {
  // variables
  int index, min = hash.tableSize, max = 0, empty = 0; 
  int contiguousCounter = 0, inuse = 0;
  char displayStr[ MAX_STR_LEN ], letters[MAX_STR_LEN];
  
  // check usage at index
  for( index = 0; index < hash.tableSize; index++ )
    {
    // if index is inuse
    if( hash.array[index].inUse )
      {
      // add a D at that index
      letters[ index ] = 'D';
      
      } 
    
    // otherwise    
    else
      {
      // add a U to that index
      letters[ index ] = 'U';    
      
      empty++;
      }
  
    }
  
  // contiguous counter
  for( index = 0; index < hash.tableSize; index++ )
    {
    if( letters[ index ] == 'D' )
      {
      contiguousCounter++;  
      inuse++;  
      }   
        
    else
      {
      // check if counter was greater than existing max    
      if( contiguousCounter > max )
        {
        // new max
        max = contiguousCounter;    
        }
          
      // check min
      if( contiguousCounter < min )   
        {
        // new min    
        min = contiguousCounter;
        }
      
      // reset counter
      contiguousCounter = 1;  
      } 
      
    }
  
  // create diplay string
  sprintf( displayStr, "Hash Table Status: %s", letters );
  
  // display status
  printf( "\n%s\n", displayStr );
  printf( "D => Data, U => Unused\n" );
  
  // display number of items in heap
  printf( "         Items in Hash Table: %d\n", inuse );
  
  // display min number of contiguous data items
  printf( "     Minimum contiguous bins: %d\n", min );
  
  // display max number of contiguous data items
  printf( "     Maximum contiguous bins: %d\n", max );
  
  // display number of empty array elements	
  printf( "        Number of empty bins: %d\n", empty );	
  	
  }

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
int toPower( int base, int exponent )
  {
  // calculate exponent to the base  for exponents >= 0
  if( exponent > 0)
    {
    // return calculation
    return base * base + toPower( base, exponent - 1 );   
    }
  
  // return failure	
  return 0;	
  }
