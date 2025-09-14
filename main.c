#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "betterlist.h"

typedef struct DropShadow_RenderConfig
{
    enum
    {
        FT_NORMAL,
        FT_DROPSHADOW
    } type;
    union
    {
        struct
        {
            Vector2 offset;
            Color color;
        } ds;
    };
} DropShadow_RenderConfig;

// -TURTLES

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

// - END TURTLES

typedef char Char;

ListDef(Char);
ListImpl(Char);

// -1 on not found
ssize_t Char_ListIndexOfLast(ListOfChar *l, Char find)
{

    ssize_t last = -1;

    for (size_t i = 0; i < l->count; i++)
    {
        if (l->items[i] == find)
            last = i;
    }

    return last;
}

#define BIGGER(NUMA, NUMB) (NUMA) > (NUMB) ? (NUMA) : (NUMB)
#define SMALLER(NUMA, NUMB) (NUMA) > (NUMB) ? (NUMB) : (NUMA)

typedef struct
{
    ListOfChar word;
    int cur_pos;

    Color target_color, work_color;

    Font *font;
} FadeText;

void FadeText_Update(FadeText *ft, int speed)
{
    if (ft->work_color.a > 180)
    {
        ft->work_color.a = 0;
        ft->cur_pos += speed;
    }

    ft->work_color.a += 30;
}

bool FadeText_Done(FadeText *tf) { return tf->cur_pos >= tf->word.count; }

void FadeText_Render(FadeText *tf, Vector2 pos, float fontsize, DropShadow_RenderConfig cfg)
{
    // i guess this is a memory leek ? not really alloc once and then deallocd by the OS
    static ListOfChar tmp_text_buffer;
    tmp_text_buffer.count = 0;

    ssize_t split = SMALLER(tf->word.count, tf->cur_pos);

    for (size_t i = 0; i < split; i++)
    {
        Char_ListPush(&tmp_text_buffer, tf->word.items[i]);
    }

    Char_ListPush(&tmp_text_buffer, 0);

    Font font = tf->font != 0 ? *tf->font : GetFontDefault();

    if (cfg.type == FT_DROPSHADOW)
    {
        Vector2 newpos = Vector2Add(cfg.ds.offset, pos);
        DrawTextEx(font, tmp_text_buffer.items, newpos, fontsize, 1, cfg.ds.color);
    }

    DrawTextEx(font, tmp_text_buffer.items, pos, fontsize, 1, tf->target_color);

    Vector2 already_drawn_size = MeasureTextEx(font, tmp_text_buffer.items, fontsize, 1);

    if (split == 0)
        return;

    if (tf->cur_pos > tf->word.count)
        return;

    ssize_t last_newline_pos = Char_ListIndexOfLast(&tmp_text_buffer, '\n');

    Vector2 on_line_len = Vector2Zero();

    if (last_newline_pos != -1)
    {

        char *from_newline = tmp_text_buffer.items + last_newline_pos;
        on_line_len = MeasureTextEx(font, from_newline, fontsize, 1);
    }
    else
    {
        on_line_len = already_drawn_size;
    }

    tmp_text_buffer.count = 0;

    Char_ListPush(&tmp_text_buffer, tf->word.items[tf->cur_pos]);
    Char_ListPush(&tmp_text_buffer, 0);

    // DrawRectangle(pos.x, pos.y, already_drawn_size.x, already_drawn_size.y, (Color){0xFE, 0xFE, 0xFE, 128});

    pos = Vector2Add(already_drawn_size, pos);
    pos.y -= fontsize;
    pos.x = on_line_len.x;

    if (cfg.type == FT_DROPSHADOW)
    {
        pos = Vector2Add(cfg.ds.offset, pos);
        DrawTextEx(font, tmp_text_buffer.items, pos, fontsize, 1, cfg.ds.color);
    }

    DrawTextEx(font, tmp_text_buffer.items, pos, fontsize, 1, tf->work_color);
}

Font Font_SpaceMono;

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(900, 1000, "TextFader");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Font_SpaceMono = LoadFontEx("SpaceMono-Regular.ttf", 48, NULL, 0);
    SetTextureFilter(Font_SpaceMono.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

#define HELLOWORLDTEXT "This is an example of speeks,\n a text to speach engine designed to make\n hailey keep talking"

    FadeText tf_name = {0};
    tf_name.word.items = HELLOWORLDTEXT;
    tf_name.word.count = sizeof(HELLOWORLDTEXT);
    tf_name.target_color = (Color){255, 255, 255, 255};
    tf_name.work_color = tf_name.target_color;
    tf_name.font = &Font_SpaceMono;

    Turtle t = {0};

    float TURN_FACTOR = 0;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_SPACE))
        {
            FadeText_Update(&tf_name, 1);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(0, 0);

        const Vector2 turtle_center = {GetScreenWidth() / 2.0, GetScreenHeight() / 2.0};

        Turtle_Clear(&t);
        Turtle_PenDown(&t, WHITE, 2);

        for (size_t w = 0; w < 3; w++)
        {
            Turtle_Turn(&t, 360 / 3);

            for (size_t i = 0; i < 2; i++)
            {
                Turtle_Goto(&t, turtle_center);
                float last_angle = t.rotation;

                // for (size_t x = 0; x < 7; x++)
                // {
                //     Turtle_Line(&t, 30);
                //     Turtle_Turn(&t, 30);
                // }

                for (size_t x = 7; x > 0; x--)
                {
                    Turtle_Line(&t, 30);
                    Turtle_Turn(&t, 30 - (x * TURN_FACTOR));
                }

                t.rotation = last_angle;
                Turtle_Turn(&t, 180);
            }
        }

        t.rotation -= 0.1;

        Turtle_Draw(&t);
        // FadeText_Render(&tf_name, (Vector2){10, 10}, 48, (DropShadow_RenderConfig){.type = FT_DROPSHADOW, .ds = {
        //                                                                                                       .offset = {2, 2},
        //                                                                                                       .color = RED,
        //                                                                                                   }});

        GuiSlider((Rectangle){100, 10, 200, 10}, "", "", &TURN_FACTOR, -3, 3);
        DrawText(TextFormat("%f"), 10,30,12,GREEN);

        EndDrawing();
    }

    return 0;
}
