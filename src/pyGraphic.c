/**
 * copyrights:  All reserved by Penguin group
 * company:     Penguin group
 * developer:   Shang Renpeng/Qin Zitong/Ren Wang/Xiao Sa/Zou Yongpin/Pang Bo
 * phone:       15201329144
 * function:    GUI Functions
 * version:     1.0
 * changlog:    2010\12\12 version0.1  Shang Rnepeng make interface
 *              2010\12\23 version0.2  Qin Zitong architect the GUI
 *              2010\12\23 version0.21 Ren Wang add save dialog
 *              2010\12\30 version0.22 Zou Yongping and Xiao Sa add open dialog
 *              2010\12\31 version0.23 Shang Renpeng add menu
 *              2011\01\06 version1.0  Pang Bo add about dialog
 **/

#include <gtk/gtk.h>
#include "../include/defination.h"
#include "../include/pyFile.h"
#include "../include/mydic.h"
#include "../include/participle.h"

static void changeView(void);   //chage view mode

static GtkWidget *makeTextView(void);   //make a textview

static GtkWidget *buttonSave(void);

static void fileOkSave(void);

static GtkWidget *buttonOpen(void);

static void fileOkOpen(void);

static void about(void);

static void get_main_menu( GtkWidget  *window,
                           GtkWidget **menubar );

static GdkPixbuf *createPixbuf(const gchar * filename);   //change icon
//From http://zetcode.com/tutorials/gtktutorial/chinese/firstprograms/

static void zhuYin(void);      //Zhu Yin function

extern indexes *root;   //my customed Trie

static GtkItemFactoryEntry menu_items[] =
{
    { "/_F文件",             NULL,           NULL,           0, "<Branch>" },
    { "/F文件/_打开",        "<control>O",   buttonOpen,    0, NULL },
    { "/F文件/_保存",        "<control>S",   buttonSave,    0, NULL },
    { "/F文件/sep1",         NULL,           NULL,           0, "<Separator>" },
    { "/F文件/关闭",         "<control>Q",   gtk_main_quit,  0, NULL },
    { "/_V视图",                 NULL,      NULL,           0, "<Branch>" },
    { "/V视图/改变排列方式",  "<control>E",   changeView,           0, NULL },
    { "/_H帮助",             "<control>H",   NULL,           0, "<LastBranch>" },
    { "/_H帮助/关于",        NULL,           about,           0, NULL },
};

GtkWidget *vboxTop = NULL;
GtkWidget *hboxTop = NULL;
GtkWidget *textInput = NULL;        //not textview but with scrolled windows
GtkWidget *textOutput = NULL;
GtkWidget *textInputView = NULL;
GtkWidget *textOutputView = NULL;   //True textview
GtkWidget *frameLeft = NULL;
GtkWidget *frameRight = NULL;
GtkWidget *frameTop = NULL;
GtkWidget *frameButtom = NULL;
GtkWidget *filesave = NULL;
GtkWidget *fileopen = NULL;

GtkTextBuffer *bufferInput = NULL;
GtkTextBuffer *bufferOutput = NULL;

BOOL hStatue = TRUE;    //indicates view mode, True indicates horizental

void tuxing(int argc, char *argv[])
{
    GtkWidget *button = NULL;
    GtkWidget *win = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *hboxButtom = NULL;
    GtkWidget *label = NULL;
    GtkWidget *menubar = NULL;

    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    //initialize GTK+
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler,NULL);

    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);     //make a windows
    gtk_container_set_border_width (GTK_CONTAINER (win), 10);
    gtk_widget_set_size_request (GTK_WIDGET (win), 600, 400);
    gtk_window_set_title (GTK_WINDOW (win), "Penguin注音软件");
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_icon(GTK_WINDOW(win), createPixbuf("icon.png"));
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);

    hboxButtom = gtk_hbox_new (FALSE, 0);   //used to show buttons
    button = gtk_button_new_with_label ("保存");
    gtk_signal_connect(GTK_OBJECT(button),"clicked",
                       GTK_SIGNAL_FUNC(buttonSave),NULL);
    gtk_box_pack_end (GTK_BOX (hboxButtom), button, FALSE, FALSE, 20);
    button = gtk_button_new_with_label ("注音");
    g_signal_connect (button, "clicked", G_CALLBACK(zhuYin), NULL);
    gtk_box_pack_end (GTK_BOX (hboxButtom), button, FALSE, FALSE, 20);
    button = gtk_button_new_with_label ("打开");
    g_signal_connect (button, "clicked", GTK_SIGNAL_FUNC(buttonOpen), NULL);
    gtk_box_pack_end (GTK_BOX (hboxButtom), button, FALSE, FALSE, 20);
    label = gtk_label_new( "An apple a day keeps doctors away!" );
    gtk_label_set_justify((GtkLabel *)label, GTK_JUSTIFY_LEFT);
    gtk_box_pack_start (GTK_BOX (hboxButtom), label, FALSE, FALSE, 20);

    gtk_widget_show(hboxButtom);

    hboxTop = gtk_hbox_new (TRUE, 10);
    //this two box to change view mode
    vboxTop = gtk_vbox_new (TRUE, 10);

    frameLeft = gtk_frame_new( "请在这里输入：" );
    frameRight = gtk_frame_new( "这里显示结果：" );
    frameTop = gtk_frame_new( "请在这里输入：" );
    frameButtom = gtk_frame_new( "这里显示结果：" );
    //four frame to cotain two text window

    textInput = makeTextView();
    textOutput = makeTextView();

    gtk_container_add (GTK_CONTAINER (frameLeft), textInput);
    gtk_container_add (GTK_CONTAINER (frameRight), textOutput);
    gtk_box_pack_start (GTK_BOX (hboxTop), frameLeft, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (hboxTop), frameRight, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (vboxTop), frameTop, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (vboxTop), frameButtom, TRUE, TRUE, 0);

    vbox = gtk_vbox_new (FALSE, 10);        //this vbox contain all

    get_main_menu (win, &menubar);
    gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, TRUE, 0);
    gtk_widget_show (menubar);

    gtk_box_pack_end (GTK_BOX(vbox), hboxButtom, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), vboxTop, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), hboxTop, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);

    gtk_widget_show_all (win);
    gtk_widget_hide (vboxTop);

    gtk_main ();
    return;
}


static GtkWidget *makeTextView(void)
{
    GtkWidget *scrolled_window;
    GtkWidget *view;
    PangoFontDescription *desc =
            pango_font_description_from_string("AR PL Ukai CN 12");
    //change font to make caractor same width.

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),    \
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    if (textInputView == NULL)
        view = textInputView = gtk_text_view_new();
    else
        view = textOutputView = gtk_text_view_new();

    gtk_container_add (GTK_CONTAINER (scrolled_window), view);

    gtk_widget_modify_font(view, desc);
    pango_font_description_free(desc);

    gtk_widget_show_all (scrolled_window);

    return scrolled_window;
}

static void changeView(void)
{
    if (hStatue == TRUE)
    {
        gtk_widget_hide_all (hboxTop);

        gtk_widget_ref(textInput);   //to remain textIput
        gtk_widget_ref(textOutput);

        gtk_container_remove (GTK_CONTAINER ( frameLeft), textInput);
        gtk_container_remove (GTK_CONTAINER ( frameRight), textOutput);

        gtk_container_add (GTK_CONTAINER ( frameTop), textInput);
        gtk_container_add (GTK_CONTAINER ( frameButtom), textOutput);

        gtk_widget_unref(textInput);
        gtk_widget_unref(textOutput);

        gtk_widget_show_all(vboxTop);

        hStatue = FALSE;
    }
    else
    {
        gtk_widget_hide_all (vboxTop);

        gtk_widget_ref(textInput);
        gtk_widget_ref(textOutput);

        gtk_container_remove (GTK_CONTAINER ( frameTop), textInput);
        gtk_container_remove (GTK_CONTAINER ( frameButtom), textOutput);

        gtk_container_add (GTK_CONTAINER ( frameLeft), textInput);
        gtk_container_add (GTK_CONTAINER ( frameRight), textOutput);

        gtk_widget_unref(textInput);
        gtk_widget_unref(textOutput);

        gtk_widget_show_all(hboxTop);

        hStatue = TRUE;
    }
}

static void get_main_menu( GtkWidget  *window,
                           GtkWidget **menubar )
{
    GtkItemFactory *item_factory;
    GtkAccelGroup *accel_group;
    gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

    accel_group = gtk_accel_group_new ();

    item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>",\
                                         accel_group);

    gtk_item_factory_create_items (item_factory, nmenu_items,
                                   menu_items, NULL);

    gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
    if (menubar)

        *menubar = gtk_item_factory_get_widget (item_factory, "<main>");
}

static GdkPixbuf *createPixbuf(const gchar * filename)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if(!pixbuf)
    {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

static GtkWidget *buttonSave(void)
{
    filesave = gtk_file_selection_new("保存文件");
    gtk_window_set_position (GTK_WINDOW (filesave), GTK_WIN_POS_CENTER);
    gtk_window_set_modal (GTK_WINDOW (filesave), TRUE); //make windows on top
    g_signal_connect(G_OBJECT(filesave),"destroy",  \
                     G_CALLBACK(gtk_widget_destroy),filesave);
    g_signal_connect(G_OBJECT (GTK_FILE_SELECTION(filesave)->ok_button),    \
                     "clicked",G_CALLBACK(fileOkSave),filesave);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION    \
                                      (filesave)->cancel_button),   \
                             "clicked",G_CALLBACK(gtk_widget_destroy),  \
                             filesave);
    gtk_window_set_icon(GTK_WINDOW(filesave), createPixbuf("icon.png"));
    gtk_widget_show_all(filesave);
    return filesave;
}

static void fileOkSave(void)
{
    gchar *tempOutput;
    GtkTextIter start,end;
    BOOL save = FALSE;

    bufferOutput = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textOutputView));

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufferOutput),&start,&end);

    tempOutput = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(bufferOutput),
                                          &start,&end,FALSE);

    save = writefile(tempOutput,strlen((char *)tempOutput), \
                     (char *)gtk_file_selection_get_filename    \
                     (GTK_FILE_SELECTION(filesave)),    \
                     strlen((char *)gtk_file_selection_get_filename \
                            (GTK_FILE_SELECTION(filesave))));

    if (save == FALSE)
    {
        return;
    }
    else
    {
        gtk_widget_hide_all(filesave);
        gtk_widget_destroy(filesave);
    }
}

static GtkWidget *buttonOpen(void)
{
    fileopen = gtk_file_selection_new("打开文件");
    gtk_window_set_position (GTK_WINDOW (fileopen), GTK_WIN_POS_CENTER);
    gtk_window_set_modal (GTK_WINDOW (fileopen), TRUE);
    g_signal_connect(G_OBJECT(fileopen),"destroy",  \
                     G_CALLBACK(gtk_widget_destroy),fileopen);
    g_signal_connect(G_OBJECT (GTK_FILE_SELECTION(fileopen)->ok_button),    \
                     "clicked",G_CALLBACK(fileOkOpen),fileopen);
    g_signal_connect_swapped(G_OBJECT (GTK_FILE_SELECTION   \
                                       (fileopen)->cancel_button),  \
                             "clicked",G_CALLBACK(gtk_widget_destroy),  \
                             fileopen);
    gtk_window_set_icon(GTK_WINDOW(fileopen), createPixbuf("icon.png"));
    gtk_widget_show_all(fileopen);
    return fileopen;
}

static void fileOkOpen(void)
{
    gchar *tempInput;

    bufferInput = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textInputView));

    tempInput = readfile((char *)gtk_file_selection_get_filename    \
                         (GTK_FILE_SELECTION(fileopen)),    \
                         strlen((char *)gtk_file_selection_get_filename \
                                (GTK_FILE_SELECTION(fileopen))));

    if (tempInput == NULL)
    {
        return;
    }
    else
    {

        gtk_text_buffer_set_text(bufferInput, tempInput, strlen(tempInput));

        gtk_widget_hide_all(fileopen);
        gtk_widget_destroy(fileopen);
    }
}

static void zhuYin(void)
{
    gchar *tempInput;
    char *tempOutput;
    GtkTextIter start,end;


    bufferInput = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textInputView));

    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufferInput),&start,&end);

    tempInput = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(bufferInput),  \
                                         &start,&end,FALSE);

    tempOutput = matchText(tempInput,strlen(tempInput));

    bufferOutput = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textOutputView));

    gtk_text_buffer_set_text(bufferOutput,tempOutput, strlen(tempOutput));
}

static void about(void)
{
    GtkWidget *dialog = NULL;
    GtkWidget *label = NULL;

    dialog = gtk_dialog_new();

    gtk_window_set_title (GTK_WINDOW (dialog), "关于Penguin注音软件");
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

    gtk_widget_set_size_request (GTK_WIDGET (dialog), 240, 120);

    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_icon(GTK_WINDOW(dialog), createPixbuf("icon.png"));

    label = gtk_label_new( "Penguin程序由Penguin项目组所有！\n\t\t快捷键："
                          "\n\tctrl+O\t\t打开\n\tctrl+S\t\t保存\n\tctrl+Q\t\t退出\n"
                          "\tctrl+E\t\t切换视图" );
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG(dialog) -> vbox),    \
                       label, TRUE, TRUE, 0);

    gtk_widget_show_all(dialog);
}
