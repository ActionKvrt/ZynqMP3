/* -----------------------------------------------------------------
 * Producer.h
 *
 * A SystemC producer module.
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

#pragma once

#include <stdint.h>
#include <systemc>

typedef int32_t sample_t;


SC_MODULE(Producer){
    // Output FIFO port
    sc_core::sc_fifo_out<sample_t> outp;

    // The producer process
    void produceTokens();


    // The module constructor
    SC_CTOR(Producer) {
        SC_THREAD(produceTokens);
    }
};
