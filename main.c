#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

#define BG (BLACK)
#define FG (GRAY)

typedef struct AnmState
{
    Vector2 center;
    int step;

    float lerp_stage_1;
    float lerp_stage_2;
    float lerp_stage_3;
} AnmState;

void AnmDraw(AnmState s)
{
    double len = Lerp(30, 20, s.lerp_stage_2);

    const int turns = 6;
    const double turn_angle = 360.0 / turns;

    static Turtle t = {0};

    // Compute offset to center the shape at s.center
    Vector2 offset = Vector2Subtract(s.center, (Vector2){len * 0.5f, len});
    Turtle_Goto(&t, offset);

    Turtle_PenDown(&t, FG, 3);
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
            .color = FG,
            .thick = t.lines.items[i].thick};

        Line_ListPush(&t.lines, l);
    }

    Turtle_Draw(&t);

    for (int i = 0; i < t.lines.count; i++)
    {
        DrawCircleV(t.lines.items[i].start, 3, PURPLE);
        DrawCircleV(t.lines.items[i].end, 3, GREEN);
    }

    static Turtle t2 = {0};

    Turtle_Clear(&t2);
    Turtle_PenDown(&t2, GOLD, 2);

    for (size_t i = 0; i < turns; i++)
    {
        Line l = t.lines.items[i];
        Turtle_Goto(&t2, l.start);
        t2.rotation = RAD2DEG * (-Vector2LineAngle(l.end, l.start));


        Turtle_Line(&t2, 60);


        // the start of these lines will be the beginning of each of the spirles
    }

    Turtle_Draw(&t2);
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
        ClearBackground(BG);
        AnmDraw(s);

        GuiSlider((Rectangle){0, 0, 100, 16}, "", "", &s.lerp_stage_1, 0, 1);
        GuiSlider((Rectangle){0, 16, 100, 16}, "", "", &s.lerp_stage_2, 0, 1);
        GuiSlider((Rectangle){0, 32, 100, 16}, "", "", &s.lerp_stage_3, 0, 1);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}