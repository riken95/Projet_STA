#include <ncurses.h>
#include <unistd.h>

int main() {
    // Initialiser ncurses
    initscr();

    // Désactiver l'affichage des touches saisies
    noecho();

    // Créer une fenêtre pour afficher le menu
    WINDOW *menuwin = newwin(10, 30, 1, 1);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    // Entier à afficher
    int entier = 0;

    while (1) {
        // Effacer la fenêtre du menu
        wclear(menuwin);
        box(menuwin, 0, 0);

        // Afficher l'entier à la position (2, 2) dans la fenêtre du menu
        mvwprintw(menuwin, 2, 2, "Entier : %d", entier);

        // Rafraîchir uniquement la fenêtre du menu
        wrefresh(menuwin);

        // Attendre 1 seconde
        sleep(1);

        // Mettre à jour l'entier
        entier++;

        // Condition de sortie de la boucle (par exemple, si l'utilisateur appuie sur 'q')
        if (getch() == 'q') {
            break;
        }
    }

    // Finaliser ncurses
    endwin();

    // Fin du programme
    return 0;
}
