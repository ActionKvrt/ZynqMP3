/* -----------------------------------------------------------------
 * Producer.cpp
 *
 * Systemc producer module.
 *
 * Author:  Jens Rudolf (Uni Rostock)
 * Date:    2015-24-05
 *
 *
 * Changelog:
 *      2015-05-24 - Initial version
 *	2015-07-02 - Port to 32 bit FIFO interface
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

#include "Producer.h"


#include <fstream>
#include <iostream>
#include <systemc>

using namespace std;
using namespace sc_core;


// The producer process
void Producer::produceTokens() {

    // Say hello, if you're alive
    cout << sc_time_stamp()
	 << ":\tproduceTokens(): Starting up!"
	 << endl;

    // Token buffer
    sample_t buffer[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0' };

    unsigned buflen = sizeof(buffer) / sizeof(sample_t);

    // Put every token onto the output FIFO
    for (int i = 0; i < buflen; ++i)
        while (!outp.nb_write(buffer[i]))
            wait(10, SC_NS);
	
    // Say hello, if you're done
    cout << sc_time_stamp()
         << ":\tproduceTokens(): All tokens on FIFO!"
	 << endl;
}
