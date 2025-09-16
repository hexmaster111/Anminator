

#ifndef TURTLE_H
#define TURTLE_H
#include "betterlist.h"
#include <raylib.h>

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

typedef struct Line
{
    float thick;
    Vector2 start, end;
    Color color;
} Line;

ListDef(Line);

typedef struct Turtle
{
    Vector2 pos;
    float rotation, line_thick;
    bool pen_down;
    Color pen_color;

    ListOfLine lines;
    ListOfStamp stamps;
} Turtle;

void Turtle_Line(Turtle *t, float length);
void Turtle_Turn(Turtle *t, float angle);
void Turtle_Draw(Turtle *t);
void Turtle_Clear(Turtle *t);
void Turtle_PenDown(Turtle *t, Color c, float size);
void Turtle_PenUp(Turtle *t);
void Turtle_Turn(Turtle *t, float angle);
void Turtle_Goto(Turtle *t, Vector2 pos);
void Turtle_Stamp(Turtle *t, enum Shape shape, Color c, float size);

#endif //TURTLE_H

#ifdef TURTLE_IMPL


ListImpl(Stamp);
ListImpl(Line);




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

    // DrawCircleV(t->pos, 6, WHITE);
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
void Turtle_Turn(Turtle *t, float angle)
{
    t->rotation += angle;
    if (t->rotation > 360)
    {
        t->rotation -= 360;
    }
}

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


#endif // TURTLE_IMPL