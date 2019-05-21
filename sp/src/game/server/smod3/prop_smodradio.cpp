#include "cbase.h"
#include "props.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CProp_SMODRadio : public CPhysicsProp
{
public:
	DECLARE_CLASS( CProp_SMODRadio, CPhysicsProp );
	DECLARE_DATADESC();
	
	CProp_SMODRadio( void );
	
	void Precache( void );
	void Spawn( void );
	void RadioThink(void);
private:
	bool bSoundPlaying;
};

LINK_ENTITY_TO_CLASS(prop_smodradio, CProp_SMODRadio);

BEGIN_DATADESC(CProp_SMODRadio)
	DEFINE_THINKFUNC(RadioThink),
END_DATADESC()

CProp_SMODRadio::CProp_SMODRadio()
{
	bSoundPlaying = false;
}

void CProp_SMODRadio::Precache( void )
{
	PrecacheScriptSound("SMOD3.Radio");
	PrecacheModel( "models/props_lab/citizenradio.mdl" );
	BaseClass::Precache();
}

void CProp_SMODRadio::Spawn( void )
{ 
	Precache( );
	SetModel( "models/props_lab/citizenradio.mdl" );
	AddEFlags( EFL_NO_ROTORWASH_PUSH );
	SetThink( &CProp_SMODRadio::RadioThink );
	SetNextThink( gpGlobals->curtime + 0.01f );
	BaseClass::Spawn( );
}

void CProp_SMODRadio::RadioThink( void )
{
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);
		if ( pPlayer )
		{
			if( pPlayer->FInViewCone( GetAbsOrigin() ) && pPlayer->FVisible( GetAbsOrigin() ) )
			{
				if (!bSoundPlaying)
				{
					EmitSound("SMOD3.Radio");
					bSoundPlaying = true;
				}
			}
			else
			{
				if (bSoundPlaying)
				{
					StopSound(entindex(), CHAN_STATIC, "SMOD3.Radio");
					bSoundPlaying = false;
				}
			}
		}
	}
	SetNextThink( gpGlobals->curtime + random->RandomFloat( 0.1, 0.2f ) );
}