#pragma once

//defines custom player names

#include <string>

namespace KAR::CustomPlayer
{
static std::string TACO_CUSTOM_PLAYER_NAME = "Taco";

#define TACO_CUSTOM_PLAYER_MESSAGES_COUNT 2
static const char* TACO_CUSTOM_PLAYER_ENTRANCE_MESSAGEs[TACO_CUSTOM_PLAYER_MESSAGES_COUNT] = {
    "You have been marked for death", "Watchout for Time Bombs!"};

static std::string SUPPORT_PVP_CUSTOM_PLAYER_NAME = "SupportPvP";
static std::string SUPPORT_PVPS_CUSTOM_PLAYER_NAME = "SupportPvPs";

#define SUPPORT_CUSTOM_PLAYER_MESSAGES_COUNT 1
static const char* SUPPORT_CUSTOM_PLAYER_ENTRANCE_MESSAGEs[SUPPORT_CUSTOM_PLAYER_MESSAGES_COUNT] = {
    "Supporting YOU!"};

static std::string PLANTT_CUSTOM_PLAYER_NAME_1 = "plantt";
static std::string PLANTT_CUSTOM_PLAYER_NAME_2 = "Plantt";

#define PLANTT_CUSTOM_PLAYER_MESSAGES_COUNT 1
static const char* PLANTT_CUSTOM_PLAYER_ENTRANCE_MESSAGEs[PLANTT_CUSTOM_PLAYER_MESSAGES_COUNT] = {
    "The ELO Reaper has come forth"};
}