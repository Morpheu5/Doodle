#!/bin/sh

make -C Doodle distclean
make -C DoodleSDK distclean
make -C DoodleApps/Tonalizer distclean
make -C DoodleApps/Metronome distclean
make -C DoodleApps/Sequencer distclean
make -C DoodleGRs/Glyph distclean
make -C DoodleGRs/Pinch distclean
make -C DoodleGRs/MultiTap distclean
make -C DoodleGRs/BezierGlyphEngine distclean
make -C DoodleGRs/PolyGlyphEngine distclean
make -C DoodleGRs/MusicTouch distclean
make -C DoodleGRs/StraightStroke distclean
make -C BezierGlyphBuilder distclean

rm -rf Doodle/debug Doodle/release
rm -rf DoodleApps/Tonalizer/debug DoodleApps/Tonalizer/release
rm -rf DoodleApps/Metronome/debug DoodleApps/Metronome/release
rm -rf DoodleApps/Sequencer/debug DoodleApps/Metronome/release
rm -rf DoodleSDK/debug DoodleSDK/release
rm -rf DoodleGRs/Glyph/debug DoodleGRs/Glyph/release
rm -rf DoodleGRs/Pinch/debug DoodleGRs/Pinch/release
rm -rf DoodleGRs/MultiTap/debug DoodleGRs/MultiTap/release
rm -rf DoodleGRs/BezierGlyphEngine/debug DoodleGRs/BezierGlyphEngine/release
rm -rf DoodleGRs/PolyGlyphEngine/debug DoodleGRs/PolyGlyphEngine/release
rm -rf DoodleGRs/MusicTouch/debug DoodleGRs/MusicTouch/release
rm -rf DoodleGRs/StraightStroke/debug DoodleGRs/StraightStroke/release
rm -rf BezierGlyphBuilder/debug BezierGlyphBuilder/release
