#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Poly* Position;
typedef struct Poly {
	int coef;
	int pow;
	Position next;
} Polynom;

int readFromFile(const char* fileName, Position headFirst, Position headSecond);
int parseLineToPoly(char* buffer, Position head);
int insertSorted(Position head, int coef, int pow);
Position createNode(int coef, int pow);
int addition(Position headFirst, Position headSecond, Position sum);
int multiplication(Position headFirst, Position headSecond, Position product);
void printPoly(const char* name, Position head);
void freeMemory(Position head);

int main()
{
	Polynom headFirst = { .coef = 0, .pow = 0, .next = NULL };
	Polynom headSecond = { .coef = 0, .pow = 0, .next = NULL };
	Polynom headSum = { .coef = 0, .pow = 0, .next = NULL };
	Polynom headProduct = { .coef = 0, .pow = 0, .next = NULL };

	if (readFromFile("Poly.txt", &headFirst, &headSecond) != 0)
	{
		printf("Greska pri otvaranju ili citanju datoteke.\n");
		return -1;
	}

	printf("--- Procitani polinomi ---\n");
	printPoly("P1(x)", &headFirst);
	printPoly("P2(x)", &headSecond);

	addition(&headFirst, &headSecond, &headSum);
	printf("\n--- Rezultati ---\n");
	printPoly("P1(x) + P2(x)", &headSum);

	multiplication(&headFirst, &headSecond, &headProduct);
	printPoly("P1(x) * P2(x)", &headProduct);

	freeMemory(&headFirst);
	freeMemory(&headSecond);
	freeMemory(&headSum);
	freeMemory(&headProduct);

	return 0;
}

int readFromFile(const char* fileName, Position headFirst, Position headSecond) {
	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		return -1;
	}

	char buffer[1024];
	if (fgets(buffer, sizeof(buffer), fp) != NULL)
		parseLineToPoly(buffer, headFirst);
	if (fgets(buffer, sizeof(buffer), fp) != NULL)
		parseLineToPoly(buffer, headSecond);

	fclose(fp);
	return 0;
}

int parseLineToPoly(char* buffer, Position head) {
	int coef = 0;
	int pow = 0;
	int bytesRead = 0;
	char* currentPosition = buffer;

	while (sscanf(currentPosition, "%d %d %n", &coef, &pow, &bytesRead) == 2) {
		insertSorted(head, coef, pow);
		currentPosition += bytesRead;
	}

	return 0;
}

int insertSorted(Position head, int coef, int pow) {
	if (coef == 0)
		return 0;

	Position prev = head;
	Position curr = head->next;

	while (curr != NULL && curr->pow > pow) {
		prev = curr;
		curr = curr->next;
	}

	if (curr != NULL && curr->pow == pow) {
		curr->coef += coef;
		if (curr->coef == 0) {
			prev->next = curr->next;
			free(curr);
		}
	}
	else {
		Position newNode = createNode(coef, pow);
		if (!newNode)
			return -1;
		newNode->next = curr;
		prev->next = newNode;
	}

	return 0;
}

Position createNode(int coef, int pow) {
	Position newNode = (Position)malloc(sizeof(Polynom));
	if (!newNode) {
		printf("Greska pri alokaciji memorije.\n");
		return NULL;
	}
	newNode->coef = coef;
	newNode->pow = pow;
	newNode->next = NULL;
	return newNode;
}

int addition(Position headFirst, Position headSecond, Position sum) {
	Position p = headFirst->next;
	Position q = headSecond->next;

	while (p != NULL) {
		insertSorted(sum, p->coef, p->pow);
		p = p->next;
	}
	while (q != NULL) {
		insertSorted(sum, q->coef, q->pow);
		q = q->next;
	}
	return 0;
}

int multiplication(Position headFirst, Position headSecond, Position product) {
	for (Position p = headFirst->next; p != NULL; p = p->next) {
		for (Position q = headSecond->next; q != NULL; q = q->next) {
			insertSorted(product, p->coef * q->coef, p->pow + q->pow);
		}
	}
	return 0;
}

void printPoly(const char* name, Position head) {
	Position temp = head->next;
	printf("%s = ", name);

	if (temp == NULL) {
		printf("0\n");
		return;
	}

	while (temp != NULL) {
		if (temp->coef > 0 && temp != head->next) {
			printf(" + ");
		}
		else if (temp->coef < 0) {
			printf(" - ");
		}

		int absCoef = abs(temp->coef);
		if (temp->pow == 0) {
			printf("%d", absCoef);
		}
		else if (temp->pow == 1) {
			if (absCoef == 1) printf("x");
			else printf("%dx", absCoef);
		}
		else {
			if (absCoef == 1) printf("x^%d", temp->pow);
			else printf("%dx^%d", absCoef, temp->pow);
		}

		temp = temp->next;
	}
	printf("\n");
}

void freeMemory(Position head) {
	Position temp = head->next;
	while (temp != NULL) {
		Position toDelete = temp;
		temp = temp->next;
		free(toDelete);
	}
	head->next = NULL;
}