// cc demo_textfade.c -odemo_textfade -lraylib -lm

#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "betterlist.h"

#define TEXTFADE_IMPL
#include "textfade.h"

Font Font_SpaceMono;

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(900, 1000, "Glitch");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Font_SpaceMono = LoadFontEx("SpaceMono-Regular.ttf", 48, NULL, 0);
    SetTextureFilter(Font_SpaceMono.texture, TEXTURE_FILTER_ANISOTROPIC_16X);

#define HELLOWORLDTEXT "Demo Text;\nDemo Text\nDemo Text"

    FadeText tf_name = {0};
    tf_name.word.items = HELLOWORLDTEXT;
    tf_name.word.count = sizeof(HELLOWORLDTEXT);
    tf_name.target_color = (Color){255, 255, 255, 255};
    tf_name.work_color = tf_name.target_color;
    tf_name.font = &Font_SpaceMono;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_SPACE))
        {
            FadeText_Update(&tf_name, 1);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(0, 0);

        FadeText_Render(&tf_name, (Vector2){10, 10}, 48, (RenderConfig){.type = FT_DROPSHADOW, .dropshadow = {
                                                                                                   .offset = {2, 2},
                                                                                                   .color = GREEN,
                                                                                               }});

        FadeText_Render(&tf_name, (Vector2){10, 10 + 48 * 3}, 48, (RenderConfig){.type = FT_NORMAL});


        EndDrawing();
    }

    CloseWindow();
}