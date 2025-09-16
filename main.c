#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define TURTLE_IMPL
#include "turtle.h"

#define ARRAYLEN(ARRAY) sizeof((ARRAY)) / sizeof((ARRAY[0]))

typedef struct AnmState
{
    Vector2 center;
    int step;
} AnmState;

void AnmDraw(AnmState s)
{
    const double len = 30.0;
    const int turns = 6;
    const double turn_angle = 360.0 / turns;
    
    static Turtle t = {0};

    // todo: center should be offset by some ammount, shape starts on top left, and moves clockwise
    Turtle_Goto(&t, s.center); 
    

    Turtle_PenDown(&t, BLACK, 3);
    for (size_t i = 0; i < turns; i++)
    {
        Turtle_Line(&t, len);
        Turtle_Turn(&t, turn_angle);
    }

    for (int i = 0; i < t.lines.count; i++)
    {
        // here i should be able to pull each lines start into s.center
        t.lines.items[i].end = Vector2Lerp(t.lines.items[i].end, s.center, 0);
    }


    Turtle_Draw(&t);

    for (int i = 0; i < t.lines.count; i++)
    {
        DrawCircleV(t.lines.items[i].start, 3, PURPLE);
        DrawCircleV(t.lines.items[i].end, 3, GREEN);
    }

    Turtle_Clear(&t);
}

void AnmUpdate(AnmState *s)
{
}

int main()
{
    InitWindow(400, 400, "LAM");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    AnmState s = {
        .center = (Vector2){
            GetScreenWidth() / 2.0,
            GetScreenHeight() / 2.0}};

    while (!WindowShouldClose())
    {
        AnmUpdate(&s);

        BeginDrawing();
        ClearBackground(WHITE);
        AnmDraw(s);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}