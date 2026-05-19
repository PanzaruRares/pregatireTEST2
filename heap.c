#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrMasini;
	Masina* vector;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.vector = malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	int nodSt = pozitieNod * 2 + 1;
	int nodDr = pozitieNod * 2 + 2;
	int pozmax = pozitieNod;
	if (nodSt < heap.nrMasini && heap.vector[nodSt].id > heap.vector[pozmax].id)
	{
		pozmax = nodSt;
	}
	if (nodDr < heap.nrMasini && heap.vector[nodDr].id > heap.vector[pozmax].id)
	{
		pozmax = nodDr;
	}
	if (pozmax != pozitieNod)
	{
		Masina aux = heap.vector[pozmax];
		heap.vector[pozmax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozmax < (heap.nrMasini - 2) / 2)
		{
			filtreazaHeap(heap, pozmax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul

	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(12);
	int poz = 0;
	while (!(feof(f)))
	{
		Masina m = citireMasinaDinFisier(f);
		heap.vector[poz++] = m;
	}
	heap.nrMasini = poz;
	heap.lungime = poz;
	fclose(f);
	for (int i = (heap.nrMasini - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrMasini; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	for (int i = heap.nrMasini; i < heap.lungime; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	Masina m;
	if (heap->nrMasini<=0)
	{
		m.id = -1;
		return m;
	}
	Masina aux = heap->vector[0];
	heap->vector[0] = heap->vector[heap->nrMasini - 1];
	heap->vector[heap->nrMasini - 1] = aux;
	heap->nrMasini--;
	filtreazaHeap(*heap, 0);
	return heap->vector[heap->nrMasini];
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrMasini; i++)
	{
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
		
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;

}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini_arbori.txt");
	int nr = h.nrMasini;
	for (int i = 0; i < nr; i++)
	{
		extrageMasina(&h);
	}
	afiseazaHeapAscuns(h);
	dezalocareHeap(&h);
	afisareHeap(h);
	return 0;
}