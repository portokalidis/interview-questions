/*
 * https://techdevguide.withgoogle.com/resources/former-interview-question-volume-of-lakes/#!
 *Imagine an island that is in the shape of a bar graph. When it rains, certain areas of the island fill up with rainwater to form lakes. Any excess rainwater the island cannot hold in lakes will run off the island to the west or east and drain into the ocean.

Given an array of positive integers representing 2-D bar heights, design an algorithm (or write a function) that can compute the total volume (capacity) of water that could be held in all lakes on such an island given an array of the heights of the bars. Assume an elevation map where the width of each bar is 1.

Example: Given [1,3,2,4,1,3,1,4,5,2,2,1,4,2,2], return 15 (3 bodies of water with volumes of 1,7,7 yields total volume of 15)
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ISLAND_SZ_MAX 100


struct land {
	size_t h;
};

#if 0
static size_t calculate_right(const struct land *island, size_t num, int peak)
{
	if ((peak + 2) >= num)
		return 0;

	size_t max_h = 0, max_i = 0;
	for (int i = peak + 1; i < num; i++) {
		if (island[i].h >= max_h) {
			max_h = island[i].h;
			max_i = i;
		}
	}

	size_t vol = 0;
	for (int i = peak + 1; i < max_i; i++) {
		vol += max_h - island[i].h;
	}
	printf("Calculate right: %d -> %zu: %zu\n", peak, max_i, vol);
	vol += calculate_right(island, num, max_i);
	return vol;
}

static size_t calculate_left(const struct land *island, size_t num, int peak)
{
	if ((peak - 2) < 0)
		return 0;

	size_t max_h = 0, max_i = 0;
	for (int i = peak - 1; i >= 0; i--) {
		if (island[i].h >= max_h) {
			max_h = island[i].h;
			max_i = i;
		}
	}

	size_t vol = 0;
	for (int i = peak - 1; i >= max_i; i--) {
		vol += max_h - island[i].h;
	}
	printf("Calculate left: %zu <- %d: %zu\n", max_i, peak, vol);
	vol += calculate_left(island, num, max_i);
	return vol;
}



static size_t calculate_volume(const struct land *island, size_t num)
{
	if (num == 0)
		return 0;

	/* Find peak */
	size_t max_h = island[0].h;
	int max_i = 0;
	for (int i = 1; i < num; i++) {
		if (island[i].h > max_h) {
			max_h = island[i].h;
			max_i = i;
		}
	}

	size_t vol = calculate_right(island, num, max_i);
	vol += calculate_left(island, num, max_i);

	return vol;
}
#endif

static size_t calculate_volume(const struct land *island, size_t num)
{
	size_t max = 0, max_i = 0, vol = 0;
	size_t w = 0;

	/* right */
	for  (int i = 0; i < num; i++) {
		if (island[i].h >= max) { /* Max height */
			vol += w;
			printf("%d: final add water %zu\n", i, vol);
			w = 0;
			max = island[i].h;
			max_i = i;
		} else {
			w += max - island[i].h;
			printf("%d: tentative add water %zu\n", i, w);
		}
	}

	/* left */
	max = w = 0;
	for (int i = num - 1; i >= max_i; i--) {
		if (island[i].h > max) {
			vol += w;
			w = 0;
			max = island[i].h;
		} else {
			w += max - island[i].h;
		}
	}

	return vol;
}

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stderr, "missing argument");
		return EXIT_FAILURE;
	}

	struct land island[ISLAND_SZ_MAX];

	/* load values */
	long h;
	char delim[] = ",";
	char *tol_end;
	char *token, *input = argv[1];
	size_t hnum = 0;
	for (;(token = strsep(&input, delim)) != NULL; hnum++) {
		if (*token == '\0')
			continue;
		h = strtol(token, &tol_end, 10);
		if (h < 0 || *tol_end != '\0') {
			fprintf(stderr, "illegal height %s\n", token);
			return EXIT_FAILURE;
		}
		assert(hnum < ISLAND_SZ_MAX);
		island[hnum].h = h;
		printf("%zu: %zu\n", hnum, island[hnum].h);
	}
	
	size_t volume = calculate_volume(island, hnum);
	printf("Volume %zu\n", volume);

	return EXIT_SUCCESS;
}

