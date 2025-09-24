#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

int main()
{

    InitWindow(400, 400, "");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Turtle t = {0};

    Turtle_Goto(&t, (Vector2){GetScreenHeight() / 2, GetScreenHeight() / 2});

    Color c[] = {
        RED,
        GREEN,
        BLUE,
        GOLD};

    float turn[] = {
        -((360.0 / 6.0) * 2),
        -((360.0 / 6.0) * 2),
        -((360.0 / 6.0) * 2),
    };

    for (size_t i = 0; i < 3; i++)
    {
        Turtle_PenDown(&t, c[i], 1);

        for (size_t tn = 0; tn < 6; tn++)
        {
            Turtle_Line(&t, 20);
            Turtle_Turn(&t, 360.0 / 6.0);
        }
        
        Turtle_Turn(&t, turn[i]);
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        Turtle_Draw(&t);

        EndDrawing();
    }

    return 0;
}
