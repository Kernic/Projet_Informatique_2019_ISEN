/*
 * common.c
 *
 *  Created on: 25 avr. 2019
 *      Author: J.-B. Pierrot
 */
#include "common.h"

signal_data *allocate_signal_data( unsigned long nb_samples )
{
	signal_data *dtptr;

	dtptr = (signal_data*)malloc(sizeof(signal_data));
	if(dtptr==NULL)
		return(NULL);

	dtptr->time = (double*)calloc(nb_samples,sizeof(double));
	if(dtptr->time==NULL)
	{
		free(dtptr);
		return(NULL);
	}

	dtptr->signal = (double*)calloc(nb_samples,sizeof(double));
	if(dtptr->time==NULL)
	{
		free(dtptr->time);
		free(dtptr);
		return(NULL);
	}

	dtptr->nb_samples = nb_samples;

	return( dtptr );
}

void free_signal_data( signal_data *data )
{
	free( data->time );
	free( data->signal );
	free( data );
}
