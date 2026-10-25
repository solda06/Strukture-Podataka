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
} Person;

int insertAfter(Position p, Position newP);
int insertBegin(Position head, char* ime, char* prezime, int godina);
int insertEnd(Position head, char* ime, char* prezime, int godina);
int printList(Position first);
Position findLName(char* lastN, Position first);
int deletePerson(char* lastN, Position head);
int insertAfterPerson(char* targetLName, Position head, char* ime, char* prezime, int godina);
int insertBeforePerson(char* targetLName, Position head, char* ime, char* prezime, int godina);
int sortList(Position head);
int writeToFile(Position first, char* filename);
int readFromFile(Position head, char* filename);

int main() {
    Person Head = { .ime = "", .prezime = "", .godina = 0, .next = NULL };
    Position p = &Head;
    int odabir = -1;
    char ime[M], prezime[M], ciljno_prezime[M];
    int godina;

    if (readFromFile(p, "osobe.txt") == 0) {
        printf("Podaci uspjesno ucitani iz 'osobe.txt'.\n");
    }
    else {
        printf("Datoteka 'osobe.txt' ne postoji ili je prazna. Zapoocinjem s praznom listom.\n");
    }

    while (odabir != 0) {
        printf("\n---------------- IZBORNIK ----------------\n");
        printf("1. Dodaj osobu na pocetak\n");
        printf("2. Dodaj osobu na kraj\n");
        printf("3. Dodaj osobu iza odredenog prezimena\n");
        printf("4. Dodaj osobu ispred odredenog prezimena\n");
        printf("5. Obrisi osobu (po prezimenu)\n");
        printf("6. Sortiraj listu po prezimenu\n");
        printf("7. Ispisi listu\n");
        printf("0. Izlaz iz programa\n");
        printf("\n Vas odabir: ");
        scanf("%d", &odabir);

        switch (odabir) {
        case 1:
            printf("Unesite ime, prezime i godinu rodenja: ");
            scanf("%s %s %d", ime, prezime, &godina);
            insertBegin(p, ime, prezime, godina);
            break;

        case 2:
            printf("Unesite ime, prezime i godinu rodenja: ");
            scanf("%s %s %d", ime, prezime, &godina);
            insertEnd(p, ime, prezime, godina);
            break;

        case 3:
            printf("Unesite prezime osobe IZA koje zelite dodati: ");
            scanf("%s", ciljno_prezime);
            printf("Unesite ime, prezime i godinu rodenja nove osobe: ");
            scanf("%s %s %d", ime, prezime, &godina);
            insertAfterPerson(ciljno_prezime, p, ime, prezime, godina);
            break;

        case 4:
            printf("Unesite prezime osobe ISPRED koje zelite dodati: ");
            scanf("%s", ciljno_prezime);
            printf("Unesite ime, prezime i godinu rodenja nove osobe: ");
            scanf("%s %s %d", ime, prezime, &godina);
            insertBeforePerson(ciljno_prezime, p, ime, prezime, godina);
            break;

        case 5:
            printf("Unesite prezime osobe koju zelite obrisati: ");
            scanf("%s", prezime);
            if (deletePerson(prezime, p) == 0) {
                printf("Osoba uspjesno obrisana.\n");
            }
            else {
                printf("Osoba nije pronadena.\n");
            }
            break;

        case 6:
            sortList(p);
            printf("Lista je sortirana po prezimenima.\n");
            break;

        case 7:
            printf("\n--- TRENUTNA LISTA ---\n");
            printList(p->next);
            break;

        case 0:
            printf("Zatvaram meni...\n");
            break;

        default:
            printf("Pogresan unos! Pokusajte ponovno.\n");
        }
    }

    if (writeToFile(p->next, "nove_osobe.txt") == 0) {
        printf("Azurirana lista je spremljena u 'nove_osobe.txt'.\n");
    }
    else {
        printf("Greska pri spremanju u novu datoteku.\n");
    }

    while (p->next != NULL) {
        deletePerson(p->next->prezime, p);
    }

    return 0;
}

int insertAfter(Position p, Position newP) {
    newP->next = p->next;
    p->next = newP;
    return 0;
}

int insertBegin(Position head, char* ime, char* prezime, int godina) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return -1;
    strcpy(newPerson->ime, ime);
    strcpy(newPerson->prezime, prezime);
    newPerson->godina = godina;
    insertAfter(head, newPerson);
    return 0;
}

int insertEnd(Position head, char* ime, char* prezime, int godina) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return -1;
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

int printList(Position first) {
    Position temp = first;
    if (temp == NULL) {
        printf("Lista je prazna.\n");
        return -1;
    }
    while (temp != NULL) {
        printf("%s %s %d\n", temp->ime, temp->prezime, temp->godina);
        temp = temp->next;
    }
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
        prev->next = curr->next;
        free(curr);
        return 0;
    }
    return -1;
}

int insertAfterPerson(char* targetLName, Position head, char* ime, char* prezime, int godina) {
    Position target = findLName(targetLName, head->next);
    if (target == NULL) return -1;

    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return -1;

    strcpy(newPerson->ime, ime);
    strcpy(newPerson->prezime, prezime);
    newPerson->godina = godina;
    insertAfter(target, newPerson);
    return 0;
}

int insertBeforePerson(char* targetLName, Position head, char* ime, char* prezime, int godina) {
    Position prev = head;
    Position curr = head->next;

    while (curr != NULL && strcmp(curr->prezime, targetLName) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) return -1;

    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) return -1;

    strcpy(newPerson->ime, ime);
    strcpy(newPerson->prezime, prezime);
    newPerson->godina = godina;
    insertAfter(prev, newPerson);
    return 0;
}

int sortList(Position head) {
    Position j, prev_j, tempEnd = NULL;
    int swapped;
    if (head->next == NULL) return 0;
    do {
        swapped = 0;
        prev_j = head;
        j = head->next;
        while (j->next != tempEnd) {
            if (strcmp(j->prezime, j->next->prezime) > 0) {
                Position temp = j->next;
                prev_j->next = temp;
                j->next = temp->next;
                temp->next = j;
                swapped = 1;
                prev_j = temp;
            }
            else {
                prev_j = j;
                j = j->next;
            }
        }
        tempEnd = j;
    } while (swapped);
    return 0;
}

int writeToFile(Position first, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) 
        return -1;
    Position temp = first;
    while (temp != NULL) {
        fprintf(file, "%s %s %d\n", temp->ime, temp->prezime, temp->godina);
        temp = temp->next;
    }
    fclose(file);
    return 0;
}

int readFromFile(Position head, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
        return -1;
    char ime[M], prezime[M];
    int godina;
    while (fscanf(file, "%s %s %d", ime, prezime, &godina) == 3) {
        insertEnd(head, ime, prezime, godina);
    }
    fclose(file);
    return 0;
}