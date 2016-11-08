/*
 ============================================================================
 Name        : MemAlloc.c
 Author      : Bart.Lamiroy@univ-lorraine.fr
 Version     : 1.0
 Copyright   : https://creativecommons.org/licenses/by-sa/3.0/fr/
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Structure de gestion de mémoire composée de :
 * - arraySize : la taille de la zone de stockage (en octets)
 * - memArray : la zone de stockage (supposée de arraySize octets)
 * - memAlloc : bitmap d'allocation des octets de memArray.
 */
struct memStruct {
	size_t arraySize;

	unsigned char* memArray;
	unsigned char* memAlloc;
};

#define BYTESIZE 8
#define BYTEMASK 0x80

/*
 * Fonction qui initialise une structure de type memStruct.
 * Si l'argument mem est NULL une nouvelle structure est allouée.
 */

struct memStruct* initMem(size_t size, struct memStruct* mem) {

	if(mem == NULL) {
		mem = (struct memStruct*) malloc(sizeof(struct memStruct*));
	}

	/*
	 * Allouer un tableau de taille demandée +
	 * allouer le tableau d'occupation corresondant initialisé à 0
	 */
	mem->arraySize = size;
	mem->memArray = (unsigned char*) malloc(sizeof(unsigned char)*size);
	mem->memAlloc = (unsigned char*) malloc(sizeof(unsigned char)*size / BYTESIZE);

	for (int i = 0; i < (size / BYTESIZE); i++) {
		mem->memAlloc[i] = 0;
	}

	return mem;
}

/*
 * Alloue un bloc de taille size dans la structure mem
 * et retourne son adresse ou NULL s'il est impossible de trouver
 * un bloc de la taille requise.
 */
void* alloc(size_t size, struct memStruct* mem) {

	void* returnAddress = NULL;

	/*
	 * Phase 1 : on parcourt le tableau memAlloc afin de trouver
	 * un endroit dans memArray ayant suffisamment de cases vides
	 *
	 */

	bool place = false;
	int compteur = 0;
	int mask = 255 >> size + 1;
	
	// dans le cas ou on néglige les effets de bords
	while (!(place)) {
		for (int i = 0; i < (mem->arraySize/BYTESIZE); i ++) {
			mask = 255 >> size + 1; //le + 1 correspond à un bit reserve pour indiquer size dans memArray
			for (int j = 0; j < (BYTESIZE/size); j ++) { //dans le cas ou on neglige les effets de bords et que size <= BYTESIZE
				if ((mem->memAlloc[i] & mask) == 0) {
					compteur = i;
					//returnAddress = *(mem->memArray[i*BYTESIZE]);
					place = true;
				} else {
					mask = mask << 1;
				}
			}
		}
	}

	/*
	 * Phase 2 : on positionne les bits trouvés à 1 dans memAlloc
	 * pour signifier que les cases correspondantes dans memArray
	 * sont maintenant allouées
	*/
	
	mem->memAlloc[compteur] = (mem->memAlloc[compteur] | mask);

	return returnAddress;
}

/*
 * Affiche le tableau d'occupation mémoire
 */

void printMemAllocation(struct memStruct* mem) {

	fprintf(stdout,"--- %ld\n", sizeof(size_t));

	for(int i=0; i<mem->arraySize /BYTESIZE; ++i) {
		fprintf(stdout,"%X\n",mem->memAlloc[i]);
	}

	fprintf(stdout,"---\n");

}

/*
 * Libère l'espace allouée au pointeur ptr dans la structure mem
 */
/**void release(void* ptr, struct memStruct* mem) {

	int releaseSize = mem->memArray[*ptr - 1]; //on a sauvegarde en memoire la place que prenait ce que l'on a mis en memoire
	int mask = 255 >> (BYTESIZE - releaseSize);
	mask = mask << *ptr/BYTESIZE;

	//memAlloc[resultat de la division de "ptr" par bytesize] = memAlloc[] 
}*/

int main(void) {

	puts("Start\n");


	struct memStruct memory;
	initMem(64, &memory);

	puts("Init\n");

	printMemAllocation(&memory);

	void* p1 = alloc(2, &memory);
	printMemAllocation(&memory);
	//void* p2 = alloc(10, &memory);

	puts("Alloc\n");

	//printMemAllocation(&memory);

	//release(p1, &memory);
	//p2 = alloc(10, &memory);


	//printMemAllocation(&memory);

	puts("End\n");
	getchar();
	return EXIT_SUCCESS;
}

