#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 50

typedef struct _Person* Position;
typedef struct _Person {
	char ime[M];
	char prezime[M];
	int godina;
	Position next;
}Person;

int insertAfter(Position p, Position newP);
int insertBegin(Position head,char* ime, char* prezime, int godina);
int printList(Position first);
Position findLName(char* lastN, Position first);
int deletePerson(char* lastN, Position head);

int main() {
	Person Head = { .ime = "",.prezime = "",.godina = 0, .next = NULL };
	Position p = &Head;
	// dodavanje elemenata
	insertBegin(p, "Ivan", "Ivic", 2006);
	insertBegin(p, "Mate", "Matic", 2000);
	insertEnd(p, "Marko", "Horvat", 2002);
	printf("Lista nakon dodavanja elemenata:\n");
	printList(p->next);
	Position trazeni = findLName("Ivic", p->next);
	if (trazeni != NULL) {
		printf("Pronadena osoba %s %s %d\n", trazeni->ime, trazeni->prezime, trazeni->godina);
	}
	else {
		printf("Osoba nije pronadena");
	}
	deletePerson("Ivic", p);
	printf("Lista nakon brisanja:\n");
	printList(p->next);
	return 0;
}

int insertAfter(Position p, Position newP) {
	newP->next = p->next;
	p->next = newP;
	return 0;
}
int insertBegin(Position head, char* ime, char* prezime, int godina) {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		return -1;
	}
	strcpy(newPerson->ime, ime);
	strcpy(newPerson->prezime, prezime);
	newPerson->godina = godina;
	insertAfter(head, newPerson);
	return 0;
}
int printList(Position first) {
	Position temp = first;
	if (temp == NULL) {
		printf("Lista je prazna\n");
		return -1;
	}
	while (temp != NULL) {
		printf("%s %s %d\n", temp->ime, temp->prezime, temp->godina);
		temp = temp->next;
		return 0;
	}
}
int insertEnd(Position head, char* ime, char* prezime, int godina) {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL)
		return -1;
	strcpy(newPerson->ime, ime);
	strcpy(newPerson->prezime, prezime);
	newPerson->godina = godina;
	Position temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
		insertAfter(temp, newPerson);
		return 0;
	}
Position findLName(char* lastN, Position first) {
	Position temp = first;
	while (temp != NULL) {
		if (strcmp(temp->prezime, lastN) == 0)
			return temp;
		temp = temp->next;
    }
	return NULL;
}
	int deletePerson(char* lastN, Position head) {
		Position prev = head;
		Position curr = head->next;
		while (curr != NULL && strcmp(curr->prezime, lastN) != 0) {
			prev = curr;
			curr = curr->next;
		}
		if (curr != NULL) {
			prev->next=curr->next;
			free(curr);
			return 0;
		}
		return -1;
	}