# Recipe Manager

A menu-driven **C console application** for adding, viewing, editing, and deleting recipes. Each recipe stores a name, up to five ingredients, and a short instruction line.

The project demonstrates structures, arrays, strings, functions, and text-file handling in C.

## Features

- **Add recipes** with a name, ingredients, and preparation instructions.
- **View recipes** in their current list order.
- **Edit recipes** using a zero-based index.
- **Delete recipes** and shift the remaining entries forward.
- **Save changes** to `recipes.txt` after adding, editing, or deleting.
- **Load saved data** when the program starts.

The current implementation has input-handling and storage limitations described below.

## Repository contents

| File | Purpose |
|---|---|
| [a.c](a.c) | Complete application source code |
| [recipes.txt](recipes.txt) | Local recipe storage; the committed file is initially empty |
| [README.md](README.md) | Setup, usage, and implementation guide |

## Build and run

### Requirements

- A C compiler with C99 or later support, such as GCC.
- A terminal or an IDE configured with a C compiler.

The program uses only standard C headers: `stdio.h`, `stdlib.h`, and `string.h`. No external libraries or database server are required.

### Download

```bash
git clone https://github.com/Asif78980/Recipe-Manager.git
cd Recipe-Manager
```

Alternatively, select **Code → Download ZIP** on GitHub and extract the project.

### Windows — GCC / MinGW

Compile in the project directory:

```powershell
gcc -std=c99 -Wall -Wextra a.c -o recipe_manager.exe
```

Run in PowerShell:

```powershell
.\recipe_manager.exe
```

In Git Bash, use `./recipe_manager.exe`.

### Linux / macOS — GCC-compatible compiler

```bash
gcc -std=c99 -Wall -Wextra a.c -o recipe_manager
./recipe_manager
```

The current code may produce a compiler warning because its menu input uses `%c` with an `int` variable. See **Current limitations** before relying on the application.

## Usage

The application displays this menu:

```text
Choose an option:
a. Add a recipe
b. View recipes
c. Edit a recipe
d. Delete a recipe
e. Quit
Your choice:
```

| Option | Action | Input |
|---|---|---|
| `a` | Add a recipe | Name, ingredients, and one instruction line |
| `b` | Display all recipes | No additional input |
| `c` | Replace a recipe's details | Recipe index, then all replacement fields |
| `d` | Delete a recipe | Recipe index |
| `e` | Exit | No additional input |

**Indices start at 0:** the first recipe is `0`, the second is `1`, and so on. The current view does not print these indices, so count recipes in display order. Indices change after a deletion.

For an initial demonstration, enter **exactly five ingredients**. Although the prompt offers a blank line to finish early, the current save/load implementation does not reliably support fewer than five.

Example data for the Add option:

| Field | Example |
|---|---|
| Name | Vegetable Omelette |
| Ingredient 1 | 2 eggs |
| Ingredient 2 | 1 chopped onion |
| Ingredient 3 | 1 chopped tomato |
| Ingredient 4 | 1 teaspoon oil |
| Ingredient 5 | Salt |
| Instructions | Beat eggs, mix vegetables and salt, then cook in oil until set. |

After the fifth ingredient, enter the instructions immediately; an extra blank line would be read as the instructions.

## Data model

Each recipe is stored in a `Recipe` structure:

```c
typedef struct {
    char name[MAX_NAME_LENGTH];
    char ingredients[MAX_INGREDIENTS][MAX_INGREDIENT_LENGTH];
    char instructions[MAX_INSTRUCTION_LENGTH];
} Recipe;
```

| Constant | Value | Purpose |
|---|---:|---|
| `MAX_RECIPES` | 100 | Number of slots in the recipe array |
| `MAX_INGREDIENTS` | 5 | Ingredient slots per recipe |
| `MAX_NAME_LENGTH` | 50 | Name buffer size in bytes |
| `MAX_INGREDIENT_LENGTH` | 50 | Ingredient buffer size in bytes |
| `MAX_INSTRUCTION_LENGTH` | 100 | Instruction buffer size in bytes |

These are buffer capacities, not guaranteed usable text lengths. Leave room for the terminating null byte and the newline captured by `fgets()`. For ordinary ASCII input, keep names and ingredients to at most **48 characters**, and instructions to at most **98 characters**, so the newline fits in the buffer.

## File storage

The application reads and writes `recipes.txt` in the **current working directory**. Run it from the project folder to use the included file.

- Startup calls `load_recipes()`.
- Add, edit, and delete call `save_recipes()`.
- Saving rewrites the full file using the current in-memory recipe list.
- If the file is absent, loading returns without adding recipes; a later successful save creates it.

The loader expects each record to contain seven lines:

1. Recipe name.
2. Ingredient 1.
3. Ingredient 2.
4. Ingredient 3.
5. Ingredient 4.
6. Ingredient 5.
7. Instructions.

There is no ingredient-count field or record separator. The writer can omit empty ingredient slots, which makes its output inconsistent with the loader's fixed seven-line expectation when fewer than five ingredients are used.

## Code guide

| Function | Responsibility |
|---|---|
| `load_recipes()` | Reads recipe records from the text file |
| `save_recipes()` | Writes the in-memory collection to the text file |
| `add_recipe()` | Collects input and appends a recipe |
| `print_recipe()` | Displays one recipe |
| `view_recipes()` | Displays the collection |
| `edit_recipe()` | Replaces the details at a selected index |
| `delete_recipe()` | Removes a recipe and compacts the array |
| `main()` | Loads data and runs the menu loop |

## Current limitations

- **Menu input type:** `choice` is declared as `int`, but `scanf(" %c", &choice)` requires a `char *`. This is undefined behavior and should be corrected before dependable use.
- **Incomplete ingredient initialization:** ending ingredient entry early leaves later slots uninitialized when adding a recipe, or may retain earlier contents when editing. Saving can therefore access invalid or stale data.
- **Storage format mismatch:** saving fewer than five ingredient lines conflicts with the loader's fixed record format.
- **Capacity checks:** adding and loading do not enforce the 100-recipe array limit.
- **Input validation:** non-numeric indices, overlong lines, and incomplete file records are not handled robustly.
- **Single-line instructions:** multiline preparation steps are not supported.
- **No search or categories:** recipes are accessed by list position.

## Possible improvements

- Correct the menu variable type and validate all input.
- Initialize recipe structures and store an explicit ingredient count.
- Use a consistent, validated record format for saving and loading.
- Enforce capacity limits and display recipe indices.
- Add search, categories, and multiline preparation steps.
