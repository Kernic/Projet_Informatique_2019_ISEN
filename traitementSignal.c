#include"common.h"
#include"InfoFile.h"


/*
 * Cette fontion permet de calculer le nombre de lignes dans le document qui est ouvert
 * Elle prend en entré le fichier ouvert et renvoie le nombre de ligne 
 */

unsigned long tailleFichier (FILE *fichier){
	debug("\033[1;33mDébut de tailleFichier\033[0m\n");
	char c='c';
	unsigned long ret=0;
	while ((c = getc(fichier))!=EOF){
		if (c=='\n') {
			ret++;
		}
	}
	rewind(fichier); // Permet de reset la lecture du fichier
	debug("\033[1;33mFin de tailleFichier\033[0m\n");
	return ret;
}

/* 
 * Cette fonction permet de lire le fichier ouvert et d'y extraire les données 
 * Elle prend en entrée le fichier ouvert et renvoie les données sous la forme de (signal_data*)
 */

signal_data* lecture(FILE *fichier)
{
	debug("\033[1;33mDébut de lecture \033[0m\n");
	// Declaration des variables 
	unsigned long nbValues=0;
	double tmp, amp;
	int i=0;

	
	// ouverture du fichier plus calcule de le nombre de lignes dans le fichier
	
	
	nbValues = tailleFichier(fichier);
	debug("Nombre de ligne = %li\n", nbValues); 
	
	// Création de la stucture + allocation de la mémoire 
	signal_data *data = allocate_signal_data(nbValues);

	// Lecture du fichier : 
	
	while (!feof(fichier))
	{
		fscanf(fichier,"%lf;%lf\n", &tmp, &amp);
		data->time[i]=tmp*1000;
		data->signal[i]=amp;
		i++;
	}

	debug("\033[1;33mFin de lecture\033[0m\n");	
	return data;
}


/*
 * Cette fonction permet de donner l'onde R de référance, 
 * Elle prend en entré les données récupéré dans la fonction lecture et renvoie la valeur seuil qui vas être utilisé dans la fontion nbR 
 */

double rPointRef(signal_data *data)
{
	debug("\033[1;33mDébut de rPointRef\033[0m\n");
	unsigned long i=0;
	unsigned long seuil;
	seuil=data->nb_samples;
	double Rref=0; 
	
	debug("seuil:%lu\n ",seuil);
	//nbre de pt R
	
	while(data->time[i]!=data->time[1500])	
	{	
		//debug("d:%lu \n",d);
		if (data->signal[i]>Rref)
		{
			Rref=data->signal[i];
		}
		i++;
	}	
	debug("R:%lf \n",Rref);
	debug("\033[1;33mFin de rPointRef\033[0m\n");
	return(Rref*0.95);
}


/*
 * Cette fonction permet de calculer le nombre de d'onde R dans le fichier choisie
 * Elle prend en entré les donées lue dans la fonction lecture, et le seuil calculé dans la fonction rPointRef
 * Elle renvoie le nombre d'onde R dans les données ce qui correspond au nombre de periode
 */

unsigned long nbR(signal_data *data,double Rref)
{
	debug("\033[1;33mDébut de nbR\033[0m\n");
	unsigned long i;
	int flag=1;
	unsigned long compteur=0;
	for(i=0;i<data->nb_samples;i++)
	{
		if(data->signal[i]>Rref && flag==1)
		{
			compteur++;
			flag=0;		
		}
		if(data->signal[i]<Rref && flag==0)
		{
			flag=1;	
		}
	}
	debug("\033[1;33mFin de nbR\033[0m\n");

	return(compteur);
}

/*
 *
 */

signal_data* Rpts(signal_data* data,double Rref,unsigned long nR)
{
	debug("\033[1;33mDébut de Rpts\033[0m\n");
	signal_data* RptsD=allocate_signal_data(nR);
	unsigned long i,stocki=0,compteR=0;
	int flag=0;
	double maxA=-1000000;
	
	for(i=0;i<(data->nb_samples);i++)
	{
		
		//Passage au-dessus de Rref: passage à flag=1
		if(data->signal[i]>Rref&&flag==0)
		{
			flag=1;
		}
		
		if(flag==1&&maxA<data->signal[i])
		{
			maxA=data->signal[i];
			stocki=i;
		}		
		
		//Passage en-dessous: Passage à flag=0
		if(data->signal[i]<Rref&&flag==1)
		{
			flag=0;
			RptsD->signal[compteR]=maxA;
			RptsD->time[compteR]=data->time[stocki];
			compteR++;
			maxA=-1000000;
			stocki=0;
		}
	}
	debug("\033[1;33mFin de Rpts\033[0m\n");
	return(RptsD);
}

/*
 * 
 */

signal_data* Qpts(signal_data* data,signal_data* dataR, unsigned long nR)
{
	debug("\033[1;33mDébut de Qpts\033[0m\n");
	signal_data* QptsD = allocate_signal_data(nR);
	unsigned long i = 0, c = 0, stocki = 0;
	double min = 1000000;
	for(i = 0 ; i<(data->nb_samples) ; i++)
	{
		if(data->time[i] > (dataR->time[c] - 150.0) && data->time[i] < dataR->time[c])
		{
			if(min > data->signal[i])
			{
				min = data->signal[i];
				stocki = i;
			}	
		}
		if(data->time[i] == dataR->time[c])
		{
			QptsD->signal[c] = min;
			QptsD->time[c] = data->time[stocki];
			c++;
			min = 1000000;
		}
	}
	debug("\033[1;33mFin de Qpts\033[0m\n");
	return(QptsD);
}

/*
 * 
 */

signal_data* Spts(signal_data* data, signal_data* dataR, unsigned long nR)
{
	debug("\033[1;33mDébut de Spts\033[0m\n");
	signal_data *SptsD = allocate_signal_data(nR);
	unsigned long i = 0,c = 0,stocki = 0;
	double min = 1000000;
	int flag = 0;

	for(i = 0 ; i < (data->nb_samples) ; i++)
	{
		if(data->time[i] < (dataR->time[c]+200.0) && data->time[i] > dataR->time[c])
		{
			flag = 1;
			if(min > data->signal[i])
			{
				min = data->signal[i];
				stocki = i;
			}	
		}
		if(data->time[i] >= (dataR->time[c] + 200) && flag == 1)
		{
			SptsD->signal[c] = min;
			SptsD->time[c] = data->time[stocki];
			c++;
			min=1000000;	
			flag = 0;
		}
	}
	debug("\033[1;33mFin de Spts\033[0m\n");
	return(SptsD);
}

/*
 * 
 */

signal_data* Ppts(signal_data* data,signal_data* dataQ, unsigned long nR)
{
	debug("\033[1;33mDébut de Ppts\033[0m\n");
	signal_data* PptsD = allocate_signal_data(nR);
	unsigned long i = 0, c = 0, stocki = 0;
	double max = -1000000;
	for(i = 0 ; i<(data->nb_samples) ; i++)
	{
		if(data->time[i] > (dataQ->time[c] - 100.0) && data->time[i] < dataQ->time[c])
		{
			if(max < data->signal[i])
			{
				max = data->signal[i];
				stocki = i;
			}	
			
		}
		if(data->time[i]==dataQ->time[c])
		{
			PptsD->signal[c] = max;
			PptsD->time[c] = data->time[stocki];
			c++;
			max = -1000000;
		}
	}
	debug("\033[1;33mFin de Ppts\033[0m\n");
	return(PptsD);
}

/* 
 * Cette fonction permet de calculer les points culminants des ondes T en ce basant sur les ondes S
 * Elle prend en entré les signaux data du fichier original et des ondes S
 * Elle renvoie une série de donnés contant les piques des ondes T
 */

signal_data* Tpts(signal_data* data,signal_data* dataS, unsigned long nR)
{
	debug("\033[1;33mDébut de Tpts\033[0m\n");
	signal_data* TptsD = allocate_signal_data(nR);
	unsigned long i = 0, c = 0, stocki = 0;
	double max = -1000000;
	int flag = 0;
	for(i = 0 ; i<(data->nb_samples) ; i++)
	{
		if(data->time[i] > (dataS->time[c]) && data->time[i] < dataS->time[c] + 300)
		{
			flag=1;
			if(max < data->signal[i])
			{
				
				max = data->signal[i];
				stocki = i;
			}	
		}
		if(data->time[i] >= dataS->time[c] + 300 && flag == 1)
		{
			TptsD->signal[c] = max;
			TptsD->time[c] = data->time[stocki];
			flag = 0;
			c++;
			max = -1000000;
		}
	}
	debug("\033[1;33mFin de Tpts\033[0m\n");
	return(TptsD);
}

/*
 * Cette fonction permmet de calculer la moyenne temporel entre deux type d'onde sur toute les periodes
 * Elle prend en entré deux structure signal_data contenant les valeurs des piques de deux ondes
 * Elle renvoie un double contenant la moyenne
 */

double moyenneSignal(signal_data *signalA, signal_data *signalB){
	debug("\033[1;33mDébut de moyenneSignal\033[0m\n");
	double moyenne = 0;
	unsigned long i;

	for (i=0 ; i<signalA->nb_samples ; i++){
		if (signalA->time[i]!=0 || signalB->time[i]==0){
			moyenne += (signalA->time[i] - signalB->time[i]);
		}
	}

	moyenne /= signalA->nb_samples;
	debug("\033[1;33mFin de moyenneSignal\033[0m\n");
	return moyenne;
}

double *moyenneBPM(signal_data *signalR, double pas){
	debug("\033[1;33mDébut de moyenneBPM\033[0m\n");
	// Le nombre de moyenne doit être au plus égale au nombre de signaux R
	double *mBPM = malloc(signalR->nb_samples * sizeof(double));

	unsigned long i = 0, j = 0;
	double timeStop = 1000 * pas;
	double moyenne=0;


	while (i < signalR->nb_samples){
		
		if (signalR->time[i] > timeStop){
			moyenne /= (pas / 60);
			mBPM[j] = moyenne; 
			debug("%lf\n", moyenne);
			moyenne = 0;
			j++;
			timeStop += 1000 * pas;
			printf("%lf\n",timeStop);
		}
		moyenne++;
		i++;
	}
	debug("\033[1;33mFin de moyenneBPM\033[0m\n");
	return mBPM;
}