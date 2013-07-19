all: Doodle

Doodle: PolyGlyphEngine BezierGlyphEngine MusicTouch StraightStroke MultiTap Pinch Sequencer Tonalizer Metronome
	cd Doodle && qmake && make && cd ..

PolyGlyphEngine: Glyph
	cd DoodleGRs/PolyGlyphEngine && qmake && make && cd ../..

BezierGlyphEngine: Glyph
	cd DoodleGRs/BezierGlyphEngine && qmake && make && cd ../..

Glyph: DoodleSDK
	cd DoodleGRs/Glyph && qmake && make && cd ../..

MusicTouch: DoodleSDK
	cd DoodleGRs/MusicTouch && qmake && make && cd ../..

StraightStroke: DoodleSDK
	cd DoodleGRs/StraightStroke && qmake && make && cd ../..

MultiTap: DoodleSDK
	cd DoodleGRs/MultiTap && qmake && make && cd ../..

Pinch: DoodleSDK
	cd DoodleGRs/Pinch && qmake && make && cd ../..

Sequencer: DoodleSDK
	cd DoodleApps/Sequencer && qmake && make && cd ../..

Tonalizer: DoodleSDK
	cd DoodleApps/Tonalizer && qmake && make && cd ../..

Metronome: DoodleSDK
	cd DoodleApps/Metronome && qmake && make && cd ../..

DoodleSDK: nop
	cd DoodleSDK && qmake && make && cd ..

distclean:
	./distclean.sh

nop:
