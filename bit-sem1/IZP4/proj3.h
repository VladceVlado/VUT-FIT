// VUT FIT
// IZP 2016/2017
// Projekt 3 dokumentace
// Soubor: proj3.h
// Autor: Vladimir Dusek, xdusek27 (1BIT)
// Datum: 12. 12. 2016

/**
 * @file
 */

/**
 * Author:  Vladimir Dusek (xdusek27)
 * Project: Simple cluster analysis
 * Description:   https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt3&csid=623120&id=11499 
 */

/**
 * @defgroup Structures
 * Declaration structures for objects and clusters
 * @{
 */

/**
 * @brief Structure object
 */
struct obj_t {
    /** Unique identification number */
    int id;
    /** Coordinate x */
    float x; 
    /** Coordinate y */
    float y;   
};

/**
 * @brief Structure cluster
 */
struct cluster_t {
    /** Number of objects in the cluster */   
    int size;   
    /** Number of maximum objects that can be in the cluster */
    int capacity; 
    /** Pointer to the array of objects in the cluster */
    struct obj_t *obj; 
    };

/** @} */

/**
 * @defgroup Constants
 * @{
 */

/**
 * @brief Cluster chunk
 *
 * Constant for expand capacity of a cluster
 */
extern const int CLUSTER_CHUNK;

/** @} */

/**
 * @defgroup Functions
 * @{
 */

/**  
 * @brief Allocation memory for objects capacity of the cluster
 *
 * @param c Pointer to the cluster
 * @param cap Capacity of objects of the cluster
 *
 * @pre c points to a cluster and cap is greater than 0
 * @post c->obj points to allocated memory and capacity is set to cap
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Free memory after objects of the cluster, set size and capacity of the cluster to 0
 *
 * @param c Pointer to the cluster
 *
 * @pre Pointer c has to point to the cluster
 * @post Allocated memory for objects is freed and capacity is set to 0
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief Set capacity of the cluster to the new capacity
 *
 * @param c Pointer to the cluster
 * @param new_cap New capacity of the cluster
 *
 * @pre Pointer c has to point to the cluster
 * @post Capacity of cluster is set on a new size
 *
 * @return Pointer to a new cluster with new capacity
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Add object to the end of the array objects in the cluster
 *
 * @param c Pointer to the cluster
 * @param obj New object
 *
 * @pre Pointer c has to point to the cluster
 * @post Object is added to the end of the array objects in the cluster
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Merges two clusters, add all objects of the cluster to another cluster
 *
 * @param c1 Pointer to the cluster where are inserted all objects of another cluster
 * @param c2 Pointer to cluster from where are objects took
 *
 * @pre Pointers c1 and c2 have to point to clusters
 * @post In the cluster c1 are all objects from c2
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Delete cluster from array of clusters
 *
 * @param carr Pointer to the array of clusters
 * @param narr Size of the array
 * @param idx Index of cluster that should be remove
 *
 * @pre Pointer carr has to point to the array of clusters
 * @post Cluster with index idx is removed from the array
 *
 * @return New size of the array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Calculate distance between two objects
 *
 * @param o1 Pointer to the the first object
 * @param o2 Pointer the the second object
 *
 * @pre o1 and o2 have to point to objects
 * @post Distance between two objects is calculated
 *
 * @return Distance between o1 and o2
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Calculate distance between two clusters by method "The furthest neighbour"
 *
 * @param c1 Pointer to the the first cluster
 * @param c2 Pointer to the the second cluster
 *
 * @pre c1 and c2 have to point to clusters which contain more than 0 objects
 * @post Distance between two clusters is calculated
 *
 * @return Distance between c1 and c2
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Find the closest neighbours
 *
 * @param carr Pointer to the array of clusters
 * @param narr Size of the array
 * @param c1 Pointer to the index of the first cluster
 * @param c2 Pointer to the index of the second cluster
 *
 * @pre carr has to point to the array of clusters
 * @post On c1 and c2 are saved indexes of the closest neighbours
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Sort clusters in the array
 *
 * @param c Pointer to the array of clusters
 *
 * @pre c has to point to the array of clusters
 * @post Objects in cluster are sorted by ID
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Print cluster with its objects
 *
 * @param c Pointer the cluster
 *
 * @pre c has to point to the cluster
 * @post All objects in cluster are printed
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Load objects from a input file and assign every of them one cluster
 *
 * @param filename Name of the input file
 * @param arr Pointer to the beginning array clusters
 *
 * @pre arr has to point to the beginning array clusters and file can be open
 * @post Data from input file are loaded, in every cluster is one object
 *
 * @return Number of loaded clusters, in every cluster is one object
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Print clusters with all objects
 *
 * @param carr Pointer to the beginning array of clusters
 * @param narr Number of clusters to print
 *
 * @pre carr has to point to the beginning array of clusters
 * @post narr clusters are printed
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @brief Free all memory
 *
 * @param clusters Pointer to the beginning array of clusters
 * @param n Size of the array
 *
 * @pre clusters has to point to the beginning array of clusters
 * @post All memory if freed
 */
void free_all(struct cluster_t *clusters, int n);

/** @} */
