#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
 
ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
 
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
 
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
}
 
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}
 
// 角色向前移动
void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	// 转向只关注水平Yaw方向，因此置0防止影响
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	// 获取相机（鼠标控制器）的朝向，并朝这个方向移动
	AddMovementInput(ControlRot.Vector(), value);
}
 
// 角色向右移动
void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	// 获取相机（鼠标控制器）的朝向，转向右侧，并朝这个方向移动
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

// 延迟生成攻击体
void ASCharacter::PrimaryAttack_TimeElapsed()
{
	// Spawn Transform Matrix， spawn的变换矩阵
	FVector RightHandLoc = GetMesh()->GetSocketLocation("Muzzle_01");
	// 朝向角色方向，在角色的中心位置生成
	// FTransform SpawnTM = FTransform(GetActorRotation(),GetActorLocation());

	// 朝向角色方向，在角色的右手位置生成
	FTransform SpawnTM = FTransform(GetActorRotation(),RightHandLoc);

	// 参数设置。
	// 此处设置碰撞检测规则为：即使碰撞也总是生成，因为粒子在角色中间生成必然碰撞
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
 
	// 所有能放置或生成的对象都是Actor
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
 
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
 
	// “UE中调用的名称”，this指针表示移动这个角色，&自定义移动方法
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
 
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	// 绑定按键动作("UE中调用的名称"；触发的时机如按下或释放；对象；具体方法实现)
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}