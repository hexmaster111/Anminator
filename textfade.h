#ifndef TEXTFADE_H
#define TEXTFADE_H

#include <raylib.h>
#include "betterlist.h"

#define BIGGER(NUMA, NUMB) (NUMA) > (NUMB) ? (NUMA) : (NUMB)
#define SMALLER(NUMA, NUMB) (NUMA) > (NUMB) ? (NUMB) : (NUMA)

typedef char Char;

ListDef(Char);
ListImpl(Char);

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


typedef struct
{
    ListOfChar word;
    int cur_pos;

    Color target_color, work_color;

    Font *font;
} FadeText;




// -1 on not found
ssize_t Char_ListIndexOfLast(ListOfChar *l, Char find);


void FadeText_Update(FadeText *ft, int speed);
bool FadeText_Done(FadeText *tf);
void FadeText_Render(FadeText *tf, Vector2 pos, float fontsize, DropShadow_RenderConfig cfg);



#endif // TEXTFADE_H

#ifdef TEXTFADE_IMPL
#undef TEXTFADE_IMPL


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


#endif // TEXTFADE_IMPL