#ifndef __TIBIASTRUCT_H__
#define __TIBIASTRUCT_H__

/*
Tibia Structures
by Evremonde
for Tibia 8.0
*/

/* structures */

struct Item
{
	int id;
	int count;
	int location;
	int container;
	std::string containerName;
	bool found;
};

#endif // #ifndef __TIBIASTRUCT_H__
