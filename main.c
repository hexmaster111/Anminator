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

static float branch_angle = 30;
static int tree_levels = 2;

void Turtle_Tree(Turtle *t, int size, int level)
{

    if (level <= 0)
        return;

    Turtle_Line(t, size);
    Turtle_Turn(t, branch_angle);
    Turtle_Tree(t, 0.8 * size, level - 1);
    Turtle_Turn(t, -(branch_angle * 2));
    Turtle_Tree(t, 0.8 * size, level - 1);
    Turtle_Turn(t, branch_angle);
    Turtle_Line(t, -size);
}

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

        // Turtle_Clear(&spirle);
        // Turtle_PenDown(&spirle, WHITE, 2);
        // for (size_t w = 0; w < 3; w++)
        // {
        //     Turtle_Turn(&spirle, 360 / 3);
        //     for (size_t i = 0; i < 2; i++)
        //     {
        //         Turtle_Goto(&spirle, turtle_center);
        //         float last_angle = spirle.rotation;
        //         for (size_t x = 7; x > 0; x--)
        //         {
        //             Turtle_Line(&spirle, 30);
        //             Turtle_Turn(&spirle, 30 - (x * arm_factor));
        //         }
        //         spirle.rotation = last_angle;
        //         Turtle_Turn(&spirle, 180);
        //     }
        // }
        // Turtle_Turn(&spirle, -0.1);
        // Turtle_Draw(&spirle);

        Turtle_Clear(&tree);
        Turtle_PenDown(&tree, WHITE, 1);
        Turtle_Goto(&tree, turtle_center);
        tree.rotation = -90;

        

        Turtle_Tree(&tree, 100, tree_levels);
        Turtle_Draw(&tree);

        // FadeText_Render(&tf_name, (Vector2){10, 10}, 48, (DropShadow_RenderConfig){.type = FT_DROPSHADOW, .ds = {
        //                                                                                                       .offset = {2, 2},
        //                                                                                                       .color = RED,
        //                                                                                                   }});

        GuiSlider((Rectangle){100, 10, 200, 10}, "", "", &arm_factor, -3, 3);
        GuiSlider((Rectangle){100, 20, 400, 10}, "", "", &branch_angle, -200, 200);
        float tmp = tree_levels;
        GuiSlider((Rectangle){100, 30, 400, 10}, "", "", &tmp, -10, 50);
        tree_levels = tmp;
        DrawText(TextFormat("%10.2f", spirle.rotation), 10, 60, 12, GREEN);

        EndDrawing();

        if(20 > GetFPS()){
            tree_levels = 5;
        }
    }

    return 0;
}
