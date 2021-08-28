/*
 * midi_luts.c
 *
 *  Created on: Aug 25, 2021
 *      Author: tyler
 */

#include <midi_luts.h>


unsigned int conv_midi_to_dac(char note) {
    // MIDI note to DAC value lookup table
    static const unsigned int dac_lut[121] = {
      0,
      33,
      67,
      101,
      136,
      170,
      204,
      238,
      272,
      306,
      340,
      374,
      409,
      443,
      477,
      511,
      545,
      579,
      613,
      648,
      682,
      716,
      750,
      784,
      818,
      852,
      886,
      921,
      955,
      989,
      1023,
      1057,
      1091,
      1125,
      1160,
      1194,
      1228,
      1262,
      1296,
      1330,
      1364,
      1398,
      1433,
      1467,
      1501,
      1535,
      1569,
      1603,
      1637,
      1672,
      1706,
      1740,
      1774,
      1808,
      1842,
      1876,
      1910,
      1945,
      1979,
      2013,
      2047,
      2081,
      2115,
      2149,
      2184,
      2218,
      2252,
      2286,
      2320,
      2354,
      2388,
      2422,
      2457,
      2491,
      2525,
      2559,
      2593,
      2627,
      2661,
      2696,
      2730,
      2764,
      2798,
      2832,
      2866,
      2900,
      2934,
      2969,
      3003,
      3037,
      3071,
      3105,
      3139,
      3173,
      3208,
      3242,
      3276,
      3310,
      3344,
      3378,
      3412,
      3446,
      3481,
      3515,
      3549,
      3583,
      3617,
      3651,
      3685,
      3720,
      3754,
      3788,
      3822,
      3856,
      3890,
      3924,
      3958,
      3993,
      4027,
      4061,
      4095   };

    unsigned int dac = dac_lut[note];
    return dac;
}


unsigned int conv_midi_to_freq(char note) {
    // MIDI note to frequency value lookup table
    static const unsigned int freq_lut[121] = {
      8,
      9,
      9,
      10,
      10,
      11,
      12,
      12,
      13,
      14,
      15,
      15,
      16,
      17,
      18,
      19,
      21,
      22,
      23,
      24,
      26,
      28,
      29,
      31,
      33,
      35,
      37,
      39,
      41,
      44,
      46,
      49,
      52,
      55,
      58,
      62,
      65,
      69,
      73,
      78,
      82,
      87,
      92,
      98,
      104,
      110,
      117,
      123,
      131,
      139,
      147,
      156,
      165,
      175,
      185,
      196,
      208,
      220,
      233,
      247,
      262,
      277,
      294,
      311,
      330,
      349,
      370,
      392,
      415,
      440,
      466,
      494,
      523,
      554,
      587,
      622,
      659,
      698,
      740,
      784,
      831,
      880,
      932,
      988,
      1047,
      1109,
      1175,
      1245,
      1319,
      1397,
      1480,
      1568,
      1661,
      1760,
      1865,
      1976,
      2093,
      2217,
      2349,
      2489,
      2637,
      2794,
      2960,
      3136,
      3322,
      3520,
      3729,
      3951,
      4186,
      4435,
      4699,
      4978,
      5274,
      5588,
      5920,
      6272,
      6645,
      7040,
      7459,
      7902,
      8372   };

    unsigned int freq = freq_lut[note];
    return freq;
}