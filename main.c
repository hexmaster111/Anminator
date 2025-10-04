// filename "reader.c"

#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

#define TEXTFADE_IMPL
#include "textfade.h"

// GuiSlider((Rectangle){0,0,300,32, }, "", "", &mover.x, -100, 100);
// GuiSlider((Rectangle){0,32,300,32, }, "", "", &mover.y, -100, 100);

int main(int argc, char *argv[])
{

    InitWindow(800, 600, "Flower");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        Vector2 center = (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0};

        BeginDrawing();
        ClearBackground(WHITE);

        static Vector2 mover = {-61, -20};
        static float lerpamount = 0;
        static float lerpamount2 = 0;
        static float speed = .5;
        static int amount = 0;

        if(IsKeyPressed(KEY_R)){
            amount=0;
            lerpamount=lerpamount2=0;
        }

        GuiSlider((Rectangle){0, 0, 300, 32}, "", "", &lerpamount, -1, 1);
        GuiSlider((Rectangle){0, 32, 300, 32}, "", "", &lerpamount2, -1, 1);

        if (1 > lerpamount)
        {
            lerpamount += GetFrameTime() * speed;
        }

        if (lerpamount >= 1 && 1 > lerpamount2)
        {
            lerpamount2 += GetFrameTime() * speed;
        }

        if (lerpamount >= 1  &&  lerpamount2 >= 1)
        {
            amount += 1;
            lerpamount = 0;
            lerpamount2 = 0;
        }

        
        Vector2 pos = center;
        pos.y = GetScreenHeight() - 20;
        for (size_t i = 0; i < amount; i++)
        {
            DrawRing(pos, 30, 40, 180, 270, 50, GREEN);
            pos = Vector2Add(pos, mover);
            DrawRing(pos, 30, 40, -90, 0, 50, GREEN);
            Vector2 tmp = (Vector2){-mover.x, mover.y};
            pos = Vector2Add(pos, tmp);
        }

        DrawRing(pos, 30, 40, 180, Lerp(180, 270, lerpamount), 50, GREEN);
        pos = Vector2Add(pos, mover);
        DrawRing(pos, 30, 40, Lerp(0, -90, lerpamount2), 0, 50, GREEN);
        Vector2 tmp = (Vector2){-mover.x, mover.y};
        pos = Vector2Add(pos, tmp);

        EndDrawing();
    }

    return 0;
}