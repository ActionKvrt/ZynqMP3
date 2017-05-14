/* -----------------------------------------------------------------
 * Consumer.h
 *
 * Systemc consumer module.
 *
 * Author:  Jens Rudolf (Uni Rostock)
 * Date:    2015-24-05
 *
 *
 * Changelog:
 *      2015-24-05 - Initial version
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

#include <systemc>

typedef int32_t sample_t;


SC_MODULE(Consumer){
	// The input FIFO port
	sc_core::sc_fifo_in<sample_t> inp;

    // The consumer process
	void consumeTokens();

    // The module constructor
	SC_CTOR(Consumer) {
		SC_THREAD(consumeTokens);
	}
};
