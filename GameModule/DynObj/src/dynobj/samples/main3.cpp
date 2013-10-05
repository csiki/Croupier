
#include <stdio.h>
#include <stdlib.h>

// DynObj support files
#include "dynobj/DynObj.h"
#include "dynobj/DynObjLib.h"
#include "dynobj/DoRunTime.h"
#include "dynobj/DynObjHolder.hpp"

#define DO_IMPLEMENT_DYNSTR
#include "dynobj/DoVTableInfo.hpp"

#include "dynobj/DynStr.h"

// Interfaces we use
#include "PersonI.h"
#include "ProfessionI.h"

struct A{ };

template<class T>
struct IsSigned {
    enum { v=T(-1)<0 };
};

void F( const DynData& dd ){
}

#include "dynobj/DoBase.hpp"
void Test(){
    A a;
    DynData dda(&a);
    F(dda);
    F(143);
     
    DynData dd("Kalle");
    printf( "dd is const char*: %s\n", dd.Get<const char*>() );

    DynStr *pds = do_new<DynStr>(&dd);
    
    exit(-1);
}

int Test4(){
    return 55;
}

class Test1 { int m_i; };
enum { Test1, Test2 };

void Test3(){
    enum { Test3 };
    int a = Test1;
    //Test3();
    printf("Hello");
}



// A printout function...
void PrintPerson( PersonI& p ){
	printf( "Person - Name: %8s  Age: %d\n", p.GetName(), p.GetAge() );
}

int main() {
    //Test();

    // Initialize, check that things have started OK
    if( !doInit() ){
        printf("Failed DoVerifyInit\n");
        exit(-1);
    }
    
    // As a service to the pimpl2 library, we load the "DynStr" lib 
    // since we know it uses it. 
	const char *err;
	if( err=DoRunTimeC::GetInstance().LoadLib("../DynStr") ){
		printf( "Could not load (DynStr) library: %s\n", err );
        exit( -1 );
    }

    // We load the library directly with DoRunTime
	if( err=DoRunTimeC::GetInstance().LoadLib("pimpl3") ){
		printf( "Could not load (pimpl3) library: %s\n", err );
        exit( -1 );
    }
    // Load a previous libraries
	if( err=DoRunTimeC::GetInstance().LoadLib("pimpl2") ){
		printf( "Could not load (pimpl2) library: %s\n", err );
        exit( -1 );
    }
	if( err=DoRunTimeC::GetInstance().LoadLib("pimpl1") ){
		printf( "Could not load (pimpl1) library: %s\n", err );
        exit( -1 );
    }
	
    // Instead of creating a pointer to a new object and later calling
    // doDestroy on it, we let DynObjHolder (or DOH for short) do the job.
    // 
    // There is some work in the background resolving the C++ type down 
    // to type strings and type ID:s. It gives us type safety as far as
    // it is possible when loading code from possibly different compilers.
    //
    // Since several libraries implement PersonI we have to specify which
    // library to use (otherwise the 1st library that implements it is chosen). 
    DynObjHolder<PersonI> p1( "pimpl1", "Johannes,29" );
    DynObjHolder<PersonI> p2( "pimpl2", "Lucas,53" );
    DynObjHolder<PersonI> p3( "pimpl3", "George,34" );

    /*
    // This causes an object leak and can be detected when using DO_ENABLE_OBJECT_TRACKING
    DynObjLib *pdol = DoRunTimeC::GetInstance().GetLib("pimpl2");
    pdol->EnableObjectTracking(true);
    do_new<PersonI>(0,"pimpl2");
    */

    if( p1().IsOk() && p2().IsOk() && p3().IsOk() ) {
    	// Show that they exist...
    	PrintPerson(p1());
    	PrintPerson(p2());
    	PrintPerson(p3());
        
    	// Use operators on the instances
    	bool r = p1()<p2(); 
    	printf( "\np1 is %s than p2\n\n", r?"less":"greater" );

        // Query type with type string
        if( p1().doGetObj("ProfessionI") ) 
            printf("p1 implements ProfessionI\n");
        else
            printf("p1 does not implement ProfessionI\n");
            
        // CanBeA - question for type with type ID
        if( p2().CanBeA(PROFESSIONI_TYPE_ID) ) 
            printf("p2 can be ProfessionI\n");
        else
            printf("p2 can NOT be ProfessionI\n");
        
        // Query type with C++ cast
        if( do_cast_str<ProfessionI*>(&p3()) ) 
            printf("p3 succeeded with do_cast<ProfessionI*>\n");
        else 
            printf("p3 failed with do_cast<ProfessionI*>\n");

    	// Use assignment operator (non virtual, inline function)
    	p3() = p1();
    	
    	// Here we go again
    	printf("\n");
    	PrintPerson(p1());
    	PrintPerson(p2());
    	PrintPerson(p3());
    }
	else{
		printf( "Failed creating one or more instances of PersonI, Status:\n" );
		printf( "p1 %p %d %s\n", p1.Get(), p1().IsOk(), p1().GetError() );
		printf( "p2 %p %d %s\n", p2.Get(), p2().IsOk(), p2().GetError() );
		printf( "p3 %p %d %s\n", p3.Get(), p3().IsOk(), p3().GetError() );
	}
        
    return 0;
}
