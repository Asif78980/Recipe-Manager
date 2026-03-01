#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 100
#define MAX_INGREDIENTS 5
#define MAX_NAME_LENGTH 50
#define MAX_INGREDIENT_LENGTH 50
#define MAX_INSTRUCTION_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    char ingredients[MAX_INGREDIENTS][MAX_INGREDIENT_LENGTH];
    char instructions[MAX_INSTRUCTION_LENGTH];
} Recipe;

Recipe recipes[MAX_RECIPES];
int num_recipes = 0;

void save_recipes() {
    FILE* file = fopen("recipes.txt", "w");
    if (file == NULL) {
        printf("Error: could not open file.\n");
        return;
    }
    for (int i = 0; i < num_recipes; i++) {
        fprintf(file, "%s", recipes[i].name);
        for (int j = 0; j < MAX_INGREDIENTS; j++) {
            if (recipes[i].ingredients[j][0] != '\0') {
                fprintf(file, "%s", recipes[i].ingredients[j]);
            }
        }
        fprintf(file, "%s", recipes[i].instructions);
    }
    fclose(file);
}

void load_recipes() {
    FILE* file = fopen("recipes.txt", "r");
    if (file == NULL) {
        return;
    }
    char line[MAX_INSTRUCTION_LENGTH];
    while (fgets(line, MAX_INSTRUCTION_LENGTH, file)) {
        Recipe recipe;
        strcpy(recipe.name, line);
        int ingredient_index = 0;
        int num = MAX_INGREDIENTS;
        while (num--) {
            fgets(line, MAX_INGREDIENT_LENGTH, file);
            strcpy(recipe.ingredients[ingredient_index], line);
            ingredient_index++;
        }
        fgets(line, MAX_INSTRUCTION_LENGTH, file);
        strcpy(recipe.instructions, line);
        recipes[num_recipes] = recipe;
        num_recipes++;
    }
    fclose(file);
}

void add_recipe() {
    Recipe recipe;
    printf("Enter recipe name: ");
    fgets(recipe.name, MAX_NAME_LENGTH, stdin);
    printf("Enter ingredients (one per line, enter blank line when done):\n");
    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        fgets(recipe.ingredients[i], MAX_INGREDIENT_LENGTH, stdin);
        if (recipe.ingredients[i][0] == '\n') {
            recipe.ingredients[i][0] = '\0';
            break;
        }
    }
    printf("Enter instructions:\n");
    fgets(recipe.instructions, MAX_INSTRUCTION_LENGTH, stdin);
    recipes[num_recipes] = recipe;
    num_recipes++;
    save_recipes();
}

void print_recipe(Recipe recipe) {
    printf("Recipe: %s", recipe.name);
    printf("Ingredients:\n");
    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        if (recipe.ingredients[i][0] == '\0') {
            break;
        }
        printf("- %s", recipe.ingredients[i]);
    }
    printf("Instructions:\n%s", recipe.instructions);
}

void view_recipes() {
    for (int i = 0; i < num_recipes; i++) {
        print_recipe(recipes[i]);
        printf("\n");
    }
}

void edit_recipe() {
    int index;
    printf("Enter index of recipe to edit: ");
    scanf("%d", &index);
    getchar(); // consume newline character
    if (index < 0 || index >= num_recipes) {
        printf("Error: invalid index");
        return;
    }
    Recipe recipe = recipes[index];
    printf("Current recipe: \n");
    print_recipe(recipe);
    printf("\nEnter new recipe information: \n");
    printf("Enter recipe name: ");
    fgets(recipe.name, MAX_NAME_LENGTH, stdin);
    printf("Enter ingredients (one per line, enter blank line when done): \n");
    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        fgets(recipe.ingredients[i], MAX_INGREDIENT_LENGTH, stdin);
        if (recipe.ingredients[i][0] == '\n') {
            recipe.ingredients[i][0] = '\0';
            break;
        }
    }
    printf("Enter instructions: \n");
    fgets(recipe.instructions, MAX_INSTRUCTION_LENGTH, stdin);
    recipes[index] = recipe;
    save_recipes();
}

void delete_recipe() {
    int index;
    printf("Enter index of recipe to delete: ");
    scanf(" %d", &index);
    getchar(); // consume newline character
    if (index < 0 || index >= num_recipes) {
        printf("Error: invalid index\n");
        return;
    }
    for (int i = index; i < num_recipes - 1; i++) {
        recipes[i] = recipes[i + 1];
    }
    num_recipes--;
    save_recipes();
}

int main() {
    load_recipes();
    int choice;
    do {
        printf("Choose an option: \n");
        printf("a. Add a recipe\n");
        printf("b. View recipes\n");
        printf("c. Edit a recipe\n");
        printf("d. Delete a recipe\n");
        printf("e. Quit\n");
        printf("Your choice: ");
        scanf(" %c", &choice);
        getchar(); // consume newline character
        switch (choice) {
        case 'a':
            add_recipe();
            break;
        case 'b':
            view_recipes();
            break;
        case 'c':
            edit_recipe();
            break;
        case 'd':
            delete_recipe();
            break;
        case 'e':
            break;
        default:
            printf("Error: invalid choice.\n");
        }
    } while (choice != 'e');
    return 0;
}
