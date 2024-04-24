#include <stdio.h>
#include <string.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define BUFF_SIZE 256

const int max_items = 100;

struct grid_item
{
    struct nk_image *img;
    char name[BUFF_SIZE];
    int quantity;
};

int main(int argc, char *argv[])
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CINV");

    // Create the Nuklear Context
    int fontSize = 14;
    Font font = LoadFont("../resources/Helvetica.ttf");
    struct nk_context *ctx = InitNuklearEx(font, fontSize);

    struct grid_item items[max_items];
    char buf[BUFF_SIZE] = "Item to be added...";
    char image_path[BUFF_SIZE] = "Path: none";
    int list_size = 0;

    int item_quantity = 0;

    int show_about = nk_false;

    struct nk_image remove_icon = LoadNuklearImage("../resources/remove.png");
    struct nk_image edit_icon = LoadNuklearImage("../resources/edit.png");
    struct nk_image settings_icon = LoadNuklearImage("../resources/settings.png");
    struct nk_image import_icon = LoadNuklearImage("../resources/import.png");
    struct nk_image export_icon = LoadNuklearImage("../resources/export.png");
    struct nk_image about_icon = LoadNuklearImage("../resources/about.png");

    while (!WindowShouldClose())
    {
        // Update the Nuklear context, along with input
        UpdateNuklear(ctx);

        if (nk_begin(ctx, "", nk_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), NK_WINDOW_NO_SCROLLBAR))
        {
            nk_menubar_begin(ctx);
            nk_layout_row_static(ctx, 0, 120, 4);
            if (nk_button_image_label(ctx, about_icon, "About", NK_BUTTON_DEFAULT))
            {
                show_about = !show_about;
            }

            if (show_about)
            {
                static struct nk_rect s = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 80, 300, 190};
                if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s))
                {
                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_label(ctx, "Cinv", NK_TEXT_LEFT);
                    nk_label(ctx, "By Jamestiago", NK_TEXT_LEFT);
                    nk_label(ctx, "Contact me at my discord jamestiago.", NK_TEXT_LEFT);
                    nk_popup_end(ctx);
                }
                else
                    show_about = nk_false;
            }

            if (nk_menu_begin_image_label(ctx, "Settings", NK_WIDGET_RIGHT, settings_icon, nk_vec2(100, 0)))
            {
                nk_menu_end(ctx);
            }

            if (nk_menu_begin_image_label(ctx, "Import", NK_WIDGET_RIGHT, import_icon, nk_vec2(100, 0)))
            {
                nk_menu_end(ctx);
            }

            if (nk_menu_begin_image_label(ctx, "Export", NK_WIDGET_RIGHT, export_icon, nk_vec2(100, 0)))
            {
                nk_menu_end(ctx);
            }
            nk_menubar_end(ctx);

            nk_layout_row_begin(ctx, NK_DYNAMIC, SCREEN_HEIGHT - 50, 2);
            nk_layout_row_push(ctx, 0.7f);
            if (nk_group_begin(ctx, "grid", NK_WINDOW_BORDER))
            {
                int row_flags = NK_WINDOW_NO_SCROLLBAR;

                nk_layout_row_dynamic(ctx, 0, 1);
                if (nk_group_begin(ctx, "grid_header", row_flags))
                {
                    nk_layout_row_dynamic(ctx, 0, 5);
                    nk_label(ctx, "Image", NK_TEXT_LEFT);
                    nk_label(ctx, "Name", NK_TEXT_LEFT);
                    nk_label(ctx, "Quantity", NK_TEXT_LEFT);
                    nk_group_end(ctx);
                }

                nk_layout_row_dynamic(ctx, 70, 1);
                for (int i = 0; i < list_size; i++)
                {
                    if (nk_group_begin(ctx, "item row", row_flags | NK_WINDOW_BORDER))
                    {
                        nk_layout_row_begin(ctx, NK_DYNAMIC, 60, 3);
                        nk_layout_row_push(ctx, 0.2f);
                        nk_image(ctx, *items[i].img);
                        nk_layout_row_push(ctx, 0.6f);
                        if (nk_group_begin(ctx, "item values", row_flags))
                        {
                            char qty[10] = {0};
                            sprintf(qty, "%d", items[i].quantity);

                            nk_layout_row_dynamic(ctx, 0, 2);
                            nk_label(ctx, items[i].name, NK_TEXT_LEFT);
                            nk_label(ctx, qty, NK_TEXT_LEFT);
                            nk_group_end(ctx);
                        }
                        nk_layout_row_push(ctx, 0.2f);
                        if (nk_group_begin(ctx, "item options", row_flags))
                        {
                            nk_layout_row_dynamic(ctx, 50, 2);
                            if (nk_button_image(ctx, edit_icon))
                            {
                                strcpy(items[i].name, buf);
                            }
                            if (nk_button_image(ctx, remove_icon))
                            {
                                UnloadNuklearImage(*items[i].img);
                                free(items[i].img);
                                items[i].img = 0;
                                for (int j = i; j < max_items - 1; j++)
                                {
                                    items[j] = items[j + 1];
                                }
                                list_size--;
                            }

                            nk_group_end(ctx);
                        }
                        nk_group_end(ctx);
                    }
                }
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.3f);
            if (nk_group_begin(ctx, "panel", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_dynamic(ctx, 0, 1);
                nk_label(ctx, "Item:", NK_TEXT_LEFT);
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, buf, sizeof(buf), nk_filter_default);
                nk_label(ctx, "Quantity:", NK_TEXT_LEFT);
                nk_property_int(ctx, "Units", 0, &item_quantity, 99999.0f, 1, 1);
                nk_label(ctx, "Drop an image here!", NK_TEXT_LEFT);
                nk_label(ctx, image_path, NK_TEXT_LEFT);
                FilePathList droppedFiles = LoadDroppedFiles();
                if (droppedFiles.count)
                {
                    strcpy(image_path, droppedFiles.paths[0]);
                }
                UnloadDroppedFiles(droppedFiles);

                if (nk_button_label(ctx, "Add"))
                {
                    if (list_size < max_items)
                    {
                        struct grid_item item = {.img = 0, .name = {0}, .quantity = item_quantity};
                        strcpy(item.name, buf);

                        if (image_path != NULL)
                        {
                            item.img = malloc(sizeof(struct nk_image));
                            *item.img = LoadNuklearImage(image_path);
                        }

                        items[list_size] = item;
                        list_size++;
                    }
                }
                nk_group_end(ctx);
            }
            nk_layout_row_end(ctx);
        }
        nk_end(ctx);

        // Render
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render the Nuklear GUI
        DrawNuklear(ctx);

        EndDrawing();
    }

    // De-initialize the Nuklear GUI
    UnloadNuklear(ctx);

    CloseWindow();
    return 0;
}