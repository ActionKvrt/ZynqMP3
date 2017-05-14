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
#pragma once


#include <systemc>
#include <stdint.h>

typedef int32_t sample_t;


SC_MODULE(Filter) {
    // Clock input
    sc_core::sc_in<bool> clk;

    // Input and output FIFO ports
    sc_core::sc_fifo_in<sample_t>    inp;
    sc_core::sc_fifo_out<sample_t>   outp;

    // The filter process
    void filterTokens();

    // The forwarding flag
    bool forward;

    // The module constructor
    SC_CTOR(Filter) {
        SC_CTHREAD(filterTokens, clk.pos());
    }
};
