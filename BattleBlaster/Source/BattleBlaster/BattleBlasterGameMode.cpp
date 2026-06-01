// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"

#include"Tower.h"
#include"BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ATower::StaticClass(),
		Towers
	);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), TowerCount);

	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
			UE_LOG(LogTemp, Display, TEXT("PlayerPawn cast is unsuccessful"));
	}

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount)
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)
			{
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable"),*Tower->GetActorNameOrLabel());
			}
		}
		LoopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
	}


	CountdownSeconds = CountDownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, 
		&ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f,true);
	
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;
	if (CountdownSeconds > 0)
	{
		//UE_LOG(LogTemp, Display, TEXT("Countdown seconds: %d"), CountdownSeconds);
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		//UE_LOG(LogTemp, Display, TEXT("Go"));
		ScreenMessageWidget->SetMessageText("Go");
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Clear Timer"));
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	
	// we have only two type
	if (DeadActor == Tank)
	{
		// Tank died
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else
	{	// A Tower died
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();
			TowerCount--;
			if (TowerCount <= 0)
			{
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory" : "Defeat";
		UE_LOG(LogTemp, Display, TEXT("Game Over: %s"), *GameOverString);
		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle,this,&ABattleBlasterGameMode::OnGameOverTimerTimeout,GameOverDelay,false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	//FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameInstance* GameInstance = GetGameInstance();
	UBattleBlasterGameInstance* BattleBlasterGameInstance;
	if (GameInstance)
	{
		BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				// Load the next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				// Reload the current level
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}


