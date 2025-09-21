// cc demo_multistage.c -odemo_multistage -lraylib -lm


#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

#define BG (BLACK)
#define FG (WHITE)

typedef struct AnmState
{
    /* Where to render the anmiation */
    Vector2 center;

    /* Controls for the animation, updated by AnmUpdate */
    float lerp_stage_0;
    float lerp_stage_1;
    float lerp_stage_2;
    float lerp_stage_3;
    float lerp_stage_4;
    float rotation;

    /* Controlled by renderer */
    Turtle t1, t2;
} AnmState;

void AnmDraw(AnmState *s)
{
    float len = Lerp(60, .1, s->lerp_stage_2);

    const int turns = 6;
    const float turn_angle = 360.0 / turns;
    const int pen_size = 3;

    // Compute offset to center the shape at s.center
    Vector2 offset = Vector2Subtract(s->center, (Vector2){len * 0.5f, len});
    Turtle_Goto(&s->t1, offset);
    s->t1.rotation = s->rotation;

    // DrawCircleV(offset, 3, RED);

    Turtle_PenDown(&s->t1, FG, pen_size);
    for (size_t i = 0; i < turns; i++)
    {
        Turtle_Line(&s->t1, len);
        Turtle_Turn(&s->t1, turn_angle);
    }

    // Compute the center of the shape using the average of line endpoints
    Vector2 shape_center = {0};
    for (int i = 0; i < s->t1.lines.count; i++)
    {
        shape_center = Vector2Add(shape_center, s->t1.lines.items[i].start);
    }
    shape_center = Vector2Scale(shape_center, 1.0f / s->t1.lines.count);

    for (int i = 0; i < s->t1.lines.count; i++)
    {
        s->t1.lines.items[i].end = Vector2Lerp(s->t1.lines.items[i].end, shape_center, s->lerp_stage_1);
    }

    int torun = s->t1.lines.count;

    for (int i = 0; i < torun; i++)
    {
        Vector2 end;
        if (i >= torun - 1)
        {
            end = s->t1.lines.items[0].end;
        }
        else
        {
            end = s->t1.lines.items[i + 1].end;
        }

        Line l = {
            .start = s->t1.lines.items[i].end,
            .end = end,
            .color = FG,
            .thick = s->t1.lines.items[i].thick};

        Line_ListPush(&s->t1.lines, l);
    }

    Turtle_Draw(&s->t1);

    // for (int i = 0; i < t.lines.count; i++)
    // {
    //     DrawCircleV(t.lines.items[i].start, 3, PURPLE);
    //     DrawCircleV(t.lines.items[i].end, 3, GREEN);
    // }

    Turtle_Clear(&s->t2);
    Turtle_PenDown(&s->t2, FG, pen_size);

    for (size_t i = 0; i < turns; i++)
    {
        Line l = s->t1.lines.items[i];
        Turtle_Goto(&s->t2, l.start);

        s->t2.rotation = RAD2DEG * (-Vector2LineAngle(l.end, l.start));

        const float end_line_len = 200.0;
        const int segments = 200;
        const float line_size_per_segment = end_line_len / segments;
        const float end_rotation = 120.0;
        const float turn_per_segment = end_rotation / segments;

        for (size_t x = 0; x < segments; x++)
        {
            float turndegs = -Lerp(0, turn_per_segment, s->lerp_stage_4);
            Turtle_Turn(&s->t2, turndegs);

            Turtle_Line(&s->t2, Lerp(0, line_size_per_segment, s->lerp_stage_3));
        }
    }

    Turtle_Draw(&s->t2);
    Turtle_Clear(&s->t1);
}

bool IsStageDone(float f)
{
    if (f >= 1.0f)
        return true;

    return false;
}

void AnmUpdate(AnmState *s)
{
    const float speed = 0.005f;

    if (!IsStageDone(s->lerp_stage_0))
    {
        s->lerp_stage_0 += speed;
        return;
    }

    if (!IsStageDone(s->lerp_stage_1))
    {
        s->lerp_stage_1 += speed;
    }

    if (!IsStageDone(s->lerp_stage_2))
    {
        s->lerp_stage_2 += speed * 2;
    }

    if (!IsStageDone(s->lerp_stage_3))
    {
        s->lerp_stage_3 += speed * 2;
    }

    if (!IsStageDone(s->lerp_stage_4))
    {
        s->lerp_stage_4 += speed;
    }

    if (
        // IsStageDone(s->lerp_stage_1) &&
        IsStageDone(s->lerp_stage_2)
        // IsStageDone(s->lerp_stage_3) &&
        // IsStageDone(s->lerp_stage_4)
    )
    {
        s->rotation = Wrap(s->rotation += 0.5, 0, 360);
    }
}

int main()
{
    InitWindow(600, 600, "LAM");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    AnmState s = {
        .center = (Vector2){
            GetScreenWidth() / 2.0,
            GetScreenHeight() / 2.0}};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R))
        {
            s.lerp_stage_0 = 0;
            s.lerp_stage_1 = 0;
            s.lerp_stage_2 = 0;
            s.lerp_stage_3 = 0;
            s.lerp_stage_4 = 0;
            s.rotation = 0;
        }

        AnmUpdate(&s);

        BeginDrawing();
        ClearBackground(BG);
        AnmDraw(&s);

        int iota = 0;
        GuiSlider((Rectangle){0, iota, 100, 16}, "", "", &s.lerp_stage_1, 0, 1);
        iota += 16;
        GuiSlider((Rectangle){0, iota, 100, 16}, "", "", &s.lerp_stage_2, 0, 1);
        iota += 16;
        GuiSlider((Rectangle){0, iota, 100, 16}, "", "", &s.lerp_stage_3, 0, 1);
        iota += 16;
        GuiSlider((Rectangle){0, iota, 100, 16}, "", "", &s.lerp_stage_4, 0, 1);
        iota += 16;
        GuiSlider((Rectangle){0, iota, 100, 16}, "", "", &s.rotation, 0, 360);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}