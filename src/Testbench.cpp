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
#ifdef   __RTL_SIMULATION__
#include "Filter_rtl_wrapper.h"
#define  Filter Filter_rtl_wrapper
#else
#include "Filter.h"
#endif

#include "Consumer.h"
#include "Producer.h"

#include <systemc>
#include <iostream>


using namespace std;
using namespace sc_core;


int sc_main(int argc, char *argv[]) {

    // Disable some irrelevant warnings
    sc_report_handler::set_actions(SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_,
                                   SC_DO_NOTHING);
    // Clock Generator
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // The modules
    Filter filter("filter");
    Producer producer("producer");
    Consumer consumer("consumer");

    // The communication channels
    sc_fifo<sample_t> channelIn("channelIn", 512);
    sc_fifo<sample_t> channelOut("channelOut", 512);

    // Connect clock to Filter
    filter.clk(clk);

    // Connect producer and filter via channelIn
    producer.outp(channelIn);
    filter.inp(channelIn);

    // Connect filter and consumer via channelOut
    filter.outp(channelOut);
    consumer.inp(channelOut);

    sc_start();
    return 0;
}
