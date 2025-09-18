// cc demo_tree.c -odemo_tree -lraylib -lm


#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "betterlist.h"

#define TURTLE_IMPL
#include "turtle.h"


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
    InitWindow(900, 1000, "Turtle Tree Demo");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Turtle spirle = {0};

    Turtle tree = {0};

    float arm_factor = 0;

    const Vector2 turtle_center = {GetScreenWidth() / 2.0, GetScreenHeight() / 2.0};

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(0, 0);

        Turtle_Clear(&tree);
        Turtle_PenDown(&tree, WHITE, 1);
        Turtle_Goto(&tree, turtle_center);
        tree.rotation = -90;

        Turtle_Tree(&tree, 100, tree_levels);
        Turtle_Draw(&tree);

        GuiSlider((Rectangle){100, 10, 200, 10}, "", "", &arm_factor, -3, 3);
        GuiSlider((Rectangle){100, 20, 400, 10}, "", "", &branch_angle, -200, 200);
        float tmp = tree_levels;
        GuiSlider((Rectangle){100, 30, 400, 10}, "", "", &tmp, -10, 50);
        tree_levels = tmp;
        DrawText(TextFormat("%10.2f", spirle.rotation), 10, 60, 12, GREEN);

        EndDrawing();

        if (20 > GetFPS())
        {
            tree_levels = 5;
        }
    }

    return 0;
}
