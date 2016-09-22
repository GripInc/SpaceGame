#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_

#include "OgreFrameListener.h"
#include "OgreVector3.h"
#include "OgreTimer.h"

#include "OIS.h"

#include <string>

#include "model/PlayerData.h"

#include "network/NetworkService.h"
#include "network/IConnectionReadyListener.h"
#include "controller/ShipInputHandler.h"

class InputController;
class Sector;
class UIController;
class SectorController;
class StationController;
class SectorView;
class BulletDebugDraw;

namespace RakNet
{
	class RakString;
}

namespace Ogre
{
	class RenderWindow;
	class Root;
	class Camera;
	class SceneNode;
}

class GameController : public Ogre::FrameListener, public OIS::KeyListener, public IConnectionReadyListener
{
public:
	GameController()
		: mSectorController(NULL),
		mStationController(NULL),
		mSwitchToSpaceData(NULL),
		mSwitchToStationData(NULL),
		mGameUpdateAccumulator(0.f)
	{}

	~GameController()
	{
	}

	void init(const std::string& _sectorFilePath, Ogre::Root* _root, Ogre::RenderWindow* _renderWindow, Ogre::SceneManager* _sceneManager, NetworkLayer& _networkLayer);
	void startGame(const RakNet::RakString& _data);

	//Prepare switch
	void prepareSwitchToStationMode(const std::string& _stationName);
	void prepareSwitchToInSpaceMode(const Ogre::Vector3& _position, const Ogre::Quaternion& _orientation, const std::string& _sectorName, UniqueId _uniqueId, RakNet::RakNetGUID _rakNetGUID, SectorTick _sectorTick);

	//Connection ready notification
	virtual void notifyIsConnected(bool _value);

	//Received sector update from server
	void receivedSectorState(RakNet::BitStream& _data) const;

protected:
	static const float GAME_UPDATE_RATE;
	float mGameUpdateAccumulator;
	Ogre::Timer mLoopTimer;

	class SwitchToSpaceData
	{
	public:
		SwitchToSpaceData() 
			: mPosition(Ogre::Vector3(0.f,0.f,0.f)),
			mOrientation(Ogre::Quaternion::IDENTITY),
			mSectorTick(0)
		{}

		Ogre::Vector3 mPosition;
		Ogre::Quaternion mOrientation;
		std::string mSectorName;
		UniqueId mUniqueId;
		RakNet::RakNetGUID mRakNetGUID;
		SectorTick mSectorTick;
	};

	class SwitchToStationData
	{
	public:
		std::string mStationName;
	};

	SwitchToSpaceData* mSwitchToSpaceData;
	SwitchToStationData* mSwitchToStationData;

	//
	void switchToStationMode(const std::string& _stationName);
	void switchToInSpaceMode(const Ogre::Vector3& _position, const Ogre::Quaternion& _orientation, const std::string& _sectorName, UniqueId _uniqueId, RakNet::RakNetGUID _rakNetGUID, SectorTick _sectorTick);

	Ogre::RenderWindow* mRenderWindow;
	Ogre::Root* mRoot;

	Ogre::SceneManager* mSceneManager;
	
	SectorController* mSectorController;
	InputController* mInputController;
	UIController* mUIController;
	StationController* mStationController;
	ShipInputHandler mShipInputHandler;

	PlayerData mPlayerData;

	const Sector* getSector(const std::string& _sectorName);

	// Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	//DEBUG
	float mDebugPanelLastRefresh;
	static const float mDebugPanelRefreshRate;
	
	virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

	long mLaggyValue;
};

#endif //_GAME_CONTROLLER_H_