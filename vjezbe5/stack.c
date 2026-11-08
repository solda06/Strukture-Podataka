#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef struct Stack* Position;
typedef struct Stack{
	int number;
	Position next;
}stack;

int readFromFile(Position head, char* Fname);
int parseAndCalculate(Position head, char* buffer);
int Push(Position head, int number);
int Pop(Position head, int* op);
int DeleteAll(Position head);

int main() {
	stack head = { .number = 0,.next = NULL };
	if (readFromFile(&head, "stack.txt") == 0)
		printf("Podatci uspjesno ucitani iz datoteke.\n");
	else {
		printf("Greska pri otvaranju datoteke!\n");
		return -1;
	}
	int finalResult = 0;
	if (Pop(&head, &finalResult) == 0 && head.next == NULL) {
		printf("Rezultat postfiksa izraza iznosi: %d\n", finalResult);
	}
	DeleteAll(&head);

	return 0;
}
int readFromFile(Position head, char *Fname) {
	FILE* fp = fopen(Fname, "r");
	if (!fp)
		return -1;
	char buffer[100];
	if (fgets(buffer, sizeof(buffer), fp) == NULL) {
		printf("Datoteka je prazna ili je doslo do greske\n");
		return -1;
    }
	fclose(fp);
	return parseAndCalculate(head,buffer);
}
int parseAndCalculate(Position head, char *buffer) {
	char* currentBuffer = buffer;
	int status = 0;
	int numBytes = 0;
	int number = 0;
	char operator=0;
	while (*currentBuffer != '\0') {
		status = sscanf(currentBuffer, "%d %n", &number, &numBytes);
		if (status == 1) {
			Push(head, number);
			currentBuffer += numBytes;
		} else {
			status = sscanf(currentBuffer, "%c %n", &operator,&numBytes);
			if(status==1){ 
				int op2 = 0, op1 = 0;
				if (Pop(head, &op2) != 0 || Pop(head, &op1) != 0) {
					printf("Greska: nedovoljno operanada!\n");
					return -1;
				}
				int result = 0;
				switch (operator) {
				case '+': result = op1 + op2; break;
				case '-': result = op1 - op2; break;
				case '*': result = op1 * op2; break;
				case'/':
					if (op2 == 0) {
						printf("Ne moze se dijeliti s nulom!\n");
						return -1;
					}
						result = op1 / op2;
						break;
				default:
					printf("Operator koji je ucitan nije valjan.\n");
					return -1; 
				}
				Push(head, result);
				currentBuffer += numBytes;
			}
		}
	}
	return 0;
}
int Push(Position head, int number) {
	Position newElement = (Position)malloc(sizeof(stack));
	if (!newElement) {
		printf("Greska pri alokaciji memorije.\n");
		return -1;
	}
	newElement->number = number;
	newElement->next = head->next;
	head->next = newElement;
	return 0;
}
int Pop(Position head, int* op) {
	Position toDelete = head->next;
	if (!toDelete)
		return -1;
	head->next = toDelete->next;
	*op = toDelete->number;
	free(toDelete);
	return 0;
}
int DeleteAll(Position head) {
	Position temp;
	while (head->next != 0) {
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
	return 0;
}