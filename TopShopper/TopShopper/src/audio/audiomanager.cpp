
#include "audiomanager.h"
#include "core/broker.h"

#include <iostream>
#include <string>
#include <vector>


using namespace std;

AudioManager::SoundEffect* rollingSound_player1 = new AudioManager::SoundEffect();
AudioManager::SoundEffect* rollingSound_ai1 = new AudioManager::SoundEffect();
AudioManager::SoundEffect* rollingSound_ai2 = new AudioManager::SoundEffect();
AudioManager::SoundEffect* rollingSound_ai3 = new AudioManager::SoundEffect();
AudioManager::SoundEffect* rollingSound_ai4 = new AudioManager::SoundEffect();
AudioManager::SoundEffect* rollingSound_ai5 = new AudioManager::SoundEffect();
AudioManager::SoundEffect* hitWallSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* dropItemSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* pickItemSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* turboSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* explosionSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* tickingSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* paperbagSound = new AudioManager::SoundEffect();
AudioManager::SoundEffect* selectSound = new AudioManager::SoundEffect();


AudioManager::AudioManager(Broker *broker)
	: _broker(broker)
{

}

AudioManager::~AudioManager() {
	// quit SDL_mixer
	Mix_Quit();
}

AudioManager::SoundEffect* AudioManager::getSoundEffect(SoundEffectTypes sound) {
	switch (sound) {
	case SoundEffectTypes::ROLL_SOUND_PLAYER1:
		return rollingSound_player1;
	case SoundEffectTypes::ROLL_SOUND_AI1:
		return rollingSound_ai1;
	case SoundEffectTypes::ROLL_SOUND_AI2:
		return rollingSound_ai2;
	case SoundEffectTypes::ROLL_SOUND_AI3:
		return rollingSound_ai3;
	case SoundEffectTypes::ROLL_SOUND_AI4:
		return rollingSound_ai4;
	case SoundEffectTypes::ROLL_SOUND_AI5:
		return rollingSound_ai5;
	case SoundEffectTypes::HITWALL_SOUND:
		return hitWallSound;
	case SoundEffectTypes::PICKITEM_SOUND:
		return pickItemSound;
	case SoundEffectTypes::DROPITEM_SOUND:
		return dropItemSound;
	case SoundEffectTypes::TURBO_SOUND:
		return turboSound;
	case SoundEffectTypes::EXPLOSION_SOUND:
		return explosionSound;
	case SoundEffectTypes::TICKING_SOUND:
		return tickingSound;
	case SoundEffectTypes::PAPERBAG_SOUND:
		return paperbagSound;
	case SoundEffectTypes::SELECT_SOUND:
		return selectSound;
	default:
		return nullptr;
	}
}

void AudioManager::changeBGM(BGMTypes BGM) {
	switch (BGM) {
	case BGMTypes::END_SCENE:
		bgm.music = loadMusic("resources/sfx/elevatorMusic.mp3");
		break;
	case BGMTypes::GAME_SCENE:
		bgm.music = loadMusic("resources/sfx/CoconutMall.mp3");
		break;
	case BGMTypes::MENU_SCENE:
		bgm.music = loadMusic("resources/sfx/JunesTheme.mp3");
		break;
	default:
		bgm.music = loadMusic("resources/sfx/JunesTheme.mp3");
		break;
	}
	//cout << "load bgm file" << endl;
	playMusic(&bgm);
}



Mix_Music* AudioManager::loadMusic(string filename) {
	//string fullPath = SDL_GetBasePath();
	//string fullPath = filename;

	//fullPath.append(filename);

	if (mMusic[filename] == nullptr) {
		mMusic[filename] = Mix_LoadMUS(filename.c_str());
		if (mMusic[filename] == NULL)
			cout << "Music Loading Error" << Mix_GetError() << endl;
	}
	return mMusic[filename];
}



Mix_Chunk* AudioManager::loadSFX(string filename) {
	//string fullPath = SDL_GetBasePath();

	//fullPath.append(filename);
	//printf("%s\n", fullPath.c_str());

	if (mSFX[filename] == nullptr) {
		mSFX[filename] = Mix_LoadWAV(filename.c_str());
		if (mSFX[filename] == NULL) {
			cout << "SFX loading error" << Mix_GetError() << endl;
		}
	}
	return mSFX[filename];
}

// play myMusic
void AudioManager::playMusic(Music *myMusic) {
	Mix_VolumeMusic(myMusic->volume);
	//Mix_PlayMusic(myMusic->music, myMusic->loop);
	if (Mix_PlayMusic(myMusic->music, myMusic->loop) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
	}
}

// pause music
void AudioManager::pauseMusic() {
	if (Mix_PlayingMusic() != 0)
		Mix_PauseMusic();
}

// resume the paused music
void AudioManager::resumeMusic() {
	if (Mix_PausedMusic() != 0)
		Mix_ResumeMusic();
}

void AudioManager::changeVolumeSFX(SoundEffect *mySfx, int volume) {
	//mySfx->volume = volume;
	Mix_Volume(mySfx->channel, volume);
}

void AudioManager::assignFreeChanel(SoundEffect *mySfx) {
	if (Mix_Playing(mySfx->channel)) {
		for (int i = 8; i < 20; i++) {
			if (!Mix_Playing(i)) {
				mySfx->channel = i;
				break;
			}
		}
	}
}

void AudioManager::changeDistanceSFX(SoundEffect *mySfx, float distance, float angle) {
	//mySfx->volume = volume;
	Mix_SetPosition(mySfx->channel, (Sint16)angle, (Uint8)distance);
}

void AudioManager::haltSFX(SoundEffect *mySfx) {
	if (Mix_Playing(mySfx->channel)) {
		Mix_FadeOutChannel(mySfx->channel, 15);
	}
}

void AudioManager::pauseAllSFX() {
	Mix_Pause(-1);
}

void AudioManager::resumeAllSFX() {
	//cout << "before resume: " << Mix_Paused(-1) << endl;
	Mix_Resume(-1);
	//cout << "after resume: " << Mix_Paused(-1) << endl;
}

void AudioManager::resetAudio() {
	Mix_HaltChannel(-1);
}

void AudioManager::playSFX(SoundEffect *mySfx) {
	if (!Mix_Playing(mySfx->channel)) {
		//Mix_SetPosition(mySfx->channel, mySfx->angle, mySfx->distance);
		Mix_PlayChannel(mySfx->channel, mySfx->sfx, mySfx->loop);
	}

	//Mix_PlayChannelTimed(mySfx->channel, mySfx->sfx, mySfx->loop, mySfx->time);
	//SDL_Delay(mySfx->time);
}

void AudioManager::init() {

	// init SDL
	SDL_Init(SDL_INIT_AUDIO);
	// init SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		cout << "mixer initialization error" << endl;
	}

	// allocate sound effect channel  
	Mix_AllocateChannels(20);

	// load sound effect from file
	//for (int i = 0; i < filenames.size(); i++) {
	//	SoundEffect sfx;
	//	sfx.filename = filenames[i];
	//	sfx.sfx = loadSFX(sfx.filename);
	//	sfx.channel = i; // allocate the channel for sound effect
	//	soundEffects.push_back(sfx);
	//}

	rollingSound_player1->filename = "resources/sfx/rollCart.wav";
	rollingSound_player1->sfx = loadSFX(rollingSound_player1->filename);
	rollingSound_player1->channel = 0;
	rollingSound_player1->loop = 4;

	rollingSound_ai1->filename = "resources/sfx/rollCart.wav";
	rollingSound_ai1->sfx = loadSFX(rollingSound_ai1->filename);
	rollingSound_ai1->channel = 1;

	rollingSound_ai2->filename = "resources/sfx/rollCart.wav";
	rollingSound_ai2->sfx = loadSFX(rollingSound_ai2->filename);
	rollingSound_ai2->channel = 2;

	rollingSound_ai3->filename = "resources/sfx/rollCart.wav";
	rollingSound_ai3->sfx = loadSFX(rollingSound_ai3->filename);
	rollingSound_ai3->channel = 3;

	rollingSound_ai4->filename = "resources/sfx/rollCart.wav";
	rollingSound_ai4->sfx = loadSFX(rollingSound_ai4->filename);
	rollingSound_ai4->channel = 4;

	rollingSound_ai5->filename = "resources/sfx/rollCart.wav";
	rollingSound_ai5->sfx = loadSFX(rollingSound_ai5->filename);
	rollingSound_ai5->channel = 5;

	paperbagSound->filename = "resources/sfx/paperBag.wav";
	paperbagSound->sfx = loadSFX(paperbagSound->filename);
	paperbagSound->channel = 6;

	hitWallSound->filename = "resources/sfx/cartHitWall.wav";
	hitWallSound->sfx = loadSFX(hitWallSound->filename);
	hitWallSound->channel = 12;

	selectSound->filename = "resources/sfx/ffselected.wav";
	selectSound->sfx = loadSFX(selectSound->filename);
	selectSound->channel = 8;

	pickItemSound->filename = "resources/sfx/pickItem.wav";
	pickItemSound->sfx = loadSFX(pickItemSound->filename);
	pickItemSound->channel = 13;

	dropItemSound->filename = "resources/sfx/itemDrop.wav";
	dropItemSound->sfx = loadSFX(dropItemSound->filename);
	dropItemSound->distance = 0;
	dropItemSound->channel = 9;
	//changeVolumeSFX(dropItemSound, 150);


	turboSound->filename = "resources/sfx/turbo.wav";
	turboSound->sfx = loadSFX(turboSound->filename);
	//turboSound->distance = 200;
	turboSound->channel = 7;
	changeVolumeSFX(turboSound, 20);

	explosionSound->filename = "resources/sfx/explosion.wav";
	explosionSound->sfx = loadSFX(explosionSound->filename);
	explosionSound->channel = 10;

	tickingSound->filename = "resources/sfx/ticking.wav";
	tickingSound->sfx = loadSFX(tickingSound->filename);
	tickingSound->channel = 11;
	tickingSound->loop = 2;




	// load music from file
	//bgm.music = loadMusic(bgm.filename);
	//cout << "load bgm file" << endl;
	//playMusic(&bgm);

	changeBGM(MENU_SCENE);





}
float AudioManager::getAngle(glm::vec3 a, glm::vec3 b) {
	//float angle = atan2((a.x * b.z) - (a.z-b.x), (a.x*b.x) + (a.z*b.z));
	float angle = acos((a.x*b.x) + (a.z*b.z));
	//angle = (angle / 3.1415926) * 360;
	angle = (angle / 3.1415926) * 180;

	return angle;
}

void AudioManager::updateSeconds(double variableDeltaTime) {
	//playSFX(dropItemSound);
	//float angle = getAngle(testV1, testV2);
	//changeDistanceSFX(dropItemSound, 200, (Sint16)angle);
	//testV2.x += 5;
	//testV2.y += 5;
	//std::cout << "float angle: " << angle << std::endl;

	// play all the sound effects insidethe soundeffect vector
	//for (int i = 0; i < soundEffects.size(); i++) {
	//	playSFX(&soundEffects[i]);
	//}
	//resumeMusic();
	//playSFX(testSoundEffect);
	//printf("music is%s playing.\n",  ? "" : " not");
	if (!Mix_PlayingMusic()) {
		playMusic(&bgm);
	}

	//cout << "rolling volume: " << rollingSound->volume << endl;


}