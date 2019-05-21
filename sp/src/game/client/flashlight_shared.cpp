#include "cbase.h"

#include "flashlight_shared.h"
#include "tier0/memdbgon.h"


ConVar r_flashlightmodels( "r_flashlightmodels", "1" );
ConVar r_shadowrendertotexture( "r_shadowrendertotexture", "0" );


#if defined( _X360 )
ConVar r_flashlight_xbox( "r_flashlight_xbox", "1", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY, "Xbox 360 Flashlight Optimization" );
#else
ConVar r_flashlight_xbox( "r_flashlight_xbox", "0", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY, "Xbox 360 Flashlight Optimization" );
#endif

ConVar r_flashlightdepthtexture( "r_flashlightdepthtexture", "1" );



 ConVar r_newflashlight( "r_newflashlight", "1", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY, "", r_newflashlightCallback_f );

 ConVar r_swingflashlight( "r_swingflashlight", "1", FCVAR_CHEAT );
 ConVar r_flashlightlockposition( "r_flashlightlockposition", "0", FCVAR_CHEAT );

 ConVar r_flashlightfov( "r_flashlightfov", "45.0", FCVAR_CHEAT );

 ConVar r_flashlightoffsetx( "r_flashlightoffsetx", "10.0", FCVAR_CHEAT );
 ConVar r_flashlightoffsety( "r_flashlightoffsety", "-20.0", FCVAR_CHEAT );
 ConVar r_flashlightoffsetz( "r_flashlightoffsetz", "24.0", FCVAR_CHEAT );

 ConVar r_flashlightnear( "r_flashlightnear", "4.0", FCVAR_CHEAT );
 ConVar r_flashlightfar( "r_flashlightfar", "750.0", FCVAR_CHEAT );

 ConVar r_flashlightconstant( "r_flashlightconstant", "0.0", FCVAR_CHEAT );
 ConVar r_flashlightlinear( "r_flashlightlinear", "100.0", FCVAR_CHEAT );
 ConVar r_flashlightquadratic( "r_flashlightquadratic", "0.0", FCVAR_CHEAT );

 ConVar r_flashlightdrawfrustum( "r_flashlightdrawfrustum", "0", FCVAR_CHEAT );
 ConVar r_flashlightvisualizetrace( "r_flashlightvisualizetrace", "0", FCVAR_CHEAT );

 ConVar r_flashlightambient( "r_flashlightambient", "0.0", FCVAR_CHEAT );
 ConVar r_flashlightshadowatten( "r_flashlightshadowatten", "0.35", FCVAR_CHEAT );
 ConVar r_flashlightladderdist( "r_flashlightladderdist", "40.0", FCVAR_CHEAT );

 ConVar mat_slopescaledepthbias_shadowmap( "mat_slopescaledepthbias_shadowmap", "2", FCVAR_CHEAT  );
 ConVar mat_depthbias_shadowmap(	"mat_depthbias_shadowmap", "0.00001", FCVAR_CHEAT   );

 ConVar mat_shadow_filter("mat_shadow_filter", "0", FCVAR_CHEAT);


void r_newflashlightCallback_f( IConVar *pConVar, const char *pOldString, float flOldValue )
{
	if( engine->GetDXSupportLevel() < 70 )
	{
		r_newflashlight.SetValue( 0 );
	}	
}