//
// Created by stefam on 5/28/23.
//
#pragma once

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

const int DEFAULT_PACMAN_SPEED = 2; //50ms tick * 4 = 200ms
const int DEFAULT_GHOST_SPEED = 4; //50ms tick * 4 = 200ms