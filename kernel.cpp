#include <stdint.h>
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

void printf(char* str)
{
    //static unsigned short* VideoMemory = (unsigned short*)0xb8000;
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
        //VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    static const int mem = 200;
    int contl = 0;
    int cont = 0;
    int conts = 0;
    char line[1000];
    void OnKeyDown(char c)
    {
        if(cont >=  1)
        {
            printf("\n");
        }
        // guardamos el caracter tecleado en un arreglo
        line[cont] = c;
        if(cont != 0 && cont % 19 == 0)
        {
            contl = contl+1;
            conts = 0;
        }
        cont = cont + 1;
        conts = conts + 1;
        // metodo para imprimir el menu persistente
        showInterface();
    }
    void showInterface()
    {
        printf("+------------------------------------------------------------------------------+"); // 1
        printf("|                                    +----------------------------------------+|"); // 2
        printf("|                                    | Autor    : Prof. Victor                ||"); // 3
        printf("|                                    | Alumno   : Alexander Baylon Ibanez     ||"); // 4
        printf("|                                    | Curso    : Sistemas Operativos         ||"); // 5
        printf("|                                    | UCSP-2020            |         ver 0.1 ||"); // 6
        printf("|                                    +----------------------------------------+|"); // 7
        printf("|                                                                              |"); // 8
        printf("|                                                                              |"); // 9
        printf("|                                                                              |"); // 10
        printf("|                                                                              |"); // 11
        printf("|                                                                              |"); // 12
        printf("|                                                                              |"); // 13
        // linea 5
        if(contl >= 5)
        {
            printf("||                          -> "); //23 espacios // 14
            for(int i=(19*(contl-5)); i<((contl-4)*19); i++)
            {
                char* foo = "-";
                foo[0] = line[i];
                printf(foo);
            }
            printf(" <-");
            for(int i=0; i<25; i++)
            {
                printf(" ");
            }
            printf("||");
        }
        else
        {
             printf("|                                                                              |"); // 14
        }
        // linea 4
        if(contl >= 4)
        {
            printf("||                          -> "); //23 espacios // 15
            for(int i=(19*(contl-4)); i<((contl-3)*19); i++)
            {
                char* foo = "-";
                foo[0] = line[i];
                printf(foo);
            }
            printf(" <-");
            for(int i=0; i<25; i++)
            {
                printf(" ");
            }
            printf("||");
        }
        else
        {
             printf("|                                                                              |"); // 15
        }
        // linea 3
        if(contl >= 3)
        {
            printf("||                          -> "); //23 espacios // 16
            for(int i=(19*(contl-3)); i<((contl-2)*19); i++)
            {
                char* foo = "-";
                foo[0] = line[i];
                printf(foo);
            }
            printf(" <-");
            for(int i=0; i<25; i++)
            {
                printf(" ");
            }
            printf("||");
        }
        else
        {
             printf("|                                                                              |"); // 16
        }
        //linea 2
        if(contl >= 2)
        {
            printf("||                          -> "); //23 espacios // 17
            for(int i=(19*(contl-2)); i<((contl-1)*19); i++)
            {
                char* foo = "-";
                foo[0] = line[i];
                printf(foo);
            }
            printf(" <-");
            for(int i=0; i<25; i++)
            {
                printf(" ");
            }
            printf("||");
        }
        else
        {
             printf("|                                                                              |"); // 17
        }
        // linea 1
        if(contl >= 1)
        {
            printf("||                          -> "); //23 espacios // 18
            for(int i=(19*(contl-1)); i<(contl*19); i++)
            {
                char* foo = "-";
                foo[0] = line[i];
                printf(foo);
            }
            printf(" <-");
            for(int i=0; i<25; i++)
            {
                printf(" ");
            }
            printf("||");
        }
        else
        {
             printf("|                                                                              |"); // 18
        }
        printf("|______________________________________________________________________________|"); // 19
        printf("| ____________________________________________________________________________ |"); // 20
        printf("||                                                                            ||"); // 21
        printf("||                             "); //29 espacios
        for(int i=(19*contl); i<cont; i++)
        {
            char* foo = "-";
            foo[0] = line[i];
            printf(foo);
        }
        for(int i=0; i<(28+(19-conts)); i++)
        {
            printf(" ");
        }
        printf("||");
        printf("||____________________________________________________________________________||"); // 23
        printf("|______________________________________________________________________________|"); // 24
    }
};



extern "C" void kernelMain(const void* multiboot_structure, uint32_t  /*multiboot_magic*/)
{
    //printf("Kernel Sanpablo-OS! --- Alexander Arturo Baylon Ibanez\n");
    //printf("Kernel Sanpablo-OS! --- Alexander Arturo Baylon Ibanez");

    GlobalDescriptorTable gdt;
    // instanciar la tabla
    InterruptManager interrupts(&gdt);
    // instanciar el hardware y activarlo
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts,&kbhandler);

    interrupts.Activate();

    while(1);
}
