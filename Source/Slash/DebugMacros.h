﻿
#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(),Location, 25.f, 12, Color, true);
#define DRAW_SPHERE_FRAME(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(),Location, 25.f, 12, Color, false, -1.f);
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation, EndLocation,FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_FRAME(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),StartLocation, EndLocation,FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT_FRAME(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false);
#define DRAW_VECTOR(StartLocation, EndLocation, Color) if(GetWorld()) \
{ \
DrawDebugLine(GetWorld(),StartLocation, EndLocation,Color, true, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, Color, true); \
}
#define DRAW_VECTOR_FRAME(StartLocation, EndLocation, Color) if(GetWorld()) \
{ \
DrawDebugLine(GetWorld(),StartLocation, EndLocation,Color, false, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, Color, false); \
}