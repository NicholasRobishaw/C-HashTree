// header files
#include "File_Input_Utility.c"
#include "HashUtilities.c"

// constants
extern const int ITEM_NOT_FOUND;
extern const bool USED_NODE;
extern const bool UNUSED_NODE;

// prototypes
void getQueryObject( StateDataType *returnedState, const char *stateName );
ProbingHashType *uploadData( char *fileName, int tableSize, int probeType );

// main function
int main( int argc, char *argv[] )
   {
    ProbingHashType *hashTest;
    StateDataType toRemove, removed, toFind, found;
    char toRemoveStr[ MAX_STR_LEN ], removedStr[ MAX_STR_LEN ];
    char toFindStr[ MAX_STR_LEN ], foundStr[ MAX_STR_LEN ];
    int primeTableSize = 67;  // recommended to be prime number (or at least odd)
                               // and roughly 1.3x the size of the data set
     
    // title
    printf( "\nHASH TABLE TEST PROGRAM\n" );
    printf( "=======================\n" );

    hashTest = uploadData( "inData.csv", primeTableSize, LINEAR_PROBING );

    
  
    // display array dump
    displayHashTable( hashTest );

    

    // show hash table status
    showHashTableStatus( *hashTest );
    
    
            
    printf( "\n\nFinding Selected States ----------------------------------\n" );

    copyString( toFindStr, "Arizona" );
    getQueryObject( &toFind, toFindStr );
    found = findItem( hashTest, toFind );
    dataToString( foundStr, found );
    printf( "Found: %s\n", foundStr );
    
    copyString( toFindStr, "Iowa" );
    getQueryObject( &toFind, toFindStr );
    found = findItem( hashTest, toFind );
    dataToString( foundStr, found );
    printf( "Found: %s\n", foundStr );
    
    copyString( toFindStr, "Minnesota" );
    getQueryObject( &toFind, toFindStr );
    found = findItem( hashTest, toFind );
    dataToString( foundStr, found );
    printf( "Found: %s\n", foundStr );
    
    
    
    
    printf( "\n\nRemoving Selected States ----------------------------------\n" );

    copyString( toRemoveStr, "Texas" );
    getQueryObject( &toRemove, toRemoveStr );
    if( removeState( &removed, toRemove, *hashTest ) )
       {
        dataToString( removedStr, removed );
        printf( "---Removed: %s\n", removedStr );
       }
    else
       {
        printf( "---Failed to remove %s\n", toRemoveStr );
       }

    copyString( toRemoveStr, "New Mexico" );
    getQueryObject( &toRemove, toRemoveStr );
    if( removeState( &removed, toRemove, *hashTest ) )
       {
        dataToString( removedStr, removed );
        printf( "---Removed: %s\n", removedStr );
       }
    else
       {
        printf( "---Failed to remove %s\n", toRemoveStr );
       }

    copyString( toRemoveStr, "Washington" );
    getQueryObject( &toRemove, toRemoveStr );
    if( removeState( &removed, toRemove, *hashTest ) )
       {
        dataToString( removedStr, removed );
        printf( "---Removed: %s\n", removedStr );
       }
    else
       {
        printf( "---Failed to remove %s\n", toRemoveStr );
       }

    copyString( toRemoveStr, "Maryland" );
    getQueryObject( &toRemove, toRemoveStr );
    if( removeState( &removed, toRemove, *hashTest ) )
       {
        dataToString( removedStr, removed );
        printf( "---Removed: %s\n", removedStr );
       }
    else
       {
        printf( "---Failed to remove %s\n", toRemoveStr );
       }

    copyString( toRemoveStr, "Florida" );
    getQueryObject( &toRemove, toRemoveStr );
    if( removeState( &removed, toRemove, *hashTest ) )
       {
        dataToString( removedStr, removed );
        printf( "---Removed: %s\n", removedStr );
       }
    else
       {
        printf( "---Failed to remove %s\n", toRemoveStr );
       }

    copyString( toRemoveStr, "Quebec" );
    getQueryObject( &toRemove, toRemoveStr );
    if( removeState( &removed, toRemove, *hashTest ) )
       {
        dataToString( removedStr, removed );
        printf( "---Removed: %s\n", removedStr );
       }
    else
       {
        printf( "---Failed to remove %s\n", toRemoveStr );
       }

    

    // display array dump
    displayHashTable( hashTest );

    

    // show hash table status
    showHashTableStatus( *hashTest );
            
    // end program

       // clear hash table
       clearHashTable( hashTest );

       // show program end
       printf( "\nEnd Program\n" );

       // return success
       return 0;
    }


void getQueryObject( StateDataType *returnedState, const char *stateName )
   {
    setHashNodeFromData( returnedState, stateName, 0.0, 0.0, 0.0, 0.0 );      
   }

    
/*
Name: getDataFromFile
Process: uploads data from file with unknown number of data sets,
         has internal Verbose Boolean to display input operation
Function input/parameters: file name (char *)
Function output/parameters: none
Function output/returned: pointer to hash table (ProbingHashType *)
Device input/file: data from HD
Device output/monitor: none
Dependencies: initializeHashTable, openInputFile, readStringToDelimiterFromFile,
              readDoubleFromFile, readCharacterFromFile, printf, 
              addItemFromData, closeInputFile, free
*/
ProbingHashType *uploadData( char *fileName, int tableSize, int probeType )
   {
    char stateNameStr[ MAX_STR_LEN ];
    double avgTemp, lowestTemp, highestTemp;
    int index = 0;
    bool verbose = false;  // Set to true to verify data upload, false otherwise
    ProbingHashType *tempHashPtr = initializeHashTable( tableSize, probeType );

    if( openInputFile( fileName ) )
       {
        if( verbose )
           {
            printf( "\n     ----- Verbose: Begin Loading Data From File\n" );
           }

        // get first state name
        readStringToDelimiterFromFile( COMMA, stateNameStr );

        while( !checkForEndOfInputFile() )
           {
            avgTemp = readDoubleFromFile();

            // ignores comma
            readCharacterFromFile();

            lowestTemp = readDoubleFromFile();

            // ignores comma
            readCharacterFromFile();

            highestTemp = readDoubleFromFile();

            if( verbose )
               {
                printf( "State Name: %s | ", stateNameStr );

                printf( "Average Temp: %5.2f | ", avgTemp );

                printf( "Lowest Temp: %5.2f | ", lowestTemp );

                printf( "Highest Temp: %5.2f\n", highestTemp );
               }

            // add to hash table
            addItemFromData( tempHashPtr, stateNameStr, 
                                             avgTemp, lowestTemp, highestTemp );

            // reprime - read next state name
            readStringToDelimiterFromFile( COMMA, stateNameStr );

            index++;
           }

        if( verbose )
           {
            printf( "\n     ----- Verbose: Items found in file: %d\n", index );
            printf( "     ----- Verbose: End Loading Data From File\n\n" );
           }

        closeInputFile();
       }

    // file not found
    else
       {
        free( tempHashPtr );
       }

    return tempHashPtr;
   }
    


