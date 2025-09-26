// filename "reader.c"

#include <raylib.h>
#include <raymath.h>

#include "betterlist.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TURTLE_IMPL
#include "turtle.h"

#define TEXTFADE_IMPL
#include "textfade.h"

/*
// words per sec or something -- sets the speed of the text fadding in
<speed(2)>
This is an example of <bold speeks> a text to speach engine <newline>
designed to make <large hailey> keep talking <newline>
// wait 5 secs before going onto the next part of the file
<delay(5)>
<clear>
<move(left_to_right, SLOW)  Keep your eyes here>
[delay(3)]
[clear]
[center_large_text  FEEL<<PLEASURE>>]
<delay(.7)>
<clear>
<togeather(timeoffset: .75)>
Each line in this block
is written to the screen
at the same time
<end togeather>
*/

Font Font_SpaceMono;
FadeText g_faders[1000] = {0};


void DoCommand(ListOfChar cmd)
{
    
}

void BuildFaders(ListOfChar fc)
{
    size_t currsor = 0;
    int fadder_idx = 0;

    ListOfChar buffer = {0};

    while (fc.count >= currsor)
    {

        if (fc.items[currsor] == '[' && fc.items[currsor - 1] != '\\')
        {

            if (buffer.count > 0)
            {
                g_faders[fadder_idx] = (FadeText){
                    .word = buffer,
                    .pos = Vector2Zero(),
                    .work_color = WHITE,
                    .target_color = WHITE,
                    .font = &Font_SpaceMono};

                fadder_idx += 1;

                buffer.cap = 0;
                buffer.items = NULL;
                buffer.count = NULL;
            }
        }

        if (fc.items[currsor] == ']' && fc.items[currsor - 1] != '\\')
        {
            if (buffer.count > 0)
            {

                DoCommand(buffer);

                fadder_idx += 1;

                buffer.cap = 0;
                buffer.items = NULL;
                buffer.count = NULL;
            }
        }

        Char_ListPush(&buffer, fc.items[currsor]);

        currsor += 1;
    }
}

int main(int argc, char *argv[])
{

    ListOfChar filedata = {0};

    if (argc < 2)
    {
        printf("usage %s filename.hyp\n", argv[0]);
        return 1;
    }

    if (!FileExists(argv[1]))
    {
        printf("file dosent exist! \'%s\'\n", argv[1]);
        return 1;
    }

    int data_size;
    filedata.items = LoadFileData(argv[1], &data_size);
    filedata.count = data_size;
    filedata.cap = -1;

    BuildFaders(filedata);

    InitWindow(600, 600, "Reader");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Font_SpaceMono = LoadFontEx("SpaceMono-Regular.ttf", 48, NULL, 0);
    SetTextureFilter(Font_SpaceMono.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        EndDrawing();
    }

    return 0;
}