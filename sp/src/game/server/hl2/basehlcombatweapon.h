//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "basehlcombatweapon_shared.h"

#ifndef BASEHLCOMBATWEAPON_H
#define BASEHLCOMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

//=========================================================
// Machine gun base class
//=========================================================
abstract_class CHLMachineGun : public CBaseHLCombatWeapon
{
public:
	DECLARE_CLASS( CHLMachineGun, CBaseHLCombatWeapon );
	DECLARE_DATADESC();

	CHLMachineGun();
	
	DECLARE_SERVERCLASS();

	void	PrimaryAttack( void );

	// Default calls through to m_hOwner, but plasma weapons can override and shoot projectiles here.
	virtual void	ItemPostFrame( void );
	virtual void	FireBullets( const FireBulletsInfo_t &info );
	virtual float	GetFireRate( void ) = 0;
	virtual int		WeaponRangeAttack1Condition( float flDot, float flDist );
	virtual bool	Deploy( void );

	virtual const Vector &GetBulletSpread( void );

	int				WeaponSoundRealtime( WeaponSound_t shoot_type );

	// utility function
	static void DoMachineGunKick( CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime );

protected:

	int	m_nShotsFired;	// Number of consecutive shots fired

	float	m_flNextSoundTime;	// real-time clock of when to make next sound
};

//=========================================================
// Machine guns capable of switching between full auto and 
// burst fire modes.
//=========================================================
// Mode settings for select fire weapons
enum
{
	FIREMODE_FULLAUTO = 1,
	FIREMODE_BURST,
	FIREMODE_SEMI,
};

//=========================================================
//	>> CHLSelectFireMachineGun
//=========================================================
class CHLSelectFireMachineGun : public CHLMachineGun
{
	DECLARE_CLASS( CHLSelectFireMachineGun, CHLMachineGun );
public:

	CHLSelectFireMachineGun( void );
	
	DECLARE_SERVERCLASS();

	virtual float	GetCycleRate( void );
	virtual float	GetFireRate(void) { return 0.1f; }
	virtual float	GetSemiFireRate(void) { return 0.5f; }

	virtual bool	Deploy( void );

	DECLARE_DATADESC();

	virtual int		GetBurstSize( void ) { return 3; };
	virtual int		GetSemiSize( void ) { return 1; };

	virtual void	PrimaryAttack( void );
	virtual void	SecondaryAttack( void );
	virtual void	FireModeLogic(int burstsize, float firerate, int firemode);

	virtual void	ItemPostFrame(void);

	virtual int		WeaponRangeAttack1Condition( float flDot, float flDist );
	virtual int		WeaponRangeAttack2Condition( float flDot, float flDist );

protected:
	int	m_iFireMode;
	bool m_bCanUseBurst;
	bool m_bCanUseSemi;
	bool m_bCanUseAuto;
	float	m_flSoonestPrimaryAttack;
};

//=========================================================
//	>> CHLDualMachineGun
//=========================================================
class CHLDualMachineGun : public CHLMachineGun
{
	DECLARE_CLASS(CHLDualMachineGun, CHLMachineGun);
public:

	CHLDualMachineGun(void);

	DECLARE_SERVERCLASS();

	DECLARE_DATADESC();

	virtual void	PrimaryAttack(void);
	virtual void	SecondaryAttack(void);
	virtual void	FireRightGun(void);
	virtual void	FireLeftGun(void);

	virtual Activity	GetLeftGunActivity(void);
	virtual Activity	GetRightGunActivity(void);

protected:
	bool m_bFlip;
	bool m_bSwitchArms;
};
#endif // BASEHLCOMBATWEAPON_H
