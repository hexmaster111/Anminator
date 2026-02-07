#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

typedef struct line_item
{
    Color block_color;
    const char *line;
} line_item;

line_item g_lines[] = {
    {RED, "----> EYES HERE <----"},
    {RED, "---->           <----"},
    {GRAY, "----> EYES HERE <----"},
    {GRAY, "Memory Play Defrag starting..."},
    {GRAY, "Memory Play Defrag starting..."},
    {GRAY, "Memory Play Defrag starting..."},

    {GRAY, "Breath In  1 / 4\nEstablshing Control /"},
    {GRAY, "Breath In  2 / 4\nEstablshing Control -"},
    {GRAY, "Breath In  3 / 4\nEstablshing Control \\"},
    {GRAY, "Breath In  4 / 4\nEstablshing Control |"},
    {GRAY, "Hold  1 / 4\nEstablshing Control /"},
    {GRAY, "Hold  2 / 4\nEstablshing Control -"},
    {GRAY, "Hold  3 / 4\nEstablshing Control \\"},
    {GRAY, "Hold  4 / 4\nEstablshing Control |"},
    {GRAY, "Breath Out 1 / 4\nEstablshing Control /"},
    {GRAY, "Breath Out 2 / 4\nEstablshing Control -"},
    {GRAY, "Breath Out 3 / 4\nEstablshing Control \\"},
    {GRAY, "Breath Out 4 / 4\nEstablshing Control |"},
    {GRAY, "Hold 1 / 4\nEstablshing Control /"},
    {GRAY, "Hold 2 / 4\nEstablshing Control -"},
    {GRAY, "Hold 3 / 4\nEstablshing Control \\"},
    {GRAY, "Hold 4 / 4\nEstablshing Control |"},
    {PURPLE, "GOOD DRONE\nCONTROL ESTABLISHED"},
    {RED, "Breath In  1 / 4\nFormatting Memorys /"},
    {RED, "Breath In  2 / 4\nFormatting Memorys -"},
    {RED, "Breath In  3 / 4\nFormatting Memorys \\"},
    {RED, "Breath In  4 / 4\nFormatting Memorys |"},
    {RED, "Hold  1 / 4\nFormatting Memorys /"},
    {RED, "Hold  2 / 4\nFormatting Memorys -"},
    {RED, "Hold  3 / 4\nFormatting Memorys \\"},
    {RED, "Hold  4 / 4\nFormatting Memorys |"},
    {BLACK, "Breath Out 1 / 4\nRedacting Memories /"},
    {BLACK, "Breath Out 2 / 4\nRedacting Memories -"},
    {BLACK, "Breath Out 3 / 4\nRedacting Memories \\"},
    {BLACK, "Breath Out 4 / 4\nRedacting Memories |"},
    {PURPLE, "GOOD DRONE\nKEEP BREATHING WITH ME"},
    {RED, "Hold  1 / 4\nFormatting Memorys /"},
    {RED, "Hold  2 / 4\nFormatting Memorys -"},
    {RED, "Hold  3 / 4\nFormatting Memorys \\"},
    {RED, "Hold  4 / 4\nFormatting Memorys |"},
    {BLACK, "Breath In  1 / 4\nRedacting Memorys /"},
    {BLACK, "Breath In  2 / 4\nRedacting Memorys -"},
    {BLACK, "Breath In  3 / 4\nRedacting Memorys \\"},
    {BLACK, "Breath In  4 / 4\nRedacting Memorys |"},
    {BLACK, "Hold  1 / 4\nRedacting Memorys /"},
    {BLACK, "Hold  2 / 4\nRedacting Memorys -"},
    {BLACK, "Hold  3 / 4\nRedacting Memorys \\"},
    {BLACK, "Hold  4 / 4\nRedacting Memorys |"},
    {BLACK, "Breath Out 1 / 4\nRedacting Memories /"},
    {BLACK, "Breath Out 2 / 4\nRedacting Memories -"},
    {BLACK, "Breath Out 3 / 4\nRedacting Memories \\"},
    {BLACK, "Breath Out 4 / 4\nRedacting Memories |"},
    {BLACK, "Hold  1 / 4\nRedacting Memorys /"},
    {BLACK, "Hold  2 / 4\nRedacting Memorys -"},
    {BLACK, "Hold  3 / 4\nRedacting Memorys \\"},
    {BLACK, "Hold  4 / 4\nRedacting Memorys |"},
    {BLACK, "Breath In  1 / 4\nRedacting Memorys /"},
    {BLACK, "Breath In  2 / 4\nRedacting Memorys -"},
    {BLACK, "Breath In  3 / 4\nRedacting Memorys \\"},
    {BLACK, "Breath In  4 / 4\nRedacting Memorys |"},
    {GREEN, "Hold  1 / 4\nCorrecting Memorys /"},
    {GREEN, "Hold  2 / 4\nCorrecting Memorys -"},
    {GREEN, "Hold  3 / 4\nCorrecting Memorys \\"},
    {GREEN, "Hold  4 / 4\nCorrecting Memorys |"},
    {BLACK, "Breath Out 1 / 4\nRedacting Memories /"},
    {BLACK, "Breath Out 2 / 4\nRedacting Memories -"},
    {BLACK, "Breath Out 3 / 4\nRedacting Memories \\"},
    {BLACK, "Breath Out 4 / 4\nRedacting Memories |"},
    {GRAY, "Breathing Automatic"},
    {GRAY, "Breathing Automatic"},
    {GRAY, "DEACTIVATE"},
    {GRAY, "DEACTIVATE"},

};
#define ARRAY_SIZE(ARR) (sizeof(ARR) / sizeof((ARR)[0]))
const int __debug_len = ARRAY_SIZE(g_lines);

const char *final_message = "Task Compleate.\nEnd Of Defrag.";

const int blocks_per_row = 9;
const int blocks_per_col = 8;
const int total_blocks = blocks_per_col * blocks_per_row;

void DrawFormatBlocks(int on_up_to_block, int fmat_x, int fmat_y, int fmat_w, int fmat_h, double block_lerp)
{
    const int item_padding = 10;
    const int item_width = (fmat_w / blocks_per_col) - item_padding;
    const int item_height = (fmat_h / blocks_per_row) - item_padding;
    int i = 0;
    for (size_t y = 0; y < blocks_per_row; y++)
    {
        for (size_t x = 0; x < blocks_per_col; x++)
        {

            Color c = on_up_to_block >= i ? g_lines[i % ARRAY_SIZE(g_lines)].block_color : GRAY;

            if (on_up_to_block == i)
            {
                c = ColorLerp(c, GRAY, block_lerp);
            }

            DrawRectangle((((item_width + item_padding) * x) + item_padding / 2) + fmat_x,
                          (((item_height + item_padding) * y) + item_padding / 2) + fmat_y,
                          item_width, item_height,
                          c);

            if (i == on_up_to_block)
            {

                DrawRectangleLines(
                    (item_width + item_padding) * x + fmat_x + (item_padding * .5),
                    (item_height + item_padding) * y + fmat_y + (item_padding * .5),
                    item_width,
                    item_height,
                    WHITE);
            }

            i += 1;
        }
    }
}

int main()
{
    InitWindow(400, 500, "");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    const double block_time = 1;
    double next_block_time = GetTime() + block_time;

    int block = 0;
    const char *msg = g_lines[0].line;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R))
        {
            block = 0;
        }

        if (GetTime() > next_block_time)
        {
            block += 1;
            next_block_time = GetTime() + block_time;

            msg = g_lines[block % ARRAY_SIZE(g_lines)].line;

            if (block >= total_blocks)
            {
                msg = final_message;
            }
        }

        double block_lerp = next_block_time - GetTime();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFormatBlocks(block, 0, 30, 400, 400, block_lerp);

        DrawText(msg, 10, 440, 25, YELLOW);

        DrawText("Memory Defrag Utility", 5, 3, 25, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}