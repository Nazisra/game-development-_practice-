/* SPDX-License-Identifier: MIT */

// Declarations manipulating symbols
#ifndef RGBDS_LINK_SECTION_H
#define RGBDS_LINK_SECTION_H

// GUIDELINE: external code MUST NOT BE AWARE of the data structure used!

#include <stdint.h>
#include <vector>

#include "link/main.hpp"

#include "linkdefs.hpp"

struct FileStackNode;
struct Section;

struct Patch {
	struct FileStackNode const *src;
	uint32_t lineNo;
	uint32_t offset;
	uint32_t pcSectionID;
	uint32_t pcOffset;
	enum PatchType type;
	std::vector<uint8_t> rpnExpression;

	struct Section const *pcSection;
};

struct Section {
	// Info contained in the object files
	char *name;
	uint16_t size;
	uint16_t offset;
	enum SectionType type;
	enum SectionModifier modifier;
	bool isAddressFixed;
	// This `struct`'s address in ROM.
	// Importantly for fragments, this does not include `offset`!
	uint16_t org;
	bool isBankFixed;
	uint32_t bank;
	bool isAlignFixed;
	uint16_t alignMask;
	uint16_t alignOfs;
	uint8_t *data; // Array of size `size`
	std::vector<struct Patch> *patches;
	// Extra info computed during linking
	std::vector<struct Symbol *> *fileSymbols;
	uint32_t nbSymbols;
	struct Symbol **symbols;
	struct Section *nextu; // The next "component" of this unionized sect
};

/*
 * Execute a callback for each section currently registered.
 * This is to avoid exposing the data structure in which sections are stored.
 * @param callback The function to call for each structure.
 */
void sect_ForEach(void (*callback)(struct Section *));

/*
 * Registers a section to be processed.
 * @param section The section to register.
 */
void sect_AddSection(struct Section *section);

/*
 * Finds a section by its name.
 * @param name The name of the section to look for
 * @return A pointer to the section, or NULL if it wasn't found
 */
struct Section *sect_GetSection(char const *name);

/*
 * `free`s all section memory that was allocated.
 */
void sect_CleanupSections(void);

/*
 * Checks if all sections meet reasonable criteria, such as max size
 */
void sect_DoSanityChecks(void);

#endif // RGBDS_LINK_SECTION_H
