// VUT FIT
// IZP 2016/2017
// Projekt 3 - Jednoducha shlukova analyza
// Soubor: proj3.c
// Autor: Vladimir Dusek, xdusek27 (1BIT)
// Datum: 11. 12. 2016

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */

#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
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

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 * Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 * Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->obj = malloc(cap * sizeof(struct obj_t));
    c->capacity = cap;
}

/*
 * Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
*/
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    c->size = 0;
    c->capacity = 0;
}

/*
 * Chunk of cluster objects. Value recommended for reallocation.
*/
const int CLUSTER_CHUNK = 10;

/*
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * TUTO FUNKCI NEMENTE
*/
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 * Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 * nevejde.
*/
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->size == c->capacity)   // kdyz je velikost shluku rovna kapacite, je potreba kapacitu navysit
    {
        if (resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL)
            fprintf(stderr, "Chyba! Neuspesna zmena kapacity clusteru.\n");
    }

    c->obj[c->size] = obj;   // pridani objektu na konec pole objektu
    c->size++;   // byl pridan objekt -> zvetsila se velikost
}

/*
 * Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
*/
void sort_cluster(struct cluster_t *c);

/*
 * Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 * Shluk 'c2' bude nezmenen.
*/
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++)   // vsechny objekty ze shluku c2
    {
        if (c1->size == c1->capacity)   // kdyz je velikost shluku rovna kapacite, je potreba kapacitu navysit
            resize_cluster(c1, c1->capacity + CLUSTER_CHUNK);

        append_cluster(c1, c2->obj[i]);   // pridat do shluku c1 objekt s indexem [i] z c2
    }

    sort_cluster(c1);   // seradit objekty ve shluku c1
}

/****************************** Prace s polem shluku ******************************/

/*
 * Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 * (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 * pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    free(carr[idx].obj);   // uvolneni pameti po poli objektu v danem clusteru
    carr[idx] = carr[narr - 1];   // na indexu shluku ktery ma byt odstranen ulozi posledni shluk v poli

    return narr - 1;   // jeden shluk jsem odstranil, vratim tedy velikost pole - 1
}

/*
 * Pocita Euklidovskou vzdalenost mezi dvema objekty.
*/
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    /*   |distance| = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))^1/2   */
    float obj_distance = sqrtf((o1->x - o2->x) * (o1->x - o2->x) + (o1->y - o2->y) * (o1->y - o2->y));

    return obj_distance;
}

/*
 * Pocita vzdalenost dvou shluku.
 * Pomoci metody nejvzdalenejsiho souseda
 *    Znamena: vzdalenost dvou shluku je definovana jako vzdalenost jejich nejvzdalenejsich objektu
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float dist_fin = 0;   // cluster distance final
    float dist_act = 0;   // cluster distance actual

    for (int i = 0; i < c1->size; i++)   // porovnat vsechny objekty z prvniho shluku se vsemi objekty z druheho shluku
    {
        for (int j = 0; j < c2->size; j++)
        {
            dist_act = obj_distance(&c1->obj[i], &c2->obj[j]);   // vzdalenost dvou aktualnich objektu
            /* kdyz je vzdalenost dvou aktualnich objektu vetsi nez hodnota v promene vzdalenost shluku, ulozi ji jako novou vzdalenost shluku */
            dist_fin = (dist_act > dist_fin) ? dist_act : dist_fin;
        }
    }

    return dist_fin;
}

/*
 * Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 * adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float dist_fin = INFINITY;   // cluster distance final
    float dist_act = 0;   // cluster distance actual

    for (int i = 0; i < narr; i++)   // porovna kazdy shluk s kazdym shlukem
    {
        for (int j = 0; j < narr; j++)
        {
            if (i != j)   // krome toho sameho
            {
                dist_act = cluster_distance(&carr[i], &carr[j]);   // vypocita vzdalenost aktualnich shluku

                if (dist_act < dist_fin)   // kdyz je mensi nez zatim nejmensi nalezena ulozi ji a zapamatuje indexy
                {
                    dist_fin = dist_act;
                    *c1 = i;
                    *c2 = j;
                }
            }
        }
    }
}

/*
 * Pomocna funkce pro razeni objektu ve shluku.
 * TUTO FUNKCI NEMENTE!
*/
static int obj_sort_compar(const void *a, const void *b)
{
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id)
        return -1;
    if (o1->id > o2->id)
        return 1;
    return 0;
}

/*
 * Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 * TUTO FUNKCI NEMENTE!
*/
void sort_cluster(struct cluster_t *c)
{
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 * Tisk shluku 'c' na stdout.
 * TUTO FUNKCI NEMENTE!
*/
void print_cluster(struct cluster_t *c)
{
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

void free_all(struct cluster_t *clusters, int n)
{
    for (int i = 0; i < n; i++)
        clear_cluster(&clusters[i]);   // uvolni pamet po objektech v kazdem finalnim shluku

    free(clusters);   // uvolni pamet po poli shlucich
}

/*
 * Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 * jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 * polozku pole (ukazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 * kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 * V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    FILE *fr;
    int n_cls_def; // pocet nactenych objektu = pocet pocatecnich clusteru

    if ((fr = fopen(filename, "r")) == NULL)   // soubor se nepodari otevrit -> chybove hlaseni
    {
        fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo otevrit.\n", filename);
        return -1;
    }

    if (fscanf(fr, "count=%d", &n_cls_def) != 1)   // spatne nacteni poctu objektu -> chybove hlaseni
    {
        fprintf(stderr, "Chyba! Vstupni soubor %s neni ve spravnem formatu.\n", filename);
        if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
            fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
        return -1;
    }

    int id;
    float x, y;
    int load_lines = 0;
    char c;

    /* alokace pameti pro pole clusteru, v tuto chvili v kazdym clusteru bude 1 objekt */
    *arr = (struct cluster_t *) malloc(n_cls_def * sizeof(struct cluster_t));

    if (*arr == NULL)   // alokace pameti se nezdari -> chybove hlaseni
    {
        fprintf(stderr, "Chyba! Nepodarilo se alokovat pamet pro pole clusteru.\n");
        if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
            fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
        free_all(*arr, load_lines);   // uvolneni pameti
        return -1;
    }

    for (int i = 0; i < n_cls_def; i++)
    {
        /* alokace pameti pro objecty v clusteru*/
        init_cluster(&(*arr)[i], 1);
        (*arr)[i].size = 1;   // nastaveni pocatecni velikosti shluku (v kazdem shluku 1 objekt)
        load_lines++;   // promena kvuli kontrole jedinecnosti ID

        if ((*arr)[i].obj == NULL)   // alokace pameti se nezdari -> chybove hlaseni
        {
            fprintf(stderr, "Chyba! Nepodarilo se alokovat pamet pro object.");
            if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
                fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
            free_all(*arr, load_lines);   // uvolneni pameti
            return -1;
        }

        c = getc(fr);
        if (c != '\n' && i != n_cls_def - 1)   // neplatny format vstupniho souboru -> chybove hlaseni
        {
            fprintf(stderr, "Chyba! Vstupni soubor je ve spatnem formatu.");
            if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
                    fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
            free_all(*arr, load_lines);   // uvolneni pameti
            return -1;
        }

        if (fscanf(fr ,"%d %f %f", &id, &x, &y) == 3)
        {
            if (x < 0 || y < 0 || x > 1000 || y > 1000 || x - (int)x != 0 || y - (int)y != 0)   // souradnice mimo povolenou mez -> chybove hlaseni
            {
                fprintf(stderr, "Chyba! Neplatne souradnice objektu.");
                if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
                    fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
                free_all(*arr, load_lines);   // uvolneni pameti
                return -1;
            }

            for (int j = 0; j < load_lines - 1; j++)   // objekty se stejnym ID -> chybove hlaseni
            {
                if ((*arr)[j].obj->id == id)
                {
                    fprintf(stderr, "Chyba! Vyskytuji se objekty se stejnym ID.");
                    if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
                        fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
                    free_all(*arr, load_lines);   // uvolneni pameti
                    return -1;
                }
            }

            (*arr)[i].obj->id = id;   // prirazeni ID objektu
            (*arr)[i].obj->x = x;   // prirazeni X souradnice objektu
            (*arr)[i].obj->y = y;   // prirazeni Y souradnice objektu

        }
        else   // vstupni soubor v chybnem formatu -> chybove hlaseni
        {
            fprintf(stderr, "Chyba! Vstupni soubor %s neni ve spravnem formatu.\n", filename);
            if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
                fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
            free_all(*arr, load_lines);   // uvolneni pameti
            return -1;
        }
    }

    if (fclose(fr) == EOF)   // soubor se nepodari zavrit -> chybove hlaseni
    {
        fprintf(stderr, "Chyba! Vstupni soubor %s se nepodarilo zavrit.\n", filename);
        free_all(*arr, load_lines);   // uvolneni pameti
        return -1;
    }

    return load_lines;   // vraci pocet nactenych shluku (v kazdem jeden objekt)
}

/*
 * Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 * Tiskne se prvnich 'narr' shluku.
 * TUTO FUNKCI NEMENTE!
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

/*
 * Program se spousti v nasledujici podobe:
 * ./proj3 SOUBOR [N]
 * SOUBOR je jmeno souboru se vstupnimi daty
 * N je volitelny argument definujici cilovy pocet shluku, N > 0, pri absenci argumentu je 1
*/
int main(int argc, char *argv[])
{
    struct cluster_t *clusters;   // pole shluku
    int n_cls_final;   // argument N, pocet vyslednych shluku
    char filename[101];   // pole pro nazev vstupniho souboru
    int n_cls;   // pocet shluku na zacatku, v kazdem shluku se nachazi 1 objekt

    if (argc > 1 && argc < 4)   // pocet argumentu je v poradku
    {
        if (strlen(argv[1]) > 100)   // nazev souboru se nevejde do pole -> chybove hlaseni
        {
            fprintf(stderr, "Chyba! Vstupni soubor ma prilis dlouhy nazev.\n");
            return 1;
        }
        else
            strcpy(filename, argv[1]);   // ulozim nazev souboru do pole

        if (argc == 2)   // kdyz neni zadan argument N, priradi se mu hodnota 1
            n_cls_final = 1;

        if (argc == 3)   // kdyz je argument N zadan pretypuje se na int a ulozi do promene
        {
            char *p_n_cls_final = argv[2];
            char *p_error;
            n_cls_final = (int) strtol(p_n_cls_final, &p_error, 10);

            if (*p_error != '\0' || n_cls_final <= 0)   // pretypovani neprobehne spravne -> chybove hlaseni
            {
                fprintf(stderr, "Chyba! Nespravne zadani argumentu N. Argument N je druhy argument programu a jedna se o prirozene cislo.\n");
                return 1;
            }
        }
    }
    else   // neplatny pocet argumentu -> chybove hlaseni
    {
        fprintf(stderr, "Chyba! Nespravne spusteni programu. Program spoustejte v nasledujici podobe ./proj3 SOUBOR [N]\n");
        return 1;
    }

    if ((n_cls = load_clusters(filename, &clusters)) < 0)   // chyba pri nacitani objektu ze vstupniho souboru -> chybove hlaseni
        return 1;

    if (n_cls_final > n_cls)   // pocet cilovych shluku je vetsi nez pocet pocatecnich shluku -> chybove hlaseni
    {
        fprintf(stderr, "Chyba! Pocet nactenych objektu je mensi nez pocet vyslednych shluku.\n");
        free_all(clusters, n_cls);   // uvolneni pameti
        return 1;
    }

    int cls1, cls2;

    while (n_cls != n_cls_final)
    {
        find_neighbours(clusters, n_cls, &cls1, &cls2);
        merge_clusters(&clusters[cls1], &clusters[cls2]);
        n_cls = remove_cluster(clusters, n_cls, cls2);
    }

    print_clusters(clusters, n_cls);   // vytisknuti vyslednych clusteru

    free_all(clusters, n_cls);   // uvolneni pameti

    return 0;
}
