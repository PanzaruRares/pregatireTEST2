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
typedef struct Nod Nod;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod
{
	Nod* stanga;
	Nod* dreapta;
	Masina m;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArboreRecursiv(Nod** radacina, Masina masinaNoua) {
	if (*radacina == NULL) {
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->dreapta = NULL;
		nodNou->stanga = NULL;
		nodNou->m = masinaNoua;

		*radacina = nodNou;
	}
	else {
		if (masinaNoua.id < (*radacina)->m.id) {
			adaugaMasinaInArboreRecursiv((&(*radacina)->stanga), masinaNoua);
		}
		else {
			adaugaMasinaInArboreRecursiv((&(*radacina)->dreapta), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		adaugaMasinaInArboreRecursiv(&radacina, citireMasinaDinFisier(f));
	}
	fclose(f);
	return radacina;
}

void afisareMasiniDinArbore(Nod* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (radacina)
	{
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasina(radacina->m);
		afisareMasiniDinArbore(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina)
	{
		Nod* stanga = (*radacina)->stanga;
		Nod* dreapta = (*radacina)->dreapta;

		free((*radacina)->m.numeSofer);
		free((*radacina)->m.model);
		free(*radacina);
		*radacina = NULL;
		dezalocareArboreDeMasini(&stanga);
		dezalocareArboreDeMasini(&dreapta);
	}
}

Masina getMasinaByID(/*arborele de masini*/Nod* radacina,int id) {
	Masina m;
	Nod* aux = radacina;
	if (aux->m.id == id)
	{	
		m = aux->m;
		m.model = malloc(strlen(aux->m.model) + 1);
		strcpy(m.model, aux->m.model);
		m.numeSofer = malloc(strlen(aux->m.numeSofer) + 1);
		strcpy(m.numeSofer, aux->m.numeSofer);
	}
	else
	{
		if (aux->m.id < id)
		{
			getMasinaByID(radacina->dreapta, id);
		}
		else
			getMasinaByID(radacina->stanga, id);
	}
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if(radacina)
		return 1 + determinaNumarNoduri(radacina->dreapta) + determinaNumarNoduri(radacina->stanga);
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina)
	{
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	return 0;
}

float calculeazaPretTotal(/*arbore de masini*/) {
	//calculeaza pretul tuturor masinilor din arbore.
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("fisier.txt");
	afisareMasiniDinArbore(radacina);
	afisareMasina(getMasinaByID(radacina,5));

	return 0;
}