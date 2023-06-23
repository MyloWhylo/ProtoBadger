#pragma once
#ifndef __CUSTOMBADGER_HPP
#define __CUSTOMBADGER_HPP
#include <stdio.h>

#include <cstdint>

#include "badger2040.hpp"
#include "common/pimoroni_common.hpp"

#define DISP_WIDTH 296
#define DISP_HEIGHT 128
#define DISP_PIXELS DISP_HEIGHT* DISP_WIDTH

using namespace pimoroni;

enum MyloArrayBits { uint8,
							uint16,
							uint32 };

struct MyloImage {
	const uint16_t countLength;
	const uint8_t* counts;
	const uint8_t* values;
	const bool isMono;
};

class MyloBadger : public Badger2040 {
  public:
	void drawMyloImage(MyloImage image);
	void protoInit();
	void waitForProtoLink();
};
#endif