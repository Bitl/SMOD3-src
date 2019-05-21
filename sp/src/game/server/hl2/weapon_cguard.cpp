//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Combine guard gun, strider destroyer
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "player.h"
#include "grenade_ar2.h"
#include "soundent.h"
#include "explode.h"
#include "shake.h"
#include "energy_wave.h"
#include "te_particlesystem.h"
#include "ndebugoverlay.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//Concussive explosion entity

class CTEConcussiveExplosion : public CTEParticleSystem
{
public:
	DECLARE_CLASS( CTEConcussiveExplosion, CTEParticleSystem );
	DECLARE_SERVERCLASS();

	CTEConcussiveExplosion( const char *name );
	virtual	~CTEConcussiveExplosion( void );

	CNetworkVector( m_vecNormal );
	CNetworkVar( float, m_flScale );
	CNetworkVar( int, m_nRadius );
	CNetworkVar( int, m_nMagnitude );
};

IMPLEMENT_SERVERCLASS_ST( CTEConcussiveExplosion, DT_TEConcussiveExplosion )
	SendPropVector( SENDINFO(m_vecNormal), -1, SPROP_COORD ),
	SendPropFloat( SENDINFO(m_flScale), 0, SPROP_NOSCALE ),
	SendPropInt( SENDINFO(m_nRadius), 32, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO(m_nMagnitude), 32, SPROP_UNSIGNED ),
END_SEND_TABLE()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTEConcussiveExplosion::CTEConcussiveExplosion( const char *name ) : BaseClass( name )
{
	m_nRadius		= 0;
	m_nMagnitude	= 0;
	m_flScale		= 0.0f;

	m_vecNormal.Init();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTEConcussiveExplosion::~CTEConcussiveExplosion( void )
{
}


// Singleton to fire TEExplosion objects
static CTEConcussiveExplosion g_TEConcussiveExplosion( "ConcussiveExplosion" );

void TE_ConcussiveExplosion( IRecipientFilter& filter, float delay,
	const Vector* pos, float scale, int radius, int magnitude, const Vector* normal )
{
	g_TEConcussiveExplosion.m_vecOrigin		= *pos;
	g_TEConcussiveExplosion.m_flScale			= scale;
	g_TEConcussiveExplosion.m_nRadius			= radius;
	g_TEConcussiveExplosion.m_nMagnitude		= magnitude;

	if ( normal )
		g_TEConcussiveExplosion.m_vecNormal	= *normal;
	else 
		g_TEConcussiveExplosion.m_vecNormal	= Vector(0,0,1);

	// Send it over the wire
	g_TEConcussiveExplosion.Create( filter, delay );
}

//Temp ent for the blast

class CConcussiveBlast : public CBaseEntity
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CConcussiveBlast, CBaseEntity );

	int		m_spriteTexture;

	CConcussiveBlast( void ) {}

	//-----------------------------------------------------------------------------
	// Purpose: 
	// Output :
	//-----------------------------------------------------------------------------
	void Precache( void )
	{
		m_spriteTexture = PrecacheModel( "sprites/lgtning.vmt" );

		BaseClass::Precache();
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	// Output :
	//-----------------------------------------------------------------------------

	void Explode( float magnitude )
	{
		//Create a concussive explosion
		CPASFilter filter( GetAbsOrigin() );

		Vector vecForward;
		AngleVectors( GetAbsAngles(), &vecForward );
		TE_ConcussiveExplosion( filter, 0.0,
			&GetAbsOrigin(),//position
			1.0f,	//scale
			256*magnitude,	//radius
			175*magnitude,	//magnitude
			&vecForward );	//normal
		
		int	colorRamp = random->RandomInt( 128, 255 );

		//Shockring
		CBroadcastRecipientFilter filter2;
		te->BeamRingPoint( filter2, 0, 
			GetAbsOrigin(),	//origin
			16,			//start radius
			300*magnitude,		//end radius
			m_spriteTexture, //texture
			0,			//halo index
			0,			//start frame
			2,			//framerate
			0.3f,		//life
			128,		//width
			16,			//spread
			0,			//amplitude
			colorRamp,	//r
			colorRamp,	//g
			255,		//g
			24,			//a
			128			//speed
			);

		//Do the radius damage
		RadiusDamage( CTakeDamageInfo( this, GetOwnerEntity(), 200, DMG_BLAST|DMG_DISSOLVE ), GetAbsOrigin(), 256, CLASS_NONE, NULL );

		UTIL_Remove( this );
	}
};

LINK_ENTITY_TO_CLASS( concussiveblast, CConcussiveBlast );

//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CConcussiveBlast )

//	DEFINE_FIELD( m_spriteTexture,	FIELD_INTEGER ),

END_DATADESC()


//-----------------------------------------------------------------------------
// Purpose: Create a concussive blast entity and detonate it
//-----------------------------------------------------------------------------
void CreateConcussiveBlast( const Vector &origin, const Vector &surfaceNormal, CBaseEntity *pOwner, float magnitude )
{
	QAngle angles;
	VectorAngles( surfaceNormal, angles );
	CConcussiveBlast *pBlast = (CConcussiveBlast *) CBaseEntity::Create( "concussiveblast", origin, angles, pOwner );

	if ( pBlast )
	{
		pBlast->Explode( magnitude );
	}
}

// Combine Guard weapon

class CWeaponCGuard : public CBaseHLCombatWeapon
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CWeaponCGuard, CBaseHLCombatWeapon );

	DECLARE_SERVERCLASS();

	CWeaponCGuard( void );
	
	void Precache( void );
	void PrimaryAttack( void );
	void AddViewKick( void );
	void DelayedFire( void );
	void ItemPostFrame( void );
	void AlertTargets( void );

	int CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	DECLARE_ACTTABLE();

protected:
	float	m_flChargeTime;
	bool	m_bFired;
};


IMPLEMENT_SERVERCLASS_ST(CWeaponCGuard, DT_WeaponCGuard)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_cguard, CWeaponCGuard );
PRECACHE_WEAPON_REGISTER( weapon_cguard );


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CWeaponCGuard )

	DEFINE_FIELD( m_flChargeTime,	FIELD_TIME ),
	DEFINE_FIELD( m_bFired,			FIELD_BOOLEAN ),
//	DEFINE_FIELD( m_beamIndex,		FIELD_INTEGER ),
//	DEFINE_FIELD( m_haloIndex,		FIELD_INTEGER ),

END_DATADESC()


//-----------------------------------------------------------------------------
// Maps base activities to weapons-specific ones so our characters do the right things.
//-----------------------------------------------------------------------------
acttable_t CWeaponCGuard::m_acttable[] = 
{
	{	ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_SNIPER_RIFLE, true }
};

IMPLEMENT_ACTTABLE( CWeaponCGuard );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponCGuard::CWeaponCGuard( void )
{
	m_flNextPrimaryAttack	= gpGlobals->curtime;
	m_flChargeTime			= gpGlobals->curtime;
	m_bFired				= true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCGuard::Precache( void )
{
	UTIL_PrecacheOther( "concussiveblast" );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCGuard::AlertTargets( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( pPlayer == NULL )
		return;

	// Fire the bullets
	Vector vecSrc	 = pPlayer->Weapon_ShootPosition( );
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	Vector	impactPoint	= vecSrc + ( vecAiming * MAX_TRACE_LENGTH );

	trace_t	tr;

	UTIL_TraceLine( vecSrc, impactPoint, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr );
	
	if ( (vecSrc-tr.endpos).Length() > 1024 )
		return;

	CSoundEnt::InsertSound( SOUND_DANGER, tr.endpos, 128, 0.5f );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCGuard::PrimaryAttack( void )
{
	if ( m_flChargeTime >= gpGlobals->curtime )
		return;
		
	AlertTargets();

	WeaponSound( SPECIAL1 );

	//UTIL_ScreenShake( GetAbsOrigin(), 10.0f, 100.0f, 2.0f, 128, SHAKE_START, false );

	m_flChargeTime	= gpGlobals->curtime + 1.0f;
	m_bFired		= false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCGuard::ItemPostFrame( void )
{
	//FIXME: UpdateLasers();

	if ( ( m_flChargeTime < gpGlobals->curtime ) && ( m_bFired == false ) )
	{
		DelayedFire();
	}

	BaseClass::ItemPostFrame();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCGuard::DelayedFire( void )
{
	if ( m_flChargeTime >= gpGlobals->curtime )
		return;

	if ( m_bFired )
		return;

	m_bFired = true;

	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( pPlayer == NULL )
		return;
	
	// Abort here to handle burst and auto fire modes
	if ( (GetMaxClip1() != -1 && m_iClip1 == 0) || (GetMaxClip1() == -1 && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType) ) )
		return;

	// MUST call sound before removing a round from the clip of a CMachineGun
	WeaponSound(SINGLE);

	pPlayer->DoMuzzleFlash();

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	if ( GetSequence() != SelectWeightedSequence( ACT_VM_PRIMARYATTACK ) )
	{
		m_flNextPrimaryAttack = gpGlobals->curtime;
	}
	
	// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
	if ( UsesClipsForAmmo1() )
	{
		m_iClip1 = m_iClip1 - 1;
	}

	// Fire the bullets
	Vector vecSrc	 = pPlayer->Weapon_ShootPosition( );
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

	//Factor in the view kick
	AddViewKick();

	Vector	impactPoint	= vecSrc + ( vecAiming * MAX_TRACE_LENGTH );

	trace_t	tr;
	UTIL_TraceHull( vecSrc, impactPoint, Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr );

	CreateConcussiveBlast( tr.endpos, tr.plane.normal, this, 1.0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCGuard::AddViewKick( void )
{
	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( pPlayer == NULL )
		return;

	color32 white = {255, 255, 255, 64};
	UTIL_ScreenFade( pPlayer, white, 0.1, 0, FFADE_IN  );

	//Disorient the player
	QAngle angles = pPlayer->GetLocalAngles();

	angles.x += random->RandomInt( -5, 5 );
	angles.y += random->RandomInt( -8, 8 );
	angles.z = 0.0f;

	SetLocalAngles( angles );

	pPlayer->SnapEyeAngles( angles );
	
	pPlayer->ViewPunch( QAngle( random->RandomInt( -8, -12 ), random->RandomInt( -2, 2 ), random->RandomInt( -8, 8 ) ) );
}
