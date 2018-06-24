// VUT FIT
// IZP 2016/2017
// Projekt 3 dokumentace
// Soubor: proj3-kostra.h
// Autor: Vladimir Dusek, xdusek27 (1BIT)
// Datum: 12. 12. 2016

/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2015/16
 * a pro dokumentaci Javadoc.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

void init_cluster(struct cluster_t *c, int cap);

void clear_cluster(struct cluster_t *c);

extern const int CLUSTER_CHUNK;

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

void append_cluster(struct cluster_t *c, struct obj_t obj);

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

int remove_cluster(struct cluster_t *carr, int narr, int idx);

float obj_distance(struct obj_t *o1, struct obj_t *o2);

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

void sort_cluster(struct cluster_t *c);

void print_cluster(struct cluster_t *c);

int load_clusters(char *filename, struct cluster_t **arr);

void print_clusters(struct cluster_t *carr, int narr);
