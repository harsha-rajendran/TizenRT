/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __MEDIA_DECODER_H
#define __MEDIA_DECODER_H

#include <tinyara/config.h>
#include <stdio.h>
#include <memory>

#ifdef CONFIG_AUDIO_CODEC
#include "streaming/audio_decoder.h"
#endif

namespace media {

class Decoder
{
public:
	Decoder(audio_type_t audioType, unsigned short channels, unsigned int sampleRate);
	Decoder(const Decoder *source);
	~Decoder();

public:
	static std::shared_ptr<Decoder> create(audio_type_t audioType, unsigned short channels, unsigned int sampleRate);
	bool init(void);
	size_t pushData(unsigned char *buf, size_t size);
	bool getFrame(unsigned char *buf, size_t *size, unsigned int *sampleRate, unsigned short *channels);
	bool empty();
	size_t getAvailSpace();

private:
#ifdef CONFIG_AUDIO_CODEC
	//static int _configFunc(void *user_data, int audio_type, void *dec_ext);
	bool mConfig(int audioType);
	audio_decoder_t mDecoder;
	audio_type_t mAudioType;
	unsigned short mChannels;
	unsigned int mSampleRate;
#endif
};
} // namespace media

#endif
