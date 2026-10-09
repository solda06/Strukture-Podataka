#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define N 50
#define MAX_GRADE 70

typedef struct Student {
	char name[N];
	char surname[N];
	int grade;
} studenti;

float relativniBroj(int grade) {

	return (float)grade / (float)MAX_GRADE * 100;
}

int main()
{
	char buffer[N] = { '\0' };
	int counter = 0;
	int i;
	studenti* student = NULL;
	FILE* fp = fopen("studenti.txt", "r");
	if (!fp)
	{
		printf("Greska u otvaranju datoteke.\n");
		return -1;
	}
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		counter++;
	}
	if (counter == 0) {
		printf("Nema podataka u datoteci.\n");
		fclose(fp);
		return 0;
	}
    rewind(fp);
	student = (studenti*)malloc(counter * sizeof(*student));
	if (!student) {
		printf("Greska pri alokaciji memorije za studente.\n");
		fclose(fp);
		return -1;
	}
	for (i = 0; i < counter; i++)
	{

		fscanf(fp, "%s %s %d", student[i].name, student[i].surname, &student[i].grade);
	}
	
    fclose(fp);

    float* relativeGrade = NULL;
    relativeGrade = (float*)malloc(counter * sizeof(float));
    if (!relativeGrade) {
        printf("Greska pri alokaciji memorije za relativne ocjene.\n");
        free(student);
        return -1;
    }

    for (i = 0; i < counter; i++)
        relativeGrade[i] = relativniBroj(student[i].grade);

    for (i = 0; i < counter; i++)
        printf("%s %s %d -> %.2f%%\n", student[i].name, student[i].surname, student[i].grade, relativeGrade[i]);

    free(relativeGrade);
    free(student);

	return 0;
}

