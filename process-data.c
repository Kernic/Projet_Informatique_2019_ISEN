#include "common.h"
#include "traitementSignal.h"
#include "InfoFile.h"
/*
 * Cette fonction permet d'éviter les problème lors de l'exécution du programme
 * Elle prend en entré argv[0] et ne ranvois rien 
 */

void Usage(char *execname)
{
	printf("\n\033[0;31m[Erreur] parametres en entree\n");
	printf("Usage : %s <numero de fichier>\033[0m\n", execname);
	

}




/* Fonction main */
int main( int argc, char *argv[] )
{
	debug("\033[1;33mDébut du main\033[0m\n"); 
	// Déclaration des variables 
	signal_data *signalLecture;
	
	//double moyenneSignal;
	
	if( argc != 2 )
	{
		Usage( argv[0] );
      return(1);
	}

	/* The file number is stored in argv[1] (character array) */
	FILE* fichier = openFile(argv[1]);
	signalLecture = lecture(fichier); // Lecture de l'amplitude en fonction du temps
	fclose(fichier);

	fichierInfo(argv[1], signalLecture, 5);
	
	// test de stockage
	debug("\nVérification du stockage : \nDonné stocké a la position 100/101 (amplitude;temps): %lf;%lf/%lf;%lf\n\n", signalLecture->time[101], signalLecture->signal[101], signalLecture->time[102], signalLecture->signal[102]); 

	// Libération de la mémoire du signal_data
	free_signal_data(signalLecture);
    debug("\033[1;33mFin du main\033[0m\n");  

	return(0);
}
