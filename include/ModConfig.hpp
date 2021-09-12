#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(Active, bool, "Active", true);
    CONFIG_VALUE(ThreeSixtyMode, bool, "ThreeSixtyMode", true);
    CONFIG_VALUE(RanStartPos, bool, "RanStartPos", true);
    CONFIG_VALUE(RanGravity, bool, "RanGravity", true);
    CONFIG_VALUE(RandomHeight, bool, "RanGravity", true);
    CONFIG_VALUE(RanWallStartPos, bool, "RanWallStartPos", true);
    CONFIG_VALUE(RanWallMidPos, bool, "RanWallMidPos", true);
    CONFIG_VALUE(RanWallEndPos, bool, "RanWallEndPos", true);
    CONFIG_VALUE(ColourSwap, bool, "ColourSwap", true);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(Active);
        CONFIG_INIT_VALUE(ThreeSixtyMode);
        CONFIG_INIT_VALUE(RanStartPos);
        CONFIG_INIT_VALUE(RanGravity);
        CONFIG_INIT_VALUE(RandomHeight);
        CONFIG_INIT_VALUE(RanWallStartPos);
        CONFIG_INIT_VALUE(RanWallMidPos);
        CONFIG_INIT_VALUE(RanWallEndPos);
        CONFIG_INIT_VALUE(ColourSwap);
    )
)