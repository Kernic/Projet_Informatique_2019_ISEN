#include"common.h"

signal_data* lecture(FILE *fichier);

double rPointRef(signal_data *data);

unsigned long nbR(signal_data *data,double Rref);

signal_data* Ppts(signal_data* data,signal_data* dataQ, unsigned long nR);
signal_data* Rpts(signal_data* data,double Rref,unsigned long nR);
signal_data* Qpts(signal_data* data,signal_data* dataR, unsigned long nR);

signal_data* Spts(signal_data* data,signal_data* dataR, unsigned long nR);
signal_data* Tpts(signal_data* data,signal_data* dataS, unsigned long nR);

double moyenneSignal(signal_data *signalA, signal_data *signalB);/*Le signalA doit toujour ce situer au temporellement plus avancé que le signalB*/

double *moyenneBPM(signal_data *signalR, double pas);