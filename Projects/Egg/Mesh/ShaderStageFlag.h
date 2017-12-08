#pragma once

#include "Enum/EnumFramework.h"

PP_MACRO_ENUM_CLASS_FLAGS(
    (Egg)(Mesh), // put in namespace "Egg::Mesh"
    ShaderStageFlag, // typename
	Vertex,
	Hull,
	Domain,
	Geometry,
	Pixel,
	Compute
);

