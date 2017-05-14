// ===========================================================================
//
// Copyright 2009-2016 University of Rostock
//
// Distributed under the OSI-approved BSD License (the "License");
// see accompanying file Copyright.txt for details.
//
// This software is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the License for more information.
//
// ===========================================================================

#include "Consumer.h"

#include <iostream>
#include <systemc>

using namespace std;
using namespace sc_core;


// The consumer process
void Consumer::consumeTokens() {
	
    sample_t sample = 0;

    // Say hello to the world	
    cout << sc_time_stamp()
	 << ":\tconsumeTokens(): Starting up!"
	 << endl;

    while (true) {

	// Read sample from input
        while(!inp.nb_read(sample))
            wait(10, SC_NS);

        if (sample == '\0')
            break;
        
        cout << sc_time_stamp()
             << ":\tconsumeTokens(): " << static_cast<char>(sample)
             << endl;
    }
	
    // Last sample received. Stopping the simulation.
    cout << "Stopping simulation." << endl;
    sc_stop();
}
