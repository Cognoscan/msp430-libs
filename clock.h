#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <msp430.h>

/// Turn off Watchdog Timer
static inline void WatchdogOff()
{
	WDTCTL = WDTPW | WDTHOLD;
}

/// Set DCO to Calibrated 1 MHz frequency
static inline void DCO1MHz()
{
	const char *CALDCO_1MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_DCO_1MHZ);
	const char *CALBC1_1MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_BC1_1MHZ);
	DCOCTL = *CALDCO_1MHZ; // Hard code for now
	BCSCTL1 = *CALBC1_1MHZ; // Hard code for now
}

/// Set DCO to Calibrated 8 MHz frequency
static inline void DCO8MHz()
{
	const char *CALDCO_8MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_DCO_8MHZ);
	const char *CALBC1_8MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_BC1_8MHZ);
	DCOCTL = *CALDCO_8MHZ; // Hard code for now
	BCSCTL1 = *CALBC1_8MHZ; // Hard code for now
}

/// Set DCO to Calibrated 12 MHz frequency
static inline void DCO12MHz()
{
	const char *CALDCO_12MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_DCO_12MHZ);
	const char *CALBC1_12MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_BC1_12MHZ);
	DCOCTL = *CALDCO_12MHZ; // Hard code for now
	BCSCTL1 = *CALBC1_12MHZ; // Hard code for now
}

/// Set DCO to Calibrated 16 MHz frequency
static inline void DCO16MHz()
{
	const char *CALDCO_16MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_DCO_16MHZ);
	const char *CALBC1_16MHZ = (char *)(TLV_DCO_30_TAG_ + 0x02 + CAL_BC1_16MHZ);
	DCOCTL = *CALDCO_16MHZ; // Hard code for now
	BCSCTL1 = *CALBC1_16MHZ; // Hard code for now
}
#endif
