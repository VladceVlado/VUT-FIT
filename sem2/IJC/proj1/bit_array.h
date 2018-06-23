/*
 * Soubor:      bit_array.h
 * Projekt:     IJC-DU1, priklad a)
 * Autor:       Vladimir Dusek, xdusek27
 * Skola:       VUT FIT 1BIT
 * Datum:       25/3/2017
 * Prekladac:   gcc 5.4.0
 * Popis:       bitove pole a prace s nim
 */


/* Podmineny preklad _BIT_ARRAY_H */
#ifndef _BIT_ARRAY_H
#define _BIT_ARRAY_H


#include <stdio.h>
#include <limits.h>
#include "error.h"


/* Definice datoveho typu pole bitu */
typedef unsigned long bit_array_t[];


/******************** POMOCNA MAKRA ********************/

/*
 * Vypocita pocet bitu v unsigned long
 */
#define ULONG_BITS (CHAR_BIT * sizeof(unsigned long))

/*
 * Vypocita pocet bitu prvku pole libovolneho typu
 */
#define elem_bits(array_name) (CHAR_BIT * sizeof(array_name[0]))

/*
 * Vypocita pocet unsigned longu nutnych k alokaci pro "aray_size" pocet bitu
 */
#define ba_ulong(array_size) \
(((array_size) % ULONG_BITS != 0) ? ((array_size) / ULONG_BITS + 1) : ((array_size) / ULONG_BITS))


/******************** POVINNA POMOCNA MAKRA ********************/

/*
 * V poli "array_name" ziska hodnotu bitu na indexu "index",
 * vrati jeho hodnotu (0 nebo 1)
 * Funguje pro libovolny datovy typ pole
 */
#define BA_GET_BIT_(array_name, index) \
((array_name[1 + (index) / elem_bits(array_name)] >> ((index) % elem_bits(array_name))) & (1UL))

/*
 * V poli "array_name" nastavi bit na indexu "index" na
 * hodnotu zadanou vyrazem "expression"
 * Funguje pro libovolny datovy typ pole
 */
#define BA_SET_BIT_(array_name, index, expression) \
((expression) ? (array_name[1 + (index) / elem_bits(array_name)] |= (1UL << (index) % elem_bits(array_name))) \
: (array_name[1 + (index) / elem_bits(array_name)] &= ~(1UL << (index) % elem_bits(array_name))))


/******************** HLAVNI MAKRA ********************/

/*
 * Definuje bitove pole "array_name" o velikosti "array_size",
 * do prvniho ulongu ulozi velikost pole a vsechny dalsi
 * bity (prvky pole) nastavi na hodnotu 0
 */
#define ba_create(array_name, array_size) \
unsigned long array_name[1 + ba_ulong(array_size)] = {(unsigned long)(array_size)}

/* Nedefinovano USE_INLINE - verze pouze s makrama */
#ifndef USE_INLINE

/*
 * Vrati velikost pole "array_name", tedy hodnotu na nultem indexu,
 * kde je velikost ulozena
 */
#define ba_size(array_name) (array_name[0])

/*
 * Zkontroluje meze pole a potom zavola "BA_SET_BIT_" se stejnymi argumenty
 */
#define ba_set_bit(array_name, index, expression) (((index) > ba_size(array_name)) ? \
(error_msg("Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) ba_size(array_name) - 1), 666UL) \
: BA_SET_BIT_(array_name, index, expression))

/*
 * Zkontroluje meze pole a potom zavola "BA_GET_BIT_" se stejnymi argumenty
 */
#define ba_get_bit(array_name, index) (((index) > ba_size(array_name)) ? \
(error_msg("Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) ba_size(array_name) - 1), 666UL) \
: BA_GET_BIT_(array_name, index))


/******************** INLINE FUNKCE ********************/

/* Definovano USE_INLINE - verze s inline funkcemi */
#else

/*
 * Definuje bitove pole "array_name" o velikosti "array_size",
 * do prvniho ulongu ulozi velikost pole a vsechny dalsi
 * bity (prvky pole) nastavi na hodnotu 0
 * NELZE NAPSAT INLINE FUNKCI, MUSI SE POUZIT MAKRO
 */
//inline void ba_create(bit_array_t array_name, unsigned long array_size);

/*
 * Vrati velikost pole "array_name", tedy hodnotu na nultem indexu,
 * kde je velikost ulozena
 */
inline unsigned long ba_size(bit_array_t array_name)
{
	return (array_name[0]);
}

/*
 * V poli "array_name" nastavi bit na indexu "index" na
 * hodnotu zadanou vyrazem "expression"
 */
inline void ba_set_bit(bit_array_t array_name, unsigned long index, int expression)
{
	if (index > array_name[0])
		error_msg("Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) ba_size(array_name) - 1);

	BA_SET_BIT_(array_name, index, expression);
}

/*
 * V poli "array_name" ziska hodnotu bitu na indexu "index",
 * vrati jeho hodnotu (0 nebo 1)
 */
inline int ba_get_bit(bit_array_t array_name, unsigned long index)
{
	if (index > array_name[0])
		error_msg("Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, (unsigned long) ba_size(array_name) - 1);

	return BA_GET_BIT_(array_name, index);
}


/* Konec USE_INLINE */
#endif

/* Konec podmineneho prekladu _BIT_ARRAY_H */
#endif






