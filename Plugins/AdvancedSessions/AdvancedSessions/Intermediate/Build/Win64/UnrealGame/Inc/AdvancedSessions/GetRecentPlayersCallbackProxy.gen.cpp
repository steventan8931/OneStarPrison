// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AdvancedSessions/Classes/GetRecentPlayersCallbackProxy.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGetRecentPlayersCallbackProxy() {}
// Cross Module References
	ADVANCEDSESSIONS_API UFunction* Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_AdvancedSessions();
	ADVANCEDSESSIONS_API UScriptStruct* Z_Construct_UScriptStruct_FBPOnlineRecentPlayer();
	ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetRecentPlayersCallbackProxy_NoRegister();
	ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetRecentPlayersCallbackProxy();
	ENGINE_API UClass* Z_Construct_UClass_UOnlineBlueprintCallProxyBase();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	ADVANCEDSESSIONS_API UScriptStruct* Z_Construct_UScriptStruct_FBPUniqueNetId();
// End Cross Module References
	struct Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics
	{
		struct _Script_AdvancedSessions_eventBlueprintGetRecentPlayersDelegate_Parms
		{
			TArray<FBPOnlineRecentPlayer> Results;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Results_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Results_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_Results;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results_Inner = { "Results", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FBPOnlineRecentPlayer, METADATA_PARAMS(nullptr, 0) }; // 3899615166
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results = { "Results", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_AdvancedSessions_eventBlueprintGetRecentPlayersDelegate_Parms, Results), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results_MetaData, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results_MetaData)) }; // 3899615166
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::NewProp_Results,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/GetRecentPlayersCallbackProxy.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_AdvancedSessions, nullptr, "BlueprintGetRecentPlayersDelegate__DelegateSignature", nullptr, nullptr, sizeof(Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::_Script_AdvancedSessions_eventBlueprintGetRecentPlayersDelegate_Parms), Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	DEFINE_FUNCTION(UGetRecentPlayersCallbackProxy::execGetAndStoreRecentPlayersList)
	{
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
		P_GET_STRUCT_REF(FBPUniqueNetId,Z_Param_Out_UniqueNetId);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UGetRecentPlayersCallbackProxy**)Z_Param__Result=UGetRecentPlayersCallbackProxy::GetAndStoreRecentPlayersList(Z_Param_WorldContextObject,Z_Param_Out_UniqueNetId);
		P_NATIVE_END;
	}
	void UGetRecentPlayersCallbackProxy::StaticRegisterNativesUGetRecentPlayersCallbackProxy()
	{
		UClass* Class = UGetRecentPlayersCallbackProxy::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetAndStoreRecentPlayersList", &UGetRecentPlayersCallbackProxy::execGetAndStoreRecentPlayersList },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics
	{
		struct GetRecentPlayersCallbackProxy_eventGetAndStoreRecentPlayersList_Parms
		{
			UObject* WorldContextObject;
			FBPUniqueNetId UniqueNetId;
			UGetRecentPlayersCallbackProxy* ReturnValue;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UniqueNetId_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_UniqueNetId;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GetRecentPlayersCallbackProxy_eventGetAndStoreRecentPlayersList_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_UniqueNetId_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_UniqueNetId = { "UniqueNetId", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GetRecentPlayersCallbackProxy_eventGetAndStoreRecentPlayersList_Parms, UniqueNetId), Z_Construct_UScriptStruct_FBPUniqueNetId, METADATA_PARAMS(Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_UniqueNetId_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_UniqueNetId_MetaData)) }; // 766028705
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GetRecentPlayersCallbackProxy_eventGetAndStoreRecentPlayersList_Parms, ReturnValue), Z_Construct_UClass_UGetRecentPlayersCallbackProxy_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_WorldContextObject,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_UniqueNetId,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "Online|AdvancedFriends" },
		{ "Comment", "// Gets the list of recent players from the OnlineSubsystem and returns it, can be retrieved later with GetStoredRecentPlayersList, can fail if no recent players are found\n" },
		{ "ModuleRelativePath", "Classes/GetRecentPlayersCallbackProxy.h" },
		{ "ToolTip", "Gets the list of recent players from the OnlineSubsystem and returns it, can be retrieved later with GetStoredRecentPlayersList, can fail if no recent players are found" },
		{ "WorldContext", "WorldContextObject" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGetRecentPlayersCallbackProxy, nullptr, "GetAndStoreRecentPlayersList", nullptr, nullptr, sizeof(Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::GetRecentPlayersCallbackProxy_eventGetAndStoreRecentPlayersList_Parms), Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGetRecentPlayersCallbackProxy);
	UClass* Z_Construct_UClass_UGetRecentPlayersCallbackProxy_NoRegister()
	{
		return UGetRecentPlayersCallbackProxy::StaticClass();
	}
	struct Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnSuccess_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSuccess;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnFailure_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnFailure;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UOnlineBlueprintCallProxyBase,
		(UObject* (*)())Z_Construct_UPackage__Script_AdvancedSessions,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UGetRecentPlayersCallbackProxy_GetAndStoreRecentPlayersList, "GetAndStoreRecentPlayersList" }, // 1307752083
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "GetRecentPlayersCallbackProxy.h" },
		{ "ModuleRelativePath", "Classes/GetRecentPlayersCallbackProxy.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnSuccess_MetaData[] = {
		{ "Comment", "// Called when the friends list successfully was retrieved\n" },
		{ "ModuleRelativePath", "Classes/GetRecentPlayersCallbackProxy.h" },
		{ "ToolTip", "Called when the friends list successfully was retrieved" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnSuccess = { "OnSuccess", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UGetRecentPlayersCallbackProxy, OnSuccess), Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnSuccess_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnSuccess_MetaData)) }; // 3278428017
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnFailure_MetaData[] = {
		{ "Comment", "// Called when there was an error retrieving the friends list\n" },
		{ "ModuleRelativePath", "Classes/GetRecentPlayersCallbackProxy.h" },
		{ "ToolTip", "Called when there was an error retrieving the friends list" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnFailure = { "OnFailure", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UGetRecentPlayersCallbackProxy, OnFailure), Z_Construct_UDelegateFunction_AdvancedSessions_BlueprintGetRecentPlayersDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnFailure_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnFailure_MetaData)) }; // 3278428017
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnSuccess,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::NewProp_OnFailure,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGetRecentPlayersCallbackProxy>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::ClassParams = {
		&UGetRecentPlayersCallbackProxy::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::PropPointers),
		0,
		0x008800A0u,
		METADATA_PARAMS(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UGetRecentPlayersCallbackProxy()
	{
		if (!Z_Registration_Info_UClass_UGetRecentPlayersCallbackProxy.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGetRecentPlayersCallbackProxy.OuterSingleton, Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UGetRecentPlayersCallbackProxy.OuterSingleton;
	}
	template<> ADVANCEDSESSIONS_API UClass* StaticClass<UGetRecentPlayersCallbackProxy>()
	{
		return UGetRecentPlayersCallbackProxy::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UGetRecentPlayersCallbackProxy);
	struct Z_CompiledInDeferFile_FID_OneStarPrison_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_OneStarPrison_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UGetRecentPlayersCallbackProxy, UGetRecentPlayersCallbackProxy::StaticClass, TEXT("UGetRecentPlayersCallbackProxy"), &Z_Registration_Info_UClass_UGetRecentPlayersCallbackProxy, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGetRecentPlayersCallbackProxy), 1570068847U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_OneStarPrison_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_2231356991(TEXT("/Script/AdvancedSessions"),
		Z_CompiledInDeferFile_FID_OneStarPrison_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_OneStarPrison_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
