#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define X_LENGTH 160
#define Y_LENGTH 80

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

static int	isinset(char achar, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (achar == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	isend(char const *s1, char const *set, int index)
{
	if (!(isinset(s1[index], set)))
		return (0);
	else
	{
		index++;
		if (!(isinset(s1[index], set)) && s1[index] != '\0')
			return (0);
		while (s1[index])
		{
			if (s1[index + 1] == '\0')
				return (1);
			if (!(isinset(s1[index], set)))
				return (0);
			index++;
		}
	}
	return (1);
}

static char	*strtrim(char *s1, char const *set)
{
	char	*array;
	int		i;
	int		len;
	int		e;

	if (!s1 || !set)
		return (NULL);
	e = 0;
	i = 0;
	len = strlen(s1) - 1;
	while (isinset(s1[i], set) && s1[i])
		i++;
	while ((isinset(s1[len], set)))
		len--;
	len++;
	len -= i;
	if ((len * (-1)) == (int)strlen(s1))
		len = 0;
	array = malloc(sizeof(char) * len + 1);
	if (array == NULL)
		return (NULL);
	while (!(isend(s1, set, i)) && s1[i])
		array[e++] = s1[i++];
	array[e] = '\0';
	free (s1);
	return (array);
}

static char	**init_map(void)
{
	char	**map = NULL;
	FILE	*file;
	int		i = 0;
	size_t line_length = X_LENGTH;

	file = fopen("starting_grid/blank.txt", "r");
	if (file == NULL)
		return (NULL);
	map = malloc(sizeof(char *) * Y_LENGTH + sizeof(char *));
	if (!map)
	{
		fclose (file);
		return (NULL);
	}
	for (int i = 0; i < Y_LENGTH; i++)
	{
		map[i] = malloc (sizeof(char) * X_LENGTH + sizeof(char));
		if (!map)
		{
			int j = 0;
			while (j < i)
				free(map[j++]);
			free(map);
			fclose (file);
			return (NULL);
		}
	}
	while (i < Y_LENGTH)
	{
		getline(&map[i], &line_length, file);
		map[i] = strtrim(map[i], "\n");
		i++;	
	}
	map[i] = NULL;
	fclose(file);
	return (map);
}

static void	free_double_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (array[i])
			free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

static void	draw_cells(SDL_Renderer *renderer, char **map)
{
	int	cell_size_x = WINDOW_WIDTH / X_LENGTH;
	int	cell_size_y = WINDOW_HEIGHT / Y_LENGTH;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (map[j])
	{
		while (map[j][i])
		{
			if (map[j][i] == '1')
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_Rect cell = {cell_size_x * i + 1, cell_size_y * j + 1, cell_size_x - 1, cell_size_y - 1};
				SDL_RenderFillRect(renderer, &cell);
			}
			else if (map[j][i] == '0')
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_Rect cell = {cell_size_x * i + 1, cell_size_y * j + 1, cell_size_x - 1, cell_size_y - 1};
				SDL_RenderFillRect(renderer, &cell);
			}
			i++;
		}
		i = 0;
		j++;
	}
}

static void	draw_grid(SDL_Renderer *renderer)
{
	int	x_start;
	int	y_start;

	x_start = WINDOW_WIDTH / X_LENGTH;
	y_start = WINDOW_HEIGHT / Y_LENGTH;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
    for (int x = x_start; x < WINDOW_WIDTH; x += x_start)
	{
        SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_HEIGHT);
    }
    for (int y = y_start; y < WINDOW_HEIGHT; y += y_start)
	{
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }
}

static void	birth_checker(char **map, char **new_map, int j, int i)
{
	int	neighbour;

	neighbour = 0;
	if (j == 0 || j == Y_LENGTH - 1 || i == 0 || i == X_LENGTH - 1)
		return ;
	if (map[j - 1][i - 1] == '1')
		neighbour++;
	if (map[j - 1][i] == '1')
		neighbour++;
	if (map[j - 1][i + 1] == '1')
		neighbour++;
	if (map[j][i - 1] == '1')
		neighbour++;
	if (map[j][i + 1] == '1')
		neighbour++;
	if (map[j + 1][i - 1] == '1')
		neighbour++;
	if (map[j + 1][i] == '1')
		neighbour++;
	if (map[j + 1][i + 1] == '1')
		neighbour++;
	if (neighbour == 3)
		new_map[j][i] = '1';
}

static void	death_checker(char **map, char **new_map, int j, int i)
{
	int	neighbour;

	neighbour = 0;
	if (j == 0 || j == Y_LENGTH - 1 || i == 0 || i == X_LENGTH - 1)
		return ;
	if (map[j - 1][i - 1] == '1')
		neighbour++;
	if (map[j - 1][i] == '1')
		neighbour++;
	if (map[j - 1][i + 1] == '1')
		neighbour++;
	if (map[j][i - 1] == '1')
		neighbour++;
	if (map[j][i + 1] == '1')
		neighbour++;
	if (map[j + 1][i - 1] == '1')
		neighbour++;
	if (map[j + 1][i] == '1')
		neighbour++;
	if (map[j + 1][i + 1] == '1')
		neighbour++;
	if (neighbour == 2 || neighbour == 3)
		new_map[j][i] = '1';
}

static void	compute_next_result(char **map)
{
	int i = 0;
	int j = 0;
	char	**new_map;

	new_map = malloc((sizeof(char *) * Y_LENGTH) + 1);
	for (int j = 0; j < Y_LENGTH; j++)
		new_map[j] = malloc(sizeof(char) * (X_LENGTH + 1));
	while (j < Y_LENGTH)
	{
		while (i < X_LENGTH)
		{
			new_map[j][i] = '0';
			i++;
		}
		new_map[j][i] = '\0';
		i = 0;
		j++;
	}
	new_map[j] = NULL;
	i = 0;
	j = 0;
	while (map[j])
	{
		while (map[j][i])
		{
			if (map[j][i] == '0')
				birth_checker(map, new_map, j, i);
			else if (map[j][i] == '1')
				death_checker(map, new_map, j, i);
			i++;
		}
		i = 0;
		j++;
	}
	i = 0;
	j = 0;
	while (map[j])
	{
		while (map[j][i])
		{
			map[j][i] = new_map[j][i]; 
			i++;
		}
		i = 0;
		j++;
	}
	free_double_array(new_map);
}

int main(void)
{
	char **map = init_map();

	if (!map)
		return (1);

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(1920, 1080, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
	while (1)
	{
		SDL_RenderClear(renderer);
		draw_grid(renderer);
		draw_cells(renderer, map);
		SDL_RenderPresent(renderer);
		compute_next_result(map);
		int i = 0;
		while (map[i])
			printf("%s\n", map[i++]);
		SDL_Delay(100);
	}
	free_double_array(map);
	return (0);
}