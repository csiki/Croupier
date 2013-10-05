
#include <stdio.h>
#include <stdlib.h>

// DynObj support 
#include "dynobj/DynObj.h"
#include "dynobj/DynObjLib.h"
#include "dynobj/DoRunTime.h"


// Interfaces we're using
#include "dynobj/DynStr.h"
#include "PersonI.h"
#include "ProfessionI.h"

int main( int argc, const char **argv ) {

    // Initialize, check that things have started OK
    if( !doInit() ){
        printf("Failed DoVerifyInit\n");
        exit(-1);
    }
    
    // As a service to the pimpl2 library, we load the "DynStr" lib 
    // since we know it uses it. 
	const char *perr = DoRunTimeC::GetInstance().LoadLib("../DynStr");
	if( perr ){
		printf( "Could not load (DynStr) library: %s\n", perr );
        exit( -1 );
    }

    // We declare this as a local instance since we never register it with DoRunTime
    DynObjLib lpimpl( "pimpl2",true );
    if( !lpimpl.IsOk() ){
        printf( "Could not load library: status %d\n", lpimpl.GetState() );
        exit( -1 );
    }
    
    PersonI *pp = (PersonI*)lpimpl.Create("PersonI",PERSONI_TYPE_ID);
    if( pp ) {
    	pp->SetName( "George" );
    	pp->SetAge( 34 );
    	printf( "Person:\n" );
    	printf( "Name: %s \t Age: %d\n", pp->GetName(), pp->GetAge() );
    	
        // Here we check if the person has a profession interface
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
