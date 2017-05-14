/* -----------------------------------------------------------------
 * Filter.cpp
 *
 * A SystemC filter module.
 *
 * Author:  Jens Rudolf (Uni Rostock)
 * Date:    2015-04-25
 *
 *
 * Changelog:
 *      2015-04-25 - Initial version
 *      2015-05-04 - Switch to SC_THREAD, add clock and reset
 *
 *
 * Copyright (c) 2015 by Jens Rudolf (Uni Rostock). All rights
 * reserved. For educational use only. NO WARRANTY!
 *
 * Report bugs to:
 *      jens.rudolf@uni-rostock.de
 *
 * -----------------------------------------------------------------
 */
#include "Filter.h"

#include <systemc>
#include <iostream>

using namespace std;
using namespace sc_core;


// The filter process
void Filter::filterTokens() {

    // Reset the forwarding flag
    forward = true;
    wait();

    while (true) {
        // Wait for clock edge
        wait();

        if (!inp.num_available() || !outp.num_free())
            continue;

        // Read sample from input FIFO
        sample_t sample = inp.read();

        // Drop every second sample
        if (forward)
            outp.write(sample);
 
        // Toggle forwarding flag
        forward = !forward;
    }
}
