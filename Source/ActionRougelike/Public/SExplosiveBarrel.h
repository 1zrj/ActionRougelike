// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"
 
class UStaticMeshComponent;
class URadialForceComponent;
 
UCLASS()
class ACTIONROUGELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASExplosiveBarrel();
 
protected:
 
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
 
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;
 
	UFUNCTION()
	void OnActorHit();
 
};
