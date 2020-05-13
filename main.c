#include <conio.h>
#include <stdio.h>
#include "field.h"
#include "console.h"
#include "read_file.h"

#define BACK_SPACE 8
#define ENTER 13
#define ESC 27
#define UP 72   // 224 72
#define DOWN 80 // 224 80
#define LEFT 75 // 224 75
#define RIGHT 77// 224 77

int main()
{
    SetConsoleTitle("Snow Simulator");

    SetConsoleOutputCP(1251);
    printf("Вы можете выбрать шрифт и его размер в свойствах этого окна.\n"
           "Укажите имя текстового файла с преградами: ");
    char nameFile[1024];
    FILE* f = NULL;
    while (!f)
    {
        fgets(nameFile, 1024, stdin);
        if (nameFile[strlen(nameFile)-1] == '\n' || nameFile[strlen(nameFile)-1] == '\r')
            nameFile[strlen(nameFile)-1] = '\0';
        f = fopen(nameFile, "r");
        if (!f)
            printf("\nНе смог открыть файл. Укажите снова: ");
    }

    Field* field = getFieldFromFile(f);
    if (!field)
    {
        printf("ERROR: invalid field initialization");
        return 0;
    }
    if (!initConsole(field))
    {
        printMessage("ERROR: invalid console initialization");
        return 0;
    }
    writeHelpMessage();

    int isRandomSnowfall = FALSE;
    for (;; Sleep(50))
    {
        int keyPressed = 0;
        if (kbhit())
        {
            int ch = getch();
            if (ch == ESC) // ESC
                break;

            if (ch == 0 || ch == 224)
            {
                ch = getch();
                if (ch == LEFT || ch == RIGHT)
                    keyPressed = ch;
            }
            else
                keyPressed = ch;
        }

        if (keyPressed == ' ' || keyPressed == ENTER)
            createNewFlakeFromCannon(field);
        else if ((keyPressed == LEFT) || (keyPressed == '4'))
            moveCannonLeft(field);
        else if ((keyPressed == RIGHT) || (keyPressed == '6'))
            moveCannonRigh(field);
        else if ((keyPressed == 'r') || (keyPressed == 'R'))
            isRandomSnowfall = !isRandomSnowfall;

        if (isRandomSnowfall)
        {
            #define MAXFLAKES 8.
            int numFlakes = (int)(rand()*MAXFLAKES/RAND_MAX);
            for (int i = 0; i < numFlakes; ++i)
                createNewFlake(field, (int)(rand()*field->size.X/RAND_MAX));
        }

        writeConsole(field);
        updateField(field);
    }
    destroyField(field);
    destroyConsole();
    fclose(f);
}
