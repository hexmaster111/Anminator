#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

#define ARRAYLEN(ARRAY) sizeof((ARRAY)) / sizeof((ARRAY[0]))

typedef struct AnmState
{
    Vector2 center;
    int step;

    float lerp_stage_1;
    float lerp_stage_2;
} AnmState;

void AnmDraw(AnmState s)
{
    const double len = 120.0;
    const int turns = 6;
    const double turn_angle = 360.0 / turns;

    static Turtle t = {0};

    // Compute offset to center the shape at s.center
    Vector2 offset = Vector2Subtract(s.center, (Vector2){len * 0.5f, len});
    Turtle_Goto(&t, offset);

    Turtle_PenDown(&t, BLACK, 3);
    for (size_t i = 0; i < turns; i++)
    {
        Turtle_Line(&t, len);
        Turtle_Turn(&t, turn_angle);
    }

    // Compute the center of the shape using the average of line endpoints
    Vector2 shape_center = {0};
    for (int i = 0; i < t.lines.count; i++)
    {
        shape_center = Vector2Add(shape_center, t.lines.items[i].start);
    }
    shape_center = Vector2Scale(shape_center, 1.0f / t.lines.count);

    for (int i = 0; i < t.lines.count; i++)
    {
        t.lines.items[i].end = Vector2Lerp(t.lines.items[i].end, shape_center, s.lerp_stage_1);
    }

    int torun = t.lines.count;

    for (int i = 0; i < torun; i++)
    {
        Vector2 end;
        if (i >= torun - 1)
        {
            end = t.lines.items[0].end;
        }
        else
        {
            end = t.lines.items[i + 1].end;
        }

        Line l = {
            .start = t.lines.items[i].end,
            .end = end,
            .color = BLACK,
            .thick = t.lines.items[i].thick};

        Line_ListPush(&t.lines, l);
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

        GuiSlider((Rectangle){0, 0, 100, 16}, "", "", &s.lerp_stage_1, 0, 1);
        GuiSlider((Rectangle){0, 16, 100, 16}, "", "", &s.lerp_stage_2, 0, 1);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}