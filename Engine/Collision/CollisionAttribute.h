#pragma once
#include <stdint.h>

const uint16_t COL_PLAYER_RAY	= 0b0100000000000001;// �v���C���[�̎��_���C
const uint16_t COL_ENEMY		= 0b0000000000000001;// �G
const uint16_t COL_DOWN_RAY		= 0b0000000000000010;// �^�����������C
const uint16_t COL_GROUND		= 0b0000000000000010;// �n��
const uint16_t COL_ENEMYBULLET	= 0b0000000000000100;
const uint16_t COL_PLAYER		= 0b1000000000000100;
const uint16_t COL_WALL			= 0b1000000000000000;
const uint16_t COL_ATTR_ALL		= 0b1111111111111111;
const uint16_t COL_ITEM			= 0b0100000000000000;