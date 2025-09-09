#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

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

void FadeText_Render(FadeText *tf, Vector2 pos, float fontsize)
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

    DrawTextEx(font, tmp_text_buffer.items, pos, fontsize, 1, tf->work_color);
}

Font Font_SpaceMono;

#include "bio.c"

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(900, 1000, "TextFader");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Font_SpaceMono = LoadFontEx("SpaceMono-Regular.ttf", 48, NULL, 0);
    SetTextureFilter(Font_SpaceMono.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

#define TF_LINE(VARNAME, CATAGORY, TEXTLINE)            \
    FadeText VARNAME = {0};                             \
    VARNAME.word.items = CATAGORY TEXTLINE;             \
    VARNAME.word.count = sizeof(CATAGORY TEXTLINE);     \
    VARNAME.target_color = (Color){255, 255, 255, 255}; \
    VARNAME.work_color = VARNAME.target_color;          \
    VARNAME.font = &Font_SpaceMono;

    TF_LINE(tf_name, "NAME:      ", NAME);
    TF_LINE(tf_age, "AGE:       ", AGE);
    TF_LINE(tf_gender, "GENDER:    ", GENDER);
    TF_LINE(tf_PRONOUNS, "PRONOUNS:  ", PRONOUNS);
    TF_LINE(tf_LOCATION, "LOCATION:  ", LOCATION);
    TF_LINE(tf_DM_STATUS, "DM_STATUS: ", DM_STATUS);
    TF_LINE(tf_ROLE, "ROLE:      ", ROLE);
    TF_LINE(tf_INFO, "INFO: ", INFO);
    TF_LINE(tf_HOBBIES, "HOBBIES:   ", HOBBIES);
    TF_LINE(tf_KINKS, "KINKS:   ", KINKS);
    TF_LINE(tf_KINKS2, "", KINKS2);
    TF_LINE(tf_UPDATED, "page last updated:   ", UPDATED);

    tf_KINKS.work_color = PURPLE;
    tf_KINKS.target_color = PURPLE;

    tf_KINKS2.work_color = PURPLE;
    tf_KINKS2.target_color = PURPLE;

    Turtle t = {0};

    float start_angle = 0;

    size_t arms = 12;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_SPACE))
        {

            FadeText_Update(&tf_name, 1);

            if (FadeText_Done(&tf_name))
            {
                FadeText_Update(&tf_age, 1);
            }

            if (FadeText_Done(&tf_name))

                if (FadeText_Done(&tf_age))
                {
                    FadeText_Update(&tf_gender, 1);
                }

            if (FadeText_Done(&tf_name))
                FadeText_Update(&tf_PRONOUNS, 1);

            if (FadeText_Done(&tf_PRONOUNS))
            {
                FadeText_Update(&tf_LOCATION, 1);
            }

            if (FadeText_Done(&tf_name))
                FadeText_Update(&tf_DM_STATUS, 1);

            if (FadeText_Done(&tf_name))
                FadeText_Update(&tf_ROLE, 1);

            if (FadeText_Done(&tf_name))
                FadeText_Update(&tf_INFO, 5);

            if (FadeText_Done(&tf_name))
            {
                FadeText_Update(&tf_HOBBIES, 3);
            }

            if (FadeText_Done(&tf_name))
                if (FadeText_Done(&tf_INFO))
                {
                    FadeText_Update(&tf_UPDATED, 1);
                }

            if (FadeText_Done(&tf_name))
            {
                FadeText_Update(&tf_KINKS, 3);
            }

            if (FadeText_Done(&tf_name))
            {
                FadeText_Update(&tf_KINKS2, 3);
            }
        }

        Turtle_PenDown(&t, RAYWHITE, 1);
        t.rotation = start_angle;

        start_angle -= 0.2;

        for (size_t x = 0; x < arms; x++)
        {
            Turtle_Goto(&t, (Vector2){GetScreenWidth() - 100, 100});

            Turtle_Turn(&t, 360.0 / arms);

            for (size_t i = 0; i < 6; i++)
            {
                Turtle_Turn(&t, 30.0);
                Turtle_Line(&t, 20.0 + ((sin(start_angle * DEG2RAD) + .1) * 50) + (i * 5));
                Turtle_Stamp(&t, S_Circle, WHITE, 5);
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        int y_iota = 10;
#define FONT_SIZE (48)
        FadeText_Render(&tf_name, (Vector2){10, y_iota}, 48);
        FadeText_Render(&tf_age, (Vector2){10, y_iota += FONT_SIZE}, 48);
        FadeText_Render(&tf_gender, (Vector2){10, y_iota += FONT_SIZE}, 48);
        FadeText_Render(&tf_PRONOUNS, (Vector2){10, y_iota += FONT_SIZE}, 48);
        FadeText_Render(&tf_LOCATION, (Vector2){10, y_iota += FONT_SIZE}, 48);
        FadeText_Render(&tf_DM_STATUS, (Vector2){10, y_iota += FONT_SIZE}, 48);
        FadeText_Render(&tf_ROLE, (Vector2){10, y_iota += FONT_SIZE}, 48);

        FadeText_Render(&tf_INFO, (Vector2){10, y_iota += 38}, 30);

        y_iota += 38 * 3;

#define LIST_FONTSIZE (38)
        FadeText_Render(&tf_HOBBIES, (Vector2){10, y_iota += LIST_FONTSIZE}, LIST_FONTSIZE);
        FadeText_Render(&tf_KINKS, (Vector2){200, y_iota += LIST_FONTSIZE}, LIST_FONTSIZE);
        FadeText_Render(&tf_KINKS2, (Vector2){500, y_iota += LIST_FONTSIZE}, LIST_FONTSIZE);

        y_iota += LIST_FONTSIZE * 6.5;

        FadeText_Render(&tf_UPDATED, (Vector2){10, y_iota += LIST_FONTSIZE}, 28);

        Turtle_Draw(&t);

        EndDrawing();

        Turtle_Clear(&t);
    }

    return 0;
}