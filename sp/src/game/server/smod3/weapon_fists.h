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
#error weapon_fists.h must not be included in hl2mp. The windows compiler will use the wrong class elsewhere if it is.
#endif

#define	FISTS_RANGE	60.0f
#define	FIST_REFIRE	0.4f

//-----------------------------------------------------------------------------
// CWeaponFists
//-----------------------------------------------------------------------------

class CWeaponFists : public CBaseHLBludgeonWeapon
{
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CWeaponFists, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	CWeaponFists();

	float		GetRange(void)		{ return	FISTS_RANGE; }
	float		GetFireRate(void)	{ return	FIST_REFIRE; }

	void		AddViewKick( void );
	float		GetDamageForActivity(Activity hitActivity, bool bIsSecondary);

	void		ItemPostFrame(void);

	void		Swing(bool bIsSecondary);
	void		Hit(trace_t &traceHit, Activity nHitActivity, bool bIsSecondary);

private:
	float	m_flSoonestPrimaryAttack;
};

#endif // WEAPON_SHOVEL_H
