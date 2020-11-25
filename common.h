/*
 * common.h
 *
 *  Created on: 25 avr. 2019
 *      Author: isen
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include <math.h>


#define BASEDIR "/home/etudiant*/BIOST3-ECG-VM/DATA"

#ifdef __DEBUG__
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif


typedef struct
{
	unsigned long nb_samples;   /* in time & signal arrays */

	double *time;               /* ms */
	double *signal;             /* mV */
}signal_data;

extern signal_data *allocate_signal_data( unsigned long nb_samples);
extern void free_signal_data( signal_data *data );

#endif /* COMMON_H_ */
