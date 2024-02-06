#pragma once

//enum
enum MESSAGE_SURF_STATE {
	msgNONE,
	msgSURFACE_SHOW,
	msgSURFACE_HIDE,
	msgSURFACE_TRANSPARENT,
	msgSURFACE_HIDE_CHOOSEABLE,
	msgSPRITE_SHOW,
	msgSPRITE_HIDE,
	msgSPRITE_TRANSPARENT,
	msgSPRITE_HIDE_CHOOSEABLE,
	msgFLASH_SHOW,
	msgFLASH_HIDE,
	msgFLASH_TRANSPARENT,
	msgFLASH_HIDE_CHOOSEABLE,
	msgTEXT_SHOW,
	msgTEXT_HIDE,
	msgTEXT_TRANSPARENT,
	msgTEXT_HIDE_CHOOSEABLE,
};
enum MESSAGE_PHISICS {
	phsNONE,
	phsTEST_SPRITE_FLASH,
	phsTEST_CHANGE_
};
enum SPRITE_TYPE {
	TPsprite_default,
	TPsprite_test
};
enum FLASH_TYPE {
	TPflash_default,
	TPflash_test,
};
enum SURF_TYPE {
	TPsurf_default,
	TPbackground,
};
enum TEXT_TYPE {
	TPtext_default,

};
enum STAGE {
	STGempty,
};
enum DELAY_TYPE {
	delayNONE,
	delayDELETE,
	delaySPRITE_NEXT,
	delaySPRITE_SHOW,
	delaySPRITE_HIDE,
	delaySPRITE_TRANSPARENT,
	delaySPRITE_HIDE_CHOOSEABLE,
	delaySPRITE_MOVE,
	delaySPRITE_CONTINUE,
	delayFLASH_NEXT,
	delayFLASH_CONTINUE
};
enum EFFECT_TYPE {
	effectNONE,

};
