#include <iostream>

#include "include/Gramophone/Gramophone.hpp"

int main(int argc, char** argv){

	Gramophone::initialize();

	Gramophone::Sound sound = Gramophone::loadSoundEffect("sounds/clock-26843.mp3");
	Gramophone::Source source = Gramophone::createSoundSource();

	Gramophone::setReverbType(Gramophone::ReverbType::Chapel);
	Gramophone::setSoundSourceEffects(source, true, false);

	Gramophone::playSource(source, sound);

	_sleep(20000);

	return 0;
}
