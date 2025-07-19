#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

vector<string> readSurnamesFromCSV(const string& filename) {
    vector<string> surnames;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return surnames;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            surnames.push_back(line);
        }
    }

    file.close();
    return surnames;
}

void copyToClipboard(const string& text) {
    string command = "echo -n '" + text + "' | xclip -selection clipboard";
    system(command.c_str());
}

// Обработчик нажатия кнопки
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    auto *surnames = static_cast<vector<string>*>(data);
    static size_t index = 0;

    if (index < surnames->size()) {
        copyToClipboard((*surnames)[index]);
        g_print("Copied to clipboard: %s\n", (*surnames)[index].c_str());

        // Обновляем текст на кнопке
        gtk_button_set_label(GTK_BUTTON(widget), (*surnames)[index].c_str());

        // Обновляем заголовок окна с текущим индексом
        GtkWidget *window = gtk_widget_get_toplevel(widget);
        string title = "Copy - " + to_string(index + 1) + " of " + to_string(surnames->size());
        gtk_window_set_title(GTK_WINDOW(window), title.c_str());

        index++;
    } else {
        g_print("No more surnames!\n");
    }
}

int main(int argc, char *argv[]) {
    string filename = "surnames.csv";
    vector<string> surnames = readSurnamesFromCSV(filename);

    if (surnames.empty()) {
        cerr << "No surnames found in the file!" << endl;
        return 1;
    }

    gtk_init(&argc, &argv);

    // Создание окна
    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Copy - 0 of 0");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Создание кнопки
    GtkWidget *button;
    button = gtk_button_new_with_label("Copy");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), &surnames);

    // Добавление кнопки в окно
    gtk_container_add(GTK_CONTAINER(window), button);

    // Отображение всех виджетов
    gtk_widget_show(button);
    gtk_widget_show(window);

    gtk_main();

    return 0;
}

