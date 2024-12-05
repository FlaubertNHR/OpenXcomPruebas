Example MOD with uses Hangar-Reworked features.
Adds two new facilities: 1x1 Garaje & 3x3 Hangar
Garage has "hangarType: 0" and allows housing one car of several types, wich have also a "hangarType:0 " defined (see crafts_MultiHangar_XCF.rul)
3x3 Hangar has a "hangarType: 1" and have capacity for 4 crafts.
In crafts_MultiHangar_XCF.rul, STR_HELICOPTER, STR_MUDRANGER & STR_LITTLE_BIRD have been defined "hangarType: 1", so they are the only ones that can be allocated  3x3 Hangar.