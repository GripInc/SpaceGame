#ifndef _SHIP_H_
#define _SHIP_H_

#include "model/Engine.h"
#include "model/Directional.h"
#include "model/DynamicObject.h"
#include "model/deserialized/ShipsSettings.h"
#include "model/InputState.h"
#include "manager/StateManager.h"

class btRigidBody;
class EngineSettings;
class DirectionalSettings;
class HardPoint;
class WeaponSettings;

class Ship : public DynamicObject
{
public:
	Ship()
		: mCurrentRollForce(0),
		mCurrentYawForce(0),
		mCurrentPitchForce(0),
		mEnginePotentialForce(0.f)
	{}

	void initShip(const ShipSettings* _shipSettings);

	virtual void instantiateObject(Ogre::SceneManager* _sceneManager, btDiscreteDynamicsWorld* _dynamicWorld, UniqueId _uniqueId);

	void addEngine(const EngineSettings& _engine);
	void addDirectional(const DirectionalSettings& _directional);
	void addWeapon(const WeaponSettings& _weapon, int _index);
	const WeaponSettings& removeWeapon(int _index);
	
	///Getters
	float getMaxYawRate() const { return mMaxYawRate; }
	float getMaxPitchRate() const { return mMaxPitchRate; }
	float getMaxRollRate() const { return mMaxRollRate; }
	virtual btVector3 getInertia() const
	{
		const ShipSettings* shipSettings = static_cast<const ShipSettings*>(mObjectSettings);
		return shipSettings->mLocalInertia * mDirectional.getInertiaMultiplier();
	}

	btAlignedObjectArray<HardPoint*>& getHardPoints() { return mHardPoints; }

	Engine& getEngine() { return mEngine; }
	Directional& getDirectional() { return mDirectional; }
	
	float mCurrentRollForce;
	float mCurrentYawForce;
	float mCurrentPitchForce;

	float mEnginePotentialForce;

	void updateForces();

	void updateHardPoints(float _deltaTime);

	void destroy();

	UniqueId getUniqueId() const { return mUniqueId; }

	//DEBUG
	const btVector3& getLinearVelocity() const;
	btRigidBody* getRigidBody() { return mRigidBody; }
	float mDebugValue;
	float mEngineRealForce;

	//States management
	virtual void saveState(SectorTick _tick);
	//When set a state, all oldest states are discarded
	virtual void setState(SectorTick _tick);
	//Override a state for a futur rewind to this state
	virtual void overrideSavedState(SectorTick _tick, const ShipState& _shipState);

	//Return true if the check passed without need to fix
	//False if a fix was needed and done
	//virtual bool checkAndFixState(const ShipState& _state);

protected:
	///Ship properties from XML
	float mMaxYawRate;
	float mMaxPitchRate;
	float mMaxRollRate;
	unsigned int mCargoSpace;

	virtual void instantiateObjectParts();

	btAlignedObjectArray<HardPoint*> mHardPoints;

	///Engine
	Engine mEngine;
	///Directional system
	Directional mDirectional;

private:
	StateManager<ShipState> mStateManager;
};

#endif //_SHIP_H_