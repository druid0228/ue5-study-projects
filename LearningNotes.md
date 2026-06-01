BattleBlaster 강의

	game.Build.cs 에 Module에 기본 이외의 것들을 추가한다.
	EnhancedInput, UMG, Niagara 등등...
	필요에 따라 모듈을 임포트하여 엔진 빌드 성능에 유리
	public과 private는 c++ 클래스의 그것과는 다르다.

	Instance unreal 게임 최상단에 있는 객체 이곳에서 레벨 이상의
	전역적인 설정을 할 수 있다
	ex level 관리 등을 이곳에서 함
 
	UGameplayStatics::OpenLevel 이 함수로 레벨을 바꿀 수 있음
	ChangeLevel같이 함수를 만들어두고 사용 가능

	#include"Kismet/GameplayStatics.h" 해당 헤더 많이 쓰임
	UGameplayStatics::GetGameMode
	UGameplayStatics::PlaySoundAtLocation 
	UGameplayStatics::GetPlayerController
	UGameplayStatics::ApplyDamage
	UGameplayStatics::GetWorldDeltaSeconds(GetWorld());


	GameMode 게임에 관련된 것들을 넣는? ㄱ
	이곳에서 PlayerController, Player Controller Class, gameStateBase 등을 설정 가능
	ScreenMessage WBP도 이곳에서 했었다.
	맵 마다 재생성 Level생성시에 생성되고 끝날때 파괴된다.
	즉 전역적인 변수를 이곳에 넣으면 안됨 그러한 상태 관리는 Instance에서
	게임 시작후의 딜레이 적의 카운트 전부 잡으면 승리 같은
	level별 조건을 이곳에서 설정하면 된다.
	

	BasePawn
	Tank와 Tower가 속성을 공유하는 것이 있으므로 이곳에서

	CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	이것 과 같이 c++ 내부에서 subobject를 생성 가능
	blueprint에서 하지 않고 이렇게 하는 이유 c++에서 해당 component들에 접근 가능하므로
	
	SetRootComponent(CapsuleComp); 계층 구조도 가능
	BaseMesh->SetupAttachment(CapsuleComp);
	SetupAttachment 함수를 이용하여 부모 노드 등록

	FRotator InterpolatedRotation = FMath::RInterpTo(
	TurretMesh->GetComponentRotation(),
	LookAtRotation,
	GetWorld()->GetDeltaSeconds(),
	10.0f);  로테이션 interp 해주는 함수

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	GetWorld 함수는 여러 곳의 인자로 자주 쓰이지만 ActorSpawn에도 쓰임

	UGameplayStatics::PlaySoundAtLocation 사운드 관련

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->ClientStartCameraShake(DeathCameraShakeClass);
	} 카메라 흔들기

	UGameplayStatics는 자주 쓰인다

		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, 
		&ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f,true);
	카운트 다운용 
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle); 핸들을 이용해서 나중에 Clear도 해줘야함
	TimeOut 함수 내부에서 측정후 clear
	간단히 check 용도로만 사용 해당 부분에서 작업을 많이하면 tick에 영향

	객체의 생성 소멸 이동 대미지 등을 엔진 관련된 함수를 이용할것
	그렇지 않다면 엔진을 사용할 이유가 없음.
	UGameplayStatics::ApplyDamage
	(
		OtherActor,
		Damage,
		MyOwner->GetInstigatorController(),
		MyOwner,
		UDamageType::StaticClass()
	);

	Health Component에서 관리한다

	Health = MaxHealth;

GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
	이 함수로 연결되어 있기 때문에 ApplyDamage를 부르면 이쪽으로 온다

AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
if (GameMode)
{
	BBGameMode = Cast<ABattleBlasterGameMode>(GameMode);
}

	HealthComponent는 BP_Tank와 BP_Tower에서 Component에 추가해서 만들었음
	C++ UActorComponent

	ActorComponent는 정말 기본적인 부품
	위치가 있는 것은 그 하위인 SceneComponent 헷갈리면 안됨
	

GetOwner()->OnTakeAnyDamage.AddDynamic f12로 따라가면서 어떤 인자가 함수에 필요한지 확인후 함수로 구현
void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)

#define DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams( SparseDelegateClass, OwningClass, DelegateName, Param1Type, Param1Name, Param2Type, Param2Name, Param3Type, Param3Name, Param4Type, Param4Name, Param5Type, Param5Name ) BODY_MACRO_COMBINE(CURRENT_FILE_ID,_,__LINE__,_DELEGATE) FUNC_DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE( SparseDelegateClass, OwningClass, DelegateName, FUNC_CONCAT( Param1Type InParam1, Param2Type InParam2, Param3Type InParam3, Param4Type InParam4, Param5Type InParam5 ), FUNC_CONCAT( *this, InParam1, InParam2, InParam3, InParam4, InParam5 ), void, Param1Type, Param2Type, Param3Type, Param4Type, Param5Type )


Shooter Sam


Behavior tree로 간단한 ai

	Character BP에서 Movement부분을 수정
	Character Movement의 Orient Rotation to Movement 를 true로 하면 회전하는 곳으로 캐릭터가 돌아서 바라본다 기본값은 true인데 false로 하면 캐릭터 자체는 고정되고 좌 우로 움직임
즉 true일 때 a , d 키를 눌러서 회전이 되면 캐릭터도 돌아갔다는 소리
	Use Controller Desired Rotation 이것을 키면 마우스 회전하는 것에 따라 캐릭터가 그쪽을 바라보게됨
	
	켜있던 것을 서로 토글하면 이 게임의 목적에 맞게 셋팅이 된다.

	Addictive Animation : 기존 base 애니메이션에서 머리나 손 등 추가적인 애니메이션을 넣을 때 사용

	머리의 pitch 구하는법 Get Control Rotation - Get Actor Rotation
	Delta(Rotator) 로테이터 함수를 사용하면 정규화되어서 90 0 270 => 90 0 -90으로 바뀜
	일반적으로 Get Actor Rotation은 0 0 0이 나옴 캐릭이 회전했을때 그 값을 사용하기 위함.

	UNiagaraSystem vs UNiagaraComponent
	System은 설계도 일회성에 사용
	Component는 지속성 active deactive도 가능
	내부적으로는 UNiagaraFunctionLibrary::Spawn을 하더라도 임시로 Component가 생성되었다가 사라지게됨

	
