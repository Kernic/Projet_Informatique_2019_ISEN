#include"common.h"
#include"traitementSignal.h"

/*
 * Cette fonction permet d'ouvrir un fichier en fonction du nombre écrit lors du lancement du programme
 * Elle prend en entré argv[1] et renvoie le fichier ouvert
 */

FILE* openFile (char *nbFichier){
	// Creation du chemain du fichier en fonction du nombre donne en entre
	char str1[16] = "./data/ECG";
	char str2[] = ".txt";
	strcat(str1, nbFichier);
	strcat(str1, str2);

	//ouverture du fichier 
	FILE *fichier = fopen (str1,"r");
	if (fichier == NULL){
		printf("\033[0;31m[Erreur] Le fichier n'existe pas, verrifier son existance dans le dossier data/\033[0m\n");
		exit(0);
	}
	
	return fichier; 
}

/*
 * Cette fonction permet de créé le fichier information à partire d'un fichier data
 * Elle prend en entré les structures signal_data cotenants les données sur les ondes ainci que le numéro de fichier entré par l'exécuteur du programme
 * Elle ne renvoie rien mais vas créé un fichier dans le dossier ./data/ 
 */

void fichierInfo(char* numFichier, signal_data* data, double pas){
	debug("\033[1;33mDébut de fichierInfo\033[0m\n");
    double Rref;
	unsigned long nPeriodes;
    double *mBPM;
	signal_data *R; 
	signal_data *Q;
	signal_data *S;
	signal_data *P;
	signal_data *T;

    /*Traitement du signal*/
    
    //Onde R	
    Rref=rPointRef(data);
	nPeriodes=nbR(data,Rref);
    debug("Nomnbre de periode : %lu\n",nPeriodes);
    R = Rpts(data, Rref, nPeriodes);

    // Récupération des ondes Q
	Q = Qpts(data, R, nPeriodes);

    // Récupération des ondes S
	S = Spts(data, R, nPeriodes); 

    // Récupération des ondes P
	P= Ppts(data, Q, nPeriodes);
	
    // Récupération des ondes T
	T = Tpts(data, S, nPeriodes);
    
    // Calcule des BPM en fonction d'un pas
    mBPM = calloc(R->nb_samples, sizeof(double));
    mBPM = moyenneBPM(R, pas);

    // Création du nom des fichier 
	char str1 [30] = "./data/ECG";
	char str2  [] = "-info.txt";
	unsigned long i=0;
	strcat(str1, numFichier);
	strcat(str1, str2);

	FILE* fichier = fopen(str1, "w+");
	
	// Nom du fichier 
	fprintf(fichier, "ECG");
	fprintf(fichier, "%s", numFichier);
	fprintf(fichier, ".txt\n\n");

	
	// BPM
	unsigned long t = (data->time[data->nb_samples - 1] / 1000);
	double nbT = (R->nb_samples); 
	fprintf(fichier, "%f\n", (nbT / t)*60);

	// Moyenne QRS 
	fprintf(fichier, "%lf\n", moyenneSignal(S, Q));

	// Moyenne QT
	fprintf(fichier, "%lf\n", moyenneSignal(T, Q));
	
	// Moyenne PQ
	fprintf(fichier, "%lf\n", moyenneSignal(Q, P));

	// Separation 
	fprintf(fichier, "###BEGIN DATA WAVES\n");

	// Data Waves
	while (i<nbT){
		// P Wave <Temps> <amplitude>
		fprintf(fichier, "%lf %lf ", P->time[i], P->signal[i]);
		
		// Q Wave <Temps> <amplitude>
		fprintf(fichier, "%lf %lf ", Q->time[i], Q->signal[i]);
		
		// R Wave <Temps> <amplitude>
		fprintf(fichier, "%lf %lf ", R->time[i], R->signal[i]);

		// S Wave <Temps> <amplitude>
		fprintf(fichier, "%lf %lf ", S->time[i] , S->signal[i]); 
		
		// T Wave <Temps> <amplitude>
		fprintf(fichier, "%lf %lf", T->time[i], T->signal[i]);
		// End of line 
		fprintf(fichier, "\n");
		i++;
	}

	// Separation 
	fprintf(fichier, "###END DATA WAVES\n");

	fprintf(fichier, "###BEGIN DATA HEARTBEAT\n");



	// Data Heartbeat
    i = 0; 
    while (i <= (R->nb_samples) && mBPM[i] != 0){
	    fprintf(fichier, "%lf\n", mBPM[i]);
        i++;
    }

	// Separation
	fprintf(fichier, "###END DATA HEARTBEAT\n");	

	// Print du pas pour programme python
	fprintf(fichier, "###BEGIN PAS\n%lf\n###END PAS\n", pas);

	fclose(fichier);

    // Libération de la mémoire
    free_signal_data(P);
    free_signal_data(Q);
    free_signal_data(R);
    free_signal_data(S);
    free_signal_data(T);

	debug("\033[1;33mFin de fichierInfo\033[0m\n");
}