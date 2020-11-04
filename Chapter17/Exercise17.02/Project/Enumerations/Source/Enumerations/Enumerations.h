// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ENUM_TO_INT32(Value) (int)Value
#define ENUM_TO_FSTRING(Enum, Value) FindObject<UEnum>(ANY_PACKAGE, TEXT(Enum), true)->GetDisplayNameTextByIndex((int32)Value).ToString()

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol UMETA(Display Name = "Glock 19"),
	Shotgun UMETA(Display Name = "Winchester M1897"),
	RocketLauncher UMETA(Display Name = "RPG"),	
	MAX
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Bullets UMETA(DisplayName = "9mm Bullets"),
	Shells UMETA(Display Name = "12 Gauge Shotgun Shells"),
	Rockets UMETA(Display Name = "RPG Rockets"),
	MAX
};