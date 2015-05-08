#ifndef BattleScenario_H
#define BattleScenario_H

#include <string>
#include <vector>
#include "Graphics/Image.h"
#include "BattleCommander.h"

class BattleScript;
class BattleSimulator;
class GroundMap;


class BattleScenario
{
	BattleSimulator* _simulator{};
	BattleScript* _script{};
	std::vector<BattleCommander*> _commanders{};
	BattleCommander* _dummyCommander{};
	std::string _legacyMapId{};
	int _teamPosition1{};
	int _teamPosition2{};
	float _deploymentTimer{};
	bool _deploymentEnabled{};

public:
	BattleScenario();
	~BattleScenario();

	BattleSimulator* GetSimulator() const { return _simulator; }

	void SetScript(BattleScript* value);
	BattleScript* GetScript() const;

	void SetTeamPosition(int team, int position);
	int GetTeamPosition(int team) const;

	BattleCommander* AddCommander(const char* playerId, int team, BattleCommanderType type);
	BattleCommander* GetCommander(const char* playerId) const;
	const std::vector<BattleCommander*>& GetCommanders() const { return _commanders; }
	BattleCommander* GetDummyCommander() const;

	void SetGroundMap(GroundMap* groundMap);
	void LoadLegacySmoothMap(const char* path, const char* legacyMapId, float size);
	void LoadLegacyRandomMap();
	void SetTiledMap(int x, int y);

	const char* GetLegacyMapId() const;

	void Tick(double secondsSinceLastTick);

	void EnableDeploymentZones(float deploymentTimer);
	void UpdateDeploymentZones(double secondsSinceLastTick);
};


#endif
