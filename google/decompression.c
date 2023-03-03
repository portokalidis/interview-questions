/*
 * In this exercise, you're going to decompress a compressed string.

Your input is a compressed string of the format number[string] and the decompressed output form should be the string written number times. For example:

The input

3[abc]4[ab]c

Would be output as

abcabcabcababababc

Other rules
Number can have more than one digit. For example, 10[a] is allowed, and just means aaaaaaaaaa

One repetition can occur inside another. For example, 2[3[a]b] decompresses into aaabaaab

Characters allowed as input include digits, small English letters and brackets [ ].

Digits are only to represent amount of repetitions.

Letters are just letters.

Brackets are only part of syntax of writing repeated substring.

Input is always valid, so no need to check its validity.

Learning objectives
This question gives you the chance to practice with strings, recursion, algorithm, compilers, automata, and loops. It’s also an opportunity to work on coding with better efficiency.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#define BLOCKSZ 4096
#define REP_STR_SZ_MAX 32


struct decompressed_data {
	size_t ddata_sz, ddata_cur;
	char ddata[1];
};

static struct decompressed_data *ddata_create(void)
{
	struct decompressed_data *d;
	d = malloc(sizeof(*d) + BLOCKSZ - 1);
	assert(d);
	d->ddata_sz = BLOCKSZ;
	d->ddata_cur = 0;
	d->ddata[0] = '\0';
	return d;
}

static inline void ddata_destroy(struct decompressed_data *d)
{
	free(d);
}

static inline struct decompressed_data *ddata_append_c(struct decompressed_data *d,
		char c)
{
	/* Enlarge storage, if needed */
	if (d->ddata_cur >= d->ddata_sz) { 
		size_t newsz = d->ddata_sz + BLOCKSZ;
		d = realloc(d->ddata, newsz);
		assert(d);
		d->ddata_sz = newsz;
	}

	d->ddata[d->ddata_cur++] = c;
	return d;
}

static struct decompressed_data *ddata_append_rep(struct decompressed_data *d,
		unsigned long rep, const struct decompressed_data *newdd)
{
	if (rep == 0)
		return d;

	size_t avail = d->ddata_sz - d->ddata_cur;
	size_t needed = rep * (newdd->ddata_cur - 1);

	if (avail < needed) {
		size_t newsz = d->ddata_sz + needed - avail + BLOCKSZ;
		d = realloc(d->ddata, newsz);
		assert(d);
		d->ddata_sz = newsz;
	}

	//printf("pre append: %ld %s\n", d->ddata_cur, d->ddata);
	while (rep--) {
		memcpy(&d->ddata[d->ddata_cur], newdd->ddata, newdd->ddata_cur - 1);
		d->ddata_cur += newdd->ddata_cur - 1;
	}

	//printf("post append: %ld %s\n", d->ddata_cur, d->ddata);
	return d;
}


static struct decompressed_data *decompress_internal(const char **cdata)
{
	struct decompressed_data *dd, *subdd;

	dd = ddata_create();

	char rep_str[REP_STR_SZ_MAX], *rep_str_end;
	unsigned rep_str_cur = 0;
	char c;
	long rep;
	const char *cd = *cdata;
	while ((c = *cd) != '\0') {
		//printf("%c\n", c);
		if (isdigit(c)) {
			rep_str[rep_str_cur++] = c;
			if (rep_str_cur >= (sizeof(rep_str) - 1))
				goto err;
			cd++;
		} else if (c == '[') {
			if (rep_str_cur == 0)
				goto err;
			rep_str[rep_str_cur] = '\0';
			rep = strtol(rep_str, &rep_str_end, 10);
			if (*rep_str_end != '\0')
				goto err;
			rep_str_cur = 0;
			cd++;
			subdd = decompress_internal(&cd);
			if (!subdd)
				goto err;
			//printf("rep: %ld x %s\n", rep, subdd->ddata);
			dd = ddata_append_rep(dd, rep, subdd);
			ddata_destroy(subdd);
		} else if (c == ']') {
			//printf("returning %s\n", dd->ddata);
			cd++;
			goto ret;
		} else if (c >= 'a' && c <= 'z') { /* letter */
			dd = ddata_append_c(dd, c);
			cd++;
		} else {
			goto err;
		}
	}

ret:
	*cdata = cd;
	ddata_append_c(dd, '\0');
	return dd;
err:
	ddata_destroy(dd);
	return NULL;
}


static inline struct decompressed_data *decompress(const char *cdata)
{
	const char *cd = cdata;
	return decompress_internal(&cd);
}

int main(int argc, char **argv)
{
	struct decompressed_data *decompressed;

	if (argc <= 1) {
		fprintf(stderr, "missing argument");
		return EXIT_FAILURE;
	}

	printf("Original: %s\n", argv[1]);
	decompressed = decompress(argv[1]);
	if (!decompressed) {
		fprintf(stderr, "decompression error\n");
		return EXIT_FAILURE;
	}
	printf("Decompressed: %s\n", decompressed->ddata);
	ddata_destroy(decompressed);

	return EXIT_SUCCESS;
}
