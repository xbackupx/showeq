/*
 * spawnshell.h
 *
 * ShowEQ Distributed under GPL
 * http://sourceforge.net/projects/seq/
 *
 * Portions Copyright 2001-2003 Zaphod (dohpaz@users.sourceforge.net).
 *
 */

/*
 * Adapted from spawnlist.h - Crazy Joe Divola (cjd1@users.sourceforge.net)
 * Date   - 7/31/2001
 */

// Major rework of SpawnShell{} classes - Zaphod (dohpaz@users.sourceforge.net)
//   Based on stuff from CJD's SpawnShell{} and stuff from SpawnList and
//   from Map.  Some optimization ideas adapted from SINS.

#ifndef SPAWNSHELL_H
#define SPAWNSHELL_H

#include "everquest.h"
#include "spawn.h"
#include "spawnmodel.h"

//----------------------------------------------------------------------
// forward declarations
class Player;
class ZoneMgr;
class FilterMgr;
class SpawnShell;
class GuildMgr;

//----------------------------------------------------------------------
// constants

// The maximum number of ID's of deleted spawns to track
const int MAX_DEAD_SPAWNIDS = 50;

//----------------------------------------------------------------------
// enumerated types

//----------------------------------------------------------------------
// type definitions
typedef QHash<uint32_t, Item*> ItemMap;
typedef QHash<uint32_t, Item*>::iterator ItemIterator;
typedef QHash<uint32_t, Item*>::const_iterator ItemConstIterator;

//----------------------------------------------------------------------
// SpawnShell
class SpawnShell : public QObject
{
	Q_OBJECT

public:
	SpawnShell(FilterMgr& filterMgr, ZoneMgr* zoneMgr, Player* player, GuildMgr* guildMgr);

	const Item* findID(spawnItemType type, int idSpawn);
	const Item* findClosestItem(spawnItemType type, int16_t x, int16_t y, double& minDistance);
	Spawn* findSpawnByName(const QString& name);

	void dumpSpawns(spawnItemType type, QTextStream& out);
	FilterMgr* filterMgr() { return &m_filterMgr; }
	const ItemMap& getConstMap(spawnItemType type) const;
	const ItemMap& spawns() const;
	const ItemMap& drops() const;
	const ItemMap& doors() const;

	SpawnModel* spawnModel() { return m_spawnModel; }

signals:
	void addItem(const Item* item);
	void delItem(const Item* item);
	void changeItem(const Item* item, uint32_t changeType);
	void killSpawn(const Item* deceased, const Item* killer, uint16_t killerId);
	void selectSpawn(const Item* item);
	void spawnConsidered(const Item* item);
	void clearItems();
	void numSpawns(int);

public slots:
	void clear();

	// slots to receive from EQPacket...
	void newGroundItem(const uint8_t*, size_t, uint8_t);
	void removeGroundItem(const uint8_t*, size_t, uint8_t);
	void newDoorSpawns(const uint8_t*, size_t, uint8_t);
	void newDoorSpawn(const doorStruct&, size_t, uint8_t);
	void zoneSpawns(const uint8_t* zspawns, size_t len);
	void zoneEntry(const uint8_t* spawn, size_t len);
	void newSpawn(const uint8_t* spawn);
	void newSpawn(const spawnStruct& s);
	void playerUpdate(const uint8_t*pupdate, size_t, uint8_t);
	void npcMoveUpdate(const uint8_t*npcupdate, size_t, uint8_t);
	bool updateSpawn(uint16_t id, Point3D<int16_t> pos, Point3D<int16_t> vel,
					 int8_t heading, int8_t deltaHeading, uint8_t animation);
	void updateSpawns(const uint8_t* updates);
	void updateSpawnInfo(const uint8_t* spawnupdate);
	void renameSpawn(const uint8_t* renameupdate);
	void illusionSpawn(const uint8_t* illusionupdate);
	void updateSpawnAppearance(const uint8_t* appearanceupdate);
	void shroudSpawn(const uint8_t* spawnupdate, size_t, uint8_t);
	void updateNpcHP(const uint8_t* hpupdate);
	void spawnWearingUpdate(const uint8_t* wearing);
	void consMessage(const uint8_t* con, size_t, uint8_t);
	void removeSpawn(const uint8_t* rmSpawn, size_t len, uint8_t dir);
	void deleteSpawn(const uint8_t* delSpawn);
	void killSpawn(const uint8_t* deadspawn);
	void respawnFromHover(const uint8_t* respawn, size_t len, uint8_t dir);
	void corpseLoc(const uint8_t* corpseLoc);
	bool updateIndividualSpawn(const spawnPositionUpdate* update)
	{
		return updateSpawn(update->spawnId, Point3D<int16_t>(update->x >> 3, update->y >> 3, update->z >> 3),
			Point3D<int16_t>(0, 0, 0), update->heading, 0, 0);
	}

	void playerChangedID(uint16_t playerID);
	void refilterSpawns();
	void refilterSpawnsRuntime();
	void saveSpawns();
	void restoreSpawns();

protected:
	void refilterSpawns(spawnItemType type);
	void refilterSpawnsRuntime(spawnItemType type);
	void deleteItem(spawnItemType type, int id);
	bool updateFilterFlags(Item* item);
	bool updateRuntimeFilterFlags(Item* item);
	int32_t fillSpawnStruct(spawnStruct *spawn, const uint8_t *data, size_t len, bool checkLen);

	ItemMap& getMap(spawnItemType type);

private:
	ZoneMgr* m_zoneMgr;
	Player* m_player;
	FilterMgr& m_filterMgr;
	GuildMgr* m_guildMgr;

	// track recently killed spawns
	uint16_t m_deadSpawnID[MAX_DEAD_SPAWNIDS];
	uint8_t m_cntDeadSpawnIDs;
	uint8_t m_posDeadSpawnIDs;

	// maps to keep track of the different types of spawns
	ItemMap m_spawns;
	ItemMap m_drops;
	ItemMap m_doors;
	ItemMap m_players;

	// timer for saving spawns
	QTimer* m_timer;

	SpawnModel* m_spawnModel;
};

inline const ItemMap& SpawnShell::getConstMap(spawnItemType type) const
{
	switch (type)
	{
		case tSpawn:
			return m_spawns;
		case tDrop:
			return m_drops;
		case tDoors:
			return m_doors;
		case tPlayer:
			return m_players;
		default:
			return m_spawns;
	}
}

inline ItemMap& SpawnShell::getMap(spawnItemType type)
{
	switch (type)
	{
		case tSpawn:
			return m_spawns;
		case tDrop:
			return m_drops;
		case tDoors:
			return m_doors;
		case tPlayer:
			return m_players;
		default:
			return m_spawns;
	}
}

inline const ItemMap& SpawnShell::spawns() const
{
	return m_spawns;
}

inline const ItemMap& SpawnShell::drops() const
{
	return m_drops;
}

inline const ItemMap& SpawnShell::doors() const
{
	return m_doors;
}

//--------------------------------------------------

#endif // SPAWNSHELL_H
