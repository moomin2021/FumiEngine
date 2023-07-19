#pragma once
#include <stdint.h>

const uint16_t COL_PLAYER_RAY = 0b1 << 0;// プレイヤーの視点レイ
const uint16_t COL_ENEMY = 0b1 << 0;// 敵
const uint16_t COL_DOWN_RAY = 0b1 << 1;// 真下を向くレイ
const uint16_t COL_GROUND = 0b1 << 1;// 地面
const uint16_t COL_ATTR_ALL = 0b1111111111111111;