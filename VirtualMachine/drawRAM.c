#include "raylib.h"
#include "VM.h"
#include <time.h>
#include <math.h>
 
void draw_ram( processor_t *proc )
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(RAM_SIZE, RAM_SIZE, "raylib [core] example - basic window");
    Color *pixels = (Color*)malloc(RAM_SIZE * RAM_SIZE * sizeof(Color));

    for (int y = 0; y < RAM_SIZE; y++)
    {
        for (int x = 0; x < RAM_SIZE; x++)
        {
            int pixel = (int)proc->RAM[y * RAM_SIZE + x];
            byte g = (byte)(((pixel >> 8)  & 0xFF)/* * sin(time(NULL))*/);
            byte r = (byte)(((pixel >> 0)  & 0xFF)/* * sin(time(NULL))*/);
            byte b = (byte)(((pixel >> 16) & 0xFF)/* * sin(time(NULL))*/);
            byte a = 255;

            pixels[y * RAM_SIZE + x] = (Color){r, g, b, a};
        }
    }

    Image img = {
    .data = pixels,          // Указатель на массив
    .width = RAM_SIZE,          // Ширина
    .height = RAM_SIZE,        // Высота
    .mipmaps = 1,             // Количество мип-уровней (для начала просто 1)
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 // Формат: 4 байта на пиксель (RGBA)
    };
    Texture2D texture = LoadTextureFromImage(img);
SetTargetFPS(0);
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // ClearBackground(RAYWHITE);
            // for (int y = 0; y < RAM_SIZE; y++)
            // {
            //     for (int x = 0; x < RAM_SIZE; x++)
            //     {
            //         if ((x + y * RAM_SIZE) % 100 == 0) printf("i draw %d pixel\n", x + y * RAM_SIZE);
            //         Color clr = {(byte)proc->RAM[x + y * RAM_SIZE],
            //                      (byte)proc->RAM[x + y * RAM_SIZE],
            //                      (byte)proc->RAM[x + y * RAM_SIZE],
            //                      255};
            //         DrawPixel(x, y, clr);
            //     }
            // }
            DrawTexture(texture, 0, 0, WHITE);
            //printf("pisunbl\n");
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadImage(img);

    CloseWindow();

    return;
}


void dump_ram( processor_t *proc )
{
    for (int i = -1; i < RAM_SIZE; i++)
        printf("%3d ", i);
    printf("\n");
    for (int i = 0; i < RAM_SIZE; i++)
    {
        printf("%3d", i);
        for (int j = 0; j < RAM_SIZE; j++)
        {
            printf("%3c ", (unsigned char)proc->RAM[RAM_SIZE * i + j]);
        }
        printf("\n");
    }

    printf("\n\n\n\n\n\n\n");
    getchar();

    return;
}
