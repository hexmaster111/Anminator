// filename "reader.c"

#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

/*
// words per sec or something -- sets the speed of the text fadding in
<speed(2)> 
This is an example of <bold speeks> a text to speach engine <newline> 
designed to make <large hailey> keep talking <newline>
// wait 5 secs before going onto the next part of the file
<delay(5)> 
<clear>
<move(left_to_right, SLOW)  Keep your eyes here>
<delay(3)>
<clear>
<center_large_text  GOOD> 
<delay(.7)>
<clear>
<togeather(timeoffset: .75)>
Each line in this block
is written to the screen
at the same time
<end togeather>
*/

float g_word_speed;

struct TextSegment {
    char *start; 
    int section_len;

    int text_height;

    enum {
        TA_Bold,
    } attributes;
};


int main()
{

    InitWindow(600, 600, "Reader");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        EndDrawing();
    }

    return 0;
}