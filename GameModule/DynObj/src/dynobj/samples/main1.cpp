
#include <stdio.h>
#include <stdlib.h>

// DynObj support 
#include "dynobj/DynObj.h"
#include "dynobj/DynObjLib.h"
#include "dynobj/DoBase.hpp"

// Interfaces we're using
#include "PersonI.h"
#include "ProfessionI.h"

int main( int argc, const char **argv ) {

    // Initialize, check that things have started OK
    if( !doInit() ){
        printf("Failed doInit\n");
        exit(-1);
    }

    // Load library
    DynObjLib lpimpl1( "pimpl1", true );
    if( !lpimpl1.IsOk() ){
        printf( "Could not load library (pimpl1): status (%d)\n", lpimpl1.GetState() );
        exit( -1 );
    }
    
    // Create object
    PersonI *pp = (PersonI*)lpimpl1.Create("PersonI",PERSONI_TYPE_ID);
    if( pp ) {
    	pp->SetName( "George" );
    	pp->SetAge( 34 );
    	printf( "Person:\n" );
    	printf( "Name: %s \t Age: %d\n", pp->GetName(), pp->GetAge() );
    	
        // Here we check if the person has a profession interface
        // (which they don't have in this case)
        ProfessionI *pi = do_cast<ProfessionI*>(pp);
        if( pi )
            printf( "%s is also a %s with %d years experience\n", 
                    pp->GetName(), pi->GetProfession(), pi->GetExperience() );
            // No need to release interface pointer, we just borrow it
        
        pp->doDestroy();
    }
    else
        printf("Failed creating instance of PersonI\n");
        
    return 0;
}
