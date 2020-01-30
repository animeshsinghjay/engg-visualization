#include <gtk/gtk.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <armadillo>
#include "../include/Two_D_to_Three_D.h"
#include "../include/Show.h"
#include <math.h>
#include "../include/Edges.h"
#include "../include/Triplet.h"
#include "../include/Graph_Imp.h"
#include "../include/Three_D_to_Two_D.h"


#define PI 3.14159265


using namespace std;
using namespace arma;


string filename = "";
string save_filename = "";
GtkWidget *entry_for_x, *entry_for_y, *entry_for_z, *scale_x, *scale_y, *scale_z;
GtkWidget *check_box_save_2d, *check_box_save_3d;
static void menu_response(GtkWidget* menu_item, gpointer window)
{
        if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "Open") == 0)       // equal
        {
                GtkWidget *dialog;
                dialog = gtk_file_chooser_dialog_new("Choose a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
                gtk_widget_show_all(dialog);
        //      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"/");
                gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
                gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
                if(resp == GTK_RESPONSE_OK){
                        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                        cout<<"Input file selected is "<<filename<<endl;
                }
                else
                        g_print("You pressed Cancel\n");
                gtk_widget_destroy(dialog);
        }
        else if (strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item)), "Save") == 0)
        {
                GtkWidget *dialog;
                dialog = gtk_file_chooser_dialog_new("Choose a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
                gtk_widget_show_all(dialog);
        //      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"/");
                gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
                gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
                if(resp == GTK_RESPONSE_OK){
                        save_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                        cout<<"Save file selected is "<<save_filename<<endl;
                }
                else
                        g_print("You pressed Cancel\n");
                gtk_widget_destroy(dialog);
        }
}

static void button_clicked(GtkWidget *widget, gpointer data){
        double xdir = atof(gtk_entry_get_text(GTK_ENTRY(entry_for_x)));
        double ydir = atof(gtk_entry_get_text(GTK_ENTRY(entry_for_y)));
        double zdir = atof(gtk_entry_get_text(GTK_ENTRY(entry_for_z)));
        Triplet dir = {xdir, ydir, zdir};
        if (filename.compare("")!=0){
                Three_D_to_Two_D T;
                char tab2[1024];
                strcpy(tab2, filename.c_str());
                T.G = T.toGraph(tab2);
                float x_angle = gtk_range_get_value(GTK_RANGE(scale_x));
                float y_angle = gtk_range_get_value(GTK_RANGE(scale_y));
                float z_angle = gtk_range_get_value(GTK_RANGE(scale_z));
                float sin_x = sin((x_angle*PI)/180);
                float cos_x = cos((x_angle*PI)/180);
                float sin_y = sin((y_angle*PI)/180);
                float cos_y = cos((y_angle*PI)/180);
                float sin_z = sin((z_angle*PI)/180);
                float cos_z = cos((z_angle*PI)/180);
                mat rot_x, rot_y, rot_z;
                rot_x<<1<<0<<0<<endr<<0<<cos_x<<(-sin_x)<<endr<<0<<sin_x<<cos_x<<endr;
                rot_y<<cos_y<<0<<sin_y<<endr<<0<<1<<0<<endr<<(-sin_y)<<0<<cos_y<<endr;
                rot_z<<cos_z<<(-sin_z)<<0<<endr<<sin_z<<cos_z<<0<<endr<<0<<0<<1<<endr;
                mat final_rot = rot_z * rot_y * rot_x;
                std::vector<Triplet> rotated_vertices;
                for (int i = 0; i < T.G.vertices.size(); ++i)
                {
                        mat coordinates;
                        coordinates<<T.G.vertices[i].one<<endr<<T.G.vertices[i].two<<endr<<T.G.vertices[i].three<<endr;
                        mat rotated_y_x = final_rot * coordinates;
                        Triplet temp = {rotated_y_x(0,0), rotated_y_x(1,0), rotated_y_x(2,0)};
                        rotated_vertices.push_back(temp);
                }
                T.G.vertices = rotated_vertices;
                Show S;
                S.show_qt_projections(T, dir);
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_box_save_2d))){
                    if (save_filename.compare("")!=0)
                    {
                        ofstream out;
                        out.open(save_filename);
                        out<<T.projected_xy.vertices.size()<<endl;
                        for (int i = 0; i < T.projected_xy.vertices.size(); ++i)
                        {
                            out<<T.projected_xy.vertices[i].one<<" "<<T.projected_xy.vertices[i].two<<" "<<T.projected_xy.vertices[i].three<<endl;
                        }
                        out<<T.projected_xy.edges;
                        out<<"--"<<endl;
                        out<<T.projected_yz.vertices.size()<<endl;
                        for (int i = 0; i < T.projected_yz.vertices.size(); ++i)
                        {
                            out<<T.projected_yz.vertices[i].one<<" "<<T.projected_yz.vertices[i].two<<" "<<T.projected_yz.vertices[i].three<<endl;
                        }
                        out<<T.projected_yz.edges;
                        out<<"--"<<endl;
                        out<<T.projected_zx.vertices.size()<<endl;
                        for (int i = 0; i < T.projected_zx.vertices.size(); ++i)
                        {
                            out<<T.projected_zx.vertices[i].one<<" "<<T.projected_zx.vertices[i].two<<" "<<T.projected_zx.vertices[i].three<<endl;
                        }
                        out<<T.projected_zx.edges;
                        out.close();
                    }
                    else{
                        cout<<"Please select where to save file"<<endl;
                    }
                }
        }
        else {
                cout<<"Please select a file"<<endl;
        }
}

static void make3d_clicked(GtkWidget *widget, gpointer data){
        if (filename!="")
        {
        		Two_D_to_Three_D T;
                Graph_Imp G_on_xy,G_on_yz,G_on_xz;
                char tab2[1024];
                strcpy(tab2, filename.c_str());
                T.toGraphAllThree(tab2,G_on_xy,G_on_yz,G_on_xz);
                Graph_Imp G3D = T.makethreed(G_on_xy, G_on_yz, G_on_xz);
                T.show_gnu_plot(G3D); 
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_box_save_3d))){
                    if (save_filename.compare("")!=0)
                    {
                        ofstream out;
                        out.open(save_filename);
                        out<<G3D.vertices.size()<<endl;
                        for (int i = 0; i < G3D.vertices.size(); ++i)
                        {
                            out<<G3D.vertices[i].one<<" "<<G3D.vertices[i].two<<" "<<G3D.vertices[i].three<<endl;
                        }
                        out<<G3D.edges;
                        out.close();
                    }
                    else{
                        cout<<"Please select where to save file"<<endl;
                    }
                }      
        }
        else{
                cout<<"Please select a file"<<endl;
        }
                
}
int main(int argc, char* argv[])
{
        gtk_init(&argc, &argv);
        GtkWidget *window, *menu_bar, *menu_item, *file_menu, *help_menu, *vbox, *button, *label, *label2, *notebook, *button_to_make_3D;
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
 
        // The menu bar is created 
        menu_bar = gtk_menu_bar_new();
 
        file_menu = gtk_menu_new();
 
        menu_item = gtk_menu_item_new_with_label("File");
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), file_menu);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
 
        menu_item = gtk_menu_item_new_with_label("Open");
        gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_item);
        g_signal_connect(menu_item, "activate", G_CALLBACK(menu_response), NULL);
 
        menu_item = gtk_menu_item_new_with_label("Save");
        gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_item);
        g_signal_connect(menu_item, "activate", G_CALLBACK(menu_response), NULL);

        // Notebook for 2D to 3D or 3D to 2D
        notebook = gtk_notebook_new();
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);


        // 3D to 2D tab
        GtkWidget *direction_label, *x_label, *y_label, *z_label, *hbox_for_x, *hbox_for_y, *hbox_for_z;
        
        label = gtk_label_new("3D to 2D");
        vbox = gtk_vbox_new(0,10);
        gtk_box_pack_start(GTK_BOX(vbox), menu_bar,0,0,0);

        // Heading for direction
        direction_label = gtk_label_new("Enter the viewing direction");
        gtk_box_pack_start(GTK_BOX(vbox), direction_label,0,0,0);

        // x part written
        x_label = gtk_label_new("Enter x-component");
        entry_for_x = gtk_entry_new();
        hbox_for_x = gtk_hbox_new(0,10);
        gtk_box_pack_start(GTK_BOX(hbox_for_x), x_label, 0, 0, 10);
        gtk_box_pack_start(GTK_BOX(hbox_for_x), entry_for_x, 0, 0, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox_for_x,0,0,10);

        // y part written
        y_label = gtk_label_new("Enter y-component");
        entry_for_y = gtk_entry_new();
        hbox_for_y = gtk_hbox_new(0,10);
        gtk_box_pack_start(GTK_BOX(hbox_for_y), y_label, 0, 0, 10);
        gtk_box_pack_start(GTK_BOX(hbox_for_y), entry_for_y, 0, 0, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox_for_y,0,0,10);

        // z part written
        z_label = gtk_label_new("Enter z-component");
        entry_for_z = gtk_entry_new();
        hbox_for_z = gtk_hbox_new(0,10);
        gtk_box_pack_start(GTK_BOX(hbox_for_z), z_label, 0, 0, 10);
        gtk_box_pack_start(GTK_BOX(hbox_for_z), entry_for_z, 0, 0, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox_for_z,0,0,10);

        //sliders declared
        GtkWidget *slider_label_x, *slider_label_y, *slider_label_z;
        GtkWidget *hbox_slider_for_x, *hbox_slider_for_y, *hbox_slider_for_z;

        // x slider
        slider_label_x = gtk_label_new("Enter angle to be rotated by with x-axis");
        scale_x = gtk_hscale_new_with_range(0,180,5);
        gtk_scale_set_value_pos(GTK_SCALE(scale_x),GTK_POS_BOTTOM);
        hbox_slider_for_x = gtk_hbox_new(1,10);
        gtk_box_pack_start(GTK_BOX(hbox_slider_for_x), slider_label_x, 1, 1, 10);
        gtk_box_pack_start(GTK_BOX(hbox_slider_for_x), scale_x, 1, 1, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox_slider_for_x,1,1,10);

        // y slider
        slider_label_y = gtk_label_new("Enter angle to be rotated by with y-axis");
        scale_y = gtk_hscale_new_with_range(0,180,5);
        gtk_scale_set_value_pos(GTK_SCALE(scale_y),GTK_POS_BOTTOM);
        hbox_slider_for_y = gtk_hbox_new(1,10);
        gtk_box_pack_start(GTK_BOX(hbox_slider_for_y), slider_label_y, 1, 1, 10);
        gtk_box_pack_start(GTK_BOX(hbox_slider_for_y), scale_y, 1, 1, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox_slider_for_y,1,1,10);

        //z slider
        slider_label_z = gtk_label_new("Enter angle to be rotated by with x-axis");
        scale_z = gtk_hscale_new_with_range(0,180,5);
        gtk_scale_set_value_pos(GTK_SCALE(scale_z),GTK_POS_BOTTOM);
        hbox_slider_for_z = gtk_hbox_new(1,10);
        gtk_box_pack_start(GTK_BOX(hbox_slider_for_z), slider_label_z, 1, 1, 10);
        gtk_box_pack_start(GTK_BOX(hbox_slider_for_z), scale_z, 1, 1, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox_slider_for_z,1,1,10);

        //check box for saving it in a file
        check_box_save_2d = gtk_check_button_new_with_label("Save the 2D Projections");
        gtk_box_pack_start(GTK_BOX(vbox), check_box_save_2d, 1,1,10);

        //button to submit these changes
        button = gtk_button_new_with_label("Make Projections");
        gtk_box_pack_start(GTK_BOX(vbox), button,1,1,10);
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
        
        //signal for button select
        g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
        g_signal_connect(entry_for_z, "activate", G_CALLBACK(button_clicked), NULL);

        // 2d to 3d
        button_to_make_3D = gtk_button_new_with_mnemonic("Generate 3D Plot");
        label = gtk_label_new("2D to 3D");
        GtkWidget *vbox_for_button;
        vbox_for_button = gtk_vbox_new(0,200);
        //check box for saving it in a file
        check_box_save_3d = gtk_check_button_new_with_label("Save the 3D Projections");
        gtk_box_pack_start(GTK_BOX(vbox_for_button), check_box_save_3d, 1,1,10);
        gtk_box_pack_start(GTK_BOX(vbox_for_button), button_to_make_3D,1,1,200);
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_for_button, label);
        g_signal_connect(button_to_make_3D, "clicked", G_CALLBACK(make3d_clicked), NULL);

        // Main container added to the window
        gtk_container_add(GTK_CONTAINER(window), notebook);

        // Setting the window title
        gtk_window_set_title(GTK_WINDOW(window),"ED Package");
        gtk_widget_show_all(window);
        gtk_main();
        return 0;
}