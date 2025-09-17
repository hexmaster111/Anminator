#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "betterlist.h"

#define TURTLE_IMPL
#include "turtle.h"

#define TEXTFADE_IMPL
#include "textfade.h"


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

    Turtle spirle = {0};

    Turtle tree = {0};

    float arm_factor = 0;

    const Vector2 turtle_center = {GetScreenWidth() / 2.0, GetScreenHeight() / 2.0};

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_SPACE))
        {
            FadeText_Update(&tf_name, 1);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(0, 0);

        Turtle_Clear(&spirle);
        Turtle_PenDown(&spirle, WHITE, 2);
        for (size_t w = 0; w < 3; w++)
        {
            Turtle_Turn(&spirle, 360 / 3);
            for (size_t i = 0; i < 2; i++)
            {
                Turtle_Goto(&spirle, turtle_center);
                float last_angle = spirle.rotation;
                for (size_t x = 7; x > 0; x--)
                {
                    Turtle_Line(&spirle, 30);
                    Turtle_Turn(&spirle, 30 - (x * arm_factor));
                }
                spirle.rotation = last_angle;
                Turtle_Turn(&spirle, 180);
            }
        }
        Turtle_Turn(&spirle, -0.1);
        Turtle_Draw(&spirle);


        // FadeText_Render(&tf_name, (Vector2){10, 10}, 48, (DropShadow_RenderConfig){.type = FT_DROPSHADOW, .ds = {
        //                                                                                                       .offset = {2, 2},
        //                                                                                                       .color = RED,
      
        EndDrawing();

    }

    return 0;
}
