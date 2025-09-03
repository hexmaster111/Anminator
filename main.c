#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

enum Shape
{
    S_Circle
};

typedef struct Stamp
{
    enum Shape shape;
    float size;
    Vector2 pos;
    Color c;
} Stamp;

ListDef(Stamp);
ListImpl(Stamp);

typedef struct Line
{
    float thick;
    Vector2 start, end;
    Color color;
} Line;

ListDef(Line);
ListImpl(Line);

typedef struct Turtle
{
    Vector2 pos;
    float rotation, line_thick;
    bool pen_down;
    Color pen_color;

    ListOfLine lines;
    ListOfStamp stamps;
} Turtle;

void Turtle_Draw(Turtle *t)
{
    for (size_t i = 0; i < t->lines.count; i++)
    {
        Line *l = Line_ListAt(&t->lines, i);
        DrawLineEx(l->start, l->end, l->thick, l->color);
    }

    for (size_t i = 0; i < t->stamps.count; i++)
    {
        Stamp *l = Stamp_ListAt(&t->stamps, i);

        switch (l->shape)
        {
        case S_Circle:
            DrawCircleLinesV(l->pos, l->size, l->c);

            break;
        }
    }

    DrawCircleV(t->pos, 6, WHITE);
}

void Turtle_Clear(Turtle *t)
{
    t->lines.count = 0;
    t->stamps.count = 0;
}

void Turtle_PenDown(Turtle *t, Color c, float size)
{
    t->pen_down = true;
    t->pen_color = c;
    t->line_thick = size;
}

void Turtle_PenUp(Turtle *t) { t->pen_down = false; }

void Turtle_Goto(Turtle *t, Vector2 pos) { t->pos = pos; }
void Turtle_Turn(Turtle *t, float angle) { t->rotation += angle; }

void Turtle_Line(Turtle *t, float length)
{

    Vector2 start = t->pos;

    t->pos.x = t->pos.x + (length * cos(t->rotation * DEG2RAD));
    t->pos.y = t->pos.y + (length * sin(t->rotation * DEG2RAD));

    if (t->pen_down)
    {
        Line_ListPush(&t->lines, (Line){.color = t->pen_color, .start = start, .end = t->pos, .thick = t->line_thick});
    }
}

void Turtle_Stamp(Turtle *t, enum Shape shape, Color c, float size)
{
    Stamp_ListPush(&t->stamps, (Stamp){.pos = t->pos, .shape = shape, .size = size, .c = c});
}

int main(int argc, char *argv[])
{
    SetConfigFlags(TEXTURE_FILTER_ANISOTROPIC_16X);
    InitWindow(800, 600, "Turtles");
    SetTargetFPS(15);

    Turtle t = {0};

    float start_angle = 0;

    size_t arms = 12;

    while (!WindowShouldClose())
    {

        Rectangle up = {20, 10, 30, 30};
        Rectangle down = {20 + 30 + 5, 10, 30, 30};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), up))
        {
            arms += 1;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), down))
        {
            arms -= 1;
        }

        Turtle_PenDown(&t, WHITE, 1);
        t.rotation = start_angle;

        start_angle -= 0.5;

        for (size_t x = 0; x < arms; x++)
        {
            Turtle_Goto(&t, (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0});

            Turtle_Turn(&t, 360.0 / arms);

            for (size_t i = 0; i < 6; i++)
            {
                Turtle_Turn(&t, 30.0);
                Turtle_Line(&t, 20.0 + (i * 5.0));
                Turtle_Stamp(&t, S_Circle, WHITE, 5);
            }
        }

        Turtle_Goto(&t, (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0});

        BeginDrawing();
        ClearBackground(BLACK);
        Turtle_Draw(&t);

        DrawText(TextFormat("%.2f, %d", t.rotation, arms), 0, 40, 12, GREEN);

        // DrawRectangleRec(up, YELLOW);
        // DrawRectangleRec(down, GREEN);
        EndDrawing();

        Turtle_Clear(&t);
    }

    return 0;
}