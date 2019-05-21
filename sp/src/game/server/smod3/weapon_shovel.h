//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_SHOVEL_H
#define WEAPON_SHOVEL_H

#include "basebludgeonweapon.h"

#if defined( _WIN32 )
#pragma once
#endif

#ifdef HL2MP
#error weapon_shovel.h must not be included in hl2mp. The windows compiler will use the wrong class elsewhere if it is.
#endif

#define	SHOVEL_RANGE	95.0f
#define	SHOVEL_REFIRE_SECONDARY	1.0f

//-----------------------------------------------------------------------------
// CWeaponShovel
//-----------------------------------------------------------------------------

class CWeaponShovel : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponShovel, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CWeaponShovel();

	float		GetRange( void )		{	return	SHOVEL_RANGE;	}
	float		GetFireRateSecondary(void)	{ return	SHOVEL_REFIRE_SECONDARY; }

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity);

	void		PrimaryAttack(void);
	void		SecondaryAttack(void);

	void		Swing(void);
	void		Hit(trace_t &traceHit, Activity nHitActivity);

	bool		Deploy(void);

private:
	bool swingOnce;
};

#endif // WEAPON_SHOVEL_H
